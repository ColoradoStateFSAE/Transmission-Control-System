#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include "engine.h"
#include "neopixels.h"

Adafruit_NeoPixel pixels(16, 32, NEO_GRB + NEO_KHZ800);

// Public

void NeoPixels::setup_neopixels() {
  pixels.begin();
}

void NeoPixels::display_rpm() {
  float rpm = engine.get_rpm();
  float percentage = (rpm - rpmRange[0]) / (rpmRange[1] - rpmRange[0]);
  int activePixels = ceil(percentage * 6);

  for (int i = 0; i < activePixels; i++) {
    if (i < 3) {
      pixels.setPixelColor(i, pixels.Color(0,255,0));
      pixels.setPixelColor(15-i, pixels.Color(0,255,0));
    } else if (i < 6) {
      pixels.setPixelColor(i, pixels.Color(255,0,0));
      pixels.setPixelColor(15-i, pixels.Color(255,0,0));
    } else if (i < 8) {
      pixels.setPixelColor(i, pixels.Color(0,0,255));
      pixels.setPixelColor(15-i, pixels.Color(0,0,255));
      pixels.setPixelColor(i+1, pixels.Color(0,0,255));
      pixels.setPixelColor(14-i, pixels.Color(0,0,255));
    }

  }

  pixels.show();
  
  int currentTime = millis();

  if(rpm >= rpmRange[2]) {
      pixels.setBrightness(255);
      pixels.show();
    if (currentTime - lastTime > 120) {
      pixels.setBrightness(0);
      pixels.show();
      lastTime = currentTime;
    }
  } else {
    pixels.setBrightness(255);
    pixels.show();
    pixels.clear();
  }
}