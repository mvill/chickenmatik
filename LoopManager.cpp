#include <Arduino.h>
#include "LinkedList.h"
#include "LoopManager.h"

LoopManager::LoopManager(){
	this->totos = LinkedList<Toto*>();
}

void LoopManager::addLooper(Looper *looper, unsigned long interval){
	Toto *toto = new Toto( looper, interval );
	this->totos.add(toto);
}

void LoopManager::loop(){

	unsigned long currentMs = millis();
	int totosSize = totos.size();
	for( int i = 0 ; i < totosSize ; i++ ){

		Toto* currentToto = totos.get(i);

		if( currentMs > currentToto->lastExec + currentToto->interval ){
			currentToto->lastExec = currentMs;

			currentToto->looper->doLoop();
		}

	}
}

