#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>
#include <EEPROM.h>

#include <FlexCAN_T4.h>
#include "Transmission.h"
#include "Clutch.h"
#include "Button.h"

FlexCAN_T4<CAN3, RX_SIZE_16, TX_SIZE_16> can;
Transmission transmission(can);
Clutch clutch(can);
Adafruit_SSD1306 oled(128, 64);

void printValues();
void display();

void setup() {
  Serial.begin(115200);

  can.begin();
  can.setBaudRate(500000);
  can.enableFIFO(true);
  can.setFIFOFilter(REJECT_ALL);
  can.setFIFOFilter(0, 1520, STD);
  can.setFIFOFilter(1, 1572, STD);
  can.setFIFOFilterRange(2, 1620, 1640, STD);

  oled.begin(SSD1306_SWITCHCAPVCC, 0x3C);
}

Button up(34);
Button down(35);

int i = 0;

void loop() {
  static unsigned long lastCanUpdate = 0;

  CAN_message_t msg;
  if(can.readFIFO(msg)) {
    if(msg.id == 1520) {
      transmission.rpm(canutil::read_data(msg, 6, 2));
      lastCanUpdate = millis();
    } else if(msg.id == 1621) {
      transmission.setDelay(canutil::read_data(msg, 2, 2));
      transmission.setOutput(canutil::read_data(msg, 4, 2));
      transmission.setTimeout(canutil::read_data(msg, 6, 2));
      printValues();
    } else if(msg.id == 1623) {
      clutch.setStart(canutil::read_data(msg, 0, 2));
      clutch.setEnd(canutil::read_data(msg, 2, 2));
      clutch.setFriction(canutil::read_data(msg, 4, 2));
      printValues();
    } else if(msg.id == 1624) { // 0x657
      clutch.write(canutil::read_data(msg, 0, 2));
    }
  }
  
  if(millis() - lastCanUpdate >= 100) {
    transmission.rpm(0);
  }

  up.update();
  down.update();

  if(up.pressed()) {
    transmission.shift(UP);
  } else if(down.pressed()) {
    transmission.shift(DOWN);
  }

  transmission.broadcast_gear(100);

  //display();
}

void printValues() {
  //Serial.println("SHIFT");
  Serial.println("DELAY: " + String(transmission.getDelay()));
  Serial.println("OUTPUT: " + String(transmission.getOutput()));
  Serial.println("TIMEOUT: " + String(transmission.getTimeout()));
  //Serial.println("CLUTCH");
  Serial.println("CLUTCH START: " + String(clutch.getStart()));
  Serial.println("CLUTCH END: " + String(clutch.getEnd()));
  Serial.println("FRICTION POINT: " + String(clutch.getFriction()));
  Serial.println("");
}

void display() {
  static long lastDisplayTime = 0;
  if(millis() - lastDisplayTime >= 500) {
    lastDisplayTime = millis();
    oled.clearDisplay();
    oled.setCursor(0, 0);
    oled.setTextColor(WHITE);
    oled.setTextSize(2);
    oled.println(transmission.getGear());
    oled.println(transmission.rpm());
    oled.setTextSize(0);
    oled.println(millis()/1000.0, 1);
    oled.display();
  }
}