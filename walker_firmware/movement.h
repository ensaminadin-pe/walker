#ifndef MOVEMENT_H
#define MOVEMENT_H

#include "types.h"

enum PositionOption : uint16
{
	POSITION_OPTION_NONE = 0
};

class PositionPoint
{
	public:
		PositionPoint() = default;
		PositionPoint(float _x, float _y, float _z): x{_x}, y{_y}, z{_z} {}

		float x; //X & Y is used in inversed kinematics
		float y;
		float z; //Z is used to calculate top view rotation
};

class PointList
{
	public:
		uint8 size;
		PositionPoint points[1];

		PointList() = default;
		virtual uint8 getSize() { return 1; }
};

class PointList2 : public PointList
{
	public:
		PointList2() = default;
		PointList2(const PositionPoint& p1, const PositionPoint& p2): points{p1, p2}
		{}
		PositionPoint points[2];
		uint8 getSize() override { return 2; }
};

class PointList3 : public PointList2
{
	public:
		PointList3() = default;
		PositionPoint points[3];
		uint8 getSize() override { return 3; }
};

class PointList4 : public PointList3
{
	public:
		PointList4() = default;
		PositionPoint points[4];
		uint8 getSize() override { return 4; }
};

class PointList5 : public PointList4
{
	public:
		PointList5() = default;
		PositionPoint points[5];
		uint8 getSize() override { return 5; }
};
/*
class PointList6 : public PointList5
{

	public:
		PositionPoint p6;

		PointList6() = default;
		PointList6(const PositionPoint& _p1,
				   const PositionPoint& _p2,
				   const PositionPoint& _p3, PositionPoint _p4, PositionPoint _p5, PositionPoint _p6): p1{_p1}, p2{_p2}, p3{_p3}, p4{_p4}, p5{_p5}, p6{_p6}
		{
		}
		uint8 getSize() override { return 6; }
};
*/

class Position
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
