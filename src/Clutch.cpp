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
    if(analogMax - analogMin < 20) { analogMin = analogMin - 20; }

	int threshold = analogMax - (analogMax - analogMin) * 0.08;
	int servoValue = getStart();

	// if(1000 >= millis() - lastThreshold) {
	// 	servoValue = getFriction();
	// }

	if(value <= threshold) {
		servoValue = map(value, threshold, analogMin, getFriction(), getEnd());
		lastThreshold = millis();
	} else if(200 < getRpm() && getRpm() < 3000) {
		servoValue = getEnd();
		lastThreshold = millis();
	}

	writeMicroseconds(servoValue);
}