#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>
#include <EEPROM.h>
#include <Servo.h>
#include <FlexCAN_T4.h>
#include "Transmission.h"
#include "Button.h"

FlexCAN_T4<CAN3, RX_SIZE_16, TX_SIZE_16> can;
Transmission transmission(can);
Servo servo;
Adafruit_SSD1306 oled(128, 64);

void servoPosition(int value);
void display();

void setup() {
  Serial.begin(115200);

  can.begin();
  can.setBaudRate(500000);
  can.enableFIFO(true);
  can.setFIFOFilter(REJECT_ALL);
  can.setFIFOFilter(0, 1520, STD);
  can.setFIFOFilter(1, 1572, STD);
  can.setFIFOFilter(2, 0x655, STD);
  can.setFIFOFilter(3, 0x656, STD);

  servo.attach(9);

  oled.begin(SSD1306_SWITCHCAPVCC, 0x3C);
}

Button up(34);
Button down(35);

void loop() {
  static unsigned long lastCanUpdate = 0;

  CAN_message_t msg;
  if(can.readFIFO(msg)) {
    if(msg.id == 1520) {
      transmission.rpm(canutil::read_data(msg, 6, 2));
      lastCanUpdate = millis();
    }
  }
  
  if(millis() - lastCanUpdate >= 100) {
    transmission.rpm(0);
  }

  up.update();
  down.update();

  if(up.pressed()) {
    transmission.shift(UP);
    Serial.println("Up pressed");
  } else if(down.pressed()) {
    transmission.shift(DOWN);
    Serial.println("Down pressed");
  }

  transmission.broadcast_gear(100);

  servoPosition(analogRead(9));

  display();
}

void servoPosition(int value) {
  int inflectionPoint = 200;
  int inflectionAngle = 90;
  
  if(value <= inflectionPoint) {
    value = map(value, 0, inflectionPoint, 0, inflectionAngle);
  } else {
    value = map(value, inflectionPoint+1, 1023, inflectionAngle, 180);
  }
  
  servo.write(value);
}

void display() {
  static long lastDisplayTime = 0;
  if(millis() - lastDisplayTime >= 500) {
    lastDisplayTime = millis();
    oled.clearDisplay();
    oled.setCursor(0, 0);
    oled.setTextColor(WHITE);
    oled.setTextSize(2);
    oled.println(transmission.gear());
    oled.println(transmission.rpm());
    oled.setTextSize(0);
    oled.println(millis()/1000.0, 1);
    oled.display();
  }
}