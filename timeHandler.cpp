
#include <RTClib.h>
#include "timeHandler.h"

RTC_DS1307 rtc;

//TimeHandler::TimeHandler(){
//
//}

void TimeHandler::setup(){

	if (! rtc.begin()) {
		Serial.println("Couldn't find RTC");
		while (1);
	}

	if (! rtc.isrunning()) {
		Serial.println("RTC is NOT running!");
		while (1);
	}
//	rtc.adjust(DateTime(2017, 9, 2, 20, 00, 00));
}

void TimeHandler::loop(){

}

void TimeHandler::setCurrentDate(DateTime datetime){
	rtc.adjust(datetime);
}

DateTime TimeHandler::getCurrentDate(){
	return rtc.now();
}
