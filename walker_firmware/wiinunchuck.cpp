#include "wiinunchuck.h"
#include "arduinopolyfill.h"

#ifndef IS_QT
	#include <Wire.h>
	#include "nunchuck.h"
#endif

WiiNunchuck::WiiNunchuck()
{
	joystick_x = 0;
	joystick_y = 0;
	z = false;
	c = false;
	pitch = 0;
	roll = 0;
	reading = false;
	read_time = 20;
}

bool WiiNunchuck::handlePacket(WiiNunchuckPacket *packet)
{
	if (!(packet->infos & WII_PACKET_INFO_UPDATE))
		return false; //Blank packet, skip

	joystick_x = packet->joystick_x;
	joystick_y = packet->joystick_y;
	readJoystick();
	pitch = packet->pitch;
	roll = packet->roll;

	//C is on when bit 1 is toggled on in buttons
	c = packet->infos & WII_PACKET_INFO_C;
	//Z is on when bit 2 is toggled on in buttons
	z = packet->infos & WII_PACKET_INFO_Z;

	update_time = millis();
	return true;
}

WiiNunchuckPacket* WiiNunchuck::buildPacket(WiiNunchuckPacket* packet)
{
	packet->infos += WII_PACKET_INFO_UPDATE;

	packet->joystick_x = joystick_x;
	packet->joystick_y = joystick_y;
	packet->pitch = pitch;
	packet->roll = roll;

	if (c)
		packet->infos += WII_PACKET_INFO_C;
	if (z)
		packet->infos += WII_PACKET_INFO_Z;

	return packet;
}

void WiiNunchuck::update(unsigned long diff)
{
	if (!reading)
		return;

	if (read_time <= diff)
	{
		read();
		read_time = 20;
	}
	else
		read_time -= diff;
}

void WiiNunchuck::initNunchuckReading()
{ //Setup nunchuck readon on I2C port
	#ifndef IS_QT
		Wire.begin();
		Wire.setClock(400000);
		nunchuk_init();
	#endif
	reading = true;
}

void WiiNunchuck::read()
{
	if (!reading)
		return;
	#ifndef IS_QT
	if (nunchuk_read())
	{
		joystick_x =  toInt8(nunchuk_joystickX());
		joystick_y = toInt8(nunchuk_joystickY());

		pitch = (int16)(nunchuk_accelX_raw() - WII_ACCELEROMETER_OFFSET_X);
		roll = (int16)(nunchuk_accelY_raw() - WII_ACCELEROMETER_OFFSET_Y);
		c = nunchuk_buttonC();
		z = nunchuk_buttonZ();
	}
	#endif
}

void WiiNunchuck::print()
{
	#ifndef IS_QT
		Serial.print(joystick_x, DEC);
		Serial.print(",");
		Serial.print(joystick_y, DEC);
		Serial.print(",");
		Serial.print(pitch, DEC);
		Serial.print(",");
		Serial.print(roll, DEC);
		Serial.print(",");
		Serial.print(c, DEC);
		Serial.print(",");
		Serial.print(z, DEC);
		Serial.print("\n");
#endif
}

int8 WiiNunchuck::toInt8(int16 number)
{
	if (number > 127)
		return 127;
	if (number < -128)
		return -128;
	return (int8)number;
}

uint8 WiiNunchuck::toUint8(uint16 number)
{
	if (number > 255)
		return 255;
	return (uint8)number;
}

void WiiNunchuck::readJoystick()
{
	unsigned int _movement = 0;
	if (joystick_x > 0)
		_movement += mapMovementValue(MOVEMENT_RIGHT_LOW, joystick_x);
	else
		_movement += mapMovementValue(MOVEMENT_LEFT_LOW, joystick_x);
	if (joystick_y > 0)
		_movement += mapMovementValue(MOVEMENT_UP_LOW, joystick_y);
	else
		_movement += mapMovementValue(MOVEMENT_DOWN_LOW, joystick_y);
	joystick_movement = _movement;
}

unsigned int WiiNunchuck::mapMovementValue(unsigned int base, signed short joystick_value)
{
	if (joystick_value < 0)
		joystick_value *= -1;
	if (joystick_value < MOVEMENT_LOW_THRESHOLD)
		return 0;
	if (joystick_value >= MOVEMENT_LOW_THRESHOLD && joystick_value < MOVEMENT_MID_THRESHOLD)
		return base;
	if (joystick_value >= MOVEMENT_MID_THRESHOLD && joystick_value < MOVEMENT_HIGH_THRESHOLD)
		return base * 2;
	if (joystick_value >= MOVEMENT_HIGH_THRESHOLD)
		return base * 4;
}
