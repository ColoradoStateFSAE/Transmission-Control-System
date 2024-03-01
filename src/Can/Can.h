#ifndef CAN_H
#define CAN_H

#include <FlexCAN_T4.h>
#include "Storage/Storage.h"
#include "Clutch/Clutch.h"
#include "AnalogInput/AnalogInput.h"
#include "canutil/canutil.h"
#include "dbc/ms3x.h"
#include "dbc/tcs.h"

class Can {
  public:
	FlexCAN_T4<CAN3, RX_SIZE_256, TX_SIZE_16> interface;

	Can(Clutch &clutch, Storage &storage);
	void begin();
	void update();
	void broadcastGear();
	void broadcastClutchPosition(int value, unsigned long frequency);
	void broadcastInput(int input, unsigned long frequency);
	void broadcastShiftSettings();
	void broadcastClutchSettings();
	void broadcast(unsigned long frequency);

  private:
	Clutch &clutch;
	Storage &storage;
	unsigned long lastCanUpdate = 0;
};

#endif
