#include "LcdManager.h"

#ifndef PositionInputManager_h
#define PositionInputManager_h

class CallbackPositionInput {
public:
	virtual void callback(long position) {

	}
};

class PositionInputManager: public GlobalHandler, public Looper {
private:

	ButtonsManager *buttonsManager;
	LcdManager *lcdManager;
	StepperManager *stepperManager;
	Button *okButton;
	Button *upButton;
	Button *downButton;

	bool showing = false;
	CallbackPositionInput *callbackPositionInput;


public:

	PositionInputManager(

			ButtonsManager *buttonsManager,
			LcdManager *lcdManager,
			StepperManager *stepperManager,
			Button *okButton,
			Button *upButton,
			Button *downButton) {

				this->buttonsManager = buttonsManager;
				this->lcdManager = lcdManager;
				this->stepperManager = stepperManager;
				this->okButton = okButton;
				this->upButton = upButton;
				this->downButton = downButton;
				this->showing = false;

				buttonsManager->addGlobalHandler(this);
			}

			bool mustCheck() {
				return showing;
			}

			void handleButtonPressed(Button *button) {
				if (button == this->okButton) {
					showing = false;
					callbackPositionInput->callback(stepperManager->currentPosition);

				}
			}

			void show(CallbackPositionInput *callbackPositionInput) {
				this->lcdManager->displayLcd("Set Position", "");
				this->callbackPositionInput = callbackPositionInput;
				showing = true;
			}

			void doLoop() {
				if (showing) {
					if( upButton->isPressed() ){
						stepperManager->step(200);
					}
					else if( downButton->isPressed() ){
						stepperManager->step(-200);
					}
				}
			}

		};

#endif
