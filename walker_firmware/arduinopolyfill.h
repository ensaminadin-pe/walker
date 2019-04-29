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

	#define RF24_PA_MIN 0

	class RF24
	{
		public:
			RF24(uint8 _pin1, uint8 _pin2);
			void begin();
			void setPALevel(uint32 level) { PALevel = level; }
			void openWritingPipe(uint32 _address) { address = _address; }
			void write(const void* buf, uint8 len);
		private:
			uint8 pin1;
			uint8 pin2;
			uint32 address;
			uint32 PALevel;
	};

	#endif // ARDUINOPOLYFILL
#endif // IS_QT
