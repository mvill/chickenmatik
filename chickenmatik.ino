#include <LiquidCrystal.h>
#include "menu.h"

const int LEFT_BUTTON_PIN = 6;
const int DOWN_BUTTON_PIN = 7;
const int UP_BUTTON_PIN = 8;
const int RIGHT_BUTTON_PIN = 9;
const int OK_BUTTON_PIN = 10;

const int LED_PIN = A0;

const int NB_BUTTONS = 5;


Button_t downButton = {
      DOWN_BUTTON_PIN,
      "DOWN",
      LOW
    };
Button_t leftButton = {
      LEFT_BUTTON_PIN,
      "LEFT",
      LOW
    };
Button_t upButton = {
      UP_BUTTON_PIN,
      "UP",
      LOW
    };
Button_t rightButton = {
      RIGHT_BUTTON_PIN,
      "RIGHT",
      LOW
    };
Button_t okButton = {
      OK_BUTTON_PIN,
      "OK",
      LOW
    };
    
Button_t buttons[NB_BUTTONS] = {downButton, leftButton, upButton, rightButton, okButton};




// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);


void handleButtonPressed(Button_t* button){
  lcd.print("                   ");
  lcd.setCursor(0, 1);
  //lcd.print(button.label);
  //lcd.print(String(button->label + " HIGH"));
  if( button->state == HIGH){
    lcd.print(String(button->label + " HIGH"));
  }
  else{
    lcd.print(String(button->label + " LOW"));
  }
  
}


void refreshButtonsState(){
  
//      Serial.println(String(" si zeo f(buttons) : " + sizeof(buttons)));
      //Serial.println(sizeof(buttons));
  
  for (int i = 0; i < NB_BUTTONS; i++) {
    Button_t* button = &buttons[i];
    int newState = digitalRead(button->pin);
    if( newState !=  button->state){
        
      Serial.println("AAAA");
      //Serial.println(button.label);
      Serial.println(button->state);
      //Serial.println(String(newState));
        button->state = newState;
      Serial.println(button->state);
      Serial.println("BBBB");
      handleButtonPressed(button);
    }
  }
}

void setup() {
  
  Serial.begin(9600);
  
  pinMode(LEFT_BUTTON_PIN, INPUT);
  pinMode(DOWN_BUTTON_PIN, INPUT);
  pinMode(UP_BUTTON_PIN, INPUT);
  pinMode(RIGHT_BUTTON_PIN, INPUT);
  pinMode(OK_BUTTON_PIN, INPUT);
  
  pinMode(LED_PIN, OUTPUT);
  
  
  
  // set up the LCD's number of columns and rows: 
  lcd.begin(16, 2);
  // Print a message to the LCD.
  lcd.print("hello, world!");
  
  
  
}                          





int lastLoopTime = 0;

const int LOOP_LED_INTERVAL = 100;
const int LED_CHANGE_INTERVAL = 1000;
int lastLedLoopTime;
int lastLedChangeTime;
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




void loop() {
  
  
  int currentMs = millis();
  if( currentMs > lastLedLoopTime + LOOP_LED_INTERVAL ){
    lastLedLoopTime = currentMs;
    loopLed();
  }
  
  
  
  
  
  
  // read the state of the pushbutton value:
  refreshButtonsState();
  
  
  // set the cursor to column 0, line 1
  // (note: line 1 is the second row, since counting begins with 0):
  lcd.setCursor(0, 1);
  // print the number of seconds since reset:
  //lcd.print(millis()/1000);
  
  /*
  if (leftButtonState == HIGH) {      
    lcd.print("LEFT ");
  } 
  else if (downButtonState == HIGH) { 
    lcd.print("DOWN ");
  }
  else if (upButtonState == HIGH) { 
    lcd.print("UP   ");
  }
  else if (rightButtonState == HIGH) { 
    lcd.print("RIGHT");
  }
  else if (okButtonState == HIGH) { 
    displayMenu(MAIN_MENU);
    lcd.print("OK   ");
  }
  else{
    lcd.print("     ");
  }*/
  
}

