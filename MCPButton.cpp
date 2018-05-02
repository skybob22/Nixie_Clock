#include "MCPButton.h"
#include <Arduino.h>
#include <MCP23S17.h>

MCPButton::MCPButton(MCP &chip,int input_p,int light_p){
	expander_chip = &chip;
	this->light_pin = light_p;
	this->input_pin = input_p;
	expander_chip->pinMode(this->input_pin,LOW); //Low = Input
	//expander_chip->pullupMode(this->input_pin,HIGH); //High = Input Pullup Enabled
	expander_chip->pinMode(this->light_pin,HIGH); //High = Output
	light_on = false;
	this->prev_state_t = 0;
}

//Current state
bool MCPButton::getState() const{
	return (this->switch_state);
}

//Rising edge
bool MCPButton::rose() const{
	return (this->switch_rose);
}

//Falling edge
bool MCPButton::fell() const{
	return (this->switch_fell);
}

//Set light status
void MCPButton::setLight(bool light_state){
	this->light_on = light_state;
}

//Toggle switch light
void MCPButton::toggleLight(){
	this->light_on = !this->light_on;
}

//Light status
bool MCPButton::lightStatus() const{
	return (this->light_on);
}

//Update switch info
void MCPButton::update(){
	expander_chip->digitalWrite(this->light_pin,this->light_on);
	switch_state = expander_chip->digitalRead(this->input_pin);
	if (this->switch_state != this->prev_state_t){
		if (switch_state == 1){
			this->switch_rose = true;
			this->switch_fell = false;
		}
		else if (switch_state == 0){
			this->switch_fell = true;
			this->switch_rose = false;
		}
		this->prev_state_t = switch_state;
	}
	else{
		this->switch_rose = false;
		this->switch_fell = false;
	}
}