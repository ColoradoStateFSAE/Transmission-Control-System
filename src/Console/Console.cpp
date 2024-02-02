#include "Console.h"

using namespace std;

void Console::begin(int baud) {
	Serial.begin(baud);
	tempmon_init();
	tempmon_Start();
}

void Console::update(unsigned long time) {
	runningAverage.add(time);
}

void Console::pause() {
	lastPauseTime = millis();
}

void Console::clear() {
	Serial.write(27);
	Serial.print("[2J");
	Serial.write(27);
	Serial.print("[H");
}

void Console::print(string s) {
	Serial.println(s.c_str());
}

void Console::printSdError() {
	clear();
	Serial.println("SD card error");
	delay(2000);
}

void Console::printInfo(float input, Storage &storage) {
	if(5000 > millis() - lastPauseTime) {
		return;
	}

	unsigned long delayTime = 250;
	if(Serial.available() > 0) {
		delayTime = 2000;
	}

	String tempColor = [&]() -> String {
		float temperature = tempmonGetTemp();
		if(temperature >= 95) {
			return RED;
		} else if(temperature >= 85) {
			return YELLOW;
		} else {
			return GREEN;
		}
	}();

	if(millis() - lastPrintTime > delayTime) {
		clear();
		Serial.println("CPU TEMP: " + String(tempColor) + String(tempmonGetTemp()) + String(" °C") + RESET);
		Serial.println("LOOP TIME");
		Serial.print("AVERAGE: " + String(runningAverage.getFastAverage()) + " μs    ");
		Serial.print("HIGH: " + String(runningAverage.getMax()) + " μs    ");
		Serial.print("LOW: " + String(runningAverage.getMin()) + " μs    ");
		Serial.print("STD DEV: " + String(runningAverage.getStandardDeviation()) + " μs");
		Serial.println();
		Serial.println("ANALOG INPUT: " + String(input) + " %");
		Serial.println();
		Serial.println("UP DELAY: " + String(storage.upDelay()));
		Serial.println("DOWN DELAY: " + String(storage.upDelay()));
		Serial.println("OUTPUT: " + String(storage.upDelay()));
		Serial.println();
		Serial.println("START: " + String(storage.start()));
		Serial.println("END: " + String(storage.end()));
		Serial.println("FRICTION: " + String(storage.friction()));

		lastPrintTime = millis();
	}
}

void Console::print(int number) {
	string s = to_string(number);
	print(s);
}

void Console::print(float number) {
	string s = to_string(number);
	print(s);
}

void Console::print(double number) {
	string s = to_string(number);
	print(s);
}
