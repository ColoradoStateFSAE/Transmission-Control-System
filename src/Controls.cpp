#include "Controls.h"

Controls::Controls(int pin) {
  buttonPin = pin;
  pinMode(buttonPin, INPUT_PULLUP);
}

// button_pressed takes a button and returns true if it was pressed
void Controls::update() {
  bool pressed = false;
  bool released = false;

  int state = digitalRead(buttonPin);

  if (state != lastButtonState) {
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) >= DEBOUNCE_TIME) {
    if (state != buttonState) {
      buttonState = state;
      if (buttonState == LOW) {
        pressed = true;
      }
    }
  }

  lastButtonState = state;

  buttonPressed = pressed;
  buttonReleased = released;
}

bool Controls::pressed() {
  return buttonPressed;
}