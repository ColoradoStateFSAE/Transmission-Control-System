#include <Arduino.h>
#include "controls.h"

// Public
// All arrays are in the format [Neutral, Up, Down]

void Controls::setup_controls() {
  pinMode(PINS[0], INPUT);
  pinMode(PINS[1], INPUT);
  pinMode(PINS[2], INPUT);
}

// button_pressed takes a button and returns true if it was pressed
bool Controls::button_pressed(int button) {
  buttonState[button] = digitalRead(PINS[button]); // Read the state of the button

  int currentTime = millis(); // Get the currest time
  int timeBetweenPresses = currentTime - lastPressTime;

  bool pressed = false; // Return variable

  if (timeBetweenPresses > DEBOUNCE_TIME) {
    // If a button was pressed, has a different state than the last time it was pressed, and
    // the time since the last press is greater than the debounce time, register a press.
    if (buttonState[button] == HIGH && buttonState[button] != lastState[button]) {
      pressed = true;
      lastPressTime = currentTime; // Set the last time a button was pressed to the current time
    } else {
      pressed = false;
    }

    lastState[button] = buttonState[button]; // Set the last state of the button to the current state
  }

  return pressed;
}