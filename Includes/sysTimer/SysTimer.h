#ifndef SYSTIMER_H
#define SYSTIMER_H

class sysTimer{
	public:
		sysTimer();															//Default constructor, takes in no arguments and sets default timer to 1sec		
		sysTimer(unsigned long num_millis);			//Optional constructor, creates timer and sets time to num_millis
		
		void setTimer(unsigned long interval);	//Changes the time between alarms
		unsigned long getTimerSet();						//Returns the current timer value (number of milliseconds between alarms)
		void zero();														//Zeros the clock so that it starts counting from whenever function is called
		bool timerAlarm();											//Checks to see if "timer_target" number of milliseconds has passed since the last alarm trigger
		
		explicit operator bool();								//Same as timerAlarm()
		
	private:
		unsigned long timer_target;
		unsigned long prev_millis;
		
};

#endif