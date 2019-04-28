#include "servodriver.h"
#include "arduinopolyfill.h"
#include <stdlib.h>
#include <stdio.h>

ServoDriver::ServoDriver()
{
	driver_count = 0;
	frequency = SERVO_DRIVER_DEFAULT_FREQUENCY;
	setupPulseRange(SERVO_DRIVER_DEFAULT_MIN_PULSE, SERVO_DRIVER_DEFAULT_MAX_PULSE);
	setupAngleRange(SERVO_DRIVER_DEFAULT_MIN_ANGLE, SERVO_DRIVER_DEFAULT_MAX_ANGLE);
	drivers = 0;
}

ServoDriver::~ServoDriver()
{
	clearDrivers();
}

/**
 * @brief ServoDriver::instance Static singleton getter
 * @return
 */
ServoDriver *ServoDriver::instance()
{
	//Static getter to allow access everywhere, singleton
	static ServoDriver instance;
	return &instance;
}

void ServoDriver::setServoCount(uint16 _servo_count)
{
	int _driver_count = _servo_count / SERVO_DRIVER_CAPACITY;
	if (_servo_count - (_driver_count * SERVO_DRIVER_CAPACITY) > 0)
		_driver_count++;
	driver_count = _driver_count;
}

void ServoDriver::setDriverCount(uint8 _driver_count)
{
	driver_count = _driver_count;
}

void ServoDriver::setFrequency(float _frequency)
{
	frequency = _frequency;
}

void ServoDriver::setupPulseRange(uint16 min, uint16 max)
{
	if (min >= max)
		min = max -1;

	min_pulse_width = min;
	max_pulse_width = max;
	total_pulse_range = (max_pulse_width - min_pulse_width);
	angle_factor = (float)total_pulse_range / 180.0f;
}

void ServoDriver::setupAngleRange(float min, float max)
{
	if (min >= max)
		min = max -1;

	min_angle = min;
	max_angle = max;
}

/**
 * @brief ServoDriver::setServo Set a target servo to a given angle
 * @param index
 * @param angle
 */
void ServoDriver::setServo(uint16 index, float angle)
{
	//1) Locate the driver
	uint8 driver_index = index / SERVO_DRIVER_CAPACITY;
	if (driver_count == 0 || !drivers || driver_index + 1 > driver_count || !drivers[driver_index])
		return; //Cannot access driver

	//2) Update the angle
	if (angle > max_angle)
		angle = max_angle;
	if (angle < min_angle)
		angle = min_angle;

	//3) Set pwm on target driver's index
	index -= (driver_index * SERVO_DRIVER_CAPACITY);
	drivers[driver_index]->setPWM(index, 0, angleToPulseWidth(angle));
}

/**
 * @brief ServoDriver::angleToPulseWidth Convert an angle to a correct PWM pulse width
 * @param angle
 * @return
 */
int ServoDriver::angleToPulseWidth(float angle)
{
	//                                                   get correct pulse factor
	//	set to 0-180° range	  get total pulse range		             |     put pulse in correct place
	// (angle - min_angle) * ((max_pulse_width - min_pulse_width) / 180) + min_pulse_width
	return min_pulse_width + (((float)angle - min_angle) * angle_factor);
}

void ServoDriver::clearDrivers()
{
	if (!drivers)
		return;
	for (uint8 i = 0; i < driver_count; i++)
	{
		if (drivers[i])
			delete drivers[i];
	}
	free(drivers);
	drivers = NULL;
}

void ServoDriver::initDrivers()
{
	//1) Cleanup
	clearDrivers();
	drivers = (Adafruit_PWMServoDriver**)malloc(sizeof(Adafruit_PWMServoDriver*) * driver_count);
	if (!drivers)
		return;

	//2) Setup drivers with incremental addresses
	uint8 addr = 0x40;
	for (uint8 i = 0; i < driver_count; i++)
	{
		//2.1) Create a new driver
		#ifdef IS_QT
			drivers[i] = new Adafruit_PWMServoDriver(0, addr);
		#else
			drivers[i] = new Adafruit_PWMServoDriver(&Wire, addr);
		#endif

		//2.2) Update adress for next driver
		addr++;

		//2.3) Init new driver
		drivers[i]->begin();
		drivers[i]->setPWMFreq(frequency);
		#ifndef IS_QT
			delay(10); // Need some time to init
		#endif
	}
}
