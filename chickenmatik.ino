#include <Arduino.h>
#include <LiquidCrystal.h>
#include <RTClib.h>
#include <Stepper.h>
#include "timeHandler.h"
#include "LoopManager.h"
#include "ButtonsManager.h"
#include "ButtonHandler.h"
#include "AnalogButton.h"
#include "HourInputManager.h"
#include "State.h"
#include "Menu.h"
#include "LcdManager.h"
#include "StepperManager.h"
#include "PositionInputManager.h"

TimeHandler *timeHandler = new TimeHandler();

//Stepper
int numberOfSteps = 48 * 64;
Stepper *stepper = new Stepper(numberOfSteps, 7, 9, 8, 6);
StepperManager *stepperManager = new StepperManager(stepper);

const int BUTTONS_PIN = A0;


uint8_t upHours = 0;
uint8_t upMinutes = 0;

uint8_t downHours = 0;
uint8_t downMinutes = 0;

long upPosition = 0;
long downPosition = 0;


// initialize the library with the numbers of the interface pins
LiquidCrystal *lcd = new LiquidCrystal(12, 11, 5, 4, 3, 2);
LcdManager *lcdManager = new LcdManager(lcd);


Button *okButton = new AnalogButton("OK", BUTTONS_PIN, 450, 699);
Button *leftButton = new AnalogButton("LEFT", BUTTONS_PIN, 300, 449);
Button *downButton = new AnalogButton("DOWN", BUTTONS_PIN, 150, 299);
Button *upButton = new AnalogButton("UP", BUTTONS_PIN, 50, 149);
Button *rightButton = new AnalogButton("RIGHT", BUTTONS_PIN, 0, 49);

ButtonsManager *buttonsManager = new ButtonsManager();

class ButtonsLooper: public Looper {
public:
	void doLoop() {
		buttonsManager->manage();
	}
};

class TimeDisplayLooper: public Looper {
private:
	String number2digit(uint16_t nb)
	{
	  String val = "";
	  if (nb < 10)
	    val = "0" + String(nb);
	  else
	    val = String(nb);

	  return val;
	}
public:
	void doLoop() {
		if (SCREEN_MAIN == currentState) {

			DateTime now = timeHandler->getCurrentDate();


			String hPoint = ":";
		    if ((now.second() % 2) == 0){
		    	hPoint = " ";
		    }

			String hourStr = number2digit(now.hour()) + hPoint + number2digit(now.minute());
//			String hourStr = String(now.hour()) + ":" + String(now.minute()) + " " + String(now.day()) + "/" + String(now.month()) + "/" + String(now.year());


			lcdManager->displayLcd(hourStr, "BBB");

		}
	}
};


// STEPPER CALLBACKS
class SetUpPositionCallback : public CallbackPositionInput{
	void callback(long position) {
		upPosition = position;
    	currentState = SCREEN_MAIN;
	}
};
SetUpPositionCallback *setUpPositionCallback = new SetUpPositionCallback();

class SetDownPositionCallback : public CallbackPositionInput{
	void callback(long position) {
		downPosition = position;
    	currentState = SCREEN_MAIN;
	}
};
SetDownPositionCallback *setDownPositionCallback = new SetDownPositionCallback();


// SET TIME CALLBACKS
class SetTimeCallback: public CallbackHourInput{
    void callback( uint8_t hours, uint8_t minutes ){
    	DateTime currentDateTime = timeHandler->getCurrentDate();
    	//	rtc.adjust(DateTime(2017, 9, 2, 20, 00, 00));
		DateTime newDateTime = DateTime(currentDateTime.year(), currentDateTime.month(), currentDateTime.day(), hours, minutes, 0);
    	timeHandler->setCurrentDate(newDateTime);
    	currentState = SCREEN_MAIN;
    }
};
SetTimeCallback *setTimeCallback = new SetTimeCallback();

class SetUpTimeCallback: public CallbackHourInput{
    void callback( uint8_t hours, uint8_t minutes ){
    	upHours = hours;
    	upMinutes = minutes;
    	currentState = SCREEN_MAIN;
    }
};
SetUpTimeCallback *setUpTimeCallback = new SetUpTimeCallback();

class SetDownTimeCallback: public CallbackHourInput{
    void callback( uint8_t hours, uint8_t minutes ){
    	downHours = hours;
    	downMinutes = minutes;
    	currentState = SCREEN_MAIN;
    }
};
SetDownTimeCallback *setDownTimeCallback = new SetDownTimeCallback();




HourInputManager *hourInputManager = new HourInputManager(buttonsManager, lcdManager, okButton, upButton, downButton);
PositionInputManager *positionInputManager = new PositionInputManager(buttonsManager, lcdManager, stepperManager, okButton, upButton, downButton);

LoopManager loopManager;


// DO TIME EXECUTABLES

class DoTimeInput: public Executable{
public:
	void execute() {
		Serial.println("CCC");
		currentState = TIME_INPUT;
		DateTime currentDatetime = timeHandler->getCurrentDate();
		hourInputManager->show(currentDatetime.hour(), currentDatetime.minute(), setTimeCallback);
	}
};
DoTimeInput *doTimeInputCallback = new DoTimeInput();


class DoUpTimeInput: public Executable{
public:
	void execute() {
		currentState = UP_TIME_INPUT;
		hourInputManager->show(upHours, upMinutes, setUpTimeCallback);
	}
};
DoUpTimeInput *doUpTimeInputCallback = new DoUpTimeInput();


class DoDownTimeInput: public Executable{
public:
	void execute() {
		currentState = DOWN_TIME_INPUT;
		hourInputManager->show(downHours, downMinutes, setDownTimeCallback);
	}
};
DoDownTimeInput *doDownTimeInputCallback = new DoDownTimeInput();


// DO STEPPER EXECUTABLES

class DoUpPositionInput : public Executable{
	void execute() {
		currentState = UP_POSITION_INPUT;
		positionInputManager->show(setUpPositionCallback);
	}
};
DoUpPositionInput *doUpPositionInput = new DoUpPositionInput();

class DoDownPositionInput : public Executable{
	void execute() {
		currentState = DOWN_POSITION_INPUT;
		positionInputManager->show(setDownPositionCallback);
	}
};
DoDownPositionInput *doDownPositionInput = new DoDownPositionInput();






class CallbackCancelMenu: public Executable{
	void execute() {
		currentState = SCREEN_MAIN;
	}
};




class MyGlobalHandler: public GlobalHandler{
private:
	MenuManager *menuManager;
public:
	MyGlobalHandler(MenuManager *menuManager){
		this->menuManager = menuManager;
	}
	virtual bool mustCheck(){
		return currentState == SCREEN_MAIN;
	}
	virtual void handleButtonPressed(Button *button){
		if( button == okButton ){
			currentState = SCREEN_MENU;
			menuManager->show();
		}
	}
};

void setup() {


	lcd->begin(16, 2);


	buttonsManager->addButton(rightButton);
	buttonsManager->addButton(leftButton);
	buttonsManager->addButton(upButton);
	buttonsManager->addButton(downButton);
	buttonsManager->addButton(okButton);


	Menu *menu = new Menu(new CallbackCancelMenu());
	menu->addItem(new MenuItem("1-Heure", doTimeInputCallback));
	menu->addItem(new MenuItem("2-Heure lever", doUpTimeInputCallback));
	menu->addItem(new MenuItem("3-Heure coucher", doDownTimeInputCallback));
	menu->addItem(new MenuItem("4-Position haute", doUpPositionInput));
	menu->addItem( new MenuItem("5-Position basse", doDownPositionInput));
	MenuManager *menuManager = new MenuManager(buttonsManager, lcdManager, menu, okButton, leftButton, upButton, downButton);





	buttonsManager->addGlobalHandler(new MyGlobalHandler(menuManager));

//	getFreeRam();

	Serial.begin(9600);

	timeHandler->setup();

	ButtonsLooper *buttonsLooper = new ButtonsLooper();
	TimeDisplayLooper *timeDisplayLooper = new TimeDisplayLooper();
	loopManager.addLooper(buttonsLooper, 50);
	loopManager.addLooper(timeDisplayLooper, 500);
	loopManager.addLooper(hourInputManager, 100);
	loopManager.addLooper(positionInputManager, 100);

	pinMode(BUTTONS_PIN, INPUT);



}

int lastLoopTime = 0;
int lastTimeDisplayTime = 0;

const int LOOP_TIME_DISPLAY_INTERVAL = 100;
const int LOOP_LED_INTERVAL = 100;
const int LED_CHANGE_INTERVAL = 1000;
const int CHECK_HOUR_POSITION_INTERVAL = 3000;
unsigned long lastLedLoopTime;
unsigned long lastLedChangeTime;
unsigned long lastCheckHourPosition;
unsigned int interval = 1000;
unsigned long lastExec = 0;
void loop() {
	loopManager.loop();
}
