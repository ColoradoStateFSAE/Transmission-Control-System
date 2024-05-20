#include "Button.h"

void Button::begin(int pin, int debounce) {
	_pin = pin;
	_debounce = debounce;
	pinMode(_pin, INPUT_PULLUP);
}

void Button::update() {
	bool pressed = false;

	int state = digitalRead(_pin);

	if (state != lastButtonState) {
		lastDebounceTime = micros();
	}

	if ((micros() - lastDebounceTime) >= _debounce) {
		if (state != buttonState) {
			buttonState = state;
			if (buttonState == LOW) {
				pressed = true;
			}
		}
	}

	lastButtonState = state;
	buttonPressed = pressed;
}

bool Button::pressed() {
	return buttonPressed;
}
