#ifndef CONTROLS_H
#define CONTROLS_H

#include <Arduino.h>
#include "Transmission.h"

enum {
  NEUTRAL = -1,
  UP = 0,
  DOWN = 1
};

class Controls {
  public:
    Controls() = delete;
    Controls(int pin);
    void update();
    bool pressed();

  private:
    const unsigned int DEBOUNCE_TIME = 5; // Debounce delay in milliseconds

    int buttonPin;
    int buttonState = HIGH;
    int lastButtonState = HIGH;
    unsigned long lastDebounceTime = 0;

    bool buttonPressed = false;
    bool buttonReleased = false;
    bool buttonHeld = false;
};

#endif