#ifndef canutil_H
#define canutil_H

#include <Arduino.h>
#include <FlexCAN_T4.h>

namespace canutil {
	int read_data(const CAN_message_t &msg, int offset, int size);
	void construct_data(CAN_message_t &msg, int data, int offset, int size);
	int twos_complement(int value);
}

#endif