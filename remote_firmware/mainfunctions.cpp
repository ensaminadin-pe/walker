#include "mainfunctions.h"
#include "radio.h"
#include "config.h"
#include <stdio.h>
#include "wiinunchuck.h"

#ifdef IS_QT
	#include "arduinopolyfill.h"
#else
	#include "stdpolyfill.h"
#endif

/// - This is the core of the program

unsigned long diff()
{
	unsigned long diff = update_time - millis();
	if (diff < 1)
	{
		delay(1);
		diff = 1;
	}

	return diff;
}

void main_setup()
{ //Arduino like setup
	/// Setup your remote here
	//1) Setup radio transmitter
	sRadio->setup(RADIO_PIN, RADIO_ADRESS, RADIO_TRANSMIT); //Radio data on pin 10, can be anything
	//2) Setup nunchuck
	direction_nunchuck.initNunchuckReading();
}

void main_loop()
{ //Arduino like loop
	//1) Setup loop diff time
	if (update_time == 0)
	{
		update_time = millis();
		return;
	}

	//2) Main logic here
	//2.1) Update nunchuck
	direction_nunchuck.read();
	//2.2) Send nunchuck datas
	sendNunchuckUpdate(diff());

	//3) Update time
	update_time = millis();
}

void sendNunchuckUpdate(unsigned long diff)
{
	if (send_nunchuck_timer <= diff)
	{
		send_nunchuck_timer = UPDATE_SPEED;
		nunchuck_packet = WiiNunchuckPacket();
		direction_nunchuck.buildPacket(&nunchuck_packet);
		direction_nunchuck.print();
		sRadio->send(&nunchuck_packet, sizeof(nunchuck_packet));
	}
	else
		send_nunchuck_timer -= diff;

}
