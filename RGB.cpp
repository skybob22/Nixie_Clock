#include "RGB.h"
#include <Arduino.h>

RGB::RGB(int rPin,int gPin,int bPin){
	this->r_pin = rPin;
	this->pin_array[0] = rPin;
	this->g_pin = gPin;
	this->pin_array[1] = gPin;
	this->b_pin = bPin;
	this->pin_array[2] = bPin;
	for (int i=0;i<3;i++){
		pinMode(pin_array[i],OUTPUT);
		rgbVal[i] = 0;
		analogWrite(pin_array[i],rgbVal[i]);
	}
}

void RGB::write(){
	for (int i=0;i<3;i++){
		analogWrite(pin_array[i],rgbVal[i]);
	}
}

void RGB::setRGB(int rVal,int gVal,int bVal){
	this->rgbVal[0] = rVal;
	this->rgbVal[1] = gVal;
	this->rgbVal[2] = bVal;
	this->write();
}

void RGB::setRed(int rVal){
	this->rgbVal[0] = rVal;
	this->write();
}

void RGB::setGreen(int gVal){
	this->rgbVal[1] = gVal;
	this->write();
}

void RGB::setBlue(int bVal){
	this->rgbVal[2] = bVal;
	this->write();
}