#include <FlexCAN_T4.h>
#include "Storage/Storage.h"
#include "AnalogInput/AnalogInput.h"
#include "Clutch/Clutch.h"
#include "Transmission/Transmission.h"
#include "Button/Button.h"
#include "Can/Can.h"
#include "Console/Console.h"

Console console;
Storage storage("settings.json");
Button up;
Button down;
AnalogInput analogInput(512);
Clutch clutch(storage);
Can can(clutch, storage);
Transmission transmission(clutch, can, storage);

void setup() {
	can.begin();

	console.begin(9600);

	if(!storage.begin()) {
		while(true) {
			console.printSdError();
		}
	}
	
	up.begin(storage.up());
	down.begin(storage.down());

	analogInput.begin(storage.clutchRight());
	analogInput.minDeadzone(20);
	analogInput.maxDeadzone(20);

	clutch.begin(storage.servo());
}

void loop() {
	int start = micros();

	// Read CAN
	can.update();

	// Handle input
	up.update();
	down.update();
	analogInput.update();

	if(up.pressed()) {
		console.pause();
		transmission.shift(Transmission::Direction::UP);
		can.broadcastGear();
	} else if(down.pressed()) {
		console.pause();
		transmission.shift(Transmission::Direction::DOWN);
		can.broadcastGear();
	}

	clutch.input = analogInput.travel();

	if(clutch.fsm.state() != Clutch::State::ANALOG_INPUT) {
		console.pause();
	}

	// Update finite-state machines
	transmission.update();
	clutch.update();
	
	// Send data over CAN
	can.broadcast(200);
	can.broadcastClutchPosition(clutch.percentage(), 20);
	can.broadcastInput(analogInput.travel(), 20);
	
	console.printInfo(analogInput.travel(), storage);
	console.update(micros()-start);
}
