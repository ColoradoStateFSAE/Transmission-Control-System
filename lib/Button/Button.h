#ifndef BUTTON_H
#define BUTTON_H

#include <Arduino.h>

class Button {
  public:
    virtual void begin(int pin, int debounce=5000);
    virtual int pin();
    virtual unsigned long debounce();
    virtual void update();
    virtual bool pressed();

  private:
    int _pin;
    unsigned long _debounce;
    
    bool buttonState = HIGH;
    bool lastButtonState = HIGH;
    bool buttonPressed = false;
    unsigned long lastDebounceTime = 0;
};

#endif
