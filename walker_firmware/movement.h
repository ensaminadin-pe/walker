#ifndef MOVEMENT_H
#define MOVEMENT_H

#include "types.h"

enum PositionOption : uint16
{
	POSITION_OPTION_NONE = 0
};

struct PositionPoint
{
	float x; //X & Y is used in inversed kinematics
	float y;
	float z; //Z is used to calculate top view rotation
};

struct Position
{
	PositionOption	option;			//Flag or something, do an action
	uint16			option_value;	//Option value, ex if flag = 1, option value will be an id of sound to play or something like that
	PositionPoint*	points;			//List of points for this position, one point per joint in correct order
	uint16			forced_delay;	//If not 0, this is the time in ms spent to execute this position, allow to bypass user modified speed
};

struct MovementFrame
{
	Position* positions;
};

struct Movement
{
	uint8			frame_count;
	uint8			type;
	MovementFrame*	frames;
};

#endif // MOVEMENT_H
