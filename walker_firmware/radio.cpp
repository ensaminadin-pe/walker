#include "radio.h"
#include "esp8266_watchdog.h"
#include <stdlib.h>

Radio::Radio()
{
	update_timer = 50;
}

Radio *Radio::instance()
{
	//Static getter to allow access everywhere, singleton
	static Radio instance;
	return &instance;
}

void Radio::setup(uint8 _transmission_pin, uint64 _address, RadioFlag _flag, uint8 _channel, uint8 power_level, uint8 _listen_size)
{
	if (radio)
	{
		delete(radio);
		radio = 0;
	}

	//1) Setup datas
	transmission_pin = _transmission_pin;
	address = _address;
	flag = _flag;
	channel = _channel;
	radio = new RF24(RF24_PIN_CE, transmission_pin);
	listen_size = _listen_size;
	listen_buffer = 0;
	esp8266_delay(); //ESP8266 watchdog delay
	radio->begin();
	esp8266_delay(); //ESP8266 watchdog delay

	//2) Setup mode
	if (flag & RADIO_TRANSMIT)
	{
		radio->openWritingPipe(channel, address);
		esp8266_delay(); //ESP8266 watchdog delay
	}
	if (flag & RADIO_RECEIVE)
	{
		setupListenBuffer();
		radio->openReadingPipe(channel, address);
		esp8266_delay(); //ESP8266 watchdog delay
		radio->startListening();
		esp8266_delay(); //ESP8266 watchdog delay
	}
	radio->setPALevel(power_level);
	esp8266_delay(); //ESP8266 watchdog delay
}

uint8* Radio::update(uint32 diff)
{

	/// TODO - CHANGE INT RETURN WITH CORRECT CONTROLLER DATA RESULT STRUCTURE
	if (update_timer <= diff)
	{
		if (listen_size == 0)
			return 0;
		if (!listen_buffer)
			setupListenBuffer();
		if (!radio->available()) //Nothing to read, keep trying
			return 0;
		esp8266_delay(100); //ESP8266 watchdog delay
		clearListenBuffer();
		radio->read(listen_buffer, listen_size);
		esp8266_delay(2); //ESP8266 watchdog delay
		update_timer = 50;
		return listen_buffer;
	}
	else
		update_timer -= diff;
	return 0;
}

void Radio::send(const void *buffer, uint8 size)
{
	if (!(flag & RADIO_TRANSMIT))
		return;

}

void Radio::setListenSize(uint8 size)
{
	listen_size = size;
	setupListenBuffer();
}

void Radio::setupListenBuffer()
{
	if (listen_buffer)
	{
		free(listen_buffer);
		listen_buffer = 0;
	}
	listen_buffer = (uint8*)malloc(sizeof(uint8) * listen_size);
	clearListenBuffer();
}

void Radio::clearListenBuffer()
{
	if (!listen_buffer)
		return;
	for (uint8 i = 0; i < listen_size; i++)
		listen_buffer[i] = 0;
}
