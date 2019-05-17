#ifdef IS_QT
	#include "arduinopolyfill.h"
	#include <sys/time.h>
	#include <unistd.h>
	#include <stdio.h>

	//#define SHOW_SERVO_DEBUG

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

	void tone(uint8 pin, uint16 note, uint16 note_time)
	{

	}

	void noTone(uint8 pin)
	{

	}

	Adafruit_PWMServoDriver::Adafruit_PWMServoDriver(TwoWire *I2C, uint8 addr)
	{
		#ifdef SHOW_SERVO_DEBUG
			printf("[Servo PWM driver] : Init with addr : %02x\n", addr);
		#endif
	}

	void Adafruit_PWMServoDriver::begin(void)
	{
		#ifdef SHOW_SERVO_DEBUG
				printf("[Servo PWM driver] : begin\n");
		#endif
	}

	void Adafruit_PWMServoDriver::reset(void)
	{
		#ifdef SHOW_SERVO_DEBUG
				printf("[Servo PWM driver] : reset\n");
		#endif
	}

	void Adafruit_PWMServoDriver::setPWMFreq(float freq)
	{
		#ifdef SHOW_SERVO_DEBUG
				printf("[Servo PWM driver] : Set frequency to %f\n", freq);
		#endif
	}

	void Adafruit_PWMServoDriver::setPWM(uint8 num, uint16 on, uint16 off)
	{
		#ifdef SHOW_SERVO_DEBUG
				printf("[Servo PWM driver] : Set PWM for [%i] to [%i] / [%i]\n", num, on, off);
		#endif
	}

	void Adafruit_PWMServoDriver::setPin(uint8 num, uint16 val, bool invert)
	{
		#ifdef SHOW_SERVO_DEBUG
				printf("[Servo PWM driver] : Set Pin for [%i] to [%i]. Inverted ? %d: %\n", num, val, invert);
		#endif
	}

	void RF24::read(const void *buf, uint8 len)
	{
	}

	void RF24::write(const void *buf, uint8 len)
	{
	}
#endif
