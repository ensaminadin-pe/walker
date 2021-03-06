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

	typedef enum
	{
		RF24_PA_MIN = 0,
		RF24_PA_LOW,
		RF24_PA_HIGH,
		RF24_PA_MAX,
		RF24_PA_ERROR
	} rf24_pa_dbm_e;

	class RF24
	{
		public:
			RF24(uint8 _pin1, uint8 _pin2) { pin1 = _pin1; pin2 = _pin2; }
			bool begin() { return true; }
			void setPALevel(uint32 level) { PALevel = level; }
			void openReadingPipe(uint32 _child, uint32 _address) { child = _child; address = _address; }
			void openWritingPipe(uint32 _address) { address = _address; }
			void startListening() {}
			void read(const void* buf, uint8 len);
			void write(const void* buf, uint8 len);
			bool available() { return true; }
			bool isChipConnected() { return true; }
			void powerUp(void) {}
			void powerDown(void) {}
		private:
			uint8 pin1;
			uint8 pin2;
			uint8 child;
			uint32 address;
			uint32 PALevel;
	};

	#endif // ARDUINOPOLYFILL
#endif // IS_QT
