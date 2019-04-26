#ifndef OSCILLATOR_H
#define OSCILLATOR_H

#include "types.h"
#ifndef IS_QT
	#include <Arduino.h>
#endif

#ifndef PI
	#define PI 3.14159
#endif
#ifndef DOUBLE_PI
	#define DOUBLE_PI 6.28318
#endif

enum OscillatorFlags: uint8
{
	OSCILLATE_NORMAL = 0, //Regular oscillation
	OSCILLATE_POSITIVE = 1, //Only positive
	OSCILLATE_NEGATIVE = 2, //Only negative
	OSCILLATE_INVERT_POSITIVE = 4, //Turn positive into negative
	OSCILLATE_INVERT_NEGATIVE = 8, //Turn negative into positive
	OSCILLATE_ONLY_POSITIVE = 16, //Only return positive results
	OSCILLATE_ONLY_NEGATIVE = 32 //Only return negative results
};

class Oscillator{

	public:
		Oscillator();
		float refresh();
		void reset();
		void start();
		void start(unsigned long ref_time);
		void stop();
		float time_to_radians(double time);
		float degrees_to_radians(float degrees);
		float degrees_to_time(float degrees);
		void setPeriod(uint16 period);
		void setAmplitude(int8 amplitude);
		void setPhase(uint16 phase);
		void setOffset(int8 offset);
		void setTime(unsigned long ref);
		void setFlag(unsigned int flag);
		void setPeriodModifier(float* modifier);
		uint16 getModifiedPeriod();
		float getOutput();
		float getPhaseProgress();
		unsigned long getTime();

	private:
		void handleFlag();
		uint16	_period;
		int8	_amplitude;
		uint16	_phase;
		int8	_offset;
		float	_output;
		bool	_stop;
		unsigned long _ref_time = 0;
		float	_delta_time = 0;
		uint8	_flag;
		float*	_period_modifier;
};

#endif // OSCILLATOR_H
