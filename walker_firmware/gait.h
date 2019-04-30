#ifndef GAIT_H
#define GAIT_H

#include "types.h"

enum GaitType: uint8
{
	GAIT_IDLE	= 0,	//Default, no movement
	GAIT_WALK	= 1,	//Moving, will be set as default gait after use
	GAIT_EMOTE	= 2,	//Emote to play, will be played a nmber of times defines by iteration
};

class Gait
{
	public:
		Gait(uint16 _id = 0);
		~Gait();
		uint16		getId()			{ return id; }
		uint8		getLegCount()	{ return leg_count; }
		uint8		getJointCount()	{ return joint_count; }
		GaitType	getType()		{ return  GAIT_IDLE; }
		uint8		getIterations()	{ return  iterations; }
		uint16		getTotalCount() { return (uint16)leg_count * (uint16)joint_count; }
		//Methods
		void		setId(uint16 _id);
		uint16		getPeriod(uint8 index);
		int8		getAmplitude(uint8 index);
		int8		getOffset(uint8 index);
		uint16		getPhase(uint8 index);
	protected:
		//Properties
		uint16		id;			// Dictionary id
		uint8		leg_count;	// Required number of legs
		uint8		joint_count;// Number of joint per leg
		uint8		type;		// Gait type : idle, walk or emote
		uint8		iterations;
		uint16		duration;	// Total time for a single whole pattern revolution
		//Gait sines datas
		uint16*		periods;	// Sine wave frequency, max is defined by servo's response time
		int8*		amplitudes;	// Servo min and max positions, affected by offset, -90 to 90
		int8*		offsets;	// Direct servo angle offset, -90 to 90
		uint16*		phases;		// Sine phase shift, 0 - 360°
		//Methods
		void setup();

		void updateDuration();

		void setPeriods(uint16* _periods);
		void setPeriod(int index, uint16 period);
		void fillPeriods(uint16 period);

		void setAmplitudes(int8* _amplitudes);
		void setAmplitude(int index, int8 amplitude);
		void fillAmplitudes(int8 amplitude);

		void setOffsets(int8* _offsets);
		void setOffset(int index, int8 offset);
		void fillOffsets(int8 offset);

		void setPhases(uint16* _phases);
		void setPhase(int index, uint16 phase);
		void fillPhases(uint16 phase);

		void setupServo(int index, uint16 period, int8 amplitude, int8 offset, uint16 phase);
};

//1 2 4 8 16 32 64 128
class Gait_4_2: public Gait
{ //4 legs and 2 joint per leg
	public:
		Gait_4_2(uint16 _id = 0): Gait(_id)
		{
			leg_count = 4;
			joint_count = 2;
			setup();
		}
};

class Gait_4_3: public Gait
{ //4 legs and 3 joints per leg
	public:
		Gait_4_3(uint16 _id = 0): Gait(_id)
		{
			leg_count = 4;
			joint_count = 3;
			setup();
		}
};

class Gait_6_2: public Gait
{ //6 legs and 2 joint per leg
	public:
		Gait_6_2(uint16 _id = 0): Gait(_id)
		{
			leg_count = 6;
			joint_count = 2;
			setup();
		}
};

class Gait_6_3: public Gait
{ //6 legs and 3 joint per leg
	public:
		Gait_6_3(uint16 _id = 0): Gait(_id)
		{
			leg_count = 6;
			joint_count = 3;
			setup();
		}
};

#endif // GAIT_H
