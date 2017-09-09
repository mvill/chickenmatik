#include "LcdManager.h"

#ifndef Menu_h
#define Menu_h

class Executable {
	void virtual execute() {

	}
};

class MenuItem {
public:
	String label;
	State menuState;
	Executable *handler;
	MenuItem(String label, State menuState, Executable *handler) {
		this->label = label;
		this->menuState = menuState;
		this->handler = handler;
	}
};

class Menu {
public:
	LinkedList<MenuItem*> items = LinkedList<MenuItem*>();
	void addItem(MenuItem *item) {
		items.add(item);
	}
};

class MenuBackButtonHandler: public ButtonHandler {
private:
	Menu *menu;
	State initialState;
public:
	MenuBackButtonHandler(Menu *menu, Button *button, State initialState) :
			ButtonHandler(button) {
		this->menu = menu;
		this->initialState = initialState;
	}

	bool mustCheck() {
		bool res = false;
		for( int i = 0 ; !res && i < this->menu->items.size() ; i++ ){
			res = (currentState == this->menu->items.get(i)->menuState);
		}
		return res;
	}
	void handleButtonPressed() {
		currentState = initialState;
	}
};

class MenuShowButtonHandler: public ButtonHandler {
private:
	LcdManager *lcdManager;
	State initialState;
	State menuState;
public:
	MenuShowButtonHandler(LcdManager *lcdManager, Button *button,
			State initialState, State menuState) :
			ButtonHandler(button) {
		this->lcdManager = lcdManager;
		this->initialState = initialState;
		this->menuState = menuState;
	}

	bool mustCheck() {
//		Serial.println("MenuShowButtonHandler.mustCheck");
		return currentState == initialState;
	}
	void handleButtonPressed() {
//		Serial.println("MenuShowButtonHandler.handleButtonPressed");
		currentState = menuState;
		this->lcdManager->displayLcd("MENU", "1-Heure");
	}
};

class MenuNavButtonHandler: public ButtonHandler {
private:
	LcdManager *lcdManager;
	State initialState;
	MenuItem *menuItem;
public:
	MenuNavButtonHandler(LcdManager *lcdManager, Button *button,
			State initialState, MenuItem *menuItem) :
			ButtonHandler(button) {
		this->lcdManager = lcdManager;
		this->initialState = initialState;
		this->menuItem = menuItem;
	}
	bool mustCheck() {
		return currentState == initialState;
	}
	void handleButtonPressed() {
		currentState = menuItem->menuState;
		this->lcdManager->displayLcd("MENU", menuItem->label);
	}
};

class MenuButtonHandlersGenerator {
private:
	LcdManager *lcdManager;
	Menu *menu;
	State initialState;
	State menuState;
	Button *okButton;
	Button *backButton;
	Button *previousButton;
	Button *nextButton;
	MenuItem* getPreviousItem(int currentItemIndex) {
		int neededIndex = currentItemIndex - 1;
		//TODO do it with modulos
		if (neededIndex < 0) {
			neededIndex = neededIndex + menu->items.size();
		}
		return menu->items.get(neededIndex);
	}
	MenuItem* getNextItem(int currentItemIndex) {
		int neededIndex = currentItemIndex + 1;
		//TODO do it with modulos
		if (neededIndex >= menu->items.size()) {
			neededIndex = neededIndex - menu->items.size();
		}
		return menu->items.get(neededIndex);
	}

public:
	MenuButtonHandlersGenerator(LcdManager *lcdManager, Menu *menu,
			State initialState, State menuState, Button *okButton,
			Button *backButton, Button *previousButton, Button *nextButton) {
		this->lcdManager = lcdManager;
		this->menu = menu;
		this->initialState = initialState;
		this->menuState = menuState;
		this->okButton = okButton;
		this->backButton = backButton;
		this->previousButton = previousButton;
		this->nextButton = nextButton;
	}
	LinkedList<ButtonHandler*> generateButtonHandlers() {
		LinkedList<ButtonHandler*> buttonHandlers =
				LinkedList<ButtonHandler*>();

		// Back to main screen
		MenuBackButtonHandler *menuBackButtonHandler =
				new MenuBackButtonHandler(menu, backButton, initialState);
		buttonHandlers.add(menuBackButtonHandler);

		// Main screen to first menu item
		MenuShowButtonHandler *menuShowButtonHandler =
				new MenuShowButtonHandler(lcdManager, okButton, initialState,
						menuState);
		buttonHandlers.add(menuShowButtonHandler);

		int nbItems = menu->items.size();
		for (int i = 0; i < nbItems; i++) {
			MenuItem *currentMenuItem = menu->items.get(i);
			MenuItem *previousMenuItem = getPreviousItem(i);
			MenuItem *nextMenuItem = getNextItem(i);
			MenuNavButtonHandler *previousButtonHandler =
					new MenuNavButtonHandler(lcdManager, nextButton,
							previousMenuItem->menuState, currentMenuItem);
			MenuNavButtonHandler *nextButtonHandler = new MenuNavButtonHandler(
					lcdManager,
					previousButton, nextMenuItem->menuState, currentMenuItem);

			buttonHandlers.add(previousButtonHandler);
			buttonHandlers.add(nextButtonHandler);

		}
		return buttonHandlers;
	}
};

#endif
