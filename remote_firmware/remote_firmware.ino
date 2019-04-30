#define IS_ARDUINO
#include <Arduino.h>
#include "mainfunctions.h"

//Libraries to install :
// - https://github.com/nRF24/RF24
// - https://github.com/infusion/Fritzing/tree/master/Nunchuk

void setup()
{
	Serial.begin(BAUD_RATE);
	main_setup();
}

void loop()
{
	main_loop();
}
