#include "Can.h"

Can::Can(Clutch &clutch, Storage &storage) : clutch(clutch), storage(storage) {

}

void Can::begin() {
	interface.begin();
	interface.setBaudRate(500000);
	interface.enableFIFO(true);
	interface.setFIFOFilter(REJECT_ALL);
	interface.setFIFOFilter(0, 1520, STD);
	interface.setFIFOFilterRange(2, 1620, 1640, STD);
}

void Can::update() {
	CAN_message_t msg;
	if(interface.readFIFO(msg)) {
		switch(msg.id) {
			case MS3X_MEGASQUIRT_GP0_FRAME_ID: {
				lastCanUpdate = millis();
				ms3x_megasquirt_gp0_t message;
				ms3x_megasquirt_gp0_unpack(&message, msg.buf, sizeof(message));
				_rpm = ms3x_megasquirt_gp0_rpm_decode(message.rpm);
				break;
			}

			case TCS_SET_SHIFT_SETTINGS_FRAME_ID: {
				tcs_set_shift_settings_t message;
				tcs_set_shift_settings_unpack(&message, msg.buf, sizeof(message));

				storage.upDelay(tcs_set_shift_settings_set_up_delay_decode(message.set_up_delay));
				storage.downDelay(tcs_set_shift_settings_set_down_delay_decode(message.set_down_delay));
				storage.output(tcs_set_shift_settings_set_output_decode(message.set_output));
				storage.timeout(tcs_set_shift_settings_set_timeout_decode(message.set_timeout));
				break;
			}

			case TCS_SET_CLUTCH_SETTINGS_FRAME_ID: {
				tcs_set_clutch_settings_t message;
				tcs_set_clutch_settings_unpack(&message, msg.buf, sizeof(message));

				storage.start(tcs_set_clutch_settings_set_start_decode(message.set_start));
				storage.end(tcs_set_clutch_settings_set_end_decode(message.set_end));
				storage.friction(tcs_set_clutch_settings_set_friction_decode(message.set_friction));
				storage.autoLaunch(tcs_set_clutch_settings_set_auto_launch_decode(message.set_auto_launch));
				break;
			}

			case TCS_SET_CLUTCH_FRAME_ID: {
				tcs_set_clutch_t message;
				tcs_set_clutch_unpack(&message, msg.buf, sizeof(message));

				clutch.state(tcs_set_clutch_set_state_decode(message.set_state));
				clutch.writeMicroseconds(tcs_clutch_position_decode(message.set_position));
				break;
			}
		}
	}

	if(millis() - lastCanUpdate >= 100) {
		_rpm = 0;
	}
}

void Can::broadcastGear() {
	CAN_message_t msg;
	msg.id = TCS_GEAR_FRAME_ID;
	tcs_gear_t message;

	message.gear = storage.gear();
	tcs_gear_pack(msg.buf, &message, sizeof(msg.buf));
	interface.write(msg);
}

void Can::broadcastClutchPosition(int value, unsigned long frequency) {
	static int lastBroadastTime = 0;
	if(millis() - lastBroadastTime > frequency) {
		lastBroadastTime = millis();
		
		CAN_message_t msg;
		msg.id = TCS_CLUTCH_FRAME_ID;

		tcs_clutch_t message;
		message.position = tcs_clutch_position_encode(clutch.position());
		message.position_percentage = tcs_clutch_position_percentage_encode(clutch.percentage());

		tcs_clutch_pack(msg.buf, &message, sizeof(msg.buf));
		interface.write(msg);
	}
}

void Can::broadcastInput(int value, unsigned long frequency) {
	static int lastBroadastTime = 0;
	if(millis() - lastBroadastTime > frequency) {
		lastBroadastTime = millis();
		
		CAN_message_t msg;
		msg.id = TCS_ANALOG_INPUT_FRAME_ID;

		tcs_analog_input_t message;
		message.input_right_travel = tcs_analog_input_input_right_travel_encode(value);
		message.input_right_raw = tcs_analog_input_input_right_raw_encode(analogRead(storage.clutchRight()));

		tcs_analog_input_pack(msg.buf, &message, sizeof(msg.buf));
		interface.write(msg);
	}
}

void Can::broadcastShiftSettings() {
	CAN_message_t msg;
	msg.id = TCS_SHIFT_SETTINGS_FRAME_ID;

	tcs_shift_settings_t message;
	message.up_delay = storage.upDelay();
	message.down_delay = storage.downDelay();
	message.output = storage.output();
	message.timeout = storage.timeout();

	tcs_shift_settings_pack(msg.buf, &message, sizeof(msg.buf));
	interface.write(msg);
}

void Can::broadcastClutchSettings() {
	CAN_message_t msg;
	msg.id = TCS_CLUTCH_SETTINGS_FRAME_ID;

	tcs_clutch_settings_t message;
	message.start = tcs_clutch_settings_start_encode(storage.start());
	message.end = tcs_clutch_settings_end_encode(storage.end());
	message.friction = tcs_clutch_settings_friction_encode(storage.friction());
	message.auto_launch = tcs_clutch_settings_auto_launch_encode(storage.autoLaunch());

	tcs_clutch_settings_pack(msg.buf, &message, sizeof(msg.buf));
	interface.write(msg);
}

void Can::broadcast(unsigned long frequency) {
	static int lastBroadastTime = millis();
	if(millis() - lastBroadastTime > frequency) {
		lastBroadastTime = millis();
		
		broadcastGear();
		broadcastShiftSettings();
		broadcastClutchSettings();
	}
}

int Can::rpm() {
	return _rpm;
}

void Can::rpm(int value) {
	_rpm = value;
}
