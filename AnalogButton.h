#include <Arduino.h>
#ifndef AnalogButton_h
#define AnalogButton_h

class AnalogButton: public Button {
private:
	int pin;
	int minAnalogValue;
	int maxAnalogValue;
public:
	AnalogButton(String label, int pin, int minAnalogValue, int maxAnalogValue):Button(label){
		this->pin = pin;
		this->minAnalogValue = minAnalogValue;
		this->maxAnalogValue = maxAnalogValue;
	}
	bool isPressed(){
		Serial.println(F("YO"));
		int state = analogRead(pin);
		Serial.println(String(state));
		bool result = state <= maxAnalogValue && state >= minAnalogValue;
		return result;
	}
};

#endif




