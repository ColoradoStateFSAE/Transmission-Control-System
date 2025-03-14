#ifndef TRANSMISSION_H
#define TRANSMISSION_H

#ifdef NATIVE
#include "../../test/mock/MockFlexCAN_T4.h"
#include "../../test/mock/MockServo.h"
#else
#include <FlexCAN_T4.h>
#include <Servo.h>
#endif

#include <Arduino.h>
#include "../FiniteStateMachine/FiniteStateMachine.h"
#include "../Storage/Storage.h"

class Transmission : public FiniteStateMachine {
  public:
    enum class Direction {
        UP,
        DOWN
    };

    enum State {
        CLUTCH_ADJUST,
        CLUTCH_MANUAL,

        SPARK_CUT,
        CLUTCH_IN,
        ENABLE_SOLENOID,
        DISABLE_SOLENOID,
        CLUTCH_OUT,
    };

    Transmission(Storage &storageRef, Servo &servoRef) : FiniteStateMachine(CLUTCH_MANUAL), storage(storageRef), servo(servoRef) {}
    virtual void begin();
	  virtual bool overRev();
    virtual void shift(Transmission::Direction direction);
    virtual void clutchInput(float value);
	  virtual void setGear(int8_t value);
    virtual void setRpm(int value);
    virtual void setClutchPosition(int position);
    virtual int clutchPosition();
    virtual int clutchPercentage();
    virtual void update();

  private:
    struct ShiftParameters {
        Direction direction;
        int pin;
        int delay;
        int releaseClutchDelay;
        bool useClutch;
    };

    ShiftParameters parameters;
    
    Storage &storage;
    Servo &servo;
	// overrev protection max rpm for gears 2-6
    int rpm = 0;
	int8_t gear = 0;
    unsigned long shiftStartTime = 0;

    float _input = 0;

    void writeMicroseconds(int value);
    const int MAXRPM[5] = {12389, 13336, 13860, 14249, 14308};

};

#endif
