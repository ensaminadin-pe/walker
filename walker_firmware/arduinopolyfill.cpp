#ifdef IS_QT
	#include "arduinopolyfill.h"
	#include <sys/time.h>
	#include <unistd.h>
	#include <stdio.h>

	unsigned long millis()
	{
		struct timeval tp;
		gettimeofday(&tp, NULL);
		return tp.tv_sec * 1000 + tp.tv_usec / 1000;
	}

	void delay(int ms)
	{
		usleep(ms * 1000);
	}

	Adafruit_PWMServoDriver::Adafruit_PWMServoDriver(TwoWire *I2C, uint8 addr)
	{
		printf("[Servo PWM driver] : Init with addr : %02x\n", addr);
	}

	void Adafruit_PWMServoDriver::begin(void)
	{
		printf("[Servo PWM driver] : begin\n");
	}

	void Adafruit_PWMServoDriver::reset(void)
	{
		printf("[Servo PWM driver] : reset\n");
	}

	void Adafruit_PWMServoDriver::setPWMFreq(float freq)
	{
		printf("[Servo PWM driver] : Set frequency to %f\n", freq);
	}

	void Adafruit_PWMServoDriver::setPWM(uint8 num, uint16 on, uint16 off)
	{
		printf("[Servo PWM driver] : Set PWM for [%i] to [%i] / [%i]\n", num, on, off);
	}

	void Adafruit_PWMServoDriver::setPin(uint8 num, uint16 val, bool invert)
	{
		printf("[Servo PWM driver] : Set Pin for [%i] to [%i]. Inverted ? %d: %\n", num, val, invert);
	}
#endif
