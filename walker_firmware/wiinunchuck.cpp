#include "wiinunchuck.h"
#include "arduinopolyfill.h"

WiiNunchuck::WiiNunchuck()
{
	joystick_x = WII_JOYSTICK_ZERO;
	joystick_y = WII_JOYSTICK_ZERO;
	z = false;
	c = false;
	pitch = 0.0f;
	roll = 0.0f;
}

bool WiiNunchuck::handlePacket(WiiNunchuckPacket *packet)
{
	if (!packet || !(packet->buttons & WII_BUTTON_UPDATE))
		return false; //Blank packet, skip

	joystick_x = packet->joystick_x;
	joystick_y = packet->joystick_y;
	//C is on when bit 1 is toggled on in buttons
	c = packet->buttons & WII_BUTTON_C;
	//Z is on when bit 2 is toggled on in buttons
	z = packet->buttons & WII_BUTTON_Z;
	pitch = decompressFloat(packet->pitch);
	roll = decompressFloat(packet->roll);

	update_time = millis();
	return true;
}

WiiNunchuckPacket* WiiNunchuck::buildPacket(WiiNunchuckPacket* packet)
{
	packet->joystick_x = joystick_x;
	packet->joystick_y = joystick_y;
	packet->pitch = compressFloat(pitch);
	packet->roll = compressFloat(roll);

	packet->buttons += WII_BUTTON_UPDATE;
	if (c)
		packet->buttons += WII_BUTTON_C;
	if (z)
		packet->buttons += WII_BUTTON_Z;

	return packet;
}

uint16 WiiNunchuck::compressFloat(float value)
{
	return (uint16)((value / WII_FLOAT_COMPRESS_RATIO) * WII_FLOAT_COMPRESS_PRECISION);
}

float WiiNunchuck::decompressFloat(uint16 value)
{
	return (((float)value / WII_FLOAT_COMPRESS_PRECISION) * WII_FLOAT_COMPRESS_RATIO);
}
