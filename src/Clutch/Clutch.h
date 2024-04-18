#ifndef CLUTCH_H
#define CLUTCH_H

#include <Arduino.h>
#include <FlexCAN_T4.h>
#include <Servo.h>
#include "Storage/Storage.h"
#include "AnalogInput/AnalogInput.h"
#include "FiniteStateMachine/FiniteStateMachine.h"
#include "canutil/canutil.h"

class Clutch : public FiniteStateMachine {
  public:
	enum State {
		IDLE,
		ANALOG_INPUT,
		
		HOLD_END,
		GOTO_FRICTION,
		HOLD_FRICTION,
		GOTO_START,
	};
	
	float input = 0;

	Clutch() = delete;
	Clutch(Storage &storageRef);
	void begin(int pin);
	void writeMicroseconds(int value);
	int position();
	int percentage();
	void update();

  private:
	Storage &storage;
	Servo servo;

	unsigned long lastBroadcastTime = 0;
	int servoPosition = -1;

	unsigned long autoLanchStartTime = 0;
	int autoLaunchPosition = 0;
};

#endif
