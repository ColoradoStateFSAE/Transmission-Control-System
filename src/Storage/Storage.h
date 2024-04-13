#ifndef STORAGE_H
#define STORAGE_H

#include <Arduino.h>
#include <EEPROM.h>

class Storage {
  public:
	void begin();
	int read(int address, short defaultValue);
	int read(int address);
	void save(int address, short value);
	void clear();

	const int UP = 41;
	const int DOWN = 40;
	const int CLUTCH_LEFT = -1;
	const int CLUTCH_RIGHT = 15;
	const int IA = 16;
	const int IB = 17;
	const int ECU_UP = 9;
	const int ECU_DOWN = 8;
	const int SERVO = 37;

	// Saved values from EEPROM
	int upDelay();
	void upDelay(int value);

	int downDelay();
	void downDelay(int value);

	int output();
	void output(int value);

	int timeout();
	void timeout(int value);

	int start();
	void start(int value);

	int end();
	void end(int value);

	int friction();
	void friction(int value);

	int autoLaunch();
	void autoLaunch(int value);

  private:
  	enum Addresses {
		UP_DELAY_ADDRESS = 0,
		DOWN_DELAY_ADDRESS = 2,
		OUTPUT_ADDRESS = 4,
		TIMEOUT_ADDRESS = 6,
		START_ADDRESS = 8,
		END_ADDRESS = 10,
		FRICTION_ADDRESS = 12,
		AUTO_LAUNCH_ADDRESS = 14
	};
};

#endif
