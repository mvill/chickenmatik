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
	virtual bool mustCheck(){
		//True by default
		return true;
	}
	virtual void handleButtonChanged(bool state){
		//Nothing by default
	}
	virtual void handleButtonPressed(){
		//Nothing by default
	}
	virtual void handleButtonReleased(){
		//Nothing by default
	}
};

class GlobalHandler{
public:

	//Methods
	virtual bool mustCheck(){
		//True by default
		return true;
	}
	virtual void handleButtonChanged(Button *button, bool state){
		//Nothing by default
	}
	virtual void handleButtonPressed(Button *button){
		//Nothing by default
	}
	virtual void handleButtonReleased(Button *button){
		//Nothing by default
	}
};

#endif
