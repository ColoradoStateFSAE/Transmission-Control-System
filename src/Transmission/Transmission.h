#ifndef TRANSMISSION_H
#define TRANSMISSION_H

#include <Arduino.h>
#include <FlexCAN_T4.h>
#include "Clutch/Clutch.h"
#include "Storage/Storage.h"
#include "FiniteStateMachine/FiniteStateMachine.h"
#include "canutil/canutil.h"

class Transmission {
  public:
  	enum {
		UP = 0,
		DOWN = 1
	};

	enum State {
		IDLE,

		UP_SPARK_CUT,
		UP_ENABLE_SOLENOID,
		UP_DISABLE_SOLENOID,

		DOWN_CLUTCH_IN,
		DOWN_ENABLE_SOLENOID,
		DOWN_DISABLE_SOLENOID,
		DOWN_CLUTCH_OUT,
	};

	FiniteStateMachine fsm;

	Transmission() = delete;
	Transmission(Clutch &clutchRef, FlexCAN_T4<CAN3, RX_SIZE_256, TX_SIZE_64> &canRef, Storage &settingsRef);

	void broadcastValues(unsigned long frequency=0);
	void shift(int direction);
	void update();

  private:
	const int OUTPUT_PINS[2] = {17, 18}; // {up, down}
	
	Clutch &clutch;
	FlexCAN_T4<CAN3, RX_SIZE_256, TX_SIZE_64> &can;
	Storage &storage;

	unsigned long shiftStartTime = 0;
	unsigned long lastBroadastTime = 0;

	void disableCombustion();
	void upRoutine();
	void downRoutine();
};

#endif
