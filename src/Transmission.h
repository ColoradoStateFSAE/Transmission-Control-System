#ifndef TRANSMISSION_H
#define TRANSMISSION_H

#include <Arduino.h>
#include <EEPROM.h>
#include <FlexCAN_T4.h>
#include <TeensyTimerTool.h>
#include "Button.h"
#include "canutil.h"

using namespace TeensyTimerTool;

enum {
  UP = 0,
  DOWN = 1
};

class Transmission {
  public:
    Transmission() = delete;
    Transmission(const FlexCAN_T4<CAN3, RX_SIZE_16, TX_SIZE_16> &canRef) : can(canRef) {
      pinMode(OUTPUT_PINS[0], OUTPUT);
      pinMode(OUTPUT_PINS[1], OUTPUT);
      pinMode(13, OUTPUT);

      if(EEPROM.read(GEAR_ADDRESS) == 0xFF) {
        EEPROM.write(GEAR_ADDRESS, 0);
      }
    }

    void gear(int value) { EEPROM.write(GEAR_ADDRESS, value); }
    int gear() { return EEPROM.read(GEAR_ADDRESS); }

    void rpm(int value) { rpmValue = value; }
    int rpm() { return rpmValue; }
    
    void broadcast_gear(unsigned long frequency=0);
    void shift(int direction);

  private:
    const int OUTPUT_PINS[2] = {41, 40}; // {up, down}
    const int DELAY_DURATION = 5;
    const int OUTPUT_DURATION = 70;
    const int TIMEOUT_DURATION = 200;

    const int GEAR_ADDRESS = 1;

    volatile bool timeout = false;

    FlexCAN_T4<CAN3, RX_SIZE_16, TX_SIZE_16> can;

    int rpmValue = 0;

    void set_timeout(int duration);
    void disable_combustion();
    void power_solenoid(int direction);
};

#endif