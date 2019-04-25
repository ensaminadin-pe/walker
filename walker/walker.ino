#define IS_ARDUINO
#include <Arduino.h>
#include "mainfunctions.h"
#include "config.h"

void setup()
{
	Serial.begin(BAUD_RATE);
	main_setup();
}

void loop()
{
	main_loop();
}
