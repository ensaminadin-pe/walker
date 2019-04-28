#ifndef SERVODRIVER_H
#define SERVODRIVER_H

#ifdef IS_QT
	#include "arduinopolyfill.h"
#else
	#include <Wire.h>
	#include <Adafruit_PWMServoDriver.h>
#endif

#include "types.h"

#define SERVO_DRIVER_DEFAULT_FREQUENCY	60.0f
#define SERVO_DRIVER_DEFAULT_MIN_ANGLE	0.0f
#define SERVO_DRIVER_DEFAULT_MAX_ANGLE	180.0f
#define SERVO_DRIVER_DEFAULT_OFFSET		0.0f
#define SERVO_DRIVER_DEFAULT_MIN_PULSE	650
#define SERVO_DRIVER_DEFAULT_MAX_PULSE	2350
#define SERVO_DRIVER_CAPACITY			16	//16 servos per driver
#define SERVO_DRIVER_TOTAL_PULSE		4095

class ServoDriver
{
	public:
		//Constructor & destructor
		ServoDriver();
		~ServoDriver();
		//Methods
		// - Setter & getters
		static ServoDriver* instance();
		void setServoCount(uint16 _servo_count);
		void setDriverCount(uint8 _driver_count);
		uint8 getDriverCount() { return driver_count; }

		void setFrequency(float _frequency);

		void setupPulseRange(uint16 min, uint16 max);

		void setupAngleRange(float min, float max);

		// - Actions
		void initDrivers();
		void setServo(uint16 index, float angle);
		int angleToPulseWidth(float angle);
	private:
		//Properties
		Adafruit_PWMServoDriver** drivers;

		uint8	driver_count;	//Number of PCA9685 connected
		uint16	min_pulse_width;//Minimum pulse width
		uint16	max_pulse_width;//Maximum pulse width
		float	frequency;		//PWM frequency
		float	min_angle;		//Minimum angle to accept on a set action
		float	max_angle;		//Maximum angle to accept on a set action
		float	offset;			//Offset of the set actions : Servo's 0 position

		//Stored for faster angle to pulse conversion
		uint16	total_pulse_range;
		float	angle_factor;

		//Methods
		void clearDrivers();
};

#define sServoDriver ServoDriver::instance()

#endif // SERVODRIVER_H
