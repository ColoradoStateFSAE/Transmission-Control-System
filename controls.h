#ifndef CONTROLS_H
#define CONTROLS_H

#include <Arduino.h>

using namespace std;

class Controls {
  public:
    // All arrays are in the format [Neutral, Up, Down]
    const int NEUTRAL = 0;
    const int UP = 1;
    const int DOWN = 2;

    void setup_controls();

    bool button_pressed(int);

  private:
    const int PINS[3] = {33, 34, 35}; // Define input pins

    const int DEBOUNCE_TIME = 0; // Debounce delay in milliseconds

    int buttonState[3] = {LOW, LOW, LOW}; // The state of each button
    int lastState[3] = {LOW, LOW, LOW}; // The last state of each button, used to only run code once when the button is pressed

    int lastPressTime = 0; // The last time a button was pressed
};

#endif