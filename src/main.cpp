#include <FlexCAN_T4.h>
#include "Storage/Storage.h"
#include "AnalogInput/AnalogInput.h"
#include "Clutch/Clutch.h"
#include "Transmission/Transmission.h"
#include "Button/Button.h"
#include "CanData/CanData.h"
#include "Console/Console.h"

FlexCAN_T4<CAN3, RX_SIZE_256, TX_SIZE_64> can;
Console console;
Storage storage("settings.json");
Button up;
Button down;
AnalogInput analogInput(512);
Clutch clutch(can, storage);
CanData canData(clutch, can, storage);
Transmission transmission(clutch, can, storage);

void setup() {
	can.begin();
	can.setBaudRate(500000);
	can.enableFIFO(true);
	can.setFIFOFilter(REJECT_ALL);
	can.setFIFOFilter(0, 1520, STD);
	can.setFIFOFilterRange(2, 1620, 1640, STD);

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
	canData.update();

	// Handle input
	up.update();
	down.update();
	analogInput.update();

	if(up.pressed()) {
		console.pause();
		transmission.shift(Transmission::Direction::UP);
		canData.broadcastGear();
	} else if(down.pressed()) {
		console.pause();
		transmission.shift(Transmission::Direction::DOWN);
		canData.broadcastGear();
	}

	clutch.input = analogInput.travel();

	// Update finite-state machines
	transmission.update();
	clutch.update();
	
	// Send data over CAN
	canData.broadcast(100);
	
	console.printInfo(analogInput.travel(), storage);
	console.update(micros()-start);
}
