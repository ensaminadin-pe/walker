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
 * Operation :
 *	. Check connection
 *  . Ability to reset PSI connection to module
 *  . Detect timeout
 * --------------------------- */

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
	#define RF24_PIN_CE 9 //Pin D9
#elif ARDUINO_AVR_MINI
	#define RF24_PIN_CE 13 ///TODO - TEST THIS
#elif ARDUINO_ESP8266_NODEMCU
	#define RF24_PIN_CE 15 //GPIO15 = pin D8
#else
	#define RF24_PIN_CE 1 //No pin, no game
#endif

#define RADIO_TIMEOUT		1000 // 1000ms since last correct reception
#define RADIO_UPDATE_TIMER	10   // Update every 10 ms

enum RadioFlag : uint8
{
	RADIO_NONE		= 0,
	RADIO_TRANSMIT	= 1,
	RADIO_RECEIVE	= 2
};

enum RadioState : uint8
{
	RADIO_STATE_NONE			= 0,	// Just init
	RADIO_STATE_NO_CONNECTION	= 1,	// Connection to NRF24 module is broken
	RADIO_STATE_OFF				= 2,	// Do not care about radio signal
	RADIO_STATE_TIMEOUT			= 3,	// Connected to NRF24 but stopped receiving datas
	RADIO_STATE_CONNECTED		= 4,	// Connected to NRF24
	RADIO_STATE_RECEIVING		= 5		// Connected to NRF24 and receiving datas
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
		void		setup(uint8 _transmission_pin, uint64 _address, RadioFlag _flag, uint8 _channel = 0, uint8 _power_level = RF24_PA_HIGH, uint8 _listen_size = 32);

		bool		hasLink();
		bool		link(bool reset = false);

		uint8*		update(unsigned int diff);
		void		send(const void* buffer, uint8 size);
		void		setListenSize(uint8 size);
		RF24*		getRadio() { return radio; }
		RadioState	getState() { return state; }
	private:
		void	setupListenBuffer();
		void	clearListenBuffer();
		uint8		transmission_pin;
		uint64		address;
		uint8		channel;
		uint8		power_level;
		RadioState	state;
		RadioFlag	flag;
		uint16		update_timer;
		uint16		timeout_timer;
		uint8		listen_size; //Reception buffer size
		uint8*		listen_buffer;
		RF24*		radio;
};

#define sRadio Radio::instance()

#endif // RADIO_H
