#include <Arduino.h>
#ifndef ClassicButton_h
#define ClassicButton_h

class ClassicButton: public Button {
private:
	int pin;
public:
	ClassicButton(String label, int pin):Button(label){
		this->pin = pin;
	}
	bool isPressed(){
		int state = digitalRead(pin);
		bool result = state == HIGH;
		return result;
	}
};

#endif




