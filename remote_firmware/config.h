#ifndef CONFIG
#define CONFIG

// Main
#define BAUD_RATE		9600//9600 bps
#define UPDATE_SPEED	20	//20ms = 60Hz
// Radio
#define RADIO_ADRESS	123456798
#ifdef ARDUINO_ESP8266_NODEMCU
	#define RADIO_PIN		0 //GPIO0 = D3
#elif ARDUINO_AVR_NANO
	#define RADIO_PIN		9
#else
	#define RADIO_PIN		9
#endif

#endif // CONFIG
