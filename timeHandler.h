#ifndef TimeHandler_h
#define TimeHandler_h

#include <RTClib.h>

class TimeHandler
{
	int toneFrequency;

public:
	//TimeHandler();
	void setup();
	void loop();
	DateTime getCurrentDate();
	void setCurrentDate( DateTime datetime );

	//  private:
	//  void analyse();

};

#endif
