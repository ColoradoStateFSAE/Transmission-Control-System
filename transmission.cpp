#include <AsyncTimer.h>
#include "Arduino.h"
#include "transmission.h"
#include "engine.h"

namespace {
  AsyncTimer shift_duration; // Instantiate AsyncTimer object for shift pin state change
}

// Public

// get_gear returns the transmission's current gear
int Transmission::get_gear() {
  return gear;
}

// This function shifts the transmission in the inputted direction if safe_to_shift returns true
void Transmission::shift(int direction) {
  if(direction == NEUTRAL) {
    // Shift to neutral
    if(safe_to_shift(NEUTRAL)) {
      gear = 0;
      actuate(UP, NEUTRAL_DURATION);
    }
  } else if(direction == UP) {
    // Shift up one gear
    if(safe_to_shift(UP)) {
      gear++;
      actuate(UP, UP_DURATION);
    }
  } else if(direction == DOWN) {
    // Shift down one gear
    if(safe_to_shift(DOWN)) {
      gear--;
      actuate(DOWN, DOWN_DURATION);
    }
  }
}

// async updates the shift_duration object
void Transmission::async() {
  shift_duration.handle();
}

// Private

// safe_to_shift takes a shift direction (NEUTRAL, UP, DOWN) and returns true if requirements for shifting are met
bool Transmission::safe_to_shift(int direction) {
  if(direction == 0 && (gear == 1 || gear == 2)) {
    // Allowed to shift to neutral if the current gear is either 1 or 2
    return true;
  } else if (direction == 1 && gear < 5 && engine.get_rpm() > STALL_ENGINE_RPM[gear]) {
    // Allowed to shift up if the transmission is not already in 5th gear and will not stall the engine
    return true;
  } else if (direction == 2 && gear >= 2 && engine.get_rpm() < BLOW_UP_ENGINE_RPM[gear]) {
    // Allowed to shift down if in second gear or higher and will not destroy the engine
    return true;
  } else {
    return false;
  }
}

// actuate takes a shift direction (NEUTRAL, UP, DOWN) and duration in milliseconds then sets pin _ to HIGH for the given duration
void Transmission::actuate(int direction, int duration) {
  // Starting pulse code goes here
  shift_duration.setTimeout([]() {
    // Ending pulse code goes here
  }, duration);
}