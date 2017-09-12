#ifndef StepperManager_h
#define StepperManager_h

class StepperManager{
private:
	Stepper *stepper;
public:
	long currentPosition = 0;
	StepperManager(Stepper *stepper){
		this->stepper = stepper;
		this->stepper->setSpeed(9);
	}

	void step(long stepValue) {
		currentPosition += stepValue;
		stepper->step(stepValue);
	}

	void stepTo( long neededPosition ){
		this->step( neededPosition - currentPosition );
	}
};

#endif
