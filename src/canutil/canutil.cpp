#include "canutil.h"

int canutil::readData(const CAN_message_t &msg, int offset, int size) {
	int data = 0;
	for (int i = offset; i < offset + size; i++) {
		data = data << 8;
		data += msg.buf[i];
	}
	return data;
}

void canutil::constructData(CAN_message_t &msg, int data, int offset, int size) {
	for (int i = offset + size - 1; i >= offset; i -= 1) {
		msg.buf[i] = data % 256;
		data >>= 8;
	}
}
