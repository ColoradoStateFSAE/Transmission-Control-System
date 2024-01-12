#include "CanData.h"

void CanData::update(Storage &storage, Clutch &clutch) {
	CAN_message_t msg;
	if(can.readFIFO(msg)) {
		switch(msg.id) {
			case 1520:
				storage.rpm(canutil::readData(msg, 6, 2));
				lastCanUpdate = millis();
				break;

			case 1621:
				storage.upDelay(canutil::readData(msg, 0, 2));
				storage.downDelay(canutil::readData(msg, 2, 2));
				storage.output(canutil::readData(msg, 4, 2));
				storage.timeout(canutil::readData(msg, 6, 2));
				storage.print();
				break;

			case 1622:
				storage.start(canutil::readData(msg, 0, 2));
				storage.end(canutil::readData(msg, 2, 2));
				storage.friction(canutil::readData(msg, 4, 2));
				storage.print();
				break;

			case 1623:
				clutch.fsm.state(Clutch::State::IDLE);
				clutch.writeMicroseconds(canutil::readData(msg, 0, 2));
				break;

			case 1624:
				clutch.fsm.state(Clutch::State::ANALOG_INPUT);
				break;

			case 1625:
				if(msg.buf[0] == 1) {
					storage.autoLaunch(true);
				} else if(msg.buf[0] == 0) {
					storage.autoLaunch(false);
				}
				storage.print();
				break;
		}
	}

	if(millis() - lastCanUpdate >= 100) {
		storage.rpm(0);
	}
}
