#include "Display.h"

void Display::begin() {
	oled.begin(SSD1306_SWITCHCAPVCC, 0x3C);
}

void Display::printInfo(int analogInput, bool print=true) {	
	loops++;

	if(millis() - lastDisplayTime >= 500 && print) {
		oled.clearDisplay();
		oled.setCursor(0, 0);
		oled.setTextColor(WHITE);

		oled.setTextSize(1);
		oled.println("Clutch Input");
		oled.setTextSize(2);
		oled.println(analogInput);

		oled.setTextSize(1);
		oled.println();
		oled.println("Loop time");
		oled.setTextSize(2);
		oled.print((micros() - startTime) / loops);
		oled.setTextSize(1);
		oled.println(" us");

		oled.display();

		lastDisplayTime = millis();
		startTime = micros();
		loops = 0;
	}
}

void Display::sdError() {
	oled.clearDisplay();
	oled.setCursor(0, 0);
	oled.setTextColor(WHITE);
	oled.setTextSize(2);
	oled.println("SD card");
	oled.println("error");
	oled.display();
}
