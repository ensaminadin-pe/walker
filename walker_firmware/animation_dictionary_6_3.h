#ifndef ANIMATION_DICTIONARY_6_3_H
#define ANIMATION_DICTIONARY_6_3_H

#include "animation.h"
/// Benchmark : 1 movement of 5 frames that contains a pointlist of 6 points each took 756 bytes in program memory once compiled with the arduino IDE
/// Its less than 0.1% of the total space on an ESP8266-E12
/// Animations might get up to 20-30 frames, but it should be ok
/// Need to try that with the MG92B : https://github.com/glumb/feedback-servo
/// If it works, we could record animations by moving the robot

/// ----- MOVEMENTS
// - Forward walk
const Position6 move_6_3_forward_walk_positions[] {
	//				opt							opt_val	delay	LEG 1		LEG 2		LEG 3		LEG 4	LEG 5		LEG 6
	/* frame  1 */ {POSITION_OPTION_NONE,		0,		0,		{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}},
	/* frame  1 */ {POSITION_OPTION_NONE,		0,		0,		{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}},
	/* frame  1 */ {POSITION_OPTION_NONE,		0,		0,		{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}},
	/* frame  1 */ {POSITION_OPTION_NONE,		0,		0,		{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}},
	/* frame  1 */ {POSITION_OPTION_NONE,		0,		0,		{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}}
};

const Animation move_6_3_forward_walk {1, ANIMATION_TYPE_MOVEMENT, (Position*)&move_6_3_forward_walk_positions[0]};

/// ----- EMOTES


#endif
