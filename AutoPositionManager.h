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
					nowDate.day(), dataStore->upHours, dataStore->upMinutes, 0);
			DateTime downDate = DateTime(nowDate.year(), nowDate.month(),
					nowDate.day(), dataStore->downHours, dataStore->downMinutes,
					0);
//
			long nowTime = nowDate.secondstime();
			long upTime = upDate.secondstime();
			long downTime = downDate.secondstime();
//
			bool shouldBeUp = nowTime > upTime && nowTime < downTime;
//
			Serial.println("AAA");
//			Serial.println(shouldBeUp);
			Serial.println(stepperManager->currentPosition);
			Serial.println(dataStore->upPosition);
			Serial.println(dataStore->downPosition);
//
			if (shouldBeUp) {
				stepperManager->stepTo(dataStore->upPosition);
			}
			else{
				stepperManager->stepTo(dataStore->downPosition);
			}

		}

	}

};

#endif
