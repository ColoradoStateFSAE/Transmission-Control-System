#include "Neutral.h"

Neutral::Neutral(int pin, int debounce){
    neutralPin = pin;
    debounceDelay = debounce;
    pinMode(neutralPin, INPUT_PULLDOWN);
}

void Neutral::update(){
    bool engaged = false;

    int state = digitalRead(neutralPin);

    if(state != lastNeutralState){
        lastDebounceTime = millis();
    }

    if ((millis()-lastDebounceTime)>=debounceDelay){
        if (state != neutralState){
            neutralState = state;
            if (neutralState == HIGH){
                engaged = true;
            }
        }
    }
    lastNeutralState = state;

    inNeutral = engaged;

}

bool Neutral::engaged() {
    return inNeutral;
}