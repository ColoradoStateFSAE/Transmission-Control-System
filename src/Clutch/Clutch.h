#ifndef CLUTCH_H
#define CLUTCH_H

#include <Arduino.h>
#include <FlexCAN_T4.h>
#include <Servo.h>
#include "Storage/Storage.h"
#include "AnalogInput/AnalogInput.h"
#include "FiniteStateMachine/FiniteStateMachine.h"
#include "canutil/canutil.h"

class Clutch {
  public:
	enum State {
		IDLE,
		ANALOG_INPUT,
		
		HOLD_END,
		GOTO_FRICTION,
		HOLD_FRICTION,
		GOTO_START,
	};

	FiniteStateMachine fsm;
	float input = 0;

	Clutch(int _pin, FlexCAN_T4<CAN3, RX_SIZE_256, TX_SIZE_64> &canRef, Storage &storageRef);
	void writeMicroseconds(int value);
	void broadcastValues(unsigned long frequency);
	int position();
	void update();

  private:
	const int pin;
	FlexCAN_T4<CAN3, RX_SIZE_256, TX_SIZE_64> &can;
	Storage &storage;
	Servo servo;

	unsigned long lastBroadcastTime = 0;
	int servoPosition = 0;

	unsigned long autoLanchStartTime = 0;
	int autoLaunchPosition = 0;
};

#endif
