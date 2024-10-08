#include "Transmission.h"

Transmission::Transmission(Clutch &clutchRef, Can &canRef, Storage &storageRef) : FiniteStateMachine(IDLE), clutch(clutchRef), can(canRef), storage(storageRef) {
	pinMode(storage.ECU_UP, OUTPUT);
	pinMode(storage.IA, OUTPUT);
	pinMode(storage.IB, OUTPUT);
	pinMode(13, OUTPUT);

	digitalWrite(storage.ECU_UP, HIGH);
}

void Transmission::shift(Transmission::Direction direction) {
	if(state() != IDLE && state() != DOWN_CLUTCH_OUT) {
		Serial.println(String("\x1b[31m") + "Additional shift attempted" + String("\x1b[0m"));
		return;
	}

	CAN_message_t msg;
	msg.id = 522;
	can.interface.write(msg);
	
	shiftStartTime = millis();

	if(direction == UP) {
		Serial.println("\nUP");
		state(UP_SPARK_CUT);
	} else if(direction == DOWN) {
		Serial.println("\nDOWN");
		state(DOWN_CLUTCH_IN);
	}
}

void Transmission::upRoutine() {
	const int pin = storage.IA;

	switch(state()) {
		case UP_SPARK_CUT:
			runOnce([&](){
				Serial.println("COMBUSTION DISABLE: " + String(millis() - shiftStartTime));
				digitalWrite(storage.ECU_UP, LOW);
			});

			waitAndSetState(storage.upDelay(), [](){
				// Wait for the up delay
			}, UP_ENABLE_SOLENOID);
			break;

		case UP_ENABLE_SOLENOID:
			runOnce([&](){
				Serial.println("SOLENOID ENABLE: " + String(millis() - shiftStartTime));
				digitalWrite(pin, HIGH);
				digitalWrite(13, HIGH);
			});

			waitAndSetState(storage.output(), [](){
				// Wait for the output duration
			}, UP_DISABLE_SOLENOID);
			break;

		case UP_DISABLE_SOLENOID:
			runOnce([&](){
				Serial.println("SOLENOID DISABLE: " + String(millis() - shiftStartTime));
				digitalWrite(pin, LOW);
				digitalWrite(storage.ECU_UP, HIGH);
				digitalWrite(13, LOW);
			});

			state(IDLE);
			break;
	}
}

void Transmission::downRoutine() {
	const int pin = storage.IB;

	switch(state()) {
		case DOWN_CLUTCH_IN: {
			runOnce([&](){
				Serial.println("CLUTCH IN " + String(millis() - shiftStartTime));
				clutch.state(Clutch::State::IDLE);
				clutch.writeMicroseconds(storage.end());
			});

			// Allow a shorter delay if the clutch is already in
			int delay = storage.downDelay();
			if(clutch.input >= 90) {
				delay = storage.upDelay();
			}

			waitAndSetState(delay, [](){
				// Wait for the delay
			}, DOWN_ENABLE_SOLENOID);
			break;
		}

		case DOWN_ENABLE_SOLENOID: {
			runOnce([&](){
				Serial.println("SOLENOID ENABLE: " + String(millis() - shiftStartTime));
				digitalWrite(pin, HIGH);
				digitalWrite(13, HIGH);
			});

			waitAndSetState(storage.output(), [](){
				// Wait output duration
			}, DOWN_DISABLE_SOLENOID);
			break;
		}

		case DOWN_DISABLE_SOLENOID: {
			runOnce([&](){
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
				clutch.state(Clutch::State::ANALOG_INPUT);
			}

			waitAndSetState(delay, [](){

			}, nextState);
			break;
		}

		case DOWN_CLUTCH_OUT: {
			runOnce([&](){
				Serial.println("CLUTCH OUT " + String(millis() - shiftStartTime));
				clutch.writeMicroseconds(storage.start());
				clutch.state(Clutch::State::ANALOG_INPUT);
			});

			state(IDLE);
			break;
		}
	}
}

void Transmission::update() {
	upRoutine();
	downRoutine();
}
