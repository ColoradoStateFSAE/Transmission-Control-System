#ifndef ANALOG_INPUT_H
#define ANALOG_INPUT_H

#include <Arduino.h>
#include <RunningMedian.h>
#include <limits>

class AnalogInput {
  public:
  	AnalogInput() = delete;
	AnalogInput(int pin, uint8_t samples, float deadzonePercent);
	void update();
	float travel();

  private:
  	const int pin;
	const uint8_t numSamples;
	float deadzone;
	RunningMedian runningMedian = RunningMedian(numSamples);

	float min = std::numeric_limits<float>::max();
	float max = 0;
	float percentage = 0;
};

#endif
