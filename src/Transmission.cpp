#include "Transmission.h"

using namespace TeensyTimerTool;

OneShotTimer outputEnable(GPT1);
OneShotTimer outputDisable(GPT2);

void Transmission::broadcast_gear(unsigned long frequency) {
	static unsigned long lastBroadastTime = 0;
	if (millis() - lastBroadastTime >= frequency) {
		lastBroadastTime = millis();

		CAN_message_t msg;
		msg.id = 1620;
		canutil::construct_data(msg, getGear(), 0, 1);
		canutil::construct_data(msg, getDelay(), 2, 2);
		canutil::construct_data(msg, getOutput(), 4, 2);
		canutil::construct_data(msg, getTimeout(), 6, 2);
		can.write(msg);
	}
}

void Transmission::shift(int direction) {
	static unsigned long lastShift = 0;
	if((millis() - lastShift) < getTimeout()) {
		Serial.println("Attempted to shift before timeout: " + String(millis() - lastShift) + '\n');
		return;
	}
	lastShift = millis();

	if(getRpm() == 0) {
		setGear(0);
	} else if(direction == UP && getGear() == 0) {
		setGear(2);
	} else if(direction == UP && getGear() < 6) {
		setGear(getGear()+1);
	} else if(direction == DOWN && getGear() == 0) {
		setGear(1);
	} else if(direction == DOWN && getGear() > 1) {
		setGear(getGear()-1);
	}

	if(direction == UP) {
		Serial.println("UP");
	} else {
		Serial.println("DOWN");
	}

	startTime = millis();
	disable_combustion();
	power_solenoid(direction);
	broadcast_gear();
}

void Transmission::disable_combustion() {
	CAN_message_t msg;
	msg.id = 522;
	msg.buf[0] = 0b00000000; can.write(msg);
	msg.buf[0] = 0b00000001; can.write(msg);
	msg.buf[0] = 0b00000000; can.write(msg);
	Serial.println("DISABLE COMBUSTION: " + String(millis() - startTime));
}

void Transmission::power_solenoid(int direction) {
	int outputPin = OUTPUT_PINS[direction];

	int enableDelay = getDelay();

	if(direction == DOWN) {
		enableDelay = CLUTCH_DELAY;
		clutch.shiftOverride = true;
		clutch.write(clutch.getEnd());
		Serial.println("SERVO ENABLE: " + String(millis() - startTime));
	}

	// Create an interrupt timer to enable the solenoid
	outputEnable.begin([this, outputPin] {
		if(getRpm() >= 500) { digitalWrite(outputPin, HIGH); }
		Serial.println("SOLENOID ENABLE: " + String(millis() - startTime));
	});
	outputEnable.trigger(enableDelay * 1000);

	// Create an interrupt timer to disable the solenoid
	outputDisable.begin([this, outputPin, direction] {
		if(getRpm() >= 500) { digitalWrite(outputPin, LOW); }
		Serial.println("SOLENOID DISABLE: " + String(millis() - startTime));

		if(direction == DOWN) {
			clutch.write(clutch.getStart());
			clutch.shiftOverride = false;
			Serial.println("SERVO DISABLE: " + String(millis() - startTime));
		}

		Serial.println();
	});
	outputDisable.trigger((enableDelay + getOutput()) * 1000);
}
