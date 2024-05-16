#include "Clutch.h"

Clutch::Clutch(Storage &storageRef) : FiniteStateMachine(ANALOG_INPUT), storage(storageRef) {

}

void Clutch::begin(int pin) {
	servo.attach(pin);
	servo.writeMicroseconds(storage.start());
}

void Clutch::writeMicroseconds(int value) {
	value = constrain(value, storage.end(), storage.start());
	servoPosition = value;
	servo.writeMicroseconds(value);
}

int Clutch::position() {
	return servoPosition;
}

int Clutch::percentage() {
	float normalizedValue = (float)(servoPosition - storage.start()) / (storage.end() - storage.start());
	normalizedValue = round(normalizedValue * 100);
	return normalizedValue;
}

void Clutch::update() {
	switch(state()) {
		case ANALOG_INPUT: {
			int servoWrite = map(input, 0.0f, 100.0f, storage.start(), storage.end());
			writeMicroseconds(servoWrite);
			break;
		}

		case HOLD_END: {
			if(!storage.autoLaunch()) { state(State::ANALOG_INPUT); return; }

			autoLaunchPosition = storage.end();
			writeMicroseconds(autoLaunchPosition);

			if(input <= 50) {
				state(GOTO_FRICTION);
			}
			break;
		}

		case GOTO_FRICTION: {
			if(90 <= input) { state(State::ANALOG_INPUT); return; }

			runOnce([&](){
				autoLanchStartTime = millis();
				Serial.println("\nGOTO FRICTION: " + String(millis() - autoLanchStartTime));
			});

			if(!incrementOverTime(autoLaunchPosition, storage.friction(), 200)) {
				state(HOLD_FRICTION);
			}
			writeMicroseconds(autoLaunchPosition);
			break;
		}

		case HOLD_FRICTION: {
			if(90 <= input) { state(State::ANALOG_INPUT); return; }

			runOnce([&](){
				Serial.println("HOLD FRICTION: " + String(millis() - autoLanchStartTime));
			});

			waitAndSetState(500, [](){
				// Wait 500 ms
			}, GOTO_START);
			break;
		}

		case GOTO_START: {
			if(90 <= input) { state(State::ANALOG_INPUT); return; }
			
			runOnce([&](){
				Serial.println("GOTO START: " + String(millis() - autoLanchStartTime));
			});

			if(!incrementOverTime(autoLaunchPosition, storage.start(), 500)) {
				Serial.println("REACHED START: " + String(millis() - autoLanchStartTime));
				state(ANALOG_INPUT);
			}
			writeMicroseconds(autoLaunchPosition);
			break;
		}
	}
}
