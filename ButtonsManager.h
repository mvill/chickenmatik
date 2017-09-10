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
	LinkedList<GlobalHandler*> globalHandlers = LinkedList<GlobalHandler*>();
public:

	void addButton(Button *button){
		ButtonWrapper *buttonWrapper = new ButtonWrapper(button);
		buttonWrappers.add(buttonWrapper);
	}

	void addButtonHandler(ButtonHandler *buttonHandler){
		//Search for existing button
		int nbButtons = buttonWrappers.size();
		for( int i = 0 ; i < nbButtons ; i++ ){
			ButtonWrapper *currentButtonWrapper = buttonWrappers.get(i);
			if( currentButtonWrapper->button == buttonHandler->button ){
				currentButtonWrapper->handlers.add(buttonHandler);
				break;
			}
		}
	}

	void addGlobalHandler(GlobalHandler *globalHandler){
		globalHandlers.add(globalHandler);
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

				//Search for the first handler matching condition
				int nbHandlers = currentButtonWrapper->handlers.size();
				LinkedList<ButtonHandler*> matchingHandlers = LinkedList<ButtonHandler*>();
				for( int j = 0 ; j < nbHandlers ; j++ ){

					ButtonHandler *handler = currentButtonWrapper->handlers.get(j);
					if( handler->mustCheck() ){
						matchingHandlers.add(handler);
					}
				}


				//Search for matching globalHandlers
				LinkedList<GlobalHandler*> matchingGlobalHandlers = LinkedList<GlobalHandler*>();
				for(int j = 0 ; j < globalHandlers.size() ; j++){
					GlobalHandler *globalHandler = globalHandlers.get(j);
					if( globalHandler->mustCheck() ){
						matchingGlobalHandlers.add(globalHandler);
					}
				}

				//Call matchingHandlers
				int nbMatchingHandlers = matchingHandlers.size();
				for( int j = 0 ; j < nbMatchingHandlers ; j++ ){
					ButtonHandler *matchingHandler = matchingHandlers.get(j);
					matchingHandler->handleButtonChanged(newState);
					if( newState ){
						matchingHandler->handleButtonPressed();
					}
					else{
						matchingHandler->handleButtonReleased();
					}
				}

				//Call matchingGlobalHandlers
				for( int j = 0 ; j < matchingGlobalHandlers.size() ; j++ ){
					GlobalHandler *globalHandler = matchingGlobalHandlers.get(j);
					globalHandler->handleButtonChanged(button, newState);
					if( newState ){
						globalHandler->handleButtonPressed( button );
					}
					else{
						globalHandler->handleButtonReleased( button );
					}
				}

			}
		}
	}
};

#endif
