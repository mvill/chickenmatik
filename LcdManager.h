#ifndef LcdManager_h
#define LcdManager_h

class LcdManager{
private :
	LiquidCrystal *lcd;
public:
	LcdManager(LiquidCrystal *lcd){
		this->lcd = lcd;
	}
	void displayLcd(String line1, String line2) {
		lcd->clear();
		lcd->setCursor(0, 0);
		lcd->print(line1);
		lcd->setCursor(0, 1);
		lcd->print(line2);
	}
};

// LcdManager instance

#endif

