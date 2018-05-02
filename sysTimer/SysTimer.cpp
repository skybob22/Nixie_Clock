#include "sysTimer.h"
#include <arduino.h>

sysTimer::sysTimer(){
	this->timer_target = 1000;
	this->prev_millis = millis();
}

sysTimer::sysTimer(unsigned long num_millis){
	this->timer_target = num_millis;
	this->prev_millis = millis();
}

bool sysTimer::timerAlarm(){
	return (static_cast<bool>(*this));
}

void sysTimer::setTimer(unsigned long interval){
	this->timer_target = interval;
}

void sysTimer::zero(){
	this->prev_millis = millis();
}

sysTimer::operator bool(){
	if (millis() - prev_millis >= timer_target){
		prev_millis = millis();
		return (true);
	}
	else{
		return (false);
	}
}

unsigned long sysTimer::getTimerSet(){
	return (timer_target);
}