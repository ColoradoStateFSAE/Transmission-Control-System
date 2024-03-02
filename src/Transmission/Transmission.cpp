#include "Transmission.h"

Transmission::Transmission(Clutch &clutchRef, Can &canRef, Storage &settingsRef) : clutch(clutchRef), can(canRef), storage(settingsRef) {
	pinMode(13, OUTPUT);
}

void Transmission::changeGear(Transmission::Direction direction) {
	int currentGear = storage.gear();

	if(can.rpm() == 0) {
		storage.gear(0);
	} else if(direction == UP && currentGear == 0) {
		storage.gear(2);
	} else if(direction == UP && currentGear < 6) {
		storage.gear(storage.gear() + 1);
	} else if(direction == DOWN && currentGear == 0) {
		storage.gear(1);
	} else if(direction == DOWN && currentGear > 1) {
		storage.gear(storage.gear() - 1);
	}
}

void Transmission::shift(Transmission::Direction direction) {
	if(fsm.state() != IDLE && fsm.state() != DOWN_CLUTCH_OUT) {
		Serial.println(String("\x1b[31m") + "Additional shift attempted" + String("\x1b[0m"));
		return;
	}

	changeGear(direction);
	shiftStartTime = millis();

	if(direction == UP) {
		Serial.println("\nUP");
		fsm.state(UP_SPARK_CUT);
	} else if(direction == DOWN) {
		Serial.println("\nDOWN");
		fsm.state(DOWN_CLUTCH_IN);
	}
}

void Transmission::disableCombustion() {
	CAN_message_t msg;
	msg.id = 522;
	msg.buf[0] = 0b00000000; can.interface.write(msg);
	msg.buf[0] = 0b00000001; can.interface.write(msg);
	msg.buf[0] = 0b00000000; can.interface.write(msg);
}

void Transmission::upRoutine() {
	int pin = storage.IA();

	switch(fsm.state()) {
		case UP_SPARK_CUT:
			fsm.runOnce([&](){
				Serial.println("COMBUSTION DISABLE: " + String(millis() - shiftStartTime));
				disableCombustion();
			});

			fsm.waitAndSetState(storage.upDelay(), [](){
				// Wait for the up delay
			}, UP_ENABLE_SOLENOID);
			break;

		case UP_ENABLE_SOLENOID:
			fsm.runOnce([&](){
				Serial.println("SOLENOID ENABLE: " + String(millis() - shiftStartTime));
				if(can.rpm() >= 500) digitalWrite(pin, HIGH);
				digitalWrite(13, HIGH);
			});

			fsm.waitAndSetState(storage.output(), [](){
				// Wait for the output duration
			}, UP_DISABLE_SOLENOID);
			break;

		case UP_DISABLE_SOLENOID:
			fsm.runOnce([&](){
				Serial.println("SOLENOID DISABLE: " + String(millis() - shiftStartTime));
				digitalWrite(pin, LOW);
				digitalWrite(13, LOW);
			});

			fsm.state(IDLE);
			break;
	}
}

void Transmission::downRoutine() {
	int pin = storage.IB();

	switch(fsm.state()) {
		case DOWN_CLUTCH_IN: {
			fsm.runOnce([&](){
				Serial.println("CLUTCH IN " + String(millis() - shiftStartTime));
				clutch.fsm.state(Clutch::State::IDLE);
				clutch.writeMicroseconds(storage.end());
			});

			// Allow a shorter delay if the clutch is already in
			int delay = storage.downDelay();
			if(clutch.input >= 90) {
				delay = storage.upDelay();
			}

			fsm.waitAndSetState(delay, [](){
				// Wait for the delay
			}, DOWN_ENABLE_SOLENOID);
			break;
		}

		case DOWN_ENABLE_SOLENOID: {
			fsm.runOnce([&](){
				Serial.println("SOLENOID ENABLE: " + String(millis() - shiftStartTime));
				if(can.rpm() >= 500) digitalWrite(pin, HIGH);
				digitalWrite(13, HIGH);
			});

			fsm.waitAndSetState(storage.output(), [](){
				// Wait output duration
			}, DOWN_DISABLE_SOLENOID);
			break;
		}

		case DOWN_DISABLE_SOLENOID: {
			fsm.runOnce([&](){
				Serial.println("SOLENOID DISABLE: " + String(millis() - shiftStartTime));
				digitalWrite(pin, LOW);
				digitalWrite(13, LOW);
			});

			// Switch directly to IDLE if the clutch is already in
			int delay = 50;
			State nextState = DOWN_CLUTCH_OUT;
			if(clutch.input >= 90) {
				delay = 0;
				nextState = IDLE;
				clutch.fsm.state(Clutch::State::ANALOG_INPUT);
			}

			fsm.waitAndSetState(delay, [](){

			}, nextState);
			break;
		}

		case DOWN_CLUTCH_OUT: {
			fsm.runOnce([&](){
				Serial.println("CLUTCH OUT " + String(millis() - shiftStartTime));
				clutch.writeMicroseconds(storage.start());
				clutch.fsm.state(Clutch::State::ANALOG_INPUT);
			});

			fsm.state(IDLE);
			break;
		}
	}
}

void Transmission::update() {
	upRoutine();
	downRoutine();
}
