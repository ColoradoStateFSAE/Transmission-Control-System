#include "Clutch.h"

Clutch::Clutch(
	int pin,
	FlexCAN_T4<CAN3, RX_SIZE_16, TX_SIZE_16> &canRef,
	Storage &storageRef) :
	fsm(ANALOG_INPUT),
	pin(pin),
	can(canRef),
	storage(storageRef) {
		servo.attach(pin);
		servo.writeMicroseconds(storage.start());
	}

void Clutch::writeMicroseconds(int value) {
	value = constrain(value, storage.end(), storage.start());
	servoPosition = value;
	servo.writeMicroseconds(value);
}

void Clutch::broadcastValues(unsigned long frequency) {
	if (millis() - lastBroadcastTime >= frequency) {
		lastBroadcastTime = millis();

		CAN_message_t msg;
		msg.id = 1622;
		canutil::constructData(msg, storage.start(), 0, 2);
		canutil::constructData(msg, storage.end(), 2, 2);
		canutil::constructData(msg, storage.friction(), 4, 2);
		canutil::constructData(msg, servoPosition, 6, 2);
		can.write(msg);

		CAN_message_t autoLaunchMsg;
		autoLaunchMsg.id = 1625;
		autoLaunchMsg.buf[0] = storage.autoLaunch();
		can.write(autoLaunchMsg);
	}
}

int Clutch::position() {
	return servoPosition;
}

void Clutch::update() {
	switch(fsm.state()) {
		case ANALOG_INPUT: {
			int servoWrite = map(input, 0.0f, 100.0f, storage.start(), storage.end());
			writeMicroseconds(servoWrite);

			if(storage.autoLaunch() && input >= 90) {
				fsm.state(HOLD_END);
				return;
			}
			break;
		}

		case HOLD_END: {
			fsm.runOnce([&](){
				Serial.println("\nHOLD END");
			});

			autoLaunchPosition = storage.end();
			writeMicroseconds(autoLaunchPosition);

			if(input <= 50) {
				fsm.state(GOTO_FRICTION);
			} else if(!storage.autoLaunch()) {
				fsm.state(ANALOG_INPUT);
			}
			break;
		}

		case GOTO_FRICTION: {
			fsm.runOnce([&](){
				autoLanchStartTime = millis();
				Serial.println("GOTO FRICTION: " + String(millis() - autoLanchStartTime));
			});

			if(!fsm.incrementOverTime(autoLaunchPosition, storage.friction(), 200)) {
				fsm.state(HOLD_FRICTION);
			}
			writeMicroseconds(autoLaunchPosition);

			break;
		}

		case HOLD_FRICTION: {
			fsm.runOnce([&](){
				Serial.println("HOLD FRICTION: " + String(millis() - autoLanchStartTime));
			});

			fsm.waitAndSetState(500, [](){
				// Wait 500 ms
			}, GOTO_START);
			break;
		}

		case GOTO_START: {
			fsm.runOnce([&](){
				Serial.println("GOTO START: " + String(millis() - autoLanchStartTime));
			});

			if(!fsm.incrementOverTime(autoLaunchPosition, storage.start(), 500)) {
				Serial.println("REACHED START: " + String(millis() - autoLanchStartTime));
				fsm.state(ANALOG_INPUT);
			}
			writeMicroseconds(autoLaunchPosition);

			break;
		}
	}
}
