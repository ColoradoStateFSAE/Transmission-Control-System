#if !defined(STORAGE_TEST_H) && defined(NATIVE)
#define STORAGE_TEST_H

#include <gtest/gtest.h>
#include <Arduino.h>
#include "../lib/Storage/Storage.h"

using namespace fakeit;

class StorageTest : public ::testing::Test {
  protected:
    Storage storage;

    void SetUp() override {
        When(Method(ArduinoFake(EEPROM), EEPROMFake::read)).AlwaysReturn(0xFF);
        When(Method(ArduinoFake(EEPROM), EEPROMFake::update)).AlwaysReturn();
    }

    void TearDown() override {
        ArduinoFakeReset();
    }
};

TEST_F(StorageTest, defaults) {
    // Test that the defaults can be set
    storage.begin();
    
    // Test that every address is read
    for(int i=0; i<=15; i++) {
        Verify(Method(ArduinoFake(EEPROM), EEPROMFake::read).Using(i)).Once();
    }

    // UP_DELAY
    Verify(Method(ArduinoFake(EEPROM), EEPROMFake::update).Using(0, 0x00)).Once();
    Verify(Method(ArduinoFake(EEPROM), EEPROMFake::update).Using(1, 0x32)).Once();

    // DOWN_DELAY_ADDRESS
    Verify(Method(ArduinoFake(EEPROM), EEPROMFake::update).Using(2, 0x00)).Once();
    Verify(Method(ArduinoFake(EEPROM), EEPROMFake::update).Using(3, 0xC8)).Once();

    // OUTPUT_ADDRESS
    Verify(Method(ArduinoFake(EEPROM), EEPROMFake::update).Using(4, 0x00)).Once();
    Verify(Method(ArduinoFake(EEPROM), EEPROMFake::update).Using(5, 0x5A)).Once();

    // TIMEOUT_ADDRESS
    Verify(Method(ArduinoFake(EEPROM), EEPROMFake::update).Using(6, 0x01)).Once();
    Verify(Method(ArduinoFake(EEPROM), EEPROMFake::update).Using(7, 0x2C)).Once();

    // START_ADDRESS
    Verify(Method(ArduinoFake(EEPROM), EEPROMFake::update).Using(8, 0x06)).Once();
    Verify(Method(ArduinoFake(EEPROM), EEPROMFake::update).Using(9, 0x40)).Once();

    // END_ADDRESS
    Verify(Method(ArduinoFake(EEPROM), EEPROMFake::update).Using(10, 0x05)).Once();
    Verify(Method(ArduinoFake(EEPROM), EEPROMFake::update).Using(11, 0x14)).Once();

    // FRICTION_ADDRESS
    Verify(Method(ArduinoFake(EEPROM), EEPROMFake::update).Using(12, 0x05)).Once();
    Verify(Method(ArduinoFake(EEPROM), EEPROMFake::update).Using(13, 0xDC)).Once();

    // AUTO_LAUNCH_ADDRESS
    Verify(Method(ArduinoFake(EEPROM), EEPROMFake::update).Using(14, 0x00)).Once();
    Verify(Method(ArduinoFake(EEPROM), EEPROMFake::update).Using(15, 0x00)).Once();
}

TEST_F(StorageTest, setDefaults) {
    int c = 0;
    for(int i=0; i<=14; i+=2) {
        c += 1;
        When(Method(ArduinoFake(EEPROM), EEPROMFake::read).Using(i)).AlwaysReturn(c*0x10);
        When(Method(ArduinoFake(EEPROM), EEPROMFake::read).Using(i+1)).AlwaysReturn(c*0x01);
    }

    storage.begin();
    Verify(Method(ArduinoFake(EEPROM), EEPROMFake::write)).Exactly(0);

    EXPECT_EQ(storage.upDelay(), 0x1001);
    EXPECT_EQ(storage.downDelay(), 0x2002);
    EXPECT_EQ(storage.output(), 0x3003);
    EXPECT_EQ(storage.timeout(), 0x4004);
    EXPECT_EQ(storage.start(), 0x5005);
    EXPECT_EQ(storage.end(), 0x6006);
    EXPECT_EQ(storage.friction(), 0x7007);
    EXPECT_EQ(storage.autoLaunch(), true);
}

TEST_F(StorageTest, setValue) {
    storage.begin();
    ArduinoFake(EEPROM).ClearInvocationHistory();

    // UP_DELAY
    ASSERT_TRUE(storage.setUpDelay(0x01F4));
    ASSERT_FALSE(storage.setUpDelay(501));
    ASSERT_FALSE(storage.setUpDelay(-1));
    Verify(Method(ArduinoFake(EEPROM), EEPROMFake::update).Using(0, 0x01)).Once();
    Verify(Method(ArduinoFake(EEPROM), EEPROMFake::update).Using(1, 0xF4)).Once();

    // DOWN_DELAY_ADDRESS
    ASSERT_TRUE(storage.setDownDelay(0x01F4));
    ASSERT_FALSE(storage.setDownDelay(501));
    ASSERT_FALSE(storage.setDownDelay(-1));
    Verify(Method(ArduinoFake(EEPROM), EEPROMFake::update).Using(2, 0x01)).Once();
    Verify(Method(ArduinoFake(EEPROM), EEPROMFake::update).Using(3, 0xF4)).Once();

    // OUTPUT_ADDRESS
    ASSERT_TRUE(storage.setOutput(0x0096));
    ASSERT_FALSE(storage.setOutput(151));
    ASSERT_FALSE(storage.setOutput(-1));
    Verify(Method(ArduinoFake(EEPROM), EEPROMFake::update).Using(4, 0x00)).Once();
    Verify(Method(ArduinoFake(EEPROM), EEPROMFake::update).Using(5, 0x96)).Once();

    // TIMEOUT_ADDRESS
    ASSERT_TRUE(storage.setTimeout(0x01F4));
    ASSERT_FALSE(storage.setTimeout(501));
    ASSERT_FALSE(storage.setTimeout(-1));
    Verify(Method(ArduinoFake(EEPROM), EEPROMFake::update).Using(6, 0x01)).Once();
    Verify(Method(ArduinoFake(EEPROM), EEPROMFake::update).Using(7, 0xF4)).Once();

    // START_ADDRESS
    ASSERT_TRUE(storage.setStart(0x0960));
    ASSERT_FALSE(storage.setStart(2401));
    ASSERT_FALSE(storage.setStart(-1));
    Verify(Method(ArduinoFake(EEPROM), EEPROMFake::update).Using(8, 0x09)).Once();
    Verify(Method(ArduinoFake(EEPROM), EEPROMFake::update).Using(9, 0x60)).Once();

    // END_ADDRESS
    ASSERT_TRUE(storage.setEnd(0x0960));
    ASSERT_FALSE(storage.setEnd(2401));
    ASSERT_FALSE(storage.setEnd(-1));
    Verify(Method(ArduinoFake(EEPROM), EEPROMFake::update).Using(10, 0x09)).Once();
    Verify(Method(ArduinoFake(EEPROM), EEPROMFake::update).Using(11, 0x60)).Once();

    // FRICTION_ADDRESS
    ASSERT_TRUE(storage.setFriction(0x0960));
    ASSERT_FALSE(storage.setFriction(2401));
    ASSERT_FALSE(storage.setFriction(-1));
    Verify(Method(ArduinoFake(EEPROM), EEPROMFake::update).Using(12, 0x09)).Once();
    Verify(Method(ArduinoFake(EEPROM), EEPROMFake::update).Using(13, 0x60)).Once();

    // AUTO_LAUNCH_ADDRESS
    ASSERT_TRUE(storage.setAutoLaunch(0x0001));
    Verify(Method(ArduinoFake(EEPROM), EEPROMFake::update).Using(14, 0x00)).Once();
    Verify(Method(ArduinoFake(EEPROM), EEPROMFake::update).Using(15, 0x01)).Once();
}

#endif
