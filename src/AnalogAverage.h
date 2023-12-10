#ifndef MOVINGAVERAGE_H
#define MOVINGAVERAGE_H

#include <Arduino.h>
#include <RunningMedian.h>

class AnalogAverage {
  public:
	AnalogAverage() {
		pinMode(A0, INPUT);

	for (int i = 0; i < size; i++) {
			samples.add(analogRead(A0));
		}
	}

	int value() {
		return samples.getMedian();
	}

	void update() {
		samples.add(analogRead(A0));
	}

  private:
	const int size = 50;
	RunningMedian samples = RunningMedian(5);
};

#endif