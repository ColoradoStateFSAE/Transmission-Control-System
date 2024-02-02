#ifndef CONSOLE_H
#define CONSOLE_H

#include <Arduino.h>
#include <string>
#include <RunningAverage.h>
#include "Storage/Storage.h"

class Console {
  public:
	void begin(int baud);
	void update(unsigned long frequency);
	void clear();
	void pause();
	void print(std::string s);
	void print(int number);
	void print(float number);
	void print(double number);
	
	void printSdError();
	void printInfo(float input, Storage &storage);

	const String RED = "\033[0;31m";
	const String GREEN = "\033[0;32m";
	const String YELLOW = "\033[0;33m";
	const String BLUE = "\033[0;34m";
	const String MAGENTA = "\033[0;35m";
	const String CYAN = "\033[0;36m";
	const String RESET = "\033[0m";
	
  private:
	unsigned long lastPrintTime = 0;
	RunningAverage runningAverage = RunningAverage(500);
	unsigned long lastPauseTime = -5000;
};

#endif
