#include "CanData.h"

CanData::CanData(
	Clutch &clutchRef,
	FlexCAN_T4<CAN3, RX_SIZE_256, TX_SIZE_64> &canRef,
	Storage &storageRef
) :
	clutch(clutchRef),
	can(canRef),
	storage(storageRef)
{

}

void CanData::update() {
	CAN_message_t msg;
	if(can.readFIFO(msg)) {
		switch(msg.id) {
			case MS3X_MEGASQUIRT_GP0_FRAME_ID: {
				lastCanUpdate = millis();
				ms3x_megasquirt_gp0_t message;
				ms3x_megasquirt_gp0_unpack(&message, msg.buf, sizeof(message));
				auto rpm = ms3x_megasquirt_gp0_rpm_decode(message.rpm);
				storage.rpm(rpm);
				break;
			}

			case TCS_UPDATE_SHIFT_SETTINGS_FRAME_ID: {
				tcs_update_shift_settings_t message;
				tcs_update_shift_settings_unpack(&message, msg.buf, sizeof(message));
				break;
			}

			case TCS_UPDATE_CLUTCH_SETTINGS_FRAME_ID: {
				tcs_update_clutch_settings_t message;
				tcs_update_clutch_settings_unpack(&message, msg.buf, sizeof(message));
				break;
			}

			case TCS_UPDATE_CLUTCH_POSITION_FRAME_ID: {
				tcs_update_clutch_position_t message;
				tcs_update_clutch_position_unpack(&message, msg.buf, sizeof(message));
				clutch.fsm.state(Clutch::IDLE);
				clutch.writeMicroseconds(message.position);
				break;
			}

			case TCS_ANALOG_INPUT_FRAME_ID: {
				clutch.fsm.state(Clutch::ANALOG_INPUT);
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

void CanData::broadcastShift() {
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

void CanData::broadcastClutch() {
	{
		CAN_message_t msg;
		msg.id = TCS_CLUTCH_SETTINGS_FRAME_ID;
		tcs_clutch_settings_t message;

		message.start = storage.start();
		message.end = storage.end();
		message.friction = storage.friction();
		message.auto_launch = storage.autoLaunch();
		tcs_clutch_settings_pack(msg.buf, &message, sizeof(msg.buf));
		can.write(msg);
	}
	
	{
		CAN_message_t msg;
		msg.id = TCS_CLUTCH_POSITION_FRAME_ID;
		tcs_clutch_position_t message;

		message.position = storage.start();
		tcs_clutch_position_pack(msg.buf, &message, sizeof(msg.buf));
		can.write(msg);
	}

	CAN_message_t msg;
	msg.id = 1520;
	can.write(msg);
}

void CanData::broadcast(unsigned long frequency) {
	if(millis() - lastBroadastTime > frequency) {
		lastBroadastTime = millis();
		
		broadcastGear();
		broadcastShift();
		broadcastClutch();
		
		CAN_message_t clutch;
		clutch.id = TCS_CLUTCH_SETTINGS_FRAME_ID;
	}
}
