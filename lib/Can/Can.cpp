#include "Can.h"

using namespace std;

void Can::begin() {
    interface.begin();
    interface.setBaudRate(1000000);
}

void Can::handleGroup0(const CAN_message_t &msg) {
    r3_group0_t buf;
    r3_group0_unpack(&buf, msg.buf, sizeof(msg.buf));

    int rpm = r3_group0_rpm_decode(buf.rpm);
    
    transmission.setRpm(rpm);
}

void Can::handleShiftSettings(const CAN_message_t &msg) {
    tcs_set_shift_settings_t buf;
    tcs_set_shift_settings_unpack(&buf, msg.buf, sizeof(msg.buf));

    int upDelay = tcs_set_shift_settings_set_up_delay_decode(buf.set_up_delay);
    int downDelay = tcs_set_shift_settings_set_down_delay_decode(buf.set_down_delay);
    int output = tcs_set_shift_settings_set_output_decode(buf.set_output);
    int timeout = tcs_set_shift_settings_set_timeout_decode(buf.set_timeout);

    storage.setUpDelay(upDelay);
    storage.setDownDelay(downDelay);
    storage.setOutput(output);
    storage.setTimeout(timeout);
}

void Can::handleClutchSettings(const CAN_message_t &msg) {
    tcs_set_clutch_settings_t buf;
    tcs_set_clutch_settings_unpack(&buf, msg.buf, sizeof(msg.buf));

    int start = tcs_set_clutch_settings_set_start_decode(buf.set_start);
    int end = tcs_set_clutch_settings_set_end_decode(buf.set_end);
    int friction = tcs_set_clutch_settings_set_friction_decode(buf.set_friction);
    int autoLaunch = tcs_set_clutch_settings_set_auto_launch_decode(buf.set_auto_launch);

    storage.setStart(start);
    storage.setEnd(end);
    storage.setFriction(friction);
    storage.setAutoLaunch(autoLaunch);
}

void Can::handleSetClutch(const CAN_message_t &msg) {
    tcs_set_clutch_t buf;
    tcs_set_clutch_unpack(&buf, msg.buf, sizeof(msg.buf));

    int setPosition = tcs_clutch_position_decode(buf.set_position);
    int setClutchState = tcs_set_clutch_set_state_decode(buf.set_state);

    transmission.state(setClutchState);
    transmission.setClutchPosition(setPosition);
}

void Can::update() {
    // Set the RPM to 0 if an ECU message has not been recieved in 100 ms
    if(millis() - lastEcuUpdate >= 100) {
        transmission.setRpm(0);
    }

    CAN_message_t msg;
    if(!interface.read(msg)) return;

    switch(msg.id) {
        case R3_GROUP0_FRAME_ID: {
            lastEcuUpdate = millis();
            handleGroup0(msg);
            break;
        }

        case TCS_SET_SHIFT_SETTINGS_FRAME_ID: {
            handleShiftSettings(msg);
            break;
        }

        case TCS_SET_CLUTCH_SETTINGS_FRAME_ID: {
            handleClutchSettings(msg);
            break;
        }

        case TCS_SET_CLUTCH_FRAME_ID: {
            handleSetClutch(msg);
            break;
        }
    }
}

void Can::broadcastShiftSettings() {
    static unsigned long lastBroadastTime = 0;
    if(millis() - lastBroadastTime >= 200) {
        CAN_message_t msg;
        msg.id = TCS_SHIFT_SETTINGS_FRAME_ID;

        tcs_shift_settings_t buf;
        buf.up_delay = tcs_shift_settings_up_delay_encode(storage.upDelay());
        buf.down_delay = tcs_shift_settings_down_delay_encode(storage.downDelay());
        buf.output = tcs_shift_settings_output_encode(storage.output());
        buf.timeout = tcs_shift_settings_timeout_encode(storage.timeout());
        tcs_shift_settings_pack(msg.buf, &buf, sizeof(msg.buf));

        interface.write(msg);
        lastBroadastTime = millis();
    }
}

void Can::broadcastClutchSettings() {
    static unsigned long lastBroadastTime = 0;
    if(millis() - lastBroadastTime >= 200) {
        CAN_message_t msg;
        msg.id = TCS_CLUTCH_SETTINGS_FRAME_ID;

        tcs_clutch_settings_t buf;
        buf.start = tcs_clutch_settings_start_encode(storage.start());
        buf.end = tcs_clutch_settings_end_encode(storage.end());
        buf.friction = tcs_clutch_settings_friction_encode(storage.friction());
        buf.auto_launch = tcs_clutch_settings_auto_launch_encode(storage.autoLaunch());
        tcs_clutch_settings_pack(msg.buf, &buf, sizeof(msg.buf));

        interface.write(msg);
        lastBroadastTime = millis();
    }
}

void Can::broadcastClutch() {
    static unsigned long lastBroadastTime = 0;
    if(millis() - lastBroadastTime >= 50) {        
        CAN_message_t msg;
        msg.id = TCS_CLUTCH_FRAME_ID;

        tcs_clutch_t buf;
        buf.position = tcs_clutch_position_encode(transmission.clutchPosition());
        buf.position_percentage = tcs_clutch_position_percentage_encode(transmission.clutchPercentage());
        tcs_clutch_pack(msg.buf, &buf, sizeof(msg.buf));

        interface.write(msg);
        lastBroadastTime = millis();
    }
}

void Can::broadcastAnalogInput() {
    static unsigned long lastBroadastTime = 0;
    if(millis() - lastBroadastTime >= 50) {
        CAN_message_t msg;
        msg.id = TCS_ANALOG_INPUT_FRAME_ID;

        tcs_analog_input_t buf;
        buf.input_right_travel = tcs_analog_input_input_right_travel_encode(clutchRight.travel());
        buf.input_right_raw = tcs_analog_input_input_right_raw_encode(analogRead(storage.CLUTCH_RIGHT));
        buf.input_left_travel = tcs_analog_input_input_left_travel_encode(0);
        buf.input_left_raw = tcs_analog_input_input_left_raw_encode(0);
        tcs_analog_input_pack(msg.buf, &buf, sizeof(msg.buf));

        interface.write(msg);
        lastBroadastTime = millis();
    }
}
