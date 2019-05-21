#ifndef CONFIG
#define CONFIG

// Walker distances between joints
#define DIMENTION_JOINT1 25.0f
#define DIMENTION_JOINT2 41.5f
#define DIMENTION_JOINT3 89.0f

// System
#define DEBUG_ON			//Uncomment this to see debug in the serial monitor
#define BAUD_RATE		9600//9600 bps
#define UPDATE_SPEED	0	//Walker update speed, servos will be moved every 20ms (60Hz)
// Beeper
#define BEEPER_PIN		9
// Walker
#define MIN_GAIT_SPEED	0.5f
#define MAX_GAIT_SPEED	1.5f
#define GAIT_SPEED_INCREMENT 0.1f
// Servos
#define DRIVER_BOARD_COUNT		2
#define DRIVER_BOARD_FREQUENCY	120 //60Hz
#define SERVO_MIN_PULSE			345
#define SERVO_MAX_PULSE			995
#define SERVO_ANGLE_GRANULARITY	1.0f
#define SERVO_MIN_ANGLE			-60.0f
#define SERVO_MAX_ANGLE			60.0f
#define SERVO_DEFAULT_MOVEMENT	0.60f
// 0.13sec / 60 deg
// 2.16ms  / deg
// Update is every 20ms, so the walker can theorically move 10° per update
// Fabrik
#define FABRIK2D_DEFAULT_TOLERANCE 10.0f
#define FABRIK2D_MAX_ITERATIONS	10
#define FABRIK2D_TOLERANCE_INCREASE 0.1f

// Radio
#define RADIO_ADRESS	123456798
#define RADIO_TIMEOUT	1500
#ifdef ARDUINO_ESP8266_NODEMCU
	#define RADIO_PIN		0 //GPIO0 = D3
#else
	#define RADIO_PIN		10
#endif

#endif // CONFIG
