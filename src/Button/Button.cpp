#include "Button.h"

Button::Button(int pin, int debounce) {
	buttonPin = pin;
	debounceDelay = debounce;
}

void Button::begin() {
	pinMode(buttonPin, INPUT_PULLUP);
}

void Button::update() {
	bool pressed = false;

	int state = digitalRead(buttonPin);

	if (state != lastButtonState) {
		lastDebounceTime = micros();
	}

	if ((micros() - lastDebounceTime) >= debounceDelay) {
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
