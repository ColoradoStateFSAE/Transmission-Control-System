#ifndef MOVINGAVERAGE_H
#define MOVINGAVERAGE_H

#include <Arduino.h>
#include <RunningMedian.h>

class AnalogAverage {
  public:
    AnalogAverage() {
		pinMode(A0, INPUT);

		int reading = analogRead(A0);
        for (int i = 0; i < size; i++) {
			samples.add(reading);
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