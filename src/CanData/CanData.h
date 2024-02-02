#ifndef CAN_DATA_H
#define CAN_DATA_H

#include <FlexCAN_T4.h>
#include "Storage/Storage.h"
#include "Clutch/Clutch.h"
#include "canutil/canutil.h"
#include "dbc/ms3x.h"
#include "dbc/tcs.h"

class CanData {
  public:
	CanData(Clutch &clutchRef, FlexCAN_T4<CAN3, RX_SIZE_256, TX_SIZE_64> &canRef, Storage &storageRef);
	void update();
	void broadcastGear();
	void broadcastShift();
	void broadcastClutch();
	void broadcast(unsigned long frequency);

  private:
	Clutch &clutch;
	FlexCAN_T4<CAN3, RX_SIZE_256, TX_SIZE_64> &can;
	Storage &storage;
	unsigned long lastCanUpdate = 0;
	unsigned long lastBroadastTime = 0;
};

#endif
