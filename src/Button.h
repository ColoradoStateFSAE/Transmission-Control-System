#ifndef BUTTON_H
#define BUTTON_H

#include <Arduino.h>
#include "Transmission.h"

class Button {
  public:
    Button() = delete;
    Button(int pin, int debounce=5) : buttonPin(pin), debounceDelay(debounce) {
      pinMode(buttonPin, INPUT_PULLUP);
    }
    void update();
    bool pressed();

  private:
    const int buttonPin;
    const unsigned int debounceDelay;

    int buttonState = HIGH;
    int lastButtonState = HIGH;
    unsigned long lastDebounceTime = 0;

    bool buttonPressed = false;
};

#endif