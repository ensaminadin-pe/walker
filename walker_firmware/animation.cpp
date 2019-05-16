#include "animation.h"

Position::~Position()
{

}

PositionPoint *Position::getPoint(uint8 index)
{
	if (index > getPointCount() - 1)
		return 0;

	PositionPoint* points = getPoints();
	return &points[index];
}

Position2::~Position2()
{

}

Position3::~Position3()
{

}

Position4::~Position4()
{

}

Position5::~Position5()
{

}


Position6::~Position6()
{

}
