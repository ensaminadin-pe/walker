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
		virtual ~Position();
		virtual uint8 getPointCount() { return 1; }
		virtual PositionPoint* getPoints() { return &points[0]; }
		PositionPoint* getPoint(uint8 index);
	protected:
		PositionOption	option;
		uint16			option_value;
		uint16			forced_delay;
		PositionPoint	points[1];
};

/**
 * @brief The Animation struct
 * Main animation structure
 */
struct Animation
{
	uint8			frame_count;
	AnimationType	type;
	Position*		frames;
};

class Position2 : public Position
{
	public:
		Position2() = default;
		Position2(const PositionPoint& p1, const PositionPoint& p2):
			points{p1, p2} {}
		Position2(const PositionOption _option, const uint16 _option_value, const uint16 _forced_delay, const PositionPoint& p1, const PositionPoint& p2):
			option{_option}, option_value{_option_value}, forced_delay{_forced_delay},
			points{p1, p2}
		{
		}
		uint8 getPointCount() override { return 2; }
		PositionPoint* getPoints() override { return &points[0]; }
		~Position2() override;
	protected:
		PositionOption	option;
		uint16			option_value;
		uint16			forced_delay;
		PositionPoint	points[2];
};

class Position3 : public Position
{
	public:
		Position3() = default;
		Position3(const PositionPoint& p1, const PositionPoint& p2, const PositionPoint& p3):
			points{p1, p2, p3} {}
		Position3(const PositionOption _option, const uint16 _option_value, const uint16 _forced_delay, const PositionPoint& p1, const PositionPoint& p2, const PositionPoint& p3):
			option{_option}, option_value{_option_value}, forced_delay{_forced_delay},
			points{p1, p2, p3} {}
		~Position3() override;
		uint8 getPointCount() override { return 3; }
		PositionPoint* getPoints() override { return &points[0]; }
	protected:
		PositionOption	option;
		uint16			option_value;
		uint16			forced_delay;
		PositionPoint	points[3];
};

class Position4 : public Position
{
	public:
		Position4() = default;
		Position4(const PositionPoint& p1, const PositionPoint& p2, const PositionPoint& p3, const PositionPoint& p4):
			points{p1, p2, p3, p4} {}
		Position4(const PositionOption _option, const uint16 _option_value, const uint16 _forced_delay, const PositionPoint& p1, const PositionPoint& p2, const PositionPoint& p3, const PositionPoint& p4):
			option{_option}, option_value{_option_value}, forced_delay{_forced_delay},
			points{p1, p2, p3, p4} {}
		~Position4() override;
		uint8 getPointCount() override { return 4; }
		PositionPoint* getPoints() override { return &points[0]; }
	protected:
		PositionOption	option;
		uint16			option_value;
		uint16			forced_delay;
		PositionPoint	points[4];
};

class Position5 : public Position
{
	public:
		Position5() = default;
		Position5(const PositionPoint& p1, const PositionPoint& p2, const PositionPoint& p3, const PositionPoint& p4, const PositionPoint& p5):
			points{p1, p2, p3, p4, p5} {}
		Position5(const PositionOption _option, const uint16 _option_value, const uint16 _forced_delay, const PositionPoint& p1, const PositionPoint& p2, const PositionPoint& p3, const PositionPoint& p4, const PositionPoint& p5):
			option{_option}, option_value{_option_value}, forced_delay{_forced_delay},
			points{p1, p2, p3, p4, p5} {}
		~Position5() override;
		uint8 getPointCount() override { return 5; }
		PositionPoint* getPoints() override { return &points[0]; }
	protected:
		PositionOption	option;
		uint16			option_value;
		uint16			forced_delay;
		PositionPoint	points[5];
};

class Position6 : public Position
{
	public:
		Position6() = default;
		Position6(const PositionPoint& p1, const PositionPoint& p2, const PositionPoint& p3, const PositionPoint& p4, const PositionPoint& p5, const PositionPoint& p6):
			points{p1, p2, p3, p4, p5, p6} {}
		Position6(const PositionOption _option, const uint16 _option_value, const uint16 _forced_delay, const PositionPoint& p1, const PositionPoint& p2, const PositionPoint& p3, const PositionPoint& p4, const PositionPoint& p5, const PositionPoint& p6):
			option{_option}, option_value{_option_value}, forced_delay{_forced_delay},
			points{p1, p2, p3, p4, p5, p6} {}
		~Position6() override;
		uint8 getPointCount() override { return 6; }
		PositionPoint* getPoints() override { return &points[0]; }
	protected:
		PositionOption	option;
		uint16			option_value;
		uint16			forced_delay;
		PositionPoint	points[6];
};

#endif // ANIMATION_H
