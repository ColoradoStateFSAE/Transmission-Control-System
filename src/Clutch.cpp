#include "Clutch.h"

void Clutch::write(int value) {
	if(value > getStart()) value = getStart();
	if(value < getEnd()) value = getEnd();
	Serial.println("VALUE: " + String(value));
	Serial.println("END: " + String(getEnd()));
	Serial.println("START: " + String(getStart()) + "\n");

	servo.write(value);
}

void Clutch::broadcast_values(unsigned long frequency) {
	static unsigned long lastBroadastTime = 0;
	if (millis() - lastBroadastTime >= frequency) {
		lastBroadastTime = millis();

		CAN_message_t msg;
		msg.id = 1621;
		canutil::construct_data(msg, getStart(), 0, 2);
		canutil::construct_data(msg, getEnd(), 2, 2);
		canutil::construct_data(msg, getFriction(), 4, 2);
		can.write(msg);
	}
}

void Clutch::update(int value) {
	if(value > analogMax) { analogMax = value; }
	if(value < analogMin) { analogMin = value; }

	int threshold = analogMax * 0.9;
	int servoValue = 0;

	if(value <= threshold) {
		servoValue = map(value, threshold, analogMin, getFriction(), getEnd());
	} else {
		servoValue = getStart();
	}

	if(servoValue > getStart()) servoValue = getStart();
	if(servoValue < getEnd()) servoValue = getEnd();

	servo.write(servoValue);
}