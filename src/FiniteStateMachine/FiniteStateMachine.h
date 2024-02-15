#ifndef FINITE_STATE_MACHINE_H
#define FINITE_STATE_MACHINE_H

#include <Arduino.h>
#include <functional>

class FiniteStateMachine {
  public:
	typedef int State;
	
	FiniteStateMachine() = delete;
	FiniteStateMachine(State initialState);
	State state() const;
	void state(State newState);

	// Runs the action once. Reset by changing state.
	void runOnce(std::function<void()> action);

	// Call the action after waiting the duration, then change states
	void waitAndSetState(unsigned long duration, std::function<void()> action, State newState);

	// Call the action every 'time' milliseconds
	void timedLoop(unsigned long time, std::function<void()> action);

	// Increment 'value' once every millisecond over 'time' milliseconds
	bool incrementOverTime(int &value, int finish, unsigned long time);
	
  private:
	State currentState;
	unsigned long lastActionTime;
	unsigned long lastStateChangeTime;
	bool runAction = true;

	bool initializeIncrement = true;
	double incrementValue = 0;
	double amount;
	bool increasing;
	unsigned long startTime;
	unsigned long lastIncrementTime;
};

#endif // FINITESTATEMACHINE_H
