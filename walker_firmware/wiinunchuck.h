#ifndef WIINUNCHUCK_H
#define WIINUNCHUCK_H

#include "types.h"

#define WII_PACKET_INFO_UPDATE	0x01
#define WII_PACKET_INFO_C		0x02
#define WII_PACKET_INFO_Z		0x04
#define WII_PACKET_INFO_PITCH	0x08
#define WII_PACKET_INFO_ROLL	0x10

#define WII_FLOAT_COMPRESS_RATIO		8.0f
#define WII_FLOAT_COMPRESS_PRECISION	1000.0f
#define WII_ACCELEROMETER_OFFSET_X		480
#define WII_ACCELEROMETER_OFFSET_Y		512

#define MOVEMENT_UP_LOW		1
#define MOVEMENT_UP_MID		2
#define MOVEMENT_UP_HIGH	4
#define MOVEMENT_DOWN_LOW	8
#define MOVEMENT_DOWN_MID	16
#define MOVEMENT_DOWN_HIGH	32
#define MOVEMENT_LEFT_LOW	64
#define MOVEMENT_LEFT_MID	128
#define MOVEMENT_LEFT_HIGH	256
#define MOVEMENT_RIGHT_LOW	512
#define MOVEMENT_RIGHT_MID	1024
#define MOVEMENT_RIGHT_HIGH	2048

#define MOVEMENT_LOW_THRESHOLD	20
#define MOVEMENT_MID_THRESHOLD	60
#define MOVEMENT_HIGH_THRESHOLD	80

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
		//Methods
		WiiNunchuck();
		// - Communication
		bool				handlePacket(WiiNunchuckPacket* packet);
		WiiNunchuckPacket*	buildPacket(WiiNunchuckPacket* packet);
		// - Read hardware
		void				update(unsigned long diff);
		void				initNunchuckReading();
		void				read();
		// - Getters
		int8				getJoystickX()	{ return joystick_x; }
		int8				getJoystickY()	{ return joystick_y; }
		unsigned int		getJoystickMovement() { return joystick_movement; }
		bool				zPressed()		{ return z; }
		bool				cPressed()		{ return c; }
		int16				getPitch()		{ return pitch; }
		int16				getRoll()		{ return roll; }
		// - Debug
		void				print();
	private:
		//Methods
		static int8		toInt8(int16 number);
		static uint8	toUint8(uint16 number);
		void			readJoystick();
		unsigned int	mapMovementValue(unsigned int base, signed short joystick_value);
		//Properties
		unsigned int	joystick_movement; //Joystick position tanslated to movement flags
		bool			reading;
		unsigned long	read_time;
		unsigned long	update_time;
		int8			joystick_x;
		int8			joystick_y;
		bool			z;
		bool			c;
		int16			pitch;
		int16			roll;

};

#endif // WIINUNCHUCK_H
