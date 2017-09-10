#ifndef Button_h
#define Button_h

class Button {
public:
	String label;
	Button( String label ){
		this->label = label;
	}
	//TODO virtual with impl? !
	virtual bool isPressed(){
		Serial.println("isPressed Button");

	}
};

class AnalogButton: public Button {
private:
	int pin;
	int minAnalogValue;
	int maxAnalogValue;
public:
	AnalogButton(String label, int pin, int minAnalogValue, int maxAnalogValue):Button(label){
		this->pin = pin;
		this->minAnalogValue = minAnalogValue;
		this->maxAnalogValue = maxAnalogValue;
	}
	bool isPressed(){
		int state = analogRead(pin);
		bool result = state <= maxAnalogValue && state >= minAnalogValue;
		return result;
	}
};

class ClassicButton: public Button {
private:
	int pin;
public:
	ClassicButton(String label, int pin):Button(label){
		this->pin = pin;
	}
	bool isPressed(){
		int state = digitalRead(pin);
		bool result = state == HIGH;
		return result;
	}
};

#endif
