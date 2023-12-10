#include "Clutch.h"

void Clutch::write(int value) {
	if(value > getStart()) value = getStart();
	if(value < getEnd()) value = getEnd();

	servo.write(value);
}

void Clutch::analog_input(int value) {
	if(shiftOverride) {
		return;
	}

	if(value > analogMax) { analogMax = value; }
	if(value < analogMin) { analogMin = value; }

	int threshold = analogMax * 0.9;
	int servoValue = getStart();

	if(value <= threshold) {
		servoValue = map(value, threshold, analogMin, getFriction(), getEnd());
	} else if(200 < getRpm() && getRpm() < 3000) {
		servoValue = getEnd();
	}

	if(servoValue > getStart()) servoValue = getStart();
	if(servoValue < getEnd()) servoValue = getEnd();

	servo.write(servoValue);
}