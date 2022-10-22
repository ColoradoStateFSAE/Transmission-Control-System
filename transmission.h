#ifndef TRANSMISSION_H
#define TRANSMISSION_H

#include <Arduino.h>
#include <AsyncTimer.h>
#include "engine.h"

using namespace std;

class Transmission {
    public:
      int NEUTRAL = 0;
      int UP = 1;
      int DOWN = 2;

      Transmission(Engine &engine_object) : engine(engine_object) {}

      int get_gear();

      void async();

      void shift(int);

    private:
      Engine &engine;

      const int STALL_ENGINE_RPM[6] = {-1, -1, -1, -1, -1, -1}; // Table of values for each gear
      const int BLOW_UP_ENGINE_RPM[6] = {12000, 12000, 12000, 12000, 12000, 12000}; // Table of values for each gear

      const int NEUTRAL_DURATION = 25; // Shift duration in milliseconds
      const int UP_DURATION = 1000;
      const int DOWN_DURATION = 25;

      int gear = 0;
      
      bool safe_to_shift(int);

      void actuate(int, int);
};

#endif