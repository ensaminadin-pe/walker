#include "gaitdictionary_4_2.h"
#include "gaits_4_2.h"
#include <unistd.h>

#define GAIT_4_2_COUNT 1

Gait* GaitDictionary_4_2::getGait(uint8 id)
{
	// A gait is 24 bytes uninitialized
	// It will reach 24 + (2 * (leg_count * joint_count)) + (4 * (leg_count * joint_count)) bytes
	// So for a 4 legs, 2 servos : 24 + 16 + 32 = 72 bytes
	// We cannot keep it in memory, a regular ATmega328 chip has only 2k byte memory

	switch (id)
	{ //Add your gaits here
		case 1: return new Gait_4_2_Idle(id);
	}
	return 0;
}
