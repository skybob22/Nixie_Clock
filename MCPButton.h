#ifndef MCPButton_h
#define MCPButton_h

#include <MCP23S17.h>

class MCPButton{
	public:
		MCPButton(MCP &chip,int input_p,int light_p);
		
		bool getState() const;
		bool rose() const;
		bool fell() const;
		
		void setLight(bool light_state);
		void toggleLight();
		bool lightStatus() const;
		
		void update();
		
	private:
	MCP* expander_chip;	
	
	bool light_on;
	int light_pin;
	int input_pin;
	bool prev_state_t;
	bool switch_state;
	bool switch_rose;
	bool switch_fell;
};

#endif