#ifndef MOVINGAVERAGE_H
#define MOVINGAVERAGE_H

#include <Arduino.h>
#include <RunningMedian.h>

class AnalogAverage {
  public:
	AnalogAverage(int size) : size(size) {
		pinMode(A0, INPUT);

		for (int i = 0; i < size; i++) {
			samples.add(analogRead(A0));
		}
	}

	int value();
	void update();

  private:
	const int size;
	RunningMedian samples = RunningMedian(size);
};

#endif