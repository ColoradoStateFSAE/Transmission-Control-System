#ifndef CAN_UTIL_H
#define CAN_UTIL_H

#include <FlexCAN_T4.h>

namespace canutil {
	int readData(const CAN_message_t &msg, int offset, int size);
	void constructData(CAN_message_t &msg, int data, int offset, int size);
}

#endif
