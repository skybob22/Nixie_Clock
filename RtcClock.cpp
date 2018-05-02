#include "RtcClock.h"
#include <Arduino.h>
#include <RTClib.h>
#include <SysTimer.h>

RTCClock::RTCClock(){
	this->rtc_t = new RTC_DS3231;
	this->rtc_t->begin();
	if (this->rtc_t->lostPower()) {
    //Serial.println("RTC lost power, lets set the time!");
    // following line sets the RTC to the date & time this sketch was compiled
    this->rtc_t->adjust(DateTime(F(__DATE__), F(__TIME__)));
    // This line sets the RTC with an explicit date & time, for example to set
    // January 21, 2014 at 3am you would call:
    // rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
  }
	
	this->seconds = 0;
	this->minutes = 0;
	this->hours = 0;
	this->twentyfour_hour = false;
	this->pm = false;
	this->alarm_on = false;
}
RTCClock::~RTCClock(){
	delete rtc_t;
}

//Set current time equal to RTC time
void RTCClock::updateTime(){	
	DateTime current_time;
	current_time = rtc_t->now();
	this->seconds = current_time.second();
	this->minutes = current_time.minute();
	this->hours = current_time.hour();
	//Has this for 12 hour time, even if in 24 hour, doesnt hurt to have
	if (hours >= 12){
		pm = true;
	}
	else{
		pm = false;
	}
}

//Sets the state for if an alarm is turned on (Only used for the alarm timer)
void RTCClock::setAlarmState(bool state){
	this->alarm_on = state;
}

//Toggles the alarm on and off
void RTCClock::toggleAlarm(){
	this->alarm_on = !this->alarm_on;
}

//Sets the current time for the timer without updating the RTC
void RTCClock::setTime(int hour, int min, int sec){
	this->hours = hour;
	this->minutes = min;
	this->seconds = sec;
}

//Sets the RTC to a new time
void RTCClock::setRTC(int hour, int min, int sec){
	DateTime ref = rtc_t->now(); //Uses same Date, Month, Year
	int year = ref.year();
	int month = ref.month();
	int day = ref.day();
	rtc_t->adjust(DateTime(year, month, day, hour, min, sec));
	updateTime();
}

bool RTCClock::getAlarmState() const{
	return (this->alarm_on);
}

//Get current seconds
int RTCClock::getSeconds() const{
	return (this->seconds);
}

//Get current minutes
int RTCClock::getMinutes() const{
	return(this->minutes);
}

//Get hours in whatever the clock format is set to
int RTCClock::getHours() const{
	if (this->twentyfour_hour == false){
		if (this->hours>12){
			return (this->hours-12);
		}
		else if (this->hours==0){
			return (12);
		}
		else{
			return (this->hours);
		}
	}
	else{
		return (this->hours);
	}
}

//Get hours formatted to 24 hour
int RTCClock::get24Hour() const{
	return (this->hours);
}

//Overloaded == operator (2 timers)
bool operator==(const RTCClock& timer1,const RTCClock& timer2){
	return (timer1.hours == timer2.hours && timer1.minutes == timer2.minutes && timer1.seconds == timer2.seconds);
}