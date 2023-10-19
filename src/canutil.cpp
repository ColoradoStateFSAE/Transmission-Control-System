#include "canutil.h"

int canutil::read_data(const CAN_message_t &msg, int offset, int size) {
  int data = 0;
  for (int i = offset; i < offset + size; i++) {
    data = data << 8;
    data += msg.buf[i];
  }
  return data;
}

void canutil::construct_data(CAN_message_t &msg, int data, int offset, int size) {
  for (int i = offset + size - 1; i >= offset; i -= 1) {
    msg.buf[i] = data % 256;
    data >>= 8;
  }
}

int canutil::twos_complement(int value) {
  //value = ~value;
  value += 1;
  return value;
}