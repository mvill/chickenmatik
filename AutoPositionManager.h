#include "LoopManager.h"

#ifndef AutoPositionManager_h
#define AutoPositionManager_h

class AutoPositionManager: public Looper {

private:
	DataStore *dataStore;
	TimeHandler *timeHandler;
	StepperManager *stepperManager;

public:
	AutoPositionManager(DataStore *dataStore, TimeHandler *timeHandler,
			StepperManager *stepperManager) {
		this->dataStore = dataStore;
		this->timeHandler = timeHandler;
		this->stepperManager = stepperManager;
	}

	void doLoop() {

		//TODO state dependant ?
//		Serial.println("ZZZ");
		if (currentState == SCREEN_MAIN) {

			//TODO eviter trop de calcul
			//ex : enregistrer upTime et downTime lors de la saisie

			DateTime nowDate = timeHandler->getCurrentDate();
			DateTime upDate = DateTime(nowDate.year(), nowDate.month(),
					nowDate.day(), dataStore->getUpHours(), dataStore->getUpMinutes(), 0);
			DateTime downDate = DateTime(nowDate.year(), nowDate.month(),
					nowDate.day(), dataStore->getDownHours(), dataStore->getDownMinutes(),
					0);
//
			long nowTime = nowDate.secondstime();
			long upTime = upDate.secondstime();
			long downTime = downDate.secondstime();
//
			bool shouldBeUp = nowTime > upTime && nowTime < downTime;
//
//			Serial.println("AAA");
//			Serial.println(shouldBeUp);
//			Serial.println(dataStore->getCurrentPosition());
//			Serial.println(dataStore->getUpPosition());
//			Serial.println(dataStore->getDownPosition());
//
			if (shouldBeUp) {
				stepperManager->stepTo(dataStore->getUpPosition());
			}
			else{
				stepperManager->stepTo(dataStore->getDownPosition());
			}

		}

	}

};

#endif
