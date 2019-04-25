#ifndef GAITS_4_2_H
#define GAITS_4_2_H

#include "../gait.h"
#include "../types.h"

class Gait_4_2_Idle: public Gait_4_2
{ //4 legs and 2 joint per leg
	public:
		Gait_4_2_Idle(uint8 _id) : Gait_4_2(_id) {}
};

#endif // GAITS_4_2_H
