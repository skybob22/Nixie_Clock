#include "BCD.h"
#include <Arduino.h>
#include <MCP23S17.h>

BCD::BCD(){
	//Do nothing
}

BCD::BCD(int pinA,int pinB, int pinC, int pinD){
	this->pins[0] = pinA;
	this->pins[1] = pinB;
	this->pins[2] = pinC;
	this->pins[3] = pinD;
}

void BCD::attach(int pinA,int pinB, int pinC, int pinD){
	this->pins[0] = pinA;
	this->pins[1] = pinB;
	this->pins[2] = pinC;
	this->pins[3] = pinD;
}

void BCD::toBinary(int number){
	if (number < 16){	
		this->binary[0] = floor(number/8);
		int remainder = number%8;
		this->binary[1] = floor(remainder/4);
		remainder = remainder%4;
		this->binary[2] = floor(remainder/2);
		remainder = remainder%2;
		this->binary[3] = remainder;
	}
	else{
		this->binary[0] = 0;
		this->binary[1] = 0;
		this->binary[2] = 0;
		this->binary[3] = 0;
	}
}

void BCD::write(int number){
	toBinary(number);
	for (int i=0;i<4;i++){
		digitalWrite(this->pins[i],this->binary[i]);
	}
}

void BCD::clockOut(int clock_pin){
	digitalWrite(clock_pin,HIGH);
	delay(2);
	digitalWrite(clock_pin,LOW);
}

void BCD::clockOut(int number, int clock_pin){
	write(number);
	clockOut(clock_pin);
}




MCPBCD::MCPBCD(MCP &chip,int pinA,int pinB, int pinC, int pinD){
	expansion_chip = &chip;
	this->pins[0] = pinA;
	this->pins[1] = pinB;
	this->pins[2] = pinC;
	this->pins[3] = pinD;
	
	for (int i=0;i<4;i++){
		expansion_chip->pinMode(this->pins[i],HIGH); //High means pins are outputs
	}
}

void  MCPBCD::attachClockPins(int digit1,int digit2,int digit3,int digit4,int digit5,int digit6){
	this->clock_pins[0] = digit1;
	this->clock_pins[1] = digit2;
	this->clock_pins[2] = digit3;
	this->clock_pins[3] = digit4;
	this->clock_pins[4] = digit5;
	this->clock_pins[5] = digit6;
	
	for (int i=0;i<6;i++){
		this->expansion_chip->pinMode(clock_pins[i],HIGH);//High means pins are outputs
		this->expansion_chip->digitalWrite(clock_pins[i],LOW);
	}
}


void MCPBCD::write(int number){
	toBinary(number);
	for (int i=0;i<4;i++){
		this->expansion_chip->digitalWrite(this->pins[i],this->binary[i]);
	}
}

void MCPBCD::clockOut(int digit){
	this->expansion_chip->digitalWrite(clock_pins[digit],HIGH);
	delay(2);
	this->expansion_chip->digitalWrite(clock_pins[digit],LOW);
}

void MCPBCD::clockOut(int number, int digit){
	write(number);
	clockOut(digit);
}