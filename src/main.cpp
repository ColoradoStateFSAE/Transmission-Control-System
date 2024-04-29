#include <FlexCAN_T4.h>
#include <Adafruit_NeoPixel.h>
#include "Storage/Storage.h"
#include "AnalogInput/AnalogInput.h"
#include "Clutch/Clutch.h"
#include "Transmission/Transmission.h"
#include "Button/Button.h"
#include "Can/Can.h"
#include "Console/Console.h"

#define DEBUG false

Console console;
Adafruit_NeoPixel pixels(1, 5, NEO_GRB + NEO_KHZ800);
Storage storage;
Button up;
Button down;
AnalogInput clutchRight(512);
AnalogInput clutchLeft(512);

Clutch clutch(storage);
Can can(clutch, storage);
Transmission transmission(clutch, can, storage);

void setup() {
	pixels.begin();
	pixels.setPixelColor(0, pixels.Color(0, 140, 0));
	pixels.show();

	can.begin();
	console.begin(9600);
	storage.begin();
	
	up.begin(storage.UP);
	down.begin(storage.DOWN);

	clutchRight.begin(storage.CLUTCH_RIGHT);
	clutchRight.minDeadzone(20);
	clutchRight.maxDeadzone(20);

	clutchLeft.begin(storage.CLUTCH_LEFT);
	clutchLeft.minDeadzone(20);
	clutchLeft.maxDeadzone(20);

	clutch.begin(storage.SERVO);
}

void loop() {
	#if DEBUG
	int start = micros();
	#endif

	// Read CAN
	can.update();

	// Handle input
	up.update();
	down.update();
	clutchRight.update();
	clutchLeft.update();

	if(up.pressed()) {
		console.pause();
		transmission.shift(Transmission::Direction::UP);
	} else if(down.pressed()) {
		console.pause();
		transmission.shift(Transmission::Direction::DOWN);
	}

	clutch.input = max(clutchRight.travel(), clutchLeft.travel());

	if(clutch.state() != Clutch::State::ANALOG_INPUT) {
		console.pause();
	}

	// Update finite-state machines
	transmission.update();
	clutch.update();
	
	// Send data over CAN
	can.broadcast(200);
	can.broadcastClutchPosition(clutch.percentage(), 20);
	can.broadcastInput(clutchRight.travel(), 20);

	#if DEBUG
	console.printInfo(analogInput.travel(), clutchLeft.travel(), storage);
	console.update(micros()-start);
	#endif
}
