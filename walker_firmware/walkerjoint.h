#ifndef WALKERJOINT_H
#define WALKERJOINT_H

#include "types.h"
#include "oscillator.h"
#include "gait.h"

class WalkerJoint
{
	public:
		WalkerJoint(uint8 _leg_index, uint8 _joint_index);
		~WalkerJoint();
		void reset();
		void setPeriodModifier(float *modifier);
		void startOscillator()		{ oscillator.start(); }
		void stopOscillator()		{ oscillator.stop(); }
		float refreshOscillator()	{ return oscillator.refresh(); }
		float getOscillatorOutput()	{ return oscillator.getOutput(); }
		void setOscillatorTime(unsigned long time) { oscillator.setTime(time); }
		Oscillator* getOscillator() { return &oscillator; }
		void updateServoPosition();
		void setupOscillator(uint16 period, int8 amplitude, int8 offset, uint16 phase);
		void loadGait(Gait* gait);
	private:
		uint8 leg_index;		//Leg index of the join
		uint8 joint_index;		//Joint index on the leg
		uint8 servo_driver_index;
		Oscillator oscillator;
};

#endif // WALKERJOINT_H
