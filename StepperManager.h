#include "DataStore.h"

#ifndef StepperManager_h
#define StepperManager_h

class StepperManager{
private:
	Stepper *stepper;
	DataStore *dataStore;
public:
	StepperManager(Stepper *stepper, DataStore *dataStore){
		this->stepper = stepper;
		this->stepper->setSpeed(9);
		this->dataStore = dataStore;
	}

	void step(long stepValue) {
		dataStore->setCurrentPosition(dataStore->getCurrentPosition() + stepValue);
		stepper->step(stepValue);
	}

	void stepTo( long neededPosition ){
		this->step( neededPosition - dataStore->getCurrentPosition() );
	}
};

#endif
