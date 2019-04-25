/*
 * Missing Arduino functions for QT usage
 */

#ifdef IS_QT
	#ifndef ARDUINOPOLYFILL
	#define ARDUINOPOLYFILL

	#include "types.h"

	unsigned long	millis();
	void			delay(int ms);

	class TwoWire
	{
		public:
			TwoWire() {}
	};

	class Adafruit_PWMServoDriver
	{
		public:
			Adafruit_PWMServoDriver(TwoWire *I2C = 0, uint8 addr = 0x40);
			void begin(void);
			void reset(void);
			void setPWMFreq(float freq);
			void setPWM(uint8 num, uint16 on, uint16 off);
			void setPin(uint8 num, uint16 val, bool invert=false);
	};

	#endif // ARDUINOPOLYFILL
#endif // IS_QT
