#include "LcdManager.h"

#ifndef HourInputManager_h
#define HourInputManager_h

class CallbackHourInput{
public:
    virtual void callback( uint8_t hours, uint8_t minutes ){

    }
};

class HourInputManager: public GlobalHandler, public Looper {
private:

	enum HourInputState {
		HOURS,
		MINUTES
	};


	ButtonsManager *buttonsManager;
	LcdManager *lcdManager;
	Button *okButton;
	Button *upButton;
	Button *downButton;

	bool showing = false;
	uint8_t inputHours;
	uint8_t inputMinutes;
	HourInputState hourInputState;
	CallbackHourInput *callbackHourInput;

	String number2digit(uint8_t nb) {
		String val = "";
		if (nb < 10)
			val = "0" + String(nb);
		else
			val = String(nb);

		return val;
	}

	void displayInputTime() {

		// Make it blink !
		String displayedHours;
		String displayedMinutes;
		if( (((millis()/1000) % 2) == 0) ){
			switch( hourInputState ){
			case HOURS:
				displayedHours = "  ";
				displayedMinutes = number2digit(inputMinutes);
				break;
			case MINUTES:
				displayedHours = number2digit(inputHours);
				displayedMinutes = "  ";
				break;
			}
		}
		else{
			displayedHours = number2digit(inputHours);
			displayedMinutes = number2digit(inputMinutes);
		}

		String hourStr = displayedHours + ":" + displayedMinutes;
		lcdManager->displayLcd(hourStr, "");
	}

	void doLoop() {
		if( showing ){
			displayInputTime();
		}
	}

public:

	HourInputManager(
			ButtonsManager *buttonsManager,
			LcdManager *lcdManager,
			Button *okButton,
			Button *upButton,
			Button *downButton) {

		this->buttonsManager = buttonsManager;
		this->lcdManager = lcdManager;
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
		if( button == this->okButton ){
			switch( hourInputState ){
			case HOURS:
				hourInputState = MINUTES;
				break;
			case MINUTES:
				Serial.println("A1");
				showing = false;
				Serial.println("A2");
				this->callbackHourInput->callback(inputHours, inputMinutes);

				Serial.println("A3");
				break;
			}
		}
		else if( button == this->upButton ){
			switch( hourInputState ){
			case HOURS:
				inputHours = (inputHours+25)%24;
				break;
			case MINUTES:
				inputMinutes = (inputMinutes+61)%60;
				break;
			}
		}
		else if( button == this->downButton ){
			switch( hourInputState ){
			case HOURS:
				inputHours = (inputHours+23)%24;
				break;
			case MINUTES:
				inputMinutes = (inputMinutes+59)%60;
				break;
			}
		}
	}

	void show( uint8_t hours, uint8_t minutes, CallbackHourInput *callbackHourInput ){
		hourInputState = HOURS;
		this->inputHours = hours;
		this->inputMinutes = minutes;
		this->callbackHourInput = callbackHourInput;
		showing = true;
	}

};

#endif
