#ifndef DISPLAY_H
#define DISPLAY_H

#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>
#include "Storage/Storage.h"

class Display {
  public:
	Display() : oled(128, 64) { }
	void begin();
	void printInfo(int analogInput, bool print);
	void sdError();

  private:
  	Adafruit_SSD1306 oled;
	unsigned long lastDisplayTime = 0;
	unsigned long startTime = micros();
	int loops = 0;
};

#endif
