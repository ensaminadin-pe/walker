#ifndef GAITS_4_2_H
#define GAITS_4_2_H

#include "gait.h"
#include "types.h"
#include <stdio.h>

#define DEFAULT_4_2_SPEED 800

class Gait_4_2_Idle: public Gait_4_2
{ //Stand still with every joints at 90°
	public:
		Gait_4_2_Idle(uint16 _id) : Gait_4_2(_id)
		{
			fillAmplitudes(0);
		}
};

class Gait_4_2_Fold: public Gait_4_2
{
	public:
		Gait_4_2_Fold(uint16 _id) : Gait_4_2(_id)
		{
			//Leg 0		  period, amplitude, offset, phase
			setupServo(0, DEFAULT_4_2_SPEED, 0, -60, 0);
			setupServo(1, DEFAULT_4_2_SPEED, 0, -85, 180);
			//Leg 1
			setupServo(2, DEFAULT_4_2_SPEED, 0, 60, 90);
			setupServo(3, DEFAULT_4_2_SPEED, 0, 85, 90);
			//Leg 2
			setupServo(4, DEFAULT_4_2_SPEED, 0, 60, 180);
			setupServo(5, DEFAULT_4_2_SPEED, 0, 85, 0);
			//Leg 3
			setupServo(6, DEFAULT_4_2_SPEED, 0, -60, 90);
			setupServo(7, DEFAULT_4_2_SPEED, 0, -85, 90);
		}
};

class Gait_4_2_Forward: public Gait_4_2
{
	public:
		Gait_4_2_Forward(uint16 _id) : Gait_4_2(_id)
		{
			//Leg 0		  period, amplitude, offset, phase
			setupServo(0, DEFAULT_4_2_SPEED, 15, 11, 0);
			setupServo(1, DEFAULT_4_2_SPEED, 15, -11, 0);
			//Leg 1
			setupServo(2, DEFAULT_4_2_SPEED, 15, -15, 90);
			setupServo(3, DEFAULT_4_2_SPEED, 15, 15, 90);
			//Leg 2
			setupServo(4, DEFAULT_4_2_SPEED, 15, -11, 180);
			setupServo(5, DEFAULT_4_2_SPEED, 15, 11, 180);
			//Leg 3
			setupServo(6, DEFAULT_4_2_SPEED, 15, 15, 90);
			setupServo(7, DEFAULT_4_2_SPEED, 15, -15, 90);
		}
};

class Gait_4_2_Turn_Left: public Gait_4_2
{
	public:
		Gait_4_2_Turn_Left(uint16 _id) : Gait_4_2(_id)
		{
			//Leg 0
			setupServo(0, DEFAULT_4_2_SPEED, 15, 15, 180);
			setupServo(1, DEFAULT_4_2_SPEED, 15, -15, 0);
			//Leg 1
			setupServo(2, DEFAULT_4_2_SPEED, 15, 0, 90);
			setupServo(3, DEFAULT_4_2_SPEED, 15, 0, 90);
			//Leg 2
			setupServo(4, DEFAULT_4_2_SPEED, 15, -15, 0);
			setupServo(5, DEFAULT_4_2_SPEED, 15, 15, 180);
			//Leg 3
			setupServo(6, DEFAULT_4_2_SPEED, 15, 0, 90);
			setupServo(7, DEFAULT_4_2_SPEED, 15, 0, 90);
		}
};

class Gait_4_2_Turn_Right: public Gait_4_2
{
	public:
		Gait_4_2_Turn_Right(uint16 _id) : Gait_4_2(_id)
		{
			//Leg 0
			setupServo(0, DEFAULT_4_2_SPEED, 15, 15, 0);
			setupServo(1, DEFAULT_4_2_SPEED, 15, -15, 180);
			//Leg 1
			setupServo(2, DEFAULT_4_2_SPEED, 15, 0, 90);
			setupServo(3, DEFAULT_4_2_SPEED, 15, 0, 90);
			//Leg 2
			setupServo(4, DEFAULT_4_2_SPEED, 15, -15, 180);
			setupServo(5, DEFAULT_4_2_SPEED, 15, 15, 0);
			//Leg 3
			setupServo(6, DEFAULT_4_2_SPEED, 15, 0, 90);
			setupServo(7, DEFAULT_4_2_SPEED, 15, 0, 90);
		}
};

class Gait_4_2_Dance: public Gait_4_2
{
	public:
		Gait_4_2_Dance(uint16 _id) : Gait_4_2(_id)
		{
			//Leg 0
			setupServo(0, DEFAULT_4_2_SPEED, 15, 15, 0);
			setupServo(1, DEFAULT_4_2_SPEED, 15, -15, 0);
			//Leg 1
			setupServo(2, DEFAULT_4_2_SPEED, 15, 0, 0);
			setupServo(3, DEFAULT_4_2_SPEED, 15, 0, 270);
			//Leg 2
			setupServo(4, DEFAULT_4_2_SPEED, 15, -15, 0);
			setupServo(5, DEFAULT_4_2_SPEED, 15, 15, 0);
			//Leg 3
			setupServo(6, DEFAULT_4_2_SPEED, 15, 0, 90);
			setupServo(7, DEFAULT_4_2_SPEED, 15, 0, 180);
		}
};

class Gait_4_2_Leg0: public Gait_4_2
{
	public:
		Gait_4_2_Leg0(uint16 _id) : Gait_4_2(_id)
		{
			fillAmplitudes(0);
			//Leg 0
			setupServo(0, DEFAULT_4_2_SPEED, 15, 0, 0);
			setupServo(1, DEFAULT_4_2_SPEED, 0, 50, 0);
		}
};

class Gait_4_2_Leg1: public Gait_4_2
{
	public:
		Gait_4_2_Leg1(uint16 _id) : Gait_4_2(_id)
		{
			fillAmplitudes(0);
			//Leg 1
			setupServo(2, DEFAULT_4_2_SPEED, 15, 0, 0);
			setupServo(3, DEFAULT_4_2_SPEED, 0, -50, 0);
		}
};

class Gait_4_2_Leg2: public Gait_4_2
{
	public:
		Gait_4_2_Leg2(uint16 _id) : Gait_4_2(_id)
		{
			fillAmplitudes(0);
			//Leg 2
			setupServo(4, DEFAULT_4_2_SPEED, 15, 0, 0);
			setupServo(5, DEFAULT_4_2_SPEED, 0, -50, 0);
		}
};

class Gait_4_2_Leg3: public Gait_4_2
{
	public:
		Gait_4_2_Leg3(uint16 _id) : Gait_4_2(_id)
		{
			fillAmplitudes(0);
			//Leg 3
			setupServo(6, DEFAULT_4_2_SPEED, 15, 0, 0);
			setupServo(7, DEFAULT_4_2_SPEED, 0, 50, 0);
		}
};
#endif // GAITS_4_2_H
