#include "Transmission.h"

Transmission::Transmission(
	Clutch &clutchRef,
	FlexCAN_T4<CAN3, RX_SIZE_16, TX_SIZE_16> &canRef,
	Storage &settingsRef) :
	fsm(IDLE),
	clutch(clutchRef),
	can(canRef),
	storage(settingsRef) {
		pinMode(OUTPUT_PINS[UP], OUTPUT);
		pinMode(OUTPUT_PINS[DOWN], OUTPUT);
		pinMode(13, OUTPUT);
	}

void Transmission::broadcastValues(unsigned long frequency) {
	if (millis() - lastBroadastTime >= frequency) {
		lastBroadastTime = millis();

		CAN_message_t gear;
		gear.id = 1620;
		canutil::constructData(gear, storage.gear(), 0, 1);
		can.write(gear);

		CAN_message_t timing;
		timing.id = 1621;
		canutil::constructData(timing, storage.upDelay(), 0, 2);
		canutil::constructData(timing, storage.downDelay(), 2, 2);
		canutil::constructData(timing, storage.output(), 4, 2);
		canutil::constructData(timing, storage.timeout(), 6, 2);
		can.write(timing);
	}
}

void Transmission::shift(int direction) {
	if(fsm.state() != IDLE && fsm.state() != DOWN_CLUTCH_OUT) {
		Serial.println(String("\x1b[31m") + "Additional shift attempted" + String("\x1b[0m"));
		return;
	}

	int currentGear = storage.gear();

	if(storage.rpm() == 0) {
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

	broadcastValues();
	shiftStartTime = millis();

	if(direction == UP) {
		Serial.println("\nUP");
		fsm.state(UP_SPARK_CUT);
	} else {
		Serial.println("\nDOWN");
		fsm.state(DOWN_CLUTCH_IN);
	}
}

void Transmission::disableCombustion() {
	CAN_message_t msg;
	msg.id = 522;
	msg.buf[0] = 0b00000000; can.write(msg);
	msg.buf[0] = 0b00000001; can.write(msg);
	msg.buf[0] = 0b00000000; can.write(msg);
}

void Transmission::upRoutine() {
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
				if(storage.rpm() >= 500) digitalWrite(OUTPUT_PINS[UP], HIGH);
				digitalWrite(13, HIGH);
			});

			fsm.waitAndSetState(storage.output(), [](){
				// Wait for the output duration
			}, UP_DISABLE_SOLENOID);
			break;

		case UP_DISABLE_SOLENOID:
			fsm.runOnce([&](){
				Serial.println("SOLENOID DISABLE: " + String(millis() - shiftStartTime));
				digitalWrite(OUTPUT_PINS[UP], LOW);
				digitalWrite(13, LOW);
			});

			fsm.state(IDLE);
			break;
	}
}

void Transmission::downRoutine() {
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
				digitalWrite(OUTPUT_PINS[DOWN], HIGH);
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
				digitalWrite(OUTPUT_PINS[DOWN], LOW);
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
