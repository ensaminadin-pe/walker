#include "servodriver.h"
#include "arduinopolyfill.h"
#include "config.h"
#include <stdlib.h>
#include <stdio.h>

ServoDriver::ServoDriver()
{
	drivers = NULL;
	pulse_table = NULL;
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

/**
 * @brief ServoDriver::setupDrivers Setup X driver boards with a frequency of Y
 * @param board_count
 * @param frequency
 */
void ServoDriver::setupDrivers(uint8 board_count, uint16 frequency)
{
	//1) Cleanup
	clearDrivers();

	//2) Allocate driver space
	driver_frequency = frequency;
	driver_count = board_count;
	drivers = (Adafruit_PWMServoDriver**)malloc(sizeof(Adafruit_PWMServoDriver*) * driver_count);
	if (!drivers)
		return;

	//3) Setup drivers with incremental addresses
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
		drivers[i]->setPWMFreq(driver_frequency);
		#ifndef IS_QT
			delay(10); // Need some time to init
		#endif
	}
}

/**
 * @brief ServoDriver::clearDrivers Clear allocated drivers
 */
void ServoDriver::clearDrivers()
{
	//1) If no drivers, skip
	if (!drivers)
		return;

	//2) Delete every drivers
	for (uint8 i = 0; i < driver_count; i++)
	{
		if (drivers[i])
			delete drivers[i];
	}

	//3) Free allocated memory
	free(drivers);
	drivers = NULL;
}

/**
 * @brief ServoDriver::setupPulseRange Setup the min and max pulse range and build a pulse table for every rotation defined by the granularity, ex granularity = 0.5, one entry per 0.5 degree
 * @param min
 * @param max
 * @param granularity
 */
void ServoDriver::setupPulseRange(uint16 min, uint16 max, float granularity)
{
	//1) Check datas
	if (min >= max)
		min = max - 1;

	//2) Store pulse width
	min_pulse_width = min;
	max_pulse_width = max;

	//3) Build pulse table for given granularity
	pulse_table_granularity = 180.0f / (180.0f / granularity);
	pulse_table_size = 180.0f / pulse_table_granularity; //Convert to integer
	if (pulse_table)
	{//Clear pulse table if needed
		free(pulse_table);
		pulse_table = NULL;
	}
	//Allocate pulse table space, this might take too much memory if your granularity is too low
	pulse_table = (uint16*)malloc(sizeof(uint16) * pulse_table_size);
	if (!pulse_table)
		return;
	//Fill the pulse table
	float angle_factor = ((float)(max_pulse_width - min_pulse_width)) / 180.0f;
	float angle = 0.0f;
	for (int i = 0; i < pulse_table_size; i++)
	{
		pulse_table[i] = min_pulse_width + (angle * angle_factor);
		angle += pulse_table_granularity;
	}
}

/**
 * @brief ServoDriver::setupAngleRange Setup angle range for the servos
 * @param min
 * @param max
 */
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
void ServoDriver::setServo(uint8 board, uint8 index, float angle, uint16 offset)
{
	//1) Check datas
	if (!drivers || board > (driver_count - 1) || !drivers[board] || index > 15)
		return; //Doesnt exist

	//2) Update the angle
	if (angle > max_angle)
		angle = max_angle;
	if (angle < min_angle)
		angle = min_angle;



	//3) Find correct PWM
	uint16 pulse_index = (uint16)((angle - min_angle) / pulse_table_granularity);
	if (pulse_index < 0)
		pulse_index = 0;
	else if (pulse_index > pulse_table_size - 1)
		pulse_index = pulse_table_size - 1;

	//4) Handle pulse width offset
	uint16 pulse = pulse_table[pulse_index];
	if (offset != 0)
	{ //Apply servo offset
		if (offset > pulse)
			pulse = 0;
		pulse += offset;

		//Fix pulse width
		if (pulse < min_pulse_width)
			pulse = min_pulse_width;
		else if (pulse > max_pulse_width)
			pulse = max_pulse_width;
	}

	//5) Set pwm on target servo
	//printf("SET SERVO %i;%i TO %f (%i)\n", board, index, angle, pulse);
	drivers[board]->setPWM(index, 0, pulse);
}
