#ifndef CLUTCH_H
#define CLUTCH_H

#include <Arduino.h>
#include <EEPROM.h>
#include <Servo.h>
#include "canutil.h"

class Clutch {
  public:
      const int windowSize = 20;
      int readings[20];
      int readIndex = 0;
      int total = 0;
    
    Clutch() = delete;
    Clutch(const FlexCAN_T4<CAN3, RX_SIZE_16, TX_SIZE_16> &canRef) : can(canRef) {
      uint16_t saved;
      EEPROM.get(START_ADDRESS, saved); if(saved == 0xFFFF) setStart(112);
      EEPROM.get(END_ADDRESS, saved); if(saved == 0xFFFF) setEnd(60);
      EEPROM.get(FRICTION_ADDRESS, saved); if(saved == 0xFFFF) setFriction(60);

      servo.attach(0);
      servo.write(getStart());

      for (int i = 0; i < windowSize; i++) {
        readings[i] = 0;
      }
    }

    int window() {
        total = total - readings[readIndex];
        readings[readIndex] = analogRead(A0);
        total = total + readings[readIndex];
        readIndex = readIndex + 1;
        if (readIndex >= windowSize) {
            readIndex = 0;
        }

        return total / windowSize;
    }

    void setStart(uint16_t value) { EEPROM.put(START_ADDRESS, value); }
    uint16_t getStart() { uint16_t saved; EEPROM.get(START_ADDRESS, saved); return saved; }

    void setEnd(uint16_t value) { EEPROM.put(END_ADDRESS, value); }
    uint16_t getEnd() { uint16_t saved; EEPROM.get(END_ADDRESS, saved); return saved; }

    void setFriction(uint16_t value) { EEPROM.put(FRICTION_ADDRESS, value); }
    uint16_t getFriction() { uint16_t saved; EEPROM.get(FRICTION_ADDRESS, saved); return saved; }

    void write(int value) {
        if(value > getStart()) { value = getStart(); }
        if(value < getEnd()) value = getEnd();
        Serial.println("VALUE: " + String(value));
        Serial.println("END: " + String(getEnd()));
        Serial.println("START: " + String(getStart()) + "\n");

        servo.write(value);
    }

    void broadcast_values(unsigned long frequency) {
      static unsigned long lastBroadastTime = 0;
      if (millis() - lastBroadastTime >= frequency) {
        lastBroadastTime = millis();

        CAN_message_t msg;
        msg.id = 1621;
        canutil::construct_data(msg, getStart(), 0, 2);
        canutil::construct_data(msg, getEnd(), 2, 2);
        canutil::construct_data(msg, getFriction(), 4, 2);
        can.write(msg);
      }
    }

    int analogMin = 10000;
    int analogMax = 0;

    void position(int value) {
        value = window();
        int analog = analogRead(A0);
        if(analog > analogMax) { analogMax = analog; }
        if(analog < analogMin) { analogMin = analog; }
        int servoValue = map(value, analogMax, analogMin, getStart(), getEnd());
        // Serial.println("RAW" + String(value));
        // Serial.println("MOD: " +String(servoValue));
        // delay(200);
        
        if(servoValue > getStart()) servoValue = getStart();
        if(servoValue < getEnd()) servoValue = getEnd();
        servo.write(servoValue);
    }
    
  private:
    const int START_ADDRESS = 8;
    const int END_ADDRESS = 10;
    const int FRICTION_ADDRESS = 12;

    FlexCAN_T4<CAN3> can;

    Servo servo;
};

#endif