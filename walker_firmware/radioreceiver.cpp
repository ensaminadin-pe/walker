#include "radioreceiver.h"
#include "esp8266_watchdog.h"

RadioReceiver::RadioReceiver()
{
	update_timer = 50;
}

void RadioReceiver::setup(uint8 _transmission_pin, uint32 _address)
{
	if (radio)
	{
		delete(radio);
		radio = 0;
	}

	transmission_pin = _transmission_pin;
	address = _address;
	radio = new RF24(RF24_PIN_CE, transmission_pin);
	esp8266_delay(); //ESP8266 watchdog delay
	radio->begin();
	esp8266_delay(); //ESP8266 watchdog delay
	radio->openReadingPipe(0, address);
	radio->setPALevel(RF24_PA_MIN);
	radio->startListening();
	esp8266_delay(); //ESP8266 watchdog delay
}

int RadioReceiver::update(int diff)
{
	/// TODO - CHANGE INT RETURN WITH CORRECT CONTROLLER DATA RESULT STRUCTURE
	if (update_timer <= diff)
	{
		if (!radio->available())
			return 0;
		int msg = 0;
		radio->read(&msg, sizeof(int));    //Reading the data
		update_timer = 50;
		return msg;
	}
	else
		update_timer -= diff;
	return 0;
}
