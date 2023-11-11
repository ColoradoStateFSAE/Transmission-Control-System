#ifndef CLUTCH_H
#define CLUTCH_H

#include <Arduino.h>
#include <EEPROM.h>
#include <Servo.h>
#include "canutil.h"

class Clutch {
  public:
    Clutch() = delete;
    Clutch(const FlexCAN_T4<CAN3, RX_SIZE_16, TX_SIZE_16> &canRef) : can(canRef) {
      uint16_t saved;
      EEPROM.get(START_ADDRESS, saved); if(saved == 0xFFFF) setStart(114);
      EEPROM.get(END_ADDRESS, saved); if(saved == 0xFFFF) setEnd(65);
      EEPROM.get(FRICTION_ADDRESS, saved); if(saved == 0xFFFF) setFriction(60);

      servo.attach(0);
    }

    void setStart(uint16_t value) { EEPROM.put(START_ADDRESS, value); }
    uint16_t getStart() { uint16_t saved; EEPROM.get(START_ADDRESS, saved); return saved; }

    void setEnd(uint16_t value) { EEPROM.put(END_ADDRESS, value); }
    uint16_t getEnd() { uint16_t saved; EEPROM.get(END_ADDRESS, saved); return saved; }

    void setFriction(uint16_t value) { EEPROM.put(FRICTION_ADDRESS, value); }
    uint16_t getFriction() { uint16_t saved; EEPROM.get(FRICTION_ADDRESS, saved); return saved; }

    void write(int value) {
        servo.write(value);
    }

    void broadcast_values(unsigned long frequency) {
      static unsigned long lastBroadastTime = 0;
      if (millis() - lastBroadastTime >= frequency) {
        lastBroadastTime = millis();

        CAN_message_t msg;
        msg.id = 1622;
        canutil::construct_data(msg, getStart(), 0, 2);
        canutil::construct_data(msg, getEnd(), 2, 2);
        canutil::construct_data(msg, getFriction(), 4, 2);
        can.write(msg);
      }
    }

  private:
    const int START_ADDRESS = 8;
    const int END_ADDRESS = 10;
    const int FRICTION_ADDRESS = 12;

    FlexCAN_T4<CAN3> can;

    Servo servo;

    // void servoPosition(int value) {
//   int inflectionPoint = 200;
//   int inflectionAngle = 90;
  
//   if(value <= inflectionPoint) {
//     value = map(value, 0, inflectionPoint, 0, inflectionAngle);
//   } else {
//     value = map(value, inflectionPoint+1, 1023, inflectionAngle, 180);
//   }
  
//   servo.write(value);
// }
};

#endif