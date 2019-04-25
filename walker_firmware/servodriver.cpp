#include "servodriver.h"
#include "arduinopolyfill.h"
#include <stdlib.h>

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
	median_pulse_width = max_pulse_width - min_pulse_width;
}

void ServoDriver::setupAngleRange(float min, float max)
{
	if (min >= max)
		min = max -1;

	min_angle = min;
	max_angle = max;
	median_angle = max_angle - min_angle;
}

uint16 ServoDriver::getMiddlePulseWidth()
{
	return (uint16)(((int)min_pulse_width + (int)max_pulse_width) / 2);
}

/**
 * @brief ServoDriver::setServo Set a target servo to a given angle
 * @param index
 * @param angle
 */
void ServoDriver::setServo(uint16 index, uint16 angle)
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
int ServoDriver::angleToPulseWidth(uint16 angle)
{
	float pulse_wide = (float)((angle - min_angle) * median_pulse_width / median_angle + min_pulse_width);
	return (int)(pulse_wide / 1000000 * frequency * SERVO_DRIVER_TOTAL_PULSE);
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
