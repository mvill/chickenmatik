#include <Arduino.h>
#include <LiquidCrystal.h>
#include <RTClib.h>
#include <Stepper.h>
#include "timeHandler.h"
#include "LoopManager.h"
#include "ButtonsManager.h"
#include "ButtonHandler.h"
#include "ClassicButton.h"

TimeHandler timeHandler;

//Stepper
int nombreDePas = 48*64;
Stepper monMoteur(nombreDePas, A1, A3, A2, A0);

const int LEFT_BUTTON_PIN = 6;
const int DOWN_BUTTON_PIN = 7;
const int UP_BUTTON_PIN = 8;
const int RIGHT_BUTTON_PIN = 9;
const int OK_BUTTON_PIN = 10;

const int LED_PIN = A0;

const int NB_BUTTONS = 5;



enum State {
	SCREEN_MAIN,
	SCREEN_MENU_TIME,
	SCREEN_MENU_UP_TIME,
	SCREEN_MENU_DOWN_TIME,
	SCREEN_MENU_UP_POSITION,
	SCREEN_MENU_DOWN_POSITION,
	SCREEN_DO_DOWN_POSITION,
	SCREEN_DO_TIME_HOUR,
	SCREEN_DO_TIME_MINUTE
};
State currentState = SCREEN_MAIN;


unsigned long inputHour;
unsigned long inputMinute;



// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);


//STEP MANAGEMENT
long currentPosition = 0;
long upPosition = 0;
long downPosition = 0;

void step( long stepValue ){
  currentPosition += stepValue;
  monMoteur.step(stepValue);
}

void storeDownPosition(){
  downPosition = currentPosition;
}

void storeUpPosition(){
  upPosition = currentPosition;
}

void stepToDownPosition(){
  step( downPosition - currentPosition );
}

void stepToUpPosition(){
  step( upPosition - currentPosition );
}
//END STEP MANAGEMENT


//TIME MANAGEMENT

//24h en ms
unsigned long ONE_DAY = 24l * 60l * 60l * 1000l;
unsigned long ONE_HOUR = 60l * 60l * 1000l;
unsigned long ONE_MINUTE = 60l * 1000l;
unsigned long ONE_SECOND = 1000l;

//ms depuis minuit
unsigned long midnightTime = 0;
unsigned long upTime = ONE_HOUR * 7l;
unsigned long downTime = ONE_HOUR * 22l;

//en ms depuis minuit
unsigned long getTime(){
  unsigned long now = millis();
  unsigned long time = (now + ONE_DAY - midnightTime) % ONE_DAY;
  return time;
}

void setTime( unsigned long time ){
  //24h en ms = 24 * 60 * 60 * 1000
  midnightTime = ( millis() + ONE_DAY - time ) % ONE_DAY;
}

//Heure format HH:mm
String getFullHourStr(){
  unsigned long time = getTime();
  unsigned long hours = ( time / ONE_HOUR ) % 24l;
  unsigned long minutes = ( time / ONE_MINUTE ) % 60l;
  unsigned long seconds = ( time / ONE_SECOND ) % 60l;
  String hourStr = String(String(hours) + ":" + String(minutes)+ ":" + String(seconds));
  return hourStr;
}
//END TIME MANAGEMENT



void displayLcd( String line1, String line2 ){
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(line1);
    lcd.setCursor(0, 1);
    lcd.print(line2);
}


class Executable{
	void virtual execute(){

	}
};


class MenuItem{
public:
	String label;
	State menuState;
	Executable *handler;
	MenuItem( String label, State menuState, Executable *handler ){
		this->label = label;
		this->menuState = menuState;
		this->handler = handler;
	}
};

class Menu{
public:
	LinkedList<MenuItem*> items = LinkedList<MenuItem*>();
	void addItem(MenuItem *item){
		items.add(item);
	}
};

class MenuBackButtonHandler : public ButtonHandler{
private:
	State initialState;
	State menuState;
public:
	MenuBackButtonHandler( Button *button, State initialState, State menuState ) : ButtonHandler(button){
		this->initialState = initialState;
		this->menuState = menuState;
	}

	bool mustCheck(){
		return currentState == menuState;
	}
	void handleButtonPressed(){
		currentState = initialState;
	}
};

class MenuShowButtonHandler : public ButtonHandler{
private:
	State initialState;
	State menuState;
public:
	MenuShowButtonHandler( Button *button, State initialState, State menuState ) : ButtonHandler(button){
		this->initialState = initialState;
		this->menuState = menuState;
	}

	bool mustCheck(){
		Serial.println("MenuShowButtonHandler.mustCheck");
		return currentState == initialState;
	}
	void handleButtonPressed(){
		Serial.println("MenuShowButtonHandler.handleButtonPressed");
		currentState = menuState;
	    displayLcd("MENU", "1-Heure");
	}
};

class MenuNavButtonHandler : public ButtonHandler{
private:
	State initialState;
	MenuItem *menuItem;
public:
	MenuNavButtonHandler( Button *button, State initialState, MenuItem *menuItem ) : ButtonHandler(button){
		this->initialState = initialState;
		this->menuItem = menuItem;
	}
	bool mustCheck(){
		return currentState == initialState;
	}
	void handleButtonPressed(){

		Serial.print("menuItem->label : ");
		Serial.println(String(menuItem->label));
//
		Serial.print("initialState : ");
		Serial.println(initialState);
//
		Serial.print("button->label : ");
		Serial.println(button->label);
//
		Serial.print("old state : ");
		Serial.println(currentState);
//
		Serial.print("new state : ");
		Serial.println(menuItem->menuState);

//		Serial.println("FFFFFFFFFFFFFFF 2");


		currentState = menuItem->menuState;
	    displayLcd("MENU", menuItem->label);
	}
};

class MenuButtonHandlersGenerator{
private:
	Menu *menu;
	State initialState;
	State menuState;
	Button *okButton;
	Button *backButton;
	Button *previousButton;
	Button *nextButton;
	MenuItem* getPreviousItem( int currentItemIndex ){
		int neededIndex = currentItemIndex - 1;
		//TODO do it with modulos
		if( neededIndex < 0 ){
			neededIndex = neededIndex + menu->items.size();
		}
		return menu->items.get(neededIndex);
	}
	MenuItem* getNextItem( int currentItemIndex ){
		int neededIndex = currentItemIndex + 1;
		//TODO do it with modulos
		if( neededIndex >=  menu->items.size() ){
			neededIndex = neededIndex - menu->items.size();
		}
		return menu->items.get(neededIndex);
	}

public:
	MenuButtonHandlersGenerator( Menu *menu, State initialState, State menuState, Button *okButton, Button *backButton, Button *previousButton, Button *nextButton){
		this->menu = menu;
		this->initialState = initialState;
		this->menuState = menuState;
		this->okButton = okButton;
		this->backButton = backButton;
		this->previousButton = previousButton;
		this->nextButton = nextButton;
	}
	LinkedList<ButtonHandler*> generateButtonHandlers(){
		LinkedList<ButtonHandler*> buttonHandlers = LinkedList<ButtonHandler*>();

		// Back to main screen
		MenuBackButtonHandler *menuBackButtonHandler = new MenuBackButtonHandler(backButton, initialState, menuState);
		buttonHandlers.add(menuBackButtonHandler);

		// Main screen to first menu item
		MenuShowButtonHandler *menuShowButtonHandler = new MenuShowButtonHandler(okButton, initialState, menuState);
		buttonHandlers.add(menuShowButtonHandler);

		int nbItems = menu->items.size();
		for( int i = 0 ; i < nbItems ; i++ ){
			MenuItem *currentMenuItem = menu->items.get(i);
			MenuItem *previousMenuItem = getPreviousItem(i);
			MenuItem *nextMenuItem = getNextItem(i);
//			Serial.println("EEEEEEEEEEEE 1");
//			Serial.println(currentMenuItem->label);
//			Serial.println(previousMenuItem->label);
//			Serial.println(nextMenuItem->label);
//			Serial.println("EEEEEEEEEEEE 2");

			MenuNavButtonHandler *previousButtonHandler = new MenuNavButtonHandler(nextButton, previousMenuItem->menuState, currentMenuItem);
			MenuNavButtonHandler *nextButtonHandler = new MenuNavButtonHandler(previousButton, nextMenuItem->menuState, currentMenuItem);

			buttonHandlers.add(previousButtonHandler);
			buttonHandlers.add(nextButtonHandler);

		}
		return buttonHandlers;
	}
};



//void handleButtonPressed(Button_t* button){
//
//  if( SCREEN_MAIN.equals(currentScreen) && button->label == "OK"){
//    displayLcd("MENU", "1-Heure");
//    currentScreen = SCREEN_MENU_TIME;
//  }
//  else if(SCREEN_MENU_TIME.equals(currentScreen) && button->label == "DOWN"){
//    displayLcd("MENU", "2-Heure lever");
//    currentScreen = SCREEN_MENU_UP_TIME;
//  }
//  else if(SCREEN_MENU_TIME.equals(currentScreen) && button->label == "UP"){
//    displayLcd("MENU", "5-Position basse");
//    currentScreen = SCREEN_MENU_DOWN_POSITION;
//  }
//  else if(SCREEN_MENU_UP_TIME.equals(currentScreen) && button->label == "DOWN"){
//    displayLcd("MENU", "3-Heure coucher");
//    currentScreen = SCREEN_MENU_DOWN_TIME;
//  }
//  else if(SCREEN_MENU_UP_TIME.equals(currentScreen) && button->label == "UP"){
//    displayLcd("MENU", "1-Heure");
//    currentScreen = SCREEN_MENU_TIME;
//  }
//  else if(SCREEN_MENU_DOWN_TIME.equals(currentScreen) && button->label == "DOWN"){
//    displayLcd("MENU", "4-Position haute");
//    currentScreen = SCREEN_MENU_UP_POSITION;
//  }
//  else if(SCREEN_MENU_DOWN_TIME.equals(currentScreen) && button->label == "UP"){
//    displayLcd("MENU", "2-Heure lever");
//    currentScreen = SCREEN_MENU_UP_TIME;
//  }
//  else if(SCREEN_MENU_UP_POSITION.equals(currentScreen) && button->label == "DOWN"){
//    displayLcd("MENU", "5-Position basse");
//    currentScreen = SCREEN_MENU_DOWN_POSITION;
//  }
//  else if(SCREEN_MENU_UP_POSITION.equals(currentScreen) && button->label == "UP"){
//    displayLcd("MENU", "3-Heure coucher");
//    currentScreen = SCREEN_MENU_DOWN_TIME;
//  }
//  else if(SCREEN_MENU_DOWN_POSITION.equals(currentScreen) && button->label == "DOWN"){
//    displayLcd("MENU", "1-Heure");
//    currentScreen = SCREEN_MENU_TIME;
//  }
//  else if(SCREEN_MENU_DOWN_POSITION.equals(currentScreen) && button->label == "UP"){
//    displayLcd("MENU", "4-Position haute");
//    currentScreen = SCREEN_MENU_UP_POSITION;
//  }
//
//  //Reglage de l'heure
//  else if(SCREEN_MENU_TIME.equals(currentScreen) && button->label == "OK"){
//    inputHour = ( getTime() / ONE_HOUR ) % 24 ;
//    inputMinute = ( getTime() / ONE_MINUTE ) % 60 ;
//    //inputHour = 0l;
//    //inputMinute = 0l ;
//    displayLcd("Saisie heure", String(inputHour) + ":" + String(inputMinute) );
//    currentScreen = SCREEN_DO_TIME_HOUR;
//  }
//  else if(SCREEN_DO_TIME_HOUR.equals(currentScreen) && button->label == "UP"){
//    inputHour = (inputHour + 1l) % 24l;
//    displayLcd("Saisie heure", String(inputHour) + ":" + String(inputMinute) );
//  }
//  else if(SCREEN_DO_TIME_HOUR.equals(currentScreen) && button->label == "DOWN"){
//    if(inputHour == 0){ inputHour = 23; } else {inputHour--;}
//    displayLcd("Saisie heure", String(inputHour) + ":" + String(inputMinute) );
//  }
//  else if(SCREEN_DO_TIME_HOUR.equals(currentScreen) && button->label == "OK"){
//    displayLcd("Saisie minute", String(inputHour) + ":" + String(inputMinute));
//    currentScreen = SCREEN_DO_TIME_MINUTE;
//  }
//  else if(SCREEN_DO_TIME_MINUTE.equals(currentScreen) && button->label == "UP"){
//    inputMinute = (inputMinute + 1l) % 60l;
//    displayLcd("Saisie minute", String(inputHour) + ":" + String(inputMinute));
//  }
//  else if(SCREEN_DO_TIME_MINUTE.equals(currentScreen) && button->label == "DOWN"){
//    if(inputMinute == 0){ inputMinute = 59; } else {inputMinute--;}
//    displayLcd("Saisie minute", String(inputHour) + ":" + String(inputMinute));
//  }
//  else if(SCREEN_DO_TIME_MINUTE.equals(currentScreen) && button->label == "OK"){
//
//    //TODO enregistrer heure
//    displayLcd("TODO2", "Saisie minute fini");
//
//    unsigned long time = inputHour * ONE_HOUR + inputMinute * ONE_MINUTE;
//    displayLcd("TODO2", String(time));
//    setTime(time);
//    currentScreen = SCREEN_MAIN;
//  }
//
//
//  //Fin reglage de l'heure
//
//
//
//
//  else if(SCREEN_MENU_UP_POSITION.equals(currentScreen) && button->label == "OK"){
//    displayLcd("Set Up position","DO UP OR DOWN");
//    currentScreen = SCREEN_DO_UP_POSITION;
//  }
//  else if(SCREEN_MENU_DOWN_POSITION.equals(currentScreen) && button->label == "OK"){
//    displayLcd("Set Down position","DO UP OR DOWN");
//    currentScreen = SCREEN_DO_DOWN_POSITION;
//  }
//  //Enregistrement de la position haute
//  else if(SCREEN_DO_UP_POSITION.equals(currentScreen) && button->label == "OK"){
//    storeUpPosition();
//    displayLcd("MENU","4-Position haute");
//    currentScreen = SCREEN_MENU_UP_POSITION;
//  }
//  //Enregistrement de la position basse
//  else if(SCREEN_DO_DOWN_POSITION.equals(currentScreen) && button->label == "OK"){
//    storeDownPosition();
//    displayLcd("MENU", "5-Position basse");
//    currentScreen = SCREEN_MENU_DOWN_POSITION;
//  }
//  //Ouverture manuelle de la porte
//  else if(SCREEN_MAIN.equals(currentScreen) && button->label == "UP"){
//    stepToUpPosition();
//  }
//  else if(SCREEN_MAIN.equals(currentScreen) && button->label == "DOWN"){
//    stepToDownPosition();
//  }
//
//
//
//}

//void setup() {
//
//  Serial.begin(9600);
//  timeHandler.setup();
//
//
//  pinMode(LEFT_BUTTON_PIN, INPUT);
//  pinMode(DOWN_BUTTON_PIN, INPUT);
//  pinMode(UP_BUTTON_PIN, INPUT);
//  pinMode(RIGHT_BUTTON_PIN, INPUT);
//  pinMode(OK_BUTTON_PIN, INPUT);
//
//  pinMode(LED_PIN, OUTPUT);
//
//
//
//  // set up the LCD's number of columns and rows:
//  lcd.begin(16, 2);
//  // Print a message to the LCD.
//  lcd.print("hello, world!");
//
//  monMoteur.setSpeed(9);
//
//
//}

class SayAaaLooper: public Looper {
public:
	void doLoop(){
		Serial.println("AAA");
	}

};
class SayBbbLooper: public Looper {
public:
	void doLoop(){
		Serial.println("BBB");
	}

};

class SayCccLooper: public Looper {
public:
	void doLoop(){
		Serial.println("CCC");
	}
};


Button *leftButtonBis = new ClassicButton("LEFT", LEFT_BUTTON_PIN );
Button *downButtonBis = new ClassicButton("DOWN", DOWN_BUTTON_PIN );
Button *upButtonBis = new ClassicButton("UP", UP_BUTTON_PIN );
Button *rightButtonBis = new ClassicButton("RIGHT", RIGHT_BUTTON_PIN );
Button *okButtonBis = new ClassicButton("OK", OK_BUTTON_PIN );

class TestButtonHandler : public ButtonHandler{
public:
	TestButtonHandler(Button *button): ButtonHandler(button){}
	void handleButtonPressed(){
		Serial.println(String(this->button->label + String(" pressed")));
	}
	void handleButtonReleased(){
		Serial.println(String(this->button->label + String(" released")));
	}
};


ButtonsManager *buttonsManager = new ButtonsManager();


class ButtonsLooper: public Looper {
public:
	void doLoop(){
		buttonsManager->manage();
	}
};

class TimeDisplayLooper: public Looper {
public:
	void doLoop(){
		  if(SCREEN_MAIN == currentState){
		    displayLcd( getFullHourStr(), "" );
		  }
	}
};

LoopManager loopManager;
void setup() {

	Serial.begin(9600);





	  // set up the LCD's number of columns and rows:
	  lcd.begin(16, 2);
	  // Print a message to the LCD.
//	  lcd.print("hello, world!");

	    displayLcd("hello, world 1 !", "hello, world 2 !");




	SayAaaLooper *sayAaaLooper = new SayAaaLooper();
	SayBbbLooper *sayBbbLooper = new SayBbbLooper();
	ButtonsLooper *buttonsLooper = new ButtonsLooper();
	TimeDisplayLooper *timeDisplayLooper = new TimeDisplayLooper();

//	loopManager.addLooper(sayAaaLooper, 1000);
//	loopManager.addLooper(sayBbbLooper, 3001);
	loopManager.addLooper(buttonsLooper, 50);
	loopManager.addLooper(timeDisplayLooper, 1);





	  pinMode(LEFT_BUTTON_PIN, INPUT);
	  pinMode(DOWN_BUTTON_PIN, INPUT);
	  pinMode(UP_BUTTON_PIN, INPUT);
	  pinMode(RIGHT_BUTTON_PIN, INPUT);
	  pinMode(OK_BUTTON_PIN, INPUT);

	  buttonsManager->addButtonHandler(new TestButtonHandler(leftButtonBis));
	  buttonsManager->addButtonHandler(new TestButtonHandler(downButtonBis));
	  buttonsManager->addButtonHandler(new TestButtonHandler(upButtonBis));
	  buttonsManager->addButtonHandler(new TestButtonHandler(rightButtonBis));
	  buttonsManager->addButtonHandler(new TestButtonHandler(okButtonBis));




	  Menu *menu = new Menu();
	  menu->addItem(new MenuItem( "1-Heure", SCREEN_MENU_TIME, NULL ));
	  menu->addItem(new MenuItem( "2-Heure lever", SCREEN_MENU_UP_TIME, NULL ));
	  menu->addItem(new MenuItem( "3-Heure coucher", SCREEN_MENU_DOWN_TIME, NULL ));
	  menu->addItem(new MenuItem( "4-Position haute", SCREEN_MENU_UP_POSITION, NULL ));
	  menu->addItem(new MenuItem( "5-Position basse", SCREEN_MENU_DOWN_POSITION, NULL ));

	  MenuButtonHandlersGenerator *menuButtonHandlersGenerator = new MenuButtonHandlersGenerator(
			  menu,
			  SCREEN_MAIN,
			  SCREEN_MENU_TIME,
			  okButtonBis,
			  leftButtonBis,
			  upButtonBis,
			  downButtonBis
			  );
	  LinkedList<ButtonHandler*> menuButtonHandlers = menuButtonHandlersGenerator->generateButtonHandlers();
	  Serial.println(menuButtonHandlers.size());
	  for( int i ; i < menuButtonHandlers.size() ; i++ ){
		  buttonsManager->addButtonHandler( menuButtonHandlers.get(i) );
	  }


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



void loopCheckHourPosition(){
  if( SCREEN_MAIN == currentState ){
    unsigned long time = getTime();
    if(time > downTime){
      if( currentPosition != downPosition ){
        stepToDownPosition();
      }
    }
    else if( time > upTime && currentPosition != upPosition ){
      stepToUpPosition();
    }
  }

  //if( getTime() > downTime && currentPosition != downPosition ){
  //  stepToDownPosition();
  //}
  //else if( getTime() > upTime && getTime() < downTime && currentPosition != upPosition ){
  //  stepToUpPosition();
  //}
}


int ledState = LOW;

void loopLed(){
  int currentMs = millis();
  if( currentMs > lastLedChangeTime + LED_CHANGE_INTERVAL){
    lastLedChangeTime = currentMs;
    if( ledState == LOW ){
      ledState = HIGH;
    }
    else{
      ledState = LOW;
    }
    digitalWrite( LED_PIN, ledState );
  }


}



//void loop() {
//
//
//
//	//test RTC
//
//    DateTime now = timeHandler.getCurrentDate();
//
//    Serial.print(now.year(), DEC);
//    Serial.print('/');
//    Serial.print(now.month(), DEC);
//    Serial.print('/');
//    Serial.print(now.day(), DEC);
//    Serial.print(" ");
//    Serial.print(now.hour(), DEC);
//    Serial.print(':');
//    Serial.print(now.minute(), DEC);
//    Serial.print(':');
//    Serial.print(now.second(), DEC);
//    Serial.println();
//
//
//
//
//  //monMoteur.step(2000);
//  //monMoteur.step(-2000);
//
//
//      if( ( currentScreen == SCREEN_DO_UP_POSITION || currentScreen == SCREEN_DO_DOWN_POSITION ) && digitalRead(UP_BUTTON_PIN) == HIGH ){
//        step(20);
//      }
//      else if( ( currentScreen == SCREEN_DO_UP_POSITION || currentScreen == SCREEN_DO_DOWN_POSITION ) && digitalRead(DOWN_BUTTON_PIN) == HIGH ){
//        step(-20);
//      }
//
//
//  int currentMs = millis();
//
//
//  if( currentMs > lastLedLoopTime + LOOP_LED_INTERVAL ){
//    lastLedLoopTime = currentMs;
//    loopLed();
//  }
//
//
//  if( currentMs > lastTimeDisplayTime + LOOP_TIME_DISPLAY_INTERVAL ){
//    lastTimeDisplayTime = currentMs;
//    loopDisplayTime();
//  }
//
//  if( currentMs > lastCheckHourPosition + CHECK_HOUR_POSITION_INTERVAL ){
//    lastCheckHourPosition = currentMs;
//    loopCheckHourPosition();
//  }
//
//
//
//
//  // read the state of the pushbutton value:
//  refreshButtonsState();
//
//
//  // set the cursor to column 0, line 1
//  // (note: line 1 is the second row, since counting begins with 0):
//  lcd.setCursor(0, 1);
//  // print the number of seconds since reset:
//  //lcd.print(millis()/1000);
//
//  /*
//  if (leftButtonState == HIGH) {
//    lcd.print("LEFT ");
//  }
//  else if (downButtonState == HIGH) {
//    lcd.print("DOWN ");
//  }
//  else if (upButtonState == HIGH) {
//    lcd.print("UP   ");
//  }
//  else if (rightButtonState == HIGH) {
//    lcd.print("RIGHT");
//  }
//  else if (okButtonState == HIGH) {
//    displayMenu(MAIN_MENU);
//    lcd.print("OK   ");
//  }
//  else{
//    lcd.print("     ");
//  }*/
//
//}


void loop() {

//	Serial.println("loop");

	loopManager.loop();
}
