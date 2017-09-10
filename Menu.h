#include <Arduino.h>
#include "LcdManager.h"

#ifndef Menu_h
#define Menu_h

class Executable {
public:
	void virtual execute() {

	}
};

class MenuItem {
public:
	String label;
	Executable *handler;
	MenuItem(String label, Executable *handler) {
		this->label = label;
		this->handler = handler;
	}
};

class Menu {
private:
public:
	Executable *callbackCancel;
	LinkedList<MenuItem*> items = LinkedList<MenuItem*>();
	Menu( Executable *callbackCancel ){
		this->callbackCancel = callbackCancel;
	}
	void addItem(MenuItem *item) {
		items.add(item);
	}
};


class MenuManager : public GlobalHandler{
private:
	ButtonsManager *buttonsManager;
	LcdManager *lcdManager;
	Menu *menu;
	Button *okButton;
	Button *backButton;
	Button *previousButton;
	Button *nextButton;

	bool showing = false;
	MenuItem *currentItem;

	int getItemIndex(MenuItem *item){
		int res = -1;
		for( int i=0 ; res == -1 && i<menu->items.size() ; i++ ){
			MenuItem *curItem = menu->items.get(i);
			if( item->label.equals(curItem->label) ){
				res = i;
			}
		}
		return res;
	}
	MenuItem* getPreviousItem(MenuItem* fromItem) {
		int neededIndex = getItemIndex(fromItem) - 1;
		if (neededIndex < 0) {
			neededIndex = neededIndex + menu->items.size();
		}
		return menu->items.get(neededIndex);
	}
	MenuItem* getNextItem(MenuItem* fromItem){

		int neededIndex = getItemIndex(fromItem) + 1;
		if (neededIndex >= menu->items.size()) {
			neededIndex = neededIndex - menu->items.size();
		}
		return menu->items.get(neededIndex);
	}
	void showItem( MenuItem* item ){
		currentItem = item;
		this->lcdManager->displayLcd("MENU", item->label);
	}
public:

	bool mustCheck(){
		return showing;
	}

	void handleButtonPressed(Button *button){
		if( button == okButton ){
			Serial.println("XXX");
			showing = false;
			currentItem->handler->execute();
		}
		else if(button == nextButton){
			MenuItem *nextItem = getNextItem( currentItem );
			showItem(nextItem);
		}
		else if(button == previousButton){
			MenuItem *previousItem = getPreviousItem(currentItem);
			showItem(previousItem);
		}
		else if( button == backButton ){
			showing = false;
			menu->callbackCancel->execute();
		}
	}

	void show(){
		showItem(menu->items.get(0));
		showing = true;
	}

	MenuManager(ButtonsManager *buttonsManager, LcdManager *lcdManager,
			Menu *menu, Button *okButton, Button *backButton,
			Button *previousButton, Button *nextButton){


		this->buttonsManager = buttonsManager;
		this->lcdManager = lcdManager;
		this->menu = menu;
		this->okButton = okButton;
		this->backButton = backButton;
		this->previousButton = previousButton;
		this->nextButton = nextButton;
		this->showing = false;

		buttonsManager->addGlobalHandler(this);
	}
};


#endif
