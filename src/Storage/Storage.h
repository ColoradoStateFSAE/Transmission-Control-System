#ifndef STORAGE_H
#define STORAGE_H

#include <ArduinoJson.h>
#include <string>
#include <SD.h>
#include <vector>

class Storage {
  public:
  	Storage() = delete;
	Storage(std::string file) {
		filePath = '/' + file;
	}

	bool begin();
	bool readPins();
	void verifyJson();
	void writeJson();
	void readJson();

	// Pin definitions
	int up();
	int down();
	int clutchLeft();
	int clutchRight();
	int IA();
	int IB();
	int ecuUp();
	int ecuDown();
	int servo();

	// Saved values from SD card
	int gear();
	void gear(int value);

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

	bool autoLaunch();
	void autoLaunch(bool value);

  private:
	const size_t JSON_SIZE = 256;
	std::string filePath;

	int _up;
	int _down;
	int _clutchLeft;
	int _clutchRight;
	int _IA;
	int _IB;
	int _ecuUp;
	int _ecuDown;
	int _servo;
	
	int _gear = 0;
	int _upDelay = 50;
	int _downDelay = 200;
	int _output = 50;
	int _timeout = 300;
	int _start = 1600;
	int _end = 1400;
	int _friction = 1500;
	bool _autoLaunch = false;
};

#endif
