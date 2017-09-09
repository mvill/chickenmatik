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
		int state = analogRead(pin);
		bool result = state <= maxAnalogValue && state >= minAnalogValue;
		return result;
	}
};

#endif




