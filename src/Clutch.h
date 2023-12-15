#ifndef CLUTCH_H
#define CLUTCH_H

#include <Arduino.h>
#include <EEPROM.h>
#include <Servo.h>
#include <limits.h>
#include "canutil.h"

class Clutch {
  public:
	Clutch() {
		for (int i = 0; i < EEPROM.length(); i++) EEPROM.write(i, 0xFF);

		uint16_t saved;
		EEPROM.get(START_ADDRESS, saved); if(saved == 0xFFFF) setStart(1690);
		EEPROM.get(END_ADDRESS, saved); if(saved == 0xFFFF) setEnd(1300);
		EEPROM.get(FRICTION_ADDRESS, saved); if(saved == 0xFFFF) setFriction(1480);

		servo.attach(0);
		servo.writeMicroseconds(getStart());
	}

	void setStart(uint16_t value) { EEPROM.put(START_ADDRESS, value); }
	uint16_t getStart() { uint16_t saved; EEPROM.get(START_ADDRESS, saved); return saved; }

	void setEnd(uint16_t value) { EEPROM.put(END_ADDRESS, value); }
	uint16_t getEnd() { uint16_t saved; EEPROM.get(END_ADDRESS, saved); return saved; }

	void setFriction(uint16_t value) { EEPROM.put(FRICTION_ADDRESS, value); }
	uint16_t getFriction() { uint16_t saved; EEPROM.get(FRICTION_ADDRESS, saved); return saved; }

	int getPosition() { return position; }

	void setRpm(int value) { rpm = value; }
	int getRpm() { return rpm; }

	void analog_input(int value);
	void writeMicroseconds(int value);

	bool shiftOverride = false;

  private:
	const int START_ADDRESS = 10;
	const int END_ADDRESS = 12;
	const int FRICTION_ADDRESS = 14;
	
	Servo servo;

	int min = getStart();
	int max = getEnd();

	int analogMin = INT_MAX;
	int analogMax = INT_MIN;
	int rpm = 1000;
	unsigned long lastThreshold = 0;
	int position = 0;
};

#endif