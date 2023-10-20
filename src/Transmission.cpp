#include "Transmission.h"

using namespace TeensyTimerTool;

OneShotTimer outputEnable(TCK);
OneShotTimer outputDisable(GPT1);
OneShotTimer timeoutTimer(GPT2);

unsigned long startTime = 0;

void Transmission::broadcast_gear(unsigned long frequency) {
  static unsigned long lastBroadastTime = 0;
  if (millis() - lastBroadastTime >= frequency) {
    lastBroadastTime = millis();

    CAN_message_t msg;
    msg.id = 1620;
    canutil::construct_data(msg, gear(), 0, 1);
    can.write(msg);
  }
}

void Transmission::shift(int direction) {
  // if(rpm() < 2000) {
  //   gear(0);
  //   return;
  // }

  if(timeout) return;

  startTime = millis();
  set_timeout(TIMEOUT_DURATION);
  
  if(direction == UP && gear() == 0) {
    gear(2);
  } else if(direction == UP && gear() < 6) {
    gear(gear()+1);
  } else if(direction == DOWN && gear() == 0) {
    gear(1);
  } else if(direction == DOWN && gear() > 1) {
    gear(gear()-1);
  }

  disable_combustion();
  power_solenoid(direction);
  broadcast_gear();
}

void Transmission::set_timeout(int duration) {
    timeout = true;
    timeoutTimer.begin([this] {
      timeout = false;
      Serial.println("TIMEOUT: " + String(millis() - startTime));
    });
    timeoutTimer.trigger((duration) * 1000);
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
  int duration = DELAY_DURATION + OUTPUT_DURATION;

  int outputPin = OUTPUT_PINS[direction];

  // Create an interrupt timer to enable the solenoid
  outputEnable.begin([outputPin] {
    digitalWrite(outputPin, HIGH); digitalWrite(13, HIGH);
    Serial.println("ENABLE: " + String(millis() - startTime));
  });
  outputEnable.trigger(DELAY_DURATION * 1000);

  // Create an interrupt timer to enable the solenoid
  outputDisable.begin([outputPin] {
    Serial.println("DISABLE: " + String(millis() - startTime));
    digitalWrite(outputPin, LOW); digitalWrite(13, LOW);
  });
  outputDisable.trigger(duration * 1000);

  Serial.println();
}