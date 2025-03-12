#include <Arduino.h>
#include "../lib/Storage/Storage.h"
#include "../lib/AnalogInput/AnalogInput.h"
#include "../lib/Transmission/Transmission.h"
#include "../lib/Button/Button.h"
#include "../lib/Can/Can.h"

#ifdef NATIVE
    #include "../test/mock/MockAdafruit_NeoPixel.h"

    using namespace fakeit;

    Mock<Can> mockCan;
    Mock<Storage> mockStorage;
    Mock<Adafruit_NeoPixel> mockPixels;
    Mock<Transmission> mockTransmission;
    Mock<Button> mockUp;
    Mock<Button> mockDown;
    Mock<AnalogInput> mockClutchRight;

    Storage& storage = mockStorage.get();
    Adafruit_NeoPixel& pixels = mockPixels.get();
    Can& can = mockCan.get();
    Transmission& transmission = mockTransmission.get();
    Button& up = mockUp.get();
    Button& down = mockDown.get();
    AnalogInput& clutchRight = mockClutchRight.get();
#else
    #include <FlexCAN_T4.h>
    #include <Servo.h>
    #include <Adafruit_NeoPixel.h>

    Storage storage;
    FlexCAN_T4<CAN1, RX_SIZE_16, TX_SIZE_16> interface;
    Servo servo;
    Adafruit_NeoPixel pixels(1, 5, NEO_GRB + NEO_KHZ800);
    Transmission transmission(storage, servo);
    AnalogInput clutchRight(512);
    Can can(interface, storage, transmission, clutchRight);
    Button up;
    Button down;
#endif

void setup() {
    storage.begin();

    pixels.begin();
    pixels.setPixelColor(0, pixels.Color(0, 140, 0));
    pixels.show();

    can.begin();
    
    up.begin(storage.UP);
    down.begin(storage.DOWN);

    clutchRight.begin(storage.CLUTCH_RIGHT);
    clutchRight.minDeadzone(10);
    clutchRight.maxDeadzone(20);

    transmission.begin();
}

void loop() {
    // Read CAN
    can.update();

    // Handle input
    up.update();
    down.update();
    clutchRight.update();
    transmission.clutchInput(clutchRight.travel());

    if(up.pressed()) {
        transmission.shift(Transmission::Direction::UP);
    } else if(down.pressed()) {
        transmission.shift(Transmission::Direction::DOWN);
    }

    transmission.update();
    
    // Send data over CAN
    can.broadcastShiftSettings();
    can.broadcastClutchSettings();
    can.broadcastClutch();
    can.broadcastAnalogInput();
}
