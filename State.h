#ifndef State_h
#define State_h

enum State {
	SCREEN_MAIN,
	SCREEN_MENU,
	SCREEN_DO_DOWN_POSITION,
	SCREEN_DO_TIME_HOUR,
	SCREEN_DO_TIME_MINUTE
};
State currentState = SCREEN_MAIN;

#endif
