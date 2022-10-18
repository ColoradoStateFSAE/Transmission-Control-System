#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>
#include "transmission.h"
#include "controls.h"

#define OLED_WIDTH 128
#define OLED_HEIGHT 64
#define OLED_ADDR 0x3C

// Create display
Adafruit_SSD1306 oled(OLED_WIDTH, OLED_HEIGHT);

Transmission transmission; // Instantiante Transmission object with engine object
Controls controls; // Instantiate Controls object

void setup() {
  Serial.begin(115200);
  controls.setup_controls();
  
  oled.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR); // Power on the display
  
  display(transmission.get_gear()); // Display the current gear
}

void loop() {
  if(controls.button_pressed(controls.NEUTRAL)) {
    transmission.shift(transmission.NEUTRAL);
  } else if (controls.button_pressed(controls.UP)) {
    transmission.shift(transmission.UP);
  } else if (controls.button_pressed(controls.DOWN)) {
    transmission.shift(transmission.DOWN);
  }

  display(transmission.get_gear());

  transmission.async(); // Required for pulse output to not interrupt the program
}

void display(int gear) {
  oled.clearDisplay();
  oled.setTextSize(4);
  oled.setTextColor(WHITE);
  oled.setCursor(0, 0);

  oled.println(gear);

  oled.display();
}
