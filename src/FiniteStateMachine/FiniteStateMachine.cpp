#include "FiniteStateMachine.h"

using namespace std;

FiniteStateMachine::FiniteStateMachine(State initialState) : currentState(initialState) {

}

FiniteStateMachine::State FiniteStateMachine::state() const {
	return currentState;
}

void FiniteStateMachine::state(State newState) {
	lastStateChangeTime = millis();
	currentState = newState;
	runAction = true;
	initializeIncrement = true;
}

void FiniteStateMachine::runOnce(function<void()> action) {
	if(runAction == true) {
		action();
		runAction = false;
	}
}

void FiniteStateMachine::waitAndSetState(unsigned long duration, function<void()> action, State newState) {
	if (millis() - lastStateChangeTime >= duration) {
		state(newState);
		action();
	}
}

void FiniteStateMachine::timedLoop(unsigned long time, function<void()> action) {
	if(millis() - lastActionTime > time) {
		lastActionTime = millis();
		action();
	}
}

bool FiniteStateMachine::incrementOverTime(int &value, int finish, unsigned long time) {
	if(initializeIncrement) {
		incrementValue = static_cast<double>(value);
		amount = (static_cast<double>(finish) - incrementValue) / time;
		increasing = finish > value;

		initializeIncrement = false;
		startTime = millis();
	}

	if(micros() - lastIncrementTime >= 1000.0) {
		lastIncrementTime = micros();
		incrementValue += amount;

		bool terminationCondition = false;
		if (increasing) {
			terminationCondition = (incrementValue >= static_cast<double>(finish));
		} else {
			terminationCondition = (incrementValue <= static_cast<double>(finish));
		}

		if(terminationCondition || millis() - startTime >= time) {
			value = finish;
			incrementValue = 0;
			amount = 0;
			return false;
		}

		value = static_cast<int>(incrementValue);
	}

	return true;
}
