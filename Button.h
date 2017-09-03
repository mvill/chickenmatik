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


#endif
