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
	//  pinMode(LEFT_BUTTON_PIN, INPUT);
	String label;
	bool isPressed(){
//		Serial.println("isPressed 1");
		int state = digitalRead(this->pin);
//		Serial.println("isPressed 2");
		bool result = state == HIGH;
//		Serial.println("isPressed 3");
		//Serial.println(String("isPressed " + this->label));
//		if(result){
//			Serial.println("isPressed true");
//		}
//		else{
//
//			Serial.println("isPressed false");
//		}
		return result;
	}
};

#endif




