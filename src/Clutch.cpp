#include "Clutch.h"

void Clutch::writeMicroseconds(int value) {
	value = constrain(value, getEnd(), getStart());
	servo.writeMicroseconds(value);
}

void Clutch::analog_input(int value) {
	if(shiftOverride) {
		return;
	}

	if(value > analogMax) { analogMax = value; }
	if(value < analogMin) { analogMin = value; }

	int threshold = analogMax * 0.9;
	int servoValue = getStart();

	if(2000 >= millis() - lastThreshold) {
		servoValue = getFriction();
	}

	if(value <= threshold) {
		servoValue = map(value, threshold, analogMin, getFriction(), getEnd());
		lastThreshold = millis();
	} else if(200 < getRpm() && getRpm() < 3000) {
		servoValue = getEnd();
		lastThreshold = millis();
	}

	// static unsigned long lastPrint = millis();
	// if(millis() - lastPrint > 400) {
	// 	Serial.println(servoValue);
	// 	lastPrint = millis();
	// }

	writeMicroseconds(servoValue);
}