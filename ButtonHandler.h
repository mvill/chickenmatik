#include <Arduino.h>
#include "Button.h"

#ifndef ButtonHandler_h
#define ButtonHandler_h

class ButtonHandler{
public:

	// Attributes
	Button *button;

	//Constructor
	ButtonHandler(Button *button){
		this->button = button;
	}

	//Methods
	virtual void handleButtonChanged(bool state){
		Serial.println("ButtonHandler.handleButtonChanged");
		//Nothing by default
	}
	virtual void handleButtonPressed(){

		Serial.println("ButtonHandler.handleButtonPressed");
		//Nothing by default
	}
	virtual void handleButtonReleased(){
		Serial.println("ButtonHandler.handleButtonReleased");
		//Nothing by default
	}
	virtual bool mustCheck(){
		//True by default
		return true;
	}
};

#endif
