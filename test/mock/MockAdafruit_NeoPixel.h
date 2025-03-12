#if !defined(MOCKADAFRUIT_NEOPIXEL_H) && defined(NATIVE)
#define MOCKADAFRUIT_NEOPIXEL_H

#include <Arduino.h>

class Adafruit_NeoPixel {
  public:
    virtual void begin() = 0;
    virtual void setPixelColor(uint16_t n, uint32_t c) = 0;
    virtual void show() = 0;
    static uint32_t Color(uint8_t r, uint8_t g, uint8_t b) {
        return ((uint32_t)r << 16) | ((uint32_t)g << 8) | b;
    }
};

#endif
