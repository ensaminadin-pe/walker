#ifndef ANIMATION_H
#define ANIMATION_H

#include "types.h"

enum PositionOption : uint16
{
	POSITION_OPTION_NONE = 0
};

enum AnimationType : uint8
{
	ANIMATION_TYPE_NONE = 0,
	ANIMATION_TYPE_MOVEMENT,
	ANIMATION_TYPE_EMOTE
};

/**
 * @brief The PositionPoint class A point in space
 */
class PositionPoint
{
	public:
		PositionPoint() = default;
		PositionPoint(float _x, float _y, float _z): x{_x}, y{_y}, z{_z} {}

		float x; //X & Y is used in inversed kinematics
		float y;
		float z; //Z is used to calculate top view rotation
};

/**
 * @brief The Position class List of manipulator tip positions
 */
class Position
{
	public:
		Position() = default;
		Position(PositionOption _option, uint16 _option_value, uint16 _forced_delay): option{_option}, option_value{_option_value}, forced_delay{_forced_delay}
		{
		}
		virtual ~Position();
		virtual uint8 getPointCount() { return 1; }
		virtual PositionPoint* getPoints() { return 0; }
		PositionPoint* getPoint(uint8 index);
	protected:
		PositionOption	option;
		uint16			option_value;
		uint16			forced_delay;
};

/**
 * @brief The Animation struct
 * Main animation structure
 */
class Animation
{
	public:
		//Properties
		Animation(const uint8 _frame_count, const uint8 _position_count, const AnimationType _type, Position* _frames):
			frame_count{_frame_count}, position_count{_position_count}, type{_type}, frames{_frames}
		{

		}
		uint8			frame_count;	//Total frame count
		uint8			position_count;	//Number of positions in each frames
		AnimationType	type;
		Position*		frames;
		//Methods
		Position*		getFrame(uint8 frame);
};

class Position1 : public Position
{
	public:
		Position1() = default;
		Position1(const PositionPoint& p1):
			points{p1} {}
		Position1(const PositionOption _option, const uint16 _option_value, const uint16 _forced_delay, const PositionPoint& p1):
			Position(_option, _option_value, _forced_delay),
			points{p1}
		{
		}
		uint8 getPointCount() override { return 1; }
		PositionPoint* getPoints() override { return &points[0]; }
		~Position1() override;
	protected:
		PositionPoint	points[1];
};

class Position2 : public Position
{
	public:
		Position2() = default;
		Position2(const PositionPoint& p1, const PositionPoint& p2):
			points{p1, p2} {}
		Position2(const PositionOption _option, const uint16 _option_value, const uint16 _forced_delay, const PositionPoint& p1, const PositionPoint& p2):
			Position(_option, _option_value, _forced_delay),
			points{p1, p2}
		{
		}
		uint8 getPointCount() override { return 2; }
		PositionPoint* getPoints() override { return &points[0]; }
		~Position2() override;
	protected:
		PositionPoint	points[2];
};

class Position3 : public Position
{
	public:
		Position3() = default;
		Position3(const PositionPoint& p1, const PositionPoint& p2, const PositionPoint& p3):
			points{p1, p2, p3} {}
		Position3(const PositionOption _option, const uint16 _option_value, const uint16 _forced_delay, const PositionPoint& p1, const PositionPoint& p2, const PositionPoint& p3):
			Position(_option, _option_value, _forced_delay),
			points{p1, p2, p3} {}
		~Position3() override;
		uint8 getPointCount() override { return 3; }
		PositionPoint* getPoints() override { return &points[0]; }
	protected:
		PositionPoint	points[3];
};

class Position4 : public Position
{
	public:
		Position4() = default;
		Position4(const PositionPoint& p1, const PositionPoint& p2, const PositionPoint& p3, const PositionPoint& p4):
			points{p1, p2, p3, p4} {}
		Position4(const PositionOption _option, const uint16 _option_value, const uint16 _forced_delay, const PositionPoint& p1, const PositionPoint& p2, const PositionPoint& p3, const PositionPoint& p4):
			Position(_option, _option_value, _forced_delay),
			points{p1, p2, p3, p4} {}
		~Position4() override;
		uint8 getPointCount() override { return 4; }
		PositionPoint* getPoints() override { return &points[0]; }
	protected:
		PositionPoint	points[4];
};

class Position5 : public Position
{
	public:
		Position5() = default;
		Position5(const PositionPoint& p1, const PositionPoint& p2, const PositionPoint& p3, const PositionPoint& p4, const PositionPoint& p5):
			points{p1, p2, p3, p4, p5} {}
		Position5(const PositionOption _option, const uint16 _option_value, const uint16 _forced_delay, const PositionPoint& p1, const PositionPoint& p2, const PositionPoint& p3, const PositionPoint& p4, const PositionPoint& p5):
			Position(_option, _option_value, _forced_delay),
			points{p1, p2, p3, p4, p5} {}
		~Position5() override;
		uint8 getPointCount() override { return 5; }
		PositionPoint* getPoints() override { return &points[0]; }
	protected:
		PositionPoint	points[5];
};

class Position6 : public Position
{
	public:
		Position6() = default;
		Position6(const PositionPoint& p1, const PositionPoint& p2, const PositionPoint& p3, const PositionPoint& p4, const PositionPoint& p5, const PositionPoint& p6):
			points{p1, p2, p3, p4, p5, p6} {}
		Position6(const PositionOption _option, const uint16 _option_value, const uint16 _forced_delay, const PositionPoint& p1, const PositionPoint& p2, const PositionPoint& p3, const PositionPoint& p4, const PositionPoint& p5, const PositionPoint& p6):
			Position(_option, _option_value, _forced_delay),
			points{p1, p2, p3, p4, p5, p6} {}
		~Position6() override;
		uint8 getPointCount() override { return 6; }
		PositionPoint* getPoints() override { return &points[0]; }
	protected:
		PositionPoint	points[6];
};

#endif // ANIMATION_H
