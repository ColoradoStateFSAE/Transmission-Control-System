#include "CanData.h"

CanData::CanData(Clutch &clutchRef, FlexCAN_T4<CAN3, RX_SIZE_256, TX_SIZE_64> &canRef, Storage &storageRef) :
	clutch(clutchRef), can(canRef), storage(storageRef) {

}

void CanData::update() {
	CAN_message_t msg;
	if(can.readFIFO(msg)) {
		switch(msg.id) {
			case MS3X_MEGASQUIRT_GP0_FRAME_ID: {
				lastCanUpdate = millis();
				ms3x_megasquirt_gp0_t message;
				ms3x_megasquirt_gp0_unpack(&message, msg.buf, sizeof(message));
				storage.rpm(ms3x_megasquirt_gp0_rpm_decode(message.rpm));
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
				clutch.writeMicroseconds(tcs_set_clutch_set_position_decode(message.set_position));
				clutch.fsm.state(tcs_set_clutch_set_state_decode(message.set_state));
				break;
			}
		}
	}

	if(millis() - lastCanUpdate >= 100) {
		storage.rpm(0);
	}
}

void CanData::broadcastGear() {
	CAN_message_t msg;
	msg.id = TCS_GEAR_FRAME_ID;
	tcs_gear_t message;

	message.gear = storage.gear();
	tcs_gear_pack(msg.buf, &message, sizeof(msg.buf));
	can.write(msg);
}

void CanData::broadcastClutchPosition(int value, unsigned long frequency) {
	static int lastBroadastTime = 0;
	if(millis() - lastBroadastTime > frequency) {
		lastBroadastTime = millis();
		
		CAN_message_t msg;
		msg.id = TCS_CLUTCH_FRAME_ID;

		tcs_clutch_t message;
		message.position = tcs_clutch_position_encode(clutch.position());
		message.position_percentage = tcs_clutch_position_percentage_encode(clutch.percentage());

		tcs_clutch_pack(msg.buf, &message, sizeof(msg.buf));
		can.write(msg);
	}
}

void CanData::broadcastInput(int value, unsigned long frequency) {
	static int lastBroadastTime = 0;
	if(millis() - lastBroadastTime > frequency) {
		lastBroadastTime = millis();
		
		CAN_message_t msg;
		msg.id = TCS_ANALOG_INPUT_FRAME_ID;

		tcs_analog_input_t message;
		message.input_right_travel = tcs_analog_input_input_right_travel_encode(value);
		message.input_right_raw = tcs_analog_input_input_right_raw_encode(analogRead(storage.clutchRight()));

		tcs_analog_input_pack(msg.buf, &message, sizeof(msg.buf));
		can.write(msg);
	}
}

void CanData::broadcastShiftSettings() {
	CAN_message_t msg;
	msg.id = TCS_SHIFT_SETTINGS_FRAME_ID;

	tcs_shift_settings_t message;
	message.up_delay = storage.upDelay();
	message.down_delay = storage.downDelay();
	message.output = storage.output();
	message.timeout = storage.timeout();

	tcs_shift_settings_pack(msg.buf, &message, sizeof(msg.buf));
	can.write(msg);
}

void CanData::broadcastClutchSettings() {
	CAN_message_t msg;
	msg.id = TCS_CLUTCH_SETTINGS_FRAME_ID;

	tcs_clutch_settings_t message;
	message.start = tcs_clutch_settings_start_encode(storage.start());
	message.end = tcs_clutch_settings_end_encode(storage.end());
	message.friction = tcs_clutch_settings_friction_encode(storage.friction());
	message.auto_launch = tcs_clutch_settings_auto_launch_encode(storage.autoLaunch());

	tcs_clutch_settings_pack(msg.buf, &message, sizeof(msg.buf));
	can.write(msg);
}

void CanData::broadcast(unsigned long frequency) {
	static int lastBroadastTime = millis();
	if(millis() - lastBroadastTime > frequency) {
		lastBroadastTime = millis();
		
		broadcastGear();
		broadcastShiftSettings();
		broadcastClutchSettings();

		CAN_message_t msg;
		msg.id = 1520;
		can.write(msg);
	}
}
