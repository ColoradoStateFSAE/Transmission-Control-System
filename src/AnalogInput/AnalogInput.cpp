#include "AnalogInput.h"

AnalogInput::AnalogInput(int pin, uint8_t samples, float deadzonePercent) : pin(pin), numSamples(samples) {
	deadzone = deadzonePercent / 100;

	for (int i = 0; i < samples; i++) {
		runningMedian.add(analogRead(pin));
	}
}

void AnalogInput::update() {
	runningMedian.add(analogRead(pin));
	float average = runningMedian.getAverage();

	// Update the maximum if a larger value is found
	if(average > max) {
		max = average;
	}

	// Update the minimum if a smaller value is found
	// and is 30 less than the maximum
	if(average < min && average <= max- 30) {
		min = average;
	}

	float deadzoneThreshold = max * (1 - deadzone);

	if(average > deadzoneThreshold) {
		percentage = 0;
	} else {
		percentage = map(average, min, deadzoneThreshold, 100.0f, 0.0f);
	}
}

float AnalogInput::travel() {
	return percentage;
}
