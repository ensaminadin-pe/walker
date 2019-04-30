#ifndef WIINUNCHUCK_H
#define WIINUNCHUCK_H

#include "types.h"

#define WII_BUTTON_UPDATE	0x01
#define WII_BUTTON_C		0x02
#define WII_BUTTON_Z		0x04
#define WII_JOYSTICK_ZERO	128

#define WII_FLOAT_COMPRESS_RATIO		8.0f
#define WII_FLOAT_COMPRESS_PRECISION	1000.0f

struct WiiNunchuckPacket
{ //Total = 7 bytes, 8 bytes in memory
	uint8 joystick_x;	//1 byte
	uint8 joystick_y;	//1 byte
	//Pitch and roll are between 0 and 360
	//Uint16 max = 65535
	//To store a decimal number in a uint16, we divide by 8 and multiply by 1000.
	//Ex : for    359.55487552
	//	    /8	   44.94435944
	//   *1000  44944.35944
	//  uint16  44944
	//Reverted	  359.552 : 2 decimals exact, third approximative, close enough
	//we barely need 1 decimal of precision
	uint16 pitch;		//2 bytes
	uint16 roll;		//2 bytes
	//buttons bit 1 = Is update
	//buttons bit 2 = Is C pressed
	//buttons bit 3 = Is Z pressed
	uint8 buttons;		//1 byte

	WiiNunchuckPacket()
	{
		joystick_x = WII_JOYSTICK_ZERO;
		joystick_y = WII_JOYSTICK_ZERO;
		pitch = 0;
		roll = 0;
		buttons = 0;
	}
};

class WiiNunchuck
{
	public:
		WiiNunchuck();
		void handlePacket(WiiNunchuckPacket* packet);
		WiiNunchuckPacket* buildPacket(WiiNunchuckPacket* packet);

		uint8	getJoystickX()	{ return joystick_x; }
		uint8	getJoystickY()	{ return joystick_y; }
		bool	zPressed()		{ return z; }
		bool	cPressed()		{ return c; }
		float	getPitch()		{ return pitch; }
		float	getRoll()		{ return roll; }
	private:
		static uint16 compressFloat(float value);
		static float decompressFloat(uint16 value);

		unsigned long update_time;
		uint8 joystick_x;
		uint8 joystick_y;
		bool z;
		bool c;
		float pitch;
		float roll;
};

#endif // WIINUNCHUCK_H
