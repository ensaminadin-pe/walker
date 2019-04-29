#define IS_ARDUINO
#include <Arduino.h>
#include "mainfunctions.h"
#include "config.h"

//Install https://github.com/adafruit/Adafruit-PWM-Servo-Driver-Library

void setup()
{
	Serial.begin(BAUD_RATE);

	/*
test controller detection
#ifdef ARDUINO_AVR_UNO ?
-DARDUINO_ARCH_AVR
-DARDUINO_{build.board}
AVR_ADK
AVR_BT
AVR_DUEMILANOVE
AVR_ESPLORA
AVR_ETHERNET
AVR_FIO
AVR_GEMMA
AVR_LEONARDO
AVR_LILYPAD
AVR_LILYPAD_USB
AVR_MEGA
AVR_MEGA2560
AVR_MICRO
AVR_MINI
AVR_NANO
AVR_NG
AVR_PRO
AVR_ROBOT_CONTROL
AVR_ROBOT_MOTOR
AVR_UNO
AVR_YUN
ARDUINO_ESP8266_NODEMCU
	*/

	main_setup();
}

void loop()
{
	main_loop();
}
