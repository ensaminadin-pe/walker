#define IS_ARDUINO
#include <Arduino.h>
#include "mainfunctions.h"
#include "config.h"

//Install https://github.com/adafruit/Adafruit-PWM-Servo-Driver-Library

void setup()
{
	Serial.begin(BAUD_RATE);
	main_setup();
}

void loop()
{
	main_loop();
}
