#include "AnalogInput.h"

AnalogInput::AnalogInput(uint16_t samples) : _samples(samples) {
	
}

void AnalogInput::begin(int pin) {
	_pin = pin;
	pinMode(_pin, INPUT_PULLUP);
	
	for (int i = 0; i < _samples; i++) {
		runningAverage.add(analogRead(_pin));
	}
}

void AnalogInput::minDeadzone(float percent) {
	_minDeadzone = percent / 100;
}

void AnalogInput::maxDeadzone(float percent) {
	_maxDeadzone = percent / 100;
}

void AnalogInput::update() {
	if(analogRead(_pin) >= 800) {
		recalibrate = true;
		return;
	}

	if(recalibrate) {
		recalibrate = false;
		min = std::numeric_limits<float>::max();
		max = 0;
		delay(50);
		for (int i = 0; i < _samples; i++) {
			runningAverage.add(analogRead(_pin));
		}
	}

	runningAverage.add(analogRead(_pin));
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
