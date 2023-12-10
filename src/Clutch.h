#ifndef CLUTCH_H
#define CLUTCH_H

#include <Arduino.h>
#include <EEPROM.h>
#include <Servo.h>
#include "canutil.h"
#include <limits.h>

class Clutch {
  public:
	Clutch() = delete;
	Clutch(const FlexCAN_T4<CAN3, RX_SIZE_16, TX_SIZE_16> &canRef) : can(canRef) {
		//for (int i = 0; i < EEPROM.length(); i++) EEPROM.write(i, 0xFF);

		uint16_t saved;
		EEPROM.get(START_ADDRESS, saved); if(saved == 0xFFFF) setStart(112);
		EEPROM.get(END_ADDRESS, saved); if(saved == 0xFFFF) setEnd(60);
		EEPROM.get(FRICTION_ADDRESS, saved); if(saved == 0xFFFF) setFriction(80);

		servo.attach(0);
		servo.write(getStart());
	}

	void setStart(uint16_t value) { EEPROM.put(START_ADDRESS, value); }
	uint16_t getStart() { uint16_t saved; EEPROM.get(START_ADDRESS, saved); return saved; }

	void setEnd(uint16_t value) { EEPROM.put(END_ADDRESS, value); }
	uint16_t getEnd() { uint16_t saved; EEPROM.get(END_ADDRESS, saved); return saved; }

	void setFriction(uint16_t value) { EEPROM.put(FRICTION_ADDRESS, value); }
	uint16_t getFriction() { uint16_t saved; EEPROM.get(FRICTION_ADDRESS, saved); return saved; }

	void broadcast_values(unsigned long frequency);

	void analog_input(int value);
	void write(int value);

  private:
	const int START_ADDRESS = 8;
	const int END_ADDRESS = 10;
	const int FRICTION_ADDRESS = 12;

	FlexCAN_T4<CAN3> can;

	Servo servo;

	int analogMin = INT_MAX;
	int analogMax = INT_MIN;
};

#endif