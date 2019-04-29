#ifndef RADIORECEIVER_H
#define RADIORECEIVER_H

#include "types.h"
#ifndef IS_QT
	#include "nRF24L01.h" //NRF24L01 library created by TMRh20 https://github.com/TMRh20/RF24
	#include "RF24.h"
	#include "SPI.h"
#else
	#include "arduinopolyfill.h"
#endif

/* ---------------------------
 * Arduino Nano pins :
 * Nano       NRF24
 * D12(MISO)  MISO	|
 * D11(MOSI)  MOSI	| Parts of the
 * D13(SCK)   SCK	| SPI bus
 * D10(SS)    CSN	|
 * D9         CE	- Data pin, can be changed
 * 3.3V       VCC
 * G or GND   GND
 * ---------------------------
 * ESP8266 pins :
 * ESP            NRF24
 * D6(HMISO)      MISO  |
 * D7(HMOSI)      MOSI  | Parts of the
 * D5(HCLK)       SCK   | SPI BUS
 * D8(HCS)        CSN   |
 * D2             CE    - Data pin, can be changed
 * 3.3V           VCC
 * G or GND       GND
 * ---------------------------
 * And then init the NRF24 like so :
 * RF24 radio(4, 15);
 * https://www.electronicwings.com/nodemcu/nodemcu-gpio-with-arduino-ide
 * 4  = GPIO4  (D2)
 * 15 = GPIO15 (D8)
 * --------------------------- */

//CE is the only pin that is board dependant and need to be declared in the RF24 constructor
//The pin is labeled CE or SS
#ifdef ARDUINO_AVR_UNO
	#define RF24_PIN_CE 10 ///TODO - TEST THIS
#elif ARDUINO_AVR_NANO
	#define RF24_PIN_CE 9 //Pin D9
#elif ARDUINO_AVR_MINI
	#define RF24_PIN_CE 13 ///TODO - TEST THIS
#elif ARDUINO_ESP8266_NODEMCU
	#define RF24_PIN_CE 4 //GPIO4 = pin D2
#else
	#define RF24_PIN_CE 1 //No pin, no game
#endif

/// - Radio receiver
/// There is basically only one pin that is configurable : The CE (SPI:SS) pin
/// The others are hardware dependant and can be found on the documentation
class RadioReceiver
{
	public:
		RadioReceiver();
		//Methods
		// - Getters
		static RadioReceiver* instance();
		// - Main
		void	setup(uint8 _transmission_pin, uint32 _address);
		int		update(int diff);
	private:
		uint16	update_timer;
		uint32	address;
		uint8	transmission_pin;
		RF24*	radio;
};

#define sRadioReceiver RadioReceiver::instance()

#endif // RADIORECEIVER_H
