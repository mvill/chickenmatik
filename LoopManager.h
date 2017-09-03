#include "LinkedList.h"

#ifndef LoopManager_h
#define LoopManager_h

class Looper{
   public:
      // pure virtual function
      virtual void doLoop();
};

class Toto{
public:
	unsigned long interval;
	Looper* looper;
	unsigned long lastExec = -1;
	Toto(){

	}
	Toto( Looper* looper, unsigned long interval ){
		this->looper = looper;
		this->interval = interval;
	}
};

class LoopManager{
private:
	LinkedList<Toto*> totos;
public:
	LoopManager();
	void addLooper( Looper *looper, unsigned long interval );
	void loop();
};

#endif LoopManager_h
