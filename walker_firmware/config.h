#ifndef CONFIG
#define CONFIG

// Main
#define BAUD_RATE		9600//9600 bps
#define UPDATE_SPEED	20	//20ms = 60Hz
// Gaits
#define GAIT_DEFAULT_DURATION	1000
#define GAIT_DEFAULT_PERIOD		1000
#define GAIT_DEFAULT_AMPLITUDE	90
// Walker
#define MIN_GAIT_SPEED	0.5f
#define MAX_GAIT_SPEED	1.5f
#define GAIT_SPEED_INCREMENT 0.1f
// Servos
#define SERVO_UPDATE_FREQUENCY 60 //60Hz
// Radio
#define RADIO_ADRESS	123456798
#ifdef ARDUINO_ESP8266_NODEMCU
	#define RADIO_PIN		0 //GPIO0 = D3
#else
	#define RADIO_PIN		10
#endif

#endif // CONFIG
