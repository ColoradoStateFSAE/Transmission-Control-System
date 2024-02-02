#include "Storage.h"

using namespace std;

bool Storage::begin() {
	if(!SD.begin(BUILTIN_SDCARD)) {
		Serial.println("Storage: SD card initialization failure");
		return false;
	}

	if(!readPins()) {
		return false;
	}

	verifyJson();
	readJson();
	return true;
}

bool Storage::readPins() {
	File file = SD.open("/pins.json", FILE_READ);
	if(!file) {
		Serial.println("Storage: Could not open 'pins.json'");
		return false;
	}

	DynamicJsonDocument doc(JSON_SIZE);
	deserializeJson(doc, file);
	file.close();

	std::vector<const char*> keys = {"up", "down", "clutchLeft", "clutchRight", "IA", "IB", "servo", "neutral"};
	for(auto i : keys) {
		if(!doc.containsKey(i) || doc[i].isNull()) {
			Serial.println("Storage: Key '" + String(i) + "' not found in 'pins.json'");
			return false;
		}
	}

	_up = doc["up"];
	_down = doc["down"];
	_clutchLeft = doc["clutchLeft"];
	_clutchRight = doc["clutchRight"];
	_IA = doc["IA"];
	_IB = doc["IB"];
	_servo = doc["servo"];
	_neutral = doc["neutral"];

	pinMode(_IA, OUTPUT);
	pinMode(_IB, OUTPUT);

	return true;
}

void Storage::verifyJson() {
	File file = SD.open(filePath.c_str());
	if(!file) {
		SD.remove(filePath.c_str());
		writeJson();
		Serial.println("Storage: '" + String(filePath.c_str()) + "' not found, loading defaults");
		return;
	}

	StaticJsonDocument<0> doc, filter;
  	bool valid = deserializeJson(doc, file, DeserializationOption::Filter(filter)) == DeserializationError::Ok;
	file.close();

	if(!valid) {
		SD.remove(filePath.c_str());
		writeJson();
		Serial.println("Storage: JSON error in '" + String(filePath.c_str()) + "', loading defaults");
	}
}

void Storage::writeJson() {
	File file = SD.open(filePath.c_str(), FILE_WRITE);
	if(!file) {
		Serial.println("Storage: Could not open '" + String(filePath.c_str()) + "'");
		return;
	}

	DynamicJsonDocument doc(JSON_SIZE);
	
	doc["gear"] = _gear;
	doc["upDelay"] = _upDelay;
	doc["downDelay"] = _downDelay;
	doc["output"] = _output;
	doc["timeout"] = _timeout;
	doc["start"] = _start;
	doc["end"] = _end;
	doc["friction"] = _friction;
	doc["autoLaunch"] = _autoLaunch;
	
	file.truncate();
	serializeJsonPretty(doc, file);

	file.close();
}

void Storage::readJson() {
	File file = SD.open(filePath.c_str(), FILE_READ);
	if(!file) {
		Serial.println("Storage: Could not open '" + String(filePath.c_str()) + "'");
		return;
	}

	DynamicJsonDocument doc(JSON_SIZE);
	deserializeJson(doc, file);
	file.close();

	_gear = doc["gear"];
	_upDelay = doc["upDelay"];
	_downDelay = doc["downDelay"];
	_output = doc["output"];
	_timeout = doc["timeout"];
	_start = doc["start"];
	_end = doc["end"];
	_friction = doc["friction"];
	_autoLaunch = doc["autoLaunch"];
}

void Storage::print() {
	Serial.println("SHIFT");
	Serial.println("> UP DELAY: " + String(_upDelay));
	Serial.println("> DOWN DELAY: " + String(_downDelay));
	Serial.println("> OUTPUT: " + String(_output));
	Serial.println("> TIMEOUT: " + String(_timeout));
	Serial.println("CLUTCH");
	Serial.println("> CLUTCH START: " + String(_start));
	Serial.println("> CLUTCH END: " + String(_end));
	Serial.println("> FRICTION POINT: " + String(_friction));
	Serial.println("> AUTO LAUNCH: " + String(_autoLaunch));
	Serial.println("");
}

int Storage::rpm() {
	return _rpm;
}

void Storage::rpm(int value) {
	_rpm = value;
}

int Storage::up() {
	return _up;
}

int Storage::down() {
	return _down;
}

int Storage::clutchLeft() {
	return _clutchLeft;
}

int Storage::clutchRight() {
	return _clutchRight;
}

int Storage::IA() {
	return _IA;
}
int Storage::IB() {
	return _IB;
}

int Storage::servo() {
	return _servo;
}

int Storage::neutral() {
	return _neutral;
}

int Storage::gear() { return _gear; }
void Storage::gear(int value) {
	if(value != _gear) {
		_gear = value;
		writeJson();
	}
}

int Storage::upDelay() { return _upDelay; }
void Storage::upDelay(int value) {
	if(value != _upDelay) {
		_upDelay = value;
		writeJson();
	}
}

int Storage::downDelay() { return _downDelay; }
void Storage::downDelay(int value) {
	if(value != _downDelay) {
		_downDelay = value;
		writeJson();
	}
}

int Storage::output() { return _output; }
void Storage::output(int value) {
	if(value != _output) {
		_output = value;
		writeJson();
	}
}

int Storage::timeout() { return _timeout; }
void Storage::timeout(int value) {
	if(value != _timeout) {
		_timeout = value;
		writeJson();
	}
}

int Storage::start() { return _start; }
void Storage::start(int value) {
	if(value != _start) {
		_start = value;
		writeJson();
	}
}

int Storage::end() { return _end; }
void Storage::end(int value) {
	if(value != _end) {
		_end = value;
		writeJson();
	}
}

int Storage::friction() { return _friction; }
void Storage::friction(int value) {
	if(value != _friction) {
		_friction = value;
		writeJson();
	}
}

bool Storage::autoLaunch() { return _autoLaunch; }
void Storage::autoLaunch(bool value) {
	if(value != _autoLaunch) {
		_autoLaunch = value;
		writeJson();
	}
}
