#include <Arduino.h>
#include "engine.h"

// Public
int Engine::get_rpm() {
  int engineRpm = analogRead(A0) * 8.79765395894;
  return engineRpm;
}