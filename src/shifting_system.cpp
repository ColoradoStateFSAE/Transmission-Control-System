#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>
#include <FlexCAN_T4.h>
#include "Storage/Storage.h"
#include "Display/Display.h"
#include "AnalogInput/AnalogInput.h"
#include "Clutch/Clutch.h"
#include "Transmission/Transmission.h"
#include "Button/Button.h"
#include "CanData/CanData.h"

FlexCAN_T4<CAN3, RX_SIZE_16, TX_SIZE_16> can;
CanData canData(can);
Display display;
Storage storage("settings.json");
Button up(41);
Button down(40);
AnalogInput analogInput(A1, 255, 5);
Clutch clutch(37, can, storage);
Transmission transmission(clutch, can, storage);

void setup() {
	can.begin();
	can.setBaudRate(500000);
	can.enableFIFO(true);
	can.setFIFOFilter(REJECT_ALL);
	can.setFIFOFilter(0, 1520, STD);
	can.setFIFOFilter(1, 1572, STD);
	can.setFIFOFilterRange(2, 1620, 1630, STD);
	
	display.begin();

	if(!storage.begin()) {
		while(true) {
			display.sdError();
		}
	}
}

void loop() {
	// Read CAN
	canData.update(storage, clutch);

	// Handle input
	up.update();
	down.update();
	analogInput.update();

	if(up.pressed()) {
		transmission.shift(Transmission::UP);
	} else if(down.pressed()) {
		transmission.shift(Transmission::DOWN);
	}

	clutch.input = analogInput.travel();

	// Update finite-state machines
	transmission.update();
	clutch.update();
	
	// Send data over CAN
	transmission.broadcastValues(100);
	clutch.broadcastValues(1000);

	// Update display
	bool print = transmission.fsm.state() == Transmission::IDLE && clutch.fsm.state() == Clutch::ANALOG_INPUT;
	display.printInfo(analogInput.travel(), print);
}
