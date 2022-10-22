#ifndef NEOPIXELS_H
#define NEOPIXELS_H

#include <Arduino.h>

using namespace std;

class NeoPixels {
  public:
    NeoPixels(Engine &engine_object) : engine(engine_object) {}

    void setup_neopixels();

    void display_rpm();

  private:
    Engine &engine;

    const int PIN = 33;
    const int NUMPIXELS = 16;

    int rpmRange[3] = {5000, 8000, 8500};

    int lastTime = 0;
};

#endif