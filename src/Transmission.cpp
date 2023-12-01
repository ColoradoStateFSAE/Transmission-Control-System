#include "Transmission.h"

using namespace TeensyTimerTool;

OneShotTimer outputEnable(GPT1);
OneShotTimer outputDisable(GPT2);

unsigned long startTime = 0;

void Transmission::broadcast_gear(unsigned long frequency) {
  static unsigned long lastBroadastTime = 0;
  if (millis() - lastBroadastTime >= frequency) {
    lastBroadastTime = millis();

    CAN_message_t msg;
    msg.id = 1620;
    canutil::construct_data(msg, getGear(), 0, 1);
    canutil::construct_data(msg, getDelay(), 2, 2);
    canutil::construct_data(msg, getOutput(), 4, 2);
    canutil::construct_data(msg, getTimeout(), 6, 2);
    can.write(msg);
  }
}

void Transmission::shift(int direction) {
  static unsigned long lastShift = 0;
  if((millis() - lastShift) < getTimeout()) {
    Serial.println("Attempted to shift before timeout: " + String(millis() - lastShift)); 
    return;
  }
  lastShift = millis();

  startTime = millis();

  if(rpm() == 0) {
    setGear(0);
  } else if(direction == UP && getGear() == 0) {
    setGear(2);
  } else if(direction == UP && getGear() < 6) {
    setGear(getGear()+1);
  } else if(direction == DOWN && getGear() == 0) {
    setGear(1);
  } else if(direction == DOWN && getGear() > 1) {
    setGear(getGear()-1);
  }

  disable_combustion();
  power_solenoid(direction);
  broadcast_gear();
}

void Transmission::disable_combustion() {
  CAN_message_t msg;
  msg.id = 522;
  canutil::construct_data(msg, 0b00000001, 0, 1);
  can.write(msg);
  canutil::construct_data(msg, 0b00000000, 0, 1);
  can.write(msg);
}

void Transmission::power_solenoid(int direction) {
  int outputPin = OUTPUT_PINS[direction];

  // Create an interrupt timer to enable the solenoid
  outputEnable.begin([this, outputPin] {
    if(rpm() >= 0) { digitalWrite(outputPin, HIGH); digitalWrite(13, HIGH); }
    Serial.println("ENABLE: " + String(millis() - startTime));
  });
  outputEnable.trigger(getDelay() * 1000);

  // Create an interrupt timer to enable the solenoid
  outputDisable.begin([this, outputPin] {
    if(rpm() >= 0) { digitalWrite(outputPin, LOW); digitalWrite(13, LOW); }
    Serial.println("DISABLE: " + String(millis() - startTime));
  });
  outputDisable.trigger((getDelay() + getOutput()) * 1000);

  Serial.println();
}
