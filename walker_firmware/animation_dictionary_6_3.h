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
	//				opt							opt_val	delay	LEG 1					LEG 2				LEG 3				LEG 4				LEG 5				LEG 6
	/* frame  1 */ {POSITION_OPTION_NONE,		0,		0,		{690.0, 900.0, 0.0},	{0, 0, 0},	{2.0, 2.1, 2.2},	{3.0, 3.1, 3.2},	{4.0, 4.1, 4.2},	{5.0, 5.1, 5.2}},
	/* frame  2 */ {POSITION_OPTION_NONE,		0,		0,		{10.0, 10.1, 0.0},	{11.0, 1.1, 1.2},	{12.0, 12.1, 12.2},	{13.0, 13.1, 13.2},	{14.0, 14.1, 14.2},	{15.0, 15.1, 15.2}},
	/* frame  3 */ {POSITION_OPTION_NONE,		0,		0,		{20.0, 20.1, 0.0},	{21.0, 1.1, 1.2},	{22.0, 22.1, 22.2},	{23.0, 23.1, 23.2},	{24.0, 24.1, 24.2},	{25.0, 25.1, 25.2}},
	/* frame  4 */ {POSITION_OPTION_NONE,		0,		0,		{30.0, 30.1, 0.0},	{31.0, 31.1, 31.2},	{32.0, 32.1, 32.2},	{33.0, 33.1, 33.2},	{34.0, 34.1, 34.2},	{35.0, 35.1, 35.2}},
	/* frame  5 */ {POSITION_OPTION_NONE,		0,		0,		{40.0, 40.1, 0.0},	{41.0, 41.1, 41.2},	{42.0, 42.1, 42.2},	{43.0, 43.1, 43.2},	{44.0, 44.1, 44.2},	{45.0, 45.1, 45.2}}
};

Animation move_6_3_forward_walk {5, 6, ANIMATION_TYPE_MOVEMENT, (Position*)&move_6_3_forward_walk_positions[0]};

const Position1 move_6_3_test_positions[] {
	//				opt							opt_val	delay	LEG 1					LEG 2				LEG 3				LEG 4				LEG 5				LEG 6
	/* frame  1 */ {POSITION_OPTION_NONE,		0,		0,		{0, 5, 9.0}},
	/* frame  1 */ {POSITION_OPTION_NONE,		0,		0,		{0, 10, 9.0}},
};
Animation move_6_3_test {2, 1, ANIMATION_TYPE_MOVEMENT, (Position*)&move_6_3_test_positions[0]};

/// ----- EMOTES

/// ----- DICTIONARY

#endif
