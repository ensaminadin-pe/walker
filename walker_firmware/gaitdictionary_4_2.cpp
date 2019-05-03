#include "gaitdictionary_4_2.h"
#include "gaits_4_2.h"
#include <unistd.h>
#include <stdio.h>

#define GAIT_4_2_COUNT 1

GaitDictionary_4_2::GaitDictionary_4_2(void) : GaitDictionary()
{

}

GaitDictionary_4_2::~GaitDictionary_4_2()
{

}

Gait *GaitDictionary_4_2::getDefaultGait()
{
	static Gait* defaultGait;
	if (!defaultGait)
		defaultGait = getGait(1);
	return defaultGait;
}

Gait* GaitDictionary_4_2::getGait(uint16 id)
{
	// A gait is 24 bytes uninitialized
	// It will reach 24 + (2 * (leg_count * joint_count)) + (4 * (leg_count * joint_count)) bytes
	// So for a 4 legs, 2 servos : 24 + 16 + 32 = 72 bytes
	// We cannot keep it in memory, a regular ATmega328 chip has only 2k byte memory
	switch (id)
	{ //Add your gaits here
		case 1: return new Gait_4_2_Idle(id);
		case 2: return new Gait_4_2_Fold(id);
		// 1000 - 1999 : Movement
		case 1000: return new Gait_4_2_Forward(id);
		case 1001: return new Gait_4_2_Turn_Left(id);
		case 1002: return new Gait_4_2_Turn_Right(id);

		case 1100: return new Gait_4_2_Crawl_Forward(id);
		case 1101: return new Gait_4_2_Crawl_Backward(id);
		case 1110: return new Gait_4_2_Crawl_Sideway_Left(id);
		case 1111: return new Gait_4_2_Crawl_Sideway_Right(id);

		case 1200: return new Gait_4_2_Strafe_Right(id);
		// 2000 - 2999 : Emotes
		case 2000: return new Gait_4_2_Dance(id);
		case 2100: return new Gait_4_2_Leg0(id);
		case 2101: return new Gait_4_2_Leg1(id);
		case 2102: return new Gait_4_2_Leg2(id);
		case 2103: return new Gait_4_2_Leg3(id);
		case 2110: return new Gait_4_2_Hello(id);
	}
	return 0;
}
