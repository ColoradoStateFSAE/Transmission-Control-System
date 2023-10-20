#ifndef BUTTON_H
#define BUTTON_H

#include <Arduino.h>
#include "Transmission.h"

class Button {
  public:
    Button() = delete;
    Button(int pin, int debounce=5);
    void update();
    bool pressed();

  private:
    unsigned int debounceDelay;

    int buttonPin;
    int buttonState = HIGH;
    int lastButtonState = HIGH;
    unsigned long lastDebounceTime = 0;

    bool buttonPressed = false;
};

#endif