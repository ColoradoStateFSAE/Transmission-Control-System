#include "Button.h"

// button_pressed takes a button and returns true if it was pressed
void Button::update() {
  bool pressed = false;

  int state = digitalRead(buttonPin);

  if (state != lastButtonState) {
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) >= debounceDelay) {
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