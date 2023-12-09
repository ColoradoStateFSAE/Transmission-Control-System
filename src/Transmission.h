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

      // for (int i = 0; i < EEPROM.length(); i++) EEPROM.write(i, 0xFF);

      uint16_t saved;
      EEPROM.get(GEAR_ADDRESS, saved); if(saved == 0xFFFF) setGear(0);
      EEPROM.get(DELAY_ADDRESS, saved); if(saved == 0xFFFF) setDelay(50);
      EEPROM.get(OUTPUT_ADDRESS, saved); if(saved == 0xFFFF) setOutput(70);
      EEPROM.get(TIMEOUT_ADDRESS, saved); if(saved == 0xFFFF) setTimeout(200);
    }

    void setGear(uint16_t value) { EEPROM.put(GEAR_ADDRESS, value); }
    uint16_t getGear() { uint16_t saved; EEPROM.get(GEAR_ADDRESS, saved); return saved; }

    void setDelay(uint16_t value) { EEPROM.put(DELAY_ADDRESS, value); }
    uint16_t getDelay() { uint16_t saved; EEPROM.get(DELAY_ADDRESS, saved); return saved; }

    void setOutput(uint16_t value) { EEPROM.put(OUTPUT_ADDRESS, value); }
    uint16_t getOutput() { uint16_t saved; EEPROM.get(OUTPUT_ADDRESS, saved); return saved; }
    
    void setTimeout(uint16_t value) { EEPROM.put(TIMEOUT_ADDRESS, value); }
    uint16_t getTimeout() { uint16_t saved; EEPROM.get(TIMEOUT_ADDRESS, saved); return saved; }

    void rpm(int value) { rpmValue = value; }
    int rpm() { return rpmValue; }
    
    void broadcast_gear(unsigned long frequency=0);
    void shift(int direction);

  private:
    const int OUTPUT_PINS[2] = {41, 40}; // {up, down}

    const int GEAR_ADDRESS = 0;
    const int DELAY_ADDRESS = 2;
    const int OUTPUT_ADDRESS = 4;
    const int TIMEOUT_ADDRESS = 6;

    volatile bool timeout = false;

    FlexCAN_T4<CAN3, RX_SIZE_16, TX_SIZE_16> can;

    int rpmValue = 0;

    void disable_combustion();
    void power_solenoid(int direction);
};

#endif