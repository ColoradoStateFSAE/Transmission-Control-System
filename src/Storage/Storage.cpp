#include "Storage.h"

using namespace std;

void Storage::begin() {
	read(UP_DELAY_ADDRESS, 50);
	read(DOWN_DELAY_ADDRESS, 200);
	read(OUTPUT_ADDRESS, 50);
	read(TIMEOUT_ADDRESS, 300);
	read(START_ADDRESS, 1600);
	read(END_ADDRESS, 1400);
	read(FRICTION_ADDRESS, 1500);
	read(AUTO_LAUNCH_ADDRESS, 0);
}

int Storage::read(int address, short defaultValue) {
	uint16_t data;
	EEPROM.get(address, data);
	if(data == 0xFFFF) {
		EEPROM.put(address, defaultValue);
	}

	return read(address);
}

int Storage::read(int address) {
	auto start = micros();
	

	short data;
	EEPROM.get(address, data);
	Serial.println(String(micros() - start));
	return data;
}

void Storage::save(int address, short value) {
	short data;
	EEPROM.get(address, data);
	if(data != value) {
		EEPROM.put(address, value);
	}
}

void Storage::clear() {
	for (int i = 0; i < EEPROM.length(); i++) {
		EEPROM.write(i, 0xFF);
	}
}

int Storage::upDelay() { return read(UP_DELAY_ADDRESS); }
void Storage::upDelay(int value) {
	save(UP_DELAY_ADDRESS, value);
}

int Storage::downDelay() { return read(DOWN_DELAY_ADDRESS); }
void Storage::downDelay(int value) {
	save(DOWN_DELAY_ADDRESS, value);
}

int Storage::output() { return read(OUTPUT_ADDRESS); }
void Storage::output(int value) {
	save(OUTPUT_ADDRESS, value);
}

int Storage::timeout() { return read(TIMEOUT_ADDRESS); }
void Storage::timeout(int value) {
	save(TIMEOUT_ADDRESS, value);
}

int Storage::start() { return read(START_ADDRESS); }
void Storage::start(int value) {
	save(START_ADDRESS, value);
}

int Storage::end() { return read(END_ADDRESS); }
void Storage::end(int value) {
	save(END_ADDRESS, value);
}

int Storage::friction() { return read(FRICTION_ADDRESS); }
void Storage::friction(int value) {
	save(FRICTION_ADDRESS, value);
}

int Storage::autoLaunch() { return read(AUTO_LAUNCH_ADDRESS); }
void Storage::autoLaunch(int value) {
	save(AUTO_LAUNCH_ADDRESS, value);
}
