#ifndef BUTTON_H
#define BUTTON_H

#include <Arduino.h>

class Button {
  public:
	void begin(int pin, int debounce=500);
	void update();
	bool pressed();

  private:
	int _pin;
	unsigned int _debounce;
	
	int buttonState = HIGH;
	int lastButtonState = HIGH;
	unsigned long lastDebounceTime = 0;
	bool buttonPressed = false;
};

#endif
