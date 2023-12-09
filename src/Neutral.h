#ifndef NEUTRAL_H
#define NEUTRAL_H

#include <Arduino.h>
#include "Transmission.h"

class Neutral {
    public:
       Neutral() = delete;
       Neutral(int pin, int debounce=5);
       void update();
       bool engaged();

    private:
        unsigned int debounceDelay;

        int neutralPin;
        int neutralState = LOW;
        int lastNeutralState = LOW;
        unsigned long lastDebounceTime = 0;

        bool inNeutral = false;
};

#endif