#ifndef MOVINGAVERAGE_H
#define MOVINGAVERAGE_H

#include <Arduino.h>

class AnalogAverage {
  public:
    AnalogAverage() {
        for (int i = 0; i < size; i++) {
			readings[i] = 0;
		}
    }

	int value() {
        return average;
    }

	void update() {
        total = total - readings[readIndex];
        readings[readIndex] = analogRead(A0);
        total = total + readings[readIndex];
        readIndex = readIndex + 1;
        if (readIndex >= size) {
            readIndex = 0;
        }

        average = total / size;
    }

  private:
    const static int size = 50;
    int readings[size];
    int readIndex = 0;
    int total = 0;
    int average = 0;
};

#endif