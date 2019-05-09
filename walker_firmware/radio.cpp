#include "radio.h"
#include "esp8266_watchdog.h"
#include <stdlib.h>

Radio::Radio()
{
	transmission_pin = 0;
	address = 0;
	channel = 0;
	power_level = 0;
	flag = RADIO_NONE;
	update_timer = RADIO_UPDATE_TIMER;
	timeout_timer = RADIO_TIMEOUT;
	state = RADIO_STATE_NONE;
	radio = NULL;
}

Radio *Radio::instance()
{
	//Static getter to allow access everywhere, singleton
	static Radio instance;
	return &instance;
}

/**
 * @brief Radio::setup Must be called before anything else to setup the radio object
 * @param _transmission_pin
 * @param _address
 * @param _flag
 * @param _channel
 * @param _power_level
 * @param _listen_size
 */
void Radio::setup(uint8 _transmission_pin, uint64 _address, RadioFlag _flag, uint8 _channel, uint8 _power_level, uint8 _listen_size)
{
	//1) Check if already setup, cannot reset after setup
	if (radio)
		return;

	//2) Setup datas
	transmission_pin = _transmission_pin;
	address = _address;
	flag = _flag;
	channel = _channel;
	power_level = _power_level;
	listen_size = _listen_size;
	listen_buffer = 0;
	radio = new RF24(transmission_pin, RF24_PIN_CE);
}

/**
 * @brief Radio::hasLink Check if the NRF24 is connected
 * @return
 */
bool Radio::hasLink()
{
	if (!radio) //No radio setup, no link
		return false;
	if (radio && radio->isChipConnected())
	{ //Radio setup but SPI is down
		state = RADIO_STATE_NO_CONNECTION;
		return false;
	}
	//All is ok
	return true;
}

/**
 * @brief Radio::link Open/reopen SPI bus to the NRF24 board
 * @param reset Force reset
 * @return
 */
bool Radio::link(bool reset)
{
	//1) Check for radio setup
	if (!radio) //Cannot link if radio was never setup
		return false;

	//2) Test reset radio
	if (hasLink() && !reset) //Do not reset current active radio
		return true;

	//3) Setup radio
	if (!radio->begin()) //This set & reset the radio
		return false;
	esp8266_delay(); //ESP8266 watchdog delay

	//4) Setup radio mode
	/// - TODO - Move this elsewhere for transmission & reception toggles
	///			 right now i just need one or the other
	if (flag & RADIO_TRANSMIT)
	{
		radio->openWritingPipe(address);
		esp8266_delay(); //ESP8266 watchdog delay
	}
	if (flag & RADIO_RECEIVE)
	{
		radio->openReadingPipe(channel, address);
		esp8266_delay(); //ESP8266 watchdog delay
		radio->startListening();
		esp8266_delay(); //ESP8266 watchdog delay
	}
	radio->setPALevel(power_level);
	esp8266_delay(); //ESP8266 watchdog delay

	state = RADIO_STATE_CONNECTED;
}

/**
 * @brief Radio::update Update radio state and read, must be in main loop
 * @param diff
 * @return
 */
uint8* Radio::update(unsigned int diff)
{
	if (update_timer <= diff)
	{
		//1) Check that radio is setup
		if (!radio) //Not setup yet, cannot do anything
			return 0;

		//2) Check that SPI link is ok, relink if not
		if (!hasLink())
		{
			if (!link(true))
				return 0; //Could not relink, try next time
		}

		//3) Main update
		update_timer = RADIO_UPDATE_TIMER; //Reset timer
		if (flag & RADIO_RECEIVE)
		{ //Listen, check for datas and keep track of timeout
			if (listen_size == 0)
				return 0;

			//1) Setup listener buffer if not set
			if (!listen_buffer)
				setupListenBuffer();

			//2) Update timeout
			if (timeout_timer <= diff)
			{
				state = RADIO_STATE_TIMEOUT;
				return 0;
			}
			else
				timeout_timer -= diff;

			//3) Check if datas sent
			if (!radio->available()) //Nothing to read, keep trying
				return 0;
			timeout_timer = RADIO_TIMEOUT; //Got datas, reset timeout
			state = RADIO_STATE_RECEIVING;

			//4) Read data
			esp8266_delay(100); //ESP8266 watchdog delay
			clearListenBuffer();
			radio->read(listen_buffer, listen_size);
			esp8266_delay(2); //ESP8266 watchdog delay

			//5) Return result buffer
			return listen_buffer;
		}
		return 0; //Nothing to report
	}
	else
		update_timer -= diff;

	return 0;
}

/**
 * @brief Radio::send Send a packet
 * @param buffer
 * @param size
 */
void Radio::send(const void *buffer, uint8 size)
{
	if (!hasLink() || !(flag & RADIO_TRANSMIT))
		return;

	esp8266_delay(100); //ESP8266 watchdog delay
	radio->write(buffer, size);
	esp8266_delay(2); //ESP8266 watchdog delay
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
