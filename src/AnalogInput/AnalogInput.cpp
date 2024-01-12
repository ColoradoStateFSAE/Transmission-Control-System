#include "AnalogInput.h"

AnalogInput::AnalogInput(int pin, uint16_t samples) : pin(pin), samples(samples) {
	analogReadResolution(12);
	for (int i = 0; i < samples; i++) {
		runningAverage.add(analogRead(pin));
	}
}

void AnalogInput::minDeadzone(float percent) {
	_minDeadzone = percent / 100;
}

void AnalogInput::maxDeadzone(float percent) {
	_maxDeadzone = percent / 100;
}

void AnalogInput::update() {
	runningAverage.add(analogRead(pin));
	float average = runningAverage.getFastAverage();

	// Update the maximum if a larger value is found
	if(average > max) {
		max = average;
	}

	// Update the minimum if a smaller value is found
	// and is 30 less than the maximum
	if(average < min && average <= max - 30) {
		min = average;
	}

	float minDeadzoneScaled = min + (max - min) * (1 - _minDeadzone);
	float maxDeadzoneScaled = max - (max - min) * (1 - _maxDeadzone);

	percentage = map(average, minDeadzoneScaled, maxDeadzoneScaled, 0.0f, 100.0f);
	percentage = constrain(percentage, 0.0f, 100.0f);	
}

float AnalogInput::travel() {
	return percentage;
}
