#include "ButtonHandler.h"

#ifndef ButtonsManager_h
#define ButtonsManager_h

class ButtonWrapper{
public:
	Button *button;
	LinkedList<ButtonHandler*> handlers = LinkedList<ButtonHandler*>();
	bool previousState;
	ButtonWrapper(Button *button){
		this->button = button;
		this->previousState = false;
	}
};

class ButtonsManager{
private:
	LinkedList<ButtonWrapper*> buttonWrappers = LinkedList<ButtonWrapper*>();
public:
	void addButtonHandler(ButtonHandler *buttonHandler){

		//Search for existing button
		int nbButtons = buttonWrappers.size();
		bool buttonAlreadyExists = false;
		for( int i = 0 ; i < nbButtons && !buttonAlreadyExists ; i++ ){
			ButtonWrapper *currentButtonWrapper = buttonWrappers.get(i);
			if( currentButtonWrapper->button == buttonHandler->button ){
				buttonAlreadyExists = true;
				currentButtonWrapper->handlers.add(buttonHandler);
				break;
			}
		}
		if( !buttonAlreadyExists ){
			ButtonWrapper *buttonWrapper = new ButtonWrapper(buttonHandler->button);
			buttonWrapper->handlers.add(buttonHandler);
			buttonWrappers.add(buttonWrapper);
		}

	}
	void manage(){

		int nbButtons = buttonWrappers.size();
		for(int i = 0; i < nbButtons; i++) {
			ButtonWrapper *currentButtonWrapper = buttonWrappers.get(i);

			bool lastState = currentButtonWrapper->previousState;
			Button *button = currentButtonWrapper->button;

			// manage button state
			boolean newState = button->isPressed();
			if( newState != lastState ){
				currentButtonWrapper->previousState = newState;
				int nbHandlers = currentButtonWrapper->handlers.size();
				for( int j = 0 ; j < nbHandlers ; j++ ){

					ButtonHandler *handler = currentButtonWrapper->handlers.get(j);

					if( handler->mustCheck() ){
						handler->handleButtonChanged(newState);
						if( newState ){
							handler->handleButtonPressed();
						}
						else{
							handler->handleButtonReleased();
						}
					}
				}
			}
//			Serial.println("manage 5");
		}
//		Serial.println("manage 6");
	}
};

#endif