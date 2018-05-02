//-----------------------------------------------------------------Libraries---------------------------------------------------------------------//
#include <SPI.h>
#include <MCP23S17.h>
#include <SysTimer.h>
#include <Wire.h>


#include <LiquidCrystal_I2C.h>


#include "RtcClock.h"
#include "BCD.h"
#include "MCPButton.h"
#include "RGB.h"

//------------------------------------------------------Definitions & Global Variables----------------------------------------------------------------//
#define aDigit 4 //Expander
#define bDigit 3 //Expander
#define cDigit 2 //Expander
#define dDigit 1 //Expander

#define hourOne 5 //Expander
#define hourTwo 6 //Expander
#define minuteOne 7 //Expander
#define minuteTwo 8 //Expander
#define secondOne 9 //Expander
#define secondTwo 10 //Expander
#define pmInd 11 //Expander PM Indicator

#define colonOneTop 12 //Expander
#define colonOneBottom 13 //Expander
#define colonTwoTop 14 //Expander
#define colonTwoBottom 15 //Expander
//Colons not used

//Pins 10,11,12,13,A4 & A5 are taken
#define rgbRedPin 5 //Arduino
#define rgbGreenPin 6 //Aduino
#define rgbBluePin 9 //Arduino

#define buttonOne 1 //Expander 2 - Alarm On/Off
#define buttonOneLight 10 //Expander 2
#define buttonTwo 3 //Expander 2 - Hours
#define buttonTwoLight 6 //Expander 2
#define buttonThree 5 //Expander 2 - Minutes
#define buttonThreeLight 8 //Expander 2
#define buttonFour 7 //Expander 2 - Alarm Set
#define buttonFourLight 4 //Expander 2
#define buttonFive 9 //Expander 2 - Clock Set
#define buttonFiveLight 2 //Expander 2

#define alarm_buzzer 3 //Arduino
//#define alarmTriggerType A0 //Ardunio (Active High vs Active Low)

//Expander LED pin = 16

#define buttonSetDelay 400 //Number of milliseconds in between number increases
#define clock_update_freq 200 //Number of milliseconds in between clock updates
#define buzzer_delay 400

enum clockstate{
	run_clock,
	set_clock,
	set_alarm,
	alarm_sounding,
	test_state,
};

MCP chipOne(1,10); //SPI expansion chip with adress 1, SS on pin 10
MCP chipTwo(2,10); //SPI expansion chip with adress 2, SS on pin 10


//-----------------Function Declarations----------------//
void displayTime(RTCClock& display_clock,MCPBCD& display);
void timeAdjust(RTCClock& display_clock,MCPButton& hours,MCPButton& minutes,MCPBCD& display);


void setup(){
	pinMode(alarm_buzzer,OUTPUT);
	chipOne.begin();
	chipTwo.begin();
	Serial.begin(9600);
}


//-------------------------------------------------Main Program--------------------------------------------//
void loop(){
	//Declare Variables
	static RGB clockLEDs(rgbRedPin,rgbGreenPin,rgbBluePin);
	
	static MCPButton alarmToggleButton(chipTwo,buttonOne,buttonOneLight);
	static MCPButton hourButton(chipTwo,buttonTwo,buttonTwoLight);
	static MCPButton minuteButton(chipTwo,buttonThree,buttonThreeLight);
	static MCPButton alarmSetButton(chipTwo,buttonFour,buttonFourLight);
	static MCPButton clockSetButton(chipTwo,buttonFive,buttonFiveLight);
	static MCPButton* array_b[] = {&alarmToggleButton,&hourButton,&minuteButton,&alarmSetButton,&clockSetButton};

	
	static MCPBCD displayBCD(chipOne,aDigit,bDigit,cDigit,dDigit);
	//static int digitPin[] = {hourOne,hourTwo,minuteOne,minuteTwo,secondOne,secondTwo};
	
	static RTCClock clock_time;
	static RTCClock alarm_time;
	static clockstate c_state = run_clock;
	
	static bool initialize = true;
	if (initialize == true){
		displayBCD.attachClockPins(hourOne,hourTwo,minuteOne,minuteTwo,secondOne,secondTwo);
		initialize = false;
	}
	
	
	
	
	for (int i=0;i<5;i++){
		array_b[i]->update();
	}
	
	static int run_number = 1;
	switch(c_state){
		case alarm_sounding:
			static sysTimer alarmInterval(buzzer_delay);
			if (run_number = 1){
				run_number = 0;
			}
			
			if (alarmInterval){
				static bool alarm_beep = 1;
				analogWrite(alarm_buzzer,100*alarm_beep);
				alarm_beep = !alarm_beep;
			}
			
			if (alarmToggleButton.fell()){
				clock_time.setAlarmState(false);
				analogWrite(alarm_buzzer,0);
				c_state = run_clock;
			}
			//Break statement intentionally missing so that the clock will continue to run while alarm is sounding
			
			
			
		case run_clock:
			static sysTimer clockUpdate(clock_update_freq);
			if (run_number = 1){
				run_number = 0;
			}
			
			if (clockUpdate){
				clock_time.updateTime();		
				displayTimeFunction(clock_time,displayBCD);
				/*int displayTime[6];
				displayTime[0] = floor(clock_time.getHours()/10);
				displayTime[1] = clock_time.getHours()%10;
				displayTime[2] = floor(clock_time.getMinutes()/10);
				displayTime[3] = clock_time.getMinutes()%10;
				displayTime[4] = floor(clock_time.getSeconds()/10);
				displayTime[5] = clock_time.getSeconds()%10;
				
				for (int i=0;i<6;i++){
					displayBCD.clockOut(displayTime[i],i);
				}*/
				
				if (clock_time.get24Hour() >= 12){
					clockLEDs.setRGB(0,0,255);
				}
				else{
					clockLEDs.setRGB(0,255,0);
				}
				
			}
			
			//Toggle the alarm on and off
			if (alarmToggleButton.fell()){
				alarm_time.toggleAlarm();
				alarmToggleButton.setLight(alarm_time.getAlarmState());
			}
			
			//Check to see if it's time for alarm to go off
			if (clock_time == alarm_time && alarm_time.getAlarmState() == true){
				c_state = alarm_sounding;
				run_number = 1;
			}
			
			//Set Clock Time
			if (clockSetButton.fell()){
				c_state = set_clock;
				run_number = 1;
			}
			
			//Set alarm time
			else if (alarmSetButton.fell()){
				c_state = set_alarm;
				run_number = 1;
			}
			
			break;
		
		
		
		case set_clock:
			static bool has_changed = false;
			if (run_number == 1){
				clockSetButton.setLight(true);
				hourButton.setLight(true);
				minuteButton.setLight(true);
				run_number = 0;
			}
			
			static sysTimer clock_set(100);
			if (clock_set){		
				if(!has_changed){
					clock_time.updateTime();
				}		
				displayTimeFunction(clock_time,displayBCD);
				
				if (clock_time.get24Hour() >= 12){
					clockLEDs.setRGB(255,0,255);
				}
				else{
					clockLEDs.setRGB(255,255,0);
				}
			}
			
			if(hourButton.getState()==0||minuteButton.getState()==0){
				timeAdjust(clock_time,hourButton,minuteButton,displayBCD);
				if (clock_time.get24Hour() >= 12){
					clockLEDs.setRGB(255,0,255);
				}
				else{
					clockLEDs.setRGB(255,255,0);
				}
				has_changed = true;
			}
			
			if (clockSetButton.getState() == true){
				c_state = run_clock;
				clockSetButton.setLight(false);
				hourButton.setLight(false);
				minuteButton.setLight(false);
				run_number = 1;
				if (has_changed){
					clock_time.setRTC(clock_time.get24Hour(),clock_time.getMinutes());
				}
				has_changed = false;
			}
			break;
		
		
		
		case set_alarm:
			if (run_number == 1){
				alarmSetButton.setLight(true);
				hourButton.setLight(true);
				minuteButton.setLight(true);
				run_number = 0;
			}
			
			static sysTimer alarm_set(100);
			if (alarm_set){				
				displayTimeFunction(alarm_time,displayBCD);
				
				if (alarm_time.get24Hour() >= 12){
					clockLEDs.setRGB(255,0,255);
				}
				else{
					clockLEDs.setRGB(255,255,0);
				}
			}
			
			if(hourButton.getState()==0||minuteButton.getState()==0){
				timeAdjust(alarm_time,hourButton,minuteButton,displayBCD);
				if (alarm_time.get24Hour() >= 12){
					clockLEDs.setRGB(255,0,255);
				}
				else{
					clockLEDs.setRGB(255,255,0);
				}
			}
			
			if (alarmSetButton.getState() == true){
				c_state = run_clock;
				alarmSetButton.setLight(false);
				hourButton.setLight(false);
				minuteButton.setLight(false);
				run_number = 1;
			}
			break;
			
			
			
		case test_state:
			//Testing and/or debug code here	
			break;
		
		
		default:
			//Do nothing, should not get here
			break;
	}
}

void displayTimeFunction(const RTCClock& display_clock,MCPBCD& display){
	int displayTime[6];
	displayTime[0] = floor(display_clock.getHours()/10);
	displayTime[1] = display_clock.getHours()%10;
	displayTime[2] = floor(display_clock.getMinutes()/10);
	displayTime[3] = display_clock.getMinutes()%10;
	displayTime[4] = floor(display_clock.getSeconds()/10);
	displayTime[5] = display_clock.getSeconds()%10;
	
	for (int i=0;i<6;i++){
		display.clockOut(displayTime[i],i);
	}
}

void timeAdjust(RTCClock& display_clock,MCPButton& hours,MCPButton& minutes,MCPBCD& display){
	if (hours.fell()){
		if (display_clock.get24Hour()<23){
			display_clock.setTime(display_clock.get24Hour()+1,display_clock.getMinutes());
		}
		else{
			display_clock.setTime(0,display_clock.getMinutes());
		}
	}
	else if (minutes.fell()){
		if (display_clock.getMinutes()<59){
			display_clock.setTime(display_clock.get24Hour(),display_clock.getMinutes()+1);
		}
		else{
			display_clock.setTime(display_clock.get24Hour(),0);
		}
	}
	
	displayTimeFunction(display_clock,display);
}


