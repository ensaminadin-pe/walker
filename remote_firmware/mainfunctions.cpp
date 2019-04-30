#include "mainfunctions.h"
#include <stdio.h>
#include "wiinunchuck.h"

#ifdef IS_QT
	#include "arduinopolyfill.h"
#else
	#include "stdpolyfill.h"
#endif

/// - This is the core of the program

static uint32 update_time;

void main_setup()
{ //Arduino like setup
	/// Setup your remote here
	//sRadioTransmitter->setup(10, 123456789); //Radio data on pin 10, can be anything
	/// Setup wii nunchuck reader
}

void main_loop()
{ //Arduino like loop
	//1) Setup loop diff time
	if (update_time == 0)
	{
		update_time = millis();
		return;
	}

	//2) Calculate loop diff time
	int diff = millis() - update_time;
	if (diff <= 1) //Less than 1ms, delay update, we need at least 1ms of diff
	{
		delay(1);
		diff = 1;
	}
	update_time = millis();

	//3) Add control logic here
	//   - remote control actions to set next gait
	//   - sensor detection and whatnot

	//4) Run walker update loop
}
