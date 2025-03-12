#if !defined(MAIN_TEST_H) && defined(NATIVE)
#define MAIN_TEST_H

#include <gtest/gtest.h>
#include <Arduino.h>
#include "../lib/Storage/Storage.h"
#include "../lib/AnalogInput/AnalogInput.h"
#include "../lib/Transmission/Transmission.h"
#include "../lib/Button/Button.h"
#include "../lib/Can/Can.h"
#include "mock/MockFlexCAN_T4.h"
#include "mock/MockServo.h"
#include "mock/MockAdafruit_NeoPixel.h"

using namespace fakeit;

extern Mock<Can> mockCan;
extern Mock<Storage> mockStorage;
extern Mock<Adafruit_NeoPixel> mockPixels;
extern Mock<Transmission> mockTransmission;
extern Mock<Button> mockUp;
extern Mock<Button> mockDown;
extern Mock<AnalogInput> mockClutchRight;

extern void setup();
extern void loop();

class MainTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Storage
        When(Method(mockStorage, begin)).AlwaysReturn();

        When(Method(mockStorage, setUpDelay)).AlwaysReturn(true);
        When(Method(mockStorage, setDownDelay)).AlwaysReturn(true);
        When(Method(mockStorage, setOutput)).AlwaysReturn(true);
        When(Method(mockStorage, setTimeout)).AlwaysReturn(true);
        When(Method(mockStorage, setStart)).AlwaysReturn(true);
        When(Method(mockStorage, setEnd)).AlwaysReturn(true);
        When(Method(mockStorage, setFriction)).AlwaysReturn(true);
        When(Method(mockStorage, setAutoLaunch)).AlwaysReturn(true);
        
        When(Method(mockStorage, upDelay)).AlwaysReturn(50);
        When(Method(mockStorage, downDelay)).AlwaysReturn(200);
        When(Method(mockStorage, output)).AlwaysReturn(90);
        When(Method(mockStorage, timeout)).AlwaysReturn(50);
        When(Method(mockStorage, start)).AlwaysReturn(1600);
        When(Method(mockStorage, end)).AlwaysReturn(1300);
        When(Method(mockStorage, friction)).AlwaysReturn(1500);
        When(Method(mockStorage, autoLaunch)).AlwaysReturn(1);

        // NeoPixel
        When(Method(mockPixels, begin)).AlwaysReturn();
        When(Method(mockPixels, setPixelColor)).AlwaysReturn();
        When(Method(mockPixels, show)).AlwaysReturn();

        // Can
        When(Method(mockCan, begin)).AlwaysReturn();
        When(Method(mockCan, update)).AlwaysReturn();
        When(Method(mockCan, broadcastShiftSettings)).AlwaysReturn();
        When(Method(mockCan, broadcastClutchSettings)).AlwaysReturn();
        When(Method(mockCan, broadcastClutch)).AlwaysReturn();
        When(Method(mockCan, broadcastAnalogInput)).AlwaysReturn();

        // Up
        When(Method(mockUp, begin)).AlwaysReturn();
        When(Method(mockUp, update)).AlwaysReturn();
        When(Method(mockUp, pressed)).AlwaysReturn(false);

        // Down
        When(Method(mockDown, begin)).AlwaysReturn();
        When(Method(mockDown, update)).AlwaysReturn();
        When(Method(mockDown, pressed)).AlwaysReturn(false);

        // Right Clutch
        When(Method(mockClutchRight, begin)).AlwaysReturn();
        When(Method(mockClutchRight, minDeadzone)).AlwaysReturn();
        When(Method(mockClutchRight, maxDeadzone)).AlwaysReturn();
        When(Method(mockClutchRight, update)).AlwaysReturn();
        When(Method(mockClutchRight, travel)).AlwaysReturn(33.3);
        
        // Transmission
        When(Method(mockTransmission, begin)).AlwaysReturn();
        When(Method(mockTransmission, shift)).AlwaysReturn();
        When(Method(mockTransmission, clutchInput)).AlwaysReturn();
        When(Method(mockTransmission, update)).AlwaysReturn();
        When(Method(mockTransmission, setRpm)).AlwaysReturn();

        When(Method(ArduinoFake(), millis)).AlwaysReturn(0);

        setup();
    }

    void TearDown() override {
        ArduinoFakeReset();
        mockStorage.ClearInvocationHistory();
        mockCan.ClearInvocationHistory();
        mockPixels.ClearInvocationHistory();
        mockTransmission.ClearInvocationHistory();
        mockUp.ClearInvocationHistory();
        mockDown.ClearInvocationHistory();
        mockClutchRight.ClearInvocationHistory();
    }
};

TEST_F(MainTest, StorageBegin) {
    Verify(Method(mockStorage, begin)).Once();
}

TEST_F(MainTest, NeoPixel) {
    Verify(Method(mockPixels, begin)).Once();
    Verify(Method(mockPixels, setPixelColor).Using(0, Adafruit_NeoPixel::Color(0, 140, 0))).Once();
    Verify(Method(mockPixels, show)).Once();
}

TEST_F(MainTest, CanBegin) {
    Verify(Method(mockCan, begin)).Once();
}

TEST_F(MainTest, ButtonBegin) {
    Verify(Method(mockUp, begin).Using(6, 5000)).Once();
    Verify(Method(mockDown, begin).Using(7, 5000)).Once();
}

TEST_F(MainTest, AnalogInputBegin) {
    Verify(Method(mockClutchRight, begin).Using(17)).Once();
    Verify(Method(mockClutchRight, minDeadzone).Using(10)).Once();
    Verify(Method(mockClutchRight, maxDeadzone).Using(20)).Once();
}

TEST_F(MainTest, TransmissionBegin) {
    Verify(Method(mockTransmission, begin)).Once();
}

TEST_F(MainTest, ButtonsUpdate) {
    loop();
    Verify(Method(mockUp, update)).Once();
    Verify(Method(mockDown, update)).Once();
}

TEST_F(MainTest, ButtonsPressed) {
    When(Method(mockUp, pressed)).AlwaysReturn(true);
    loop();
    Verify(Method(mockTransmission, shift).Using(Transmission::Direction::UP)).Once();

    When(Method(mockUp, pressed)).AlwaysReturn(false);
    When(Method(mockDown, pressed)).AlwaysReturn(true);
    loop();
    Verify(Method(mockTransmission, shift).Using(Transmission::Direction::DOWN)).Once();
}

TEST_F(MainTest, TransmissionUpdate) {
    When(Method(mockClutchRight, travel)).AlwaysReturn(33.3f);
    loop();
    Verify(Method(mockTransmission, clutchInput).Using(33.3f)).Once();
    Verify(Method(mockTransmission, update)).Once();
}

TEST_F(MainTest, canLoop) {
    loop();
    
    Verify(Method(mockCan, update)).Once();
    Verify(Method(mockCan, broadcastShiftSettings)).Once();
    Verify(Method(mockCan, broadcastClutchSettings)).Once();
    Verify(Method(mockCan, broadcastClutch)).Once();
    Verify(Method(mockCan, broadcastAnalogInput)).Once();
}

#endif
