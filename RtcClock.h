#ifndef RTCClock_h
#define RTCClock_h

#include <RTClib.h>

class RTCClock{
	public:		
		RTCClock();																		//Constructs object and connects to external I2C Real-Time-Clock
		~RTCClock();
		void updateTime();														//Updates the time from the I2C Real-Time-Clock
		void setRTC(int hour, int min, int sec=0);		//Sets the time of the external RTC based on given arguments (overwrites current settings)
		void setTime(int hour, int min, int sec=0);		//Changes the time stored in this object without overwriting the external RTC
		void setAlarmState(bool state);								//Sets the alarm to be either on or off
		void toggleAlarm();														//Toggles the current alarm state, if it's on it turns off, and if it's off it turns on
		
		int getSeconds() const;
		int getMinutes() const;
		int getHours() const;													//Returns the hours in 12-hour format
		int get24Hour() const;												//Returns the hours in 24-hour format
		bool getAlarmState() const;										//Returns whether the alarm is turned on or off
		
		friend bool operator==(const RTCClock& timer1,const RTCClock& timer2);	//Checks to see if two RTCClocks have the same time
		
	private:
		RTC_DS3231 *rtc_t;
		bool alarm_on;
		bool pm;
		bool twentyfour_hour;
		int seconds;
		int minutes;
		int hours;
};

#endif