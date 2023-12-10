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

	void write(int value) {
		if(value > getStart()) { value = getStart(); }
		if(value < getEnd()) value = getEnd();
		Serial.println("VALUE: " + String(value));
		Serial.println("END: " + String(getEnd()));
		Serial.println("START: " + String(getStart()) + "\n");

		servo.write(value);
	}

	void broadcast_values(unsigned long frequency) {
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

	void update(int value) {
		if(value > analogMax) { analogMax = value; }
		if(value < analogMin) { analogMin = value; }

		int threshold = analogMax * 0.9;
		if(value <= threshold) {
			value = map(value, threshold, analogMin, getFriction(), getEnd());
		} else {
			value = getStart();
		}

		if(value > getStart()) value = getStart();
		if(value < getEnd()) value = getEnd();

		servo.write(value);
	}


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