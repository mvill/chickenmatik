#ifndef State_h
#define State_h

enum State {
	SCREEN_MAIN,
	SCREEN_MENU,
	TIME_INPUT,
	UP_TIME_INPUT,
	DOWN_TIME_INPUT,
	UP_POSITION_INPUT,
	DOWN_POSITION_INPUT
};
State currentState = SCREEN_MAIN;

#endif
