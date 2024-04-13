#include <FlexCAN_T4.h>
#include <Adafruit_NeoPixel.h>
#include "Storage/Storage.h"
#include "AnalogInput/AnalogInput.h"
#include "Clutch/Clutch.h"
#include "Transmission/Transmission.h"
#include "Button/Button.h"
#include "Can/Can.h"
#include "Console/Console.h"

#define DEBUG true

Console console;
Adafruit_NeoPixel pixels(1, 5, NEO_GRB + NEO_KHZ800);
Storage storage;
Button up;
Button down;
AnalogInput analogInput(512);
Clutch clutch(storage);
Can can(clutch, storage);
Transmission transmission(clutch, can, storage);

void setup() {
	pixels.begin();
	pixels.setPixelColor(0, pixels.Color(0, 140, 0));
	pixels.show();

	can.begin();
	console.begin(115200);
	storage.begin();
	
	up.begin(storage.UP);
	down.begin(storage.DOWN);

	analogInput.begin(storage.CLUTCH_RIGHT);
	analogInput.minDeadzone(20);
	analogInput.maxDeadzone(20);

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
	analogInput.update();

	if(up.pressed()) {
		console.pause();
		transmission.shift(Transmission::Direction::UP);
	} else if(down.pressed()) {
		console.pause();
		transmission.shift(Transmission::Direction::DOWN);
	}

	clutch.input = analogInput.travel();

	if(clutch.state() != Clutch::State::ANALOG_INPUT) {
		console.pause();
	}

	// Update finite-state machines
	transmission.update();
	clutch.update();
	
	// Send data over CAN
	can.broadcast(200);
	can.broadcastClutchPosition(clutch.percentage(), 20);
	can.broadcastInput(analogInput.travel(), 20);
	
	#if DEBUG
	console.printInfo(analogInput.travel(), storage);
	console.update(micros()-start);
	#endif
}
