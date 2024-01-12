#ifndef ANALOG_INPUT_H
#define ANALOG_INPUT_H

#include <Arduino.h>
#include <RunningAverage.h>
#include <limits>

class AnalogInput {
  public:
  	AnalogInput() = delete;
	AnalogInput(int pin, uint16_t samples);
	void minDeadzone(float percent);
	void maxDeadzone(float percent);
	void update();
	float travel();

  private:
  	const int pin;
	uint16_t samples;
	RunningAverage runningAverage = RunningAverage(samples);

	float _minDeadzone = 0;
	float _maxDeadzone = 0;

	float min = std::numeric_limits<float>::max();
	float max = 0;
	float percentage = 0;
};

#endif
