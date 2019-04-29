#include "radioreceiver.h"

RadioReceiver::RadioReceiver()
{

}

void RadioReceiver::setup(int transmission_pin)
{
	if (radio)
	{
		delete(radio);
		radio = 0;
	}

	radio = new RF24(RF24_PIN_CE, transmission_pin);
}
