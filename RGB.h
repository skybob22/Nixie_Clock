#ifndef RGB_H
#define RGB_H

class RGB{
	public:
		RGB(int rPin,int gPin,int bPin);					//Creates an RGB object using the given pins
		
		void setRGB(int rVal,int gVal,int bVal);	//Sets the brightness of all colors at once (range from 0-255)
		void setRed(int rVal);										//Sets brightness of red only (range from 0-255)
		void setGreen(int gVal);									//Sets brightness of green only (range from 0-255)
		void setBlue(int bVal);										//Sets brightness of blue only (range from 0-255)
		
	private:
		void write();															//Sets the pwm of the pins stored in "pin_array" based on the values stored in "rgbVal"
		int r_pin;
		int g_pin;
		int b_pin;
		int pin_array[3];													//Stores the pins given in the constructor (order = red,green,blue)
		int rgbVal[3];														//Stores the brightness of the RGB values (0-255) (order = red,green,blue)
};

#endif