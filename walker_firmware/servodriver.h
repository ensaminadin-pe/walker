#ifndef SERVODRIVER_H
#define SERVODRIVER_H

#ifdef IS_QT
	#include "arduinopolyfill.h"
#else
	#include <Wire.h>
	#include <Adafruit_PWMServoDriver.h>
#endif

/*
 * Pins:
 *
 * ESP8266
 * SDA	D2
 * SCL	D1
 *
 */

#include "types.h"

class ServoDriver
{
	public:
		//Constructor & destructor
		ServoDriver();
		~ServoDriver();

		// - Setup functions
		static ServoDriver* instance();
		// - Setup X drivers boards with a frequency of Y
		void setupDrivers(uint8 board_count, uint16 frequency);
		// - Setup a pulse range of X to Y and build pulse table with a granularity of Z in degree
		//	 This way we save a lot of processing power for other stuff
		//	 Angle 0° is the middle of the pulse table, first index in the table is -90° and last is 90°
		//	 Go easy on the granularity, 0.5 is already realy fine and 1 will do most of the time
		void setupPulseRange(uint16 min, uint16 max, float granularity);
		// - Setup angle range from X to Y.  ex -60 to 60.
		void setupAngleRange(float min, float max);

		//Methods
		// - Set servo X of board Y at angle Z
		void setServo(uint8 board, uint8 index, float angle, uint16 offset = 0);
		// - Getters
		uint16	getDriverFrequency()	{ return driver_frequency; }
		uint8	getDriverCount()		{ return driver_count; }
		float	getMinAngle()			{ return min_angle; }
		float	getMaxAngle()			{ return max_angle; }
	private:
		//Properties
		Adafruit_PWMServoDriver** drivers;
		uint8	driver_count;			//Number of PCA9685 connected
		uint16	driver_frequency;		//Theorical max ~1600
		uint16	min_pulse_width;		//Minimum pulse width
		uint16	max_pulse_width;		//Maximum pulse width
		float	pulse_table_granularity;//Granularity of the pulse table, ex: between 0 and 180 => 1 = 1 entry per degree , 0.5 = 1 entry per 0.5 degree
		uint16	pulse_table_size;		//Number of entry in the pulse table
		uint16*	pulse_table;			//Table of pulse width for given granularity
		float	min_angle;				//Minimum angle to accept on a set action
		float	max_angle;				//Maximum angle to accept on a set action

		//Methods
		void clearDrivers();
};

#define sServoDriver ServoDriver::instance()

#endif // SERVODRIVER_H
