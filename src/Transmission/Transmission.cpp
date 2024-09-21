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
	
	shiftStartTime = millis();

	if(direction == UP) {
		Serial.println("\nUP");
		if(can.rpm <= 6000 && clutch.input < 90) {
			state(UP_CLUTCH_IN);
		} else {
			state(UP_SPARK_CUT);
		}
		
	} else if(direction == DOWN) {
		Serial.println("\nDOWN");
		state(DOWN_CLUTCH_IN);
	}
}

void Transmission::upRoutine() {
	const int pin = storage.IA;

	switch(state()) {

		case UP_CLUTCH_IN: {
			runOnce([&](){
				Serial.println("CLUTCH IN " + String(millis() - shiftStartTime));
				clutch.state(Clutch::State::IDLE);
				clutch.writeMicroseconds(storage.end());
			});

			// Allow a shorter delay if the clutch paddle is already in
			int delay = storage.downDelay();
			if(clutch.input >= 90) {
				delay = storage.upDelay();
			}

			// Wait for the delay and set next state
			waitAndSetState(delay, [](){}, UP_ENABLE_SOLENOID);
			break;
		}

		case UP_SPARK_CUT:
			runOnce([&](){
				Serial.println("COMBUSTION DISABLE: " + String(millis() - shiftStartTime));
				digitalWrite(storage.ECU_UP, LOW);
			});

			// Wait for the up delay and set next state
			waitAndSetState(storage.upDelay(), [](){}, UP_ENABLE_SOLENOID);
			break;

		case UP_ENABLE_SOLENOID:
			runOnce([&](){
				Serial.println("SOLENOID ENABLE: " + String(millis() - shiftStartTime));
				digitalWrite(pin, HIGH);
				digitalWrite(13, HIGH);
			});

			// Wait for the output duration and set next state
			waitAndSetState(storage.output(), [](){}, UP_DISABLE_SOLENOID);
			break;

		case UP_DISABLE_SOLENOID: {
			runOnce([&](){
				Serial.println("SOLENOID DISABLE: " + String(millis() - shiftStartTime));
				digitalWrite(pin, LOW);
				digitalWrite(13, LOW);
			});

			// Default parameters
			int delay = 50;
			State nextState = UP_CLUTCH_OUT;
			
			// Switch directly to IDLE if the clutch paddle is already in or if clutch is already out
			if(clutch.input >= 90 || clutch.position() == storage.start()) {
				delay = 0;
				nextState = IDLE;
				clutch.state(Clutch::State::ANALOG_INPUT);
			}

			// Wait for set delay and set next state
			waitAndSetState(delay, [](){}, nextState);
			break;
		}

		case UP_CLUTCH_OUT: {
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

void Transmission::downRoutine() {
	const int pin = storage.IB;

	switch(state()) {
		case DOWN_CLUTCH_IN: {
			runOnce([&](){
				Serial.println("CLUTCH IN " + String(millis() - shiftStartTime));
				clutch.state(Clutch::State::IDLE);
				clutch.writeMicroseconds(storage.end());
			});

			// Allow a shorter delay if the clutch paddle is already in
			int delay = storage.downDelay();
			if(clutch.input >= 90) {
				delay = storage.upDelay();
			}

			// Wait for the delay and set next state
			waitAndSetState(delay, [](){}, DOWN_ENABLE_SOLENOID);
			break;
		}

		case DOWN_ENABLE_SOLENOID: {
			runOnce([&](){
				Serial.println("SOLENOID ENABLE: " + String(millis() - shiftStartTime));
				digitalWrite(pin, HIGH);
				digitalWrite(13, HIGH);
			});

			// Wait output duration and set next state
			waitAndSetState(storage.output(), [](){}, DOWN_DISABLE_SOLENOID);
			break;
		}

		case DOWN_DISABLE_SOLENOID: {
			runOnce([&](){
				Serial.println("SOLENOID DISABLE: " + String(millis() - shiftStartTime));
				digitalWrite(pin, LOW);
				digitalWrite(13, LOW);
			});

			// Switch directly to IDLE if the clutch paddle is already in
			int delay = 50;
			State nextState = DOWN_CLUTCH_OUT;
			if(clutch.input >= 90) {
				delay = 0;
				nextState = IDLE;
				clutch.state(Clutch::State::ANALOG_INPUT);
			}
			
			// Wait for set delay and set next state
			waitAndSetState(delay, [](){}, nextState);
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
