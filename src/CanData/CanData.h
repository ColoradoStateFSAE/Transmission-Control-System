#ifndef CAN_DATA_H
#define CAN_DATA_H

#include <FlexCAN_T4.h>
#include "Storage/Storage.h"
#include "Clutch/Clutch.h"
#include "canutil/canutil.h"

class CanData {
  public:
	CanData(FlexCAN_T4<CAN3, RX_SIZE_16, TX_SIZE_16> &canRef) : can(canRef) { }
	void update(Storage &storage, Clutch &clutch);

private:
	FlexCAN_T4<CAN3, RX_SIZE_16, TX_SIZE_16> &can;
	unsigned long lastCanUpdate = 0;
	unsigned long lastShiftBroadastTime = 0;
};

#endif
