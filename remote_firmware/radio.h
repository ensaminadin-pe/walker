#ifndef RADIO_H
#define RADIO_H

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
//The pin is labeled CE, SS or CS
#ifdef ARDUINO_AVR_UNO
	#define RF24_PIN_CE 10 ///TODO - TEST THIS
#elif ARDUINO_AVR_NANO
	#define RF24_PIN_CE 10 //Pin D10
#elif ARDUINO_AVR_MINI
	#define RF24_PIN_CE 13 ///TODO - TEST THIS
#elif ARDUINO_ESP8266_NODEMCU
	#define RF24_PIN_CE 15 //GPIO15 = pin D8
#else
	#define RF24_PIN_CE 1 //No pin, no game
#endif

enum RadioFlag
{
	RADIO_TRANSMIT	= 1,
	RADIO_RECEIVE	= 2
};

/// - Radio receiver
/// There is basically only one pin that is configurable : The CE (SPI:SS) pin
/// The others are hardware dependant and can be found on the documentation
class Radio
{
	public:
		Radio();
		//Methods
		// - Getters
		static Radio* instance();
		// - Main
		void	setup(uint8 _transmission_pin, uint64 _address, RadioFlag _flag, uint8 _channel = 0, uint8 power_level = RF24_PA_HIGH, uint8 _listen_size = 32);
		uint8	*update(uint32 diff);
		void	send(const void* buffer, uint8 size);
		void	setListenSize(uint8 size);
		RF24*	getRadio() { return radio; }
	private:
		void	setupListenBuffer();
		void	clearListenBuffer();
		uint8		transmission_pin;
		uint64		address;
		RadioFlag	flag;
		uint8		channel;
		uint16		update_timer;
		uint8		listen_size; //Reception buffer size
		uint8*		listen_buffer;
		RF24*		radio;
};

#define sRadio Radio::instance()

#endif // RADIO_H
