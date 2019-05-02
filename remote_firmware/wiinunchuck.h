#ifndef WIINUNCHUCK_H
#define WIINUNCHUCK_H

#include "types.h"

#define WII_PACKET_INFO_UPDATE	0x01
#define WII_PACKET_INFO_C		0x02
#define WII_PACKET_INFO_Z		0x04
#define WII_PACKET_INFO_PITCH	0x08
#define WII_PACKET_INFO_ROLL		0x10

#define WII_FLOAT_COMPRESS_RATIO		8.0f
#define WII_FLOAT_COMPRESS_PRECISION	1000.0f
#define WII_ACCELEROMETER_OFFSET_X		480
#define WII_ACCELEROMETER_OFFSET_Y		512

struct WiiNunchuckPacket
{ // 5 bytes
	int8 joystick_x;	//1 byte
	int8 joystick_y;	//1 byte
	int16 pitch;		//2 bytes
	int16 roll;		//2 bytes
	//infos bit 1 = Is update			0x01	 1
	//infos bit 2 = Is C pressed		0x02	 2
	//infos bit 3 = Is Z pressed		0x04	 4
	uint8 infos;		//1 byte

	WiiNunchuckPacket()
	{
		joystick_x = 0;
		joystick_y = 0;
		pitch = 0;
		roll = 0;
		infos = 0;
	}
};

class WiiNunchuck
{
	public:
		WiiNunchuck();
		bool handlePacket(WiiNunchuckPacket* packet);
		WiiNunchuckPacket* buildPacket(WiiNunchuckPacket* packet);

		void update(unsigned long diff);
		void initNunchuckReading();
		void read();

		int8	getJoystickX()	{ return joystick_x; }
		int8	getJoystickY()	{ return joystick_y; }
		bool	zPressed()		{ return z; }
		bool	cPressed()		{ return c; }
		int16	getPitch()		{ return pitch; }
		int16	getRoll()		{ return roll; }

		void	print();
	private:
		static int8		toInt8(int16 number);
		static uint8	toUint8(uint16 number);

		bool reading;
		unsigned long read_time;
		unsigned long update_time;
		int8 joystick_x;
		int8 joystick_y;
		bool z;
		bool c;
		int16 pitch;
		int16 roll;
};

#endif // WIINUNCHUCK_H
