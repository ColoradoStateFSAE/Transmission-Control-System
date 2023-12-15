#include "AnalogAverage.h"

int AnalogAverage::value() {
	return samples.getMedian();
}

void AnalogAverage::update() {
	samples.add(analogRead(A0));
}