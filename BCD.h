#ifndef BCD_h
#define BCD_h

#include <MCP23S17.h>

class BCD{
	public:
		BCD();
		BCD(int pinA,int pinB, int pinC, int pinD);
		
		void attach(int pinA,int pinB, int pinC, int pinD);
		virtual void write(int number);
		virtual void clockOut(int clock_pin);
		virtual void clockOut(int number, int clock_pin);
		
	protected:
		void toBinary(int number);
		int pins[4];
		bool binary[4];
		
};

class MCPBCD : public BCD{
	public:
		MCPBCD(MCP &chip,int pinA,int pinB, int pinC, int pinD);
		
		void attachClockPins(int digit1,int digit2,int digit3,int digit4,int digit5,int digit6);
		
		void write(int number);
		void clockOut(int digit);
		void clockOut(int number, int digit);
		
		private:
		MCP* expansion_chip;
		int clock_pins[6];
};

#endif