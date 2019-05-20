#include "animation.h"
#include "animation.h"
#include "types.h"

Position::~Position()
{

}

PositionPoint *Position::getPoint(uint8 index)
{
	if (index > getPointCount() - 1)
		return 0;

	PositionPoint* points = getPoints();
	if (!points)
		return 0;
	return &points[index];
}

Position1::~Position1()
{

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

Position *Animation::getFrame(uint8 frame)
{
	//1) Check if target frame is within reach
	if (frame > frame_count - 1)
		return 0;

	//2) Get begin of frame list
	Position* target_frame = &frames[0];
	if (frame == 0)
		return target_frame;

	//3) Since positions size is variable, we need to compute the position of the desired frame
	target_frame = (Position*)
			(
				//1) Transform to byte pointer to move one byte at a time in memory
				((uint8*)target_frame) +
				//2) Move X positions that contains each Y pointers
				(sizeof(Position) + (sizeof (PositionPoint) * position_count)) * frame
			);

	return target_frame;
}

bool Animation::isLastFrame(uint8 frame_index)
{
	return (frame_index == frame_count - 1);
}

uint8	Animation::getNextFrameIndex(uint8 current_frame)
{
	//1) Check if we reached the last frame
	if (isLastFrame(current_frame))
	{ //Reached the last frame
		if (type != ANIMATION_TYPE_MOVEMENT)
			return current_frame; //This is not a loopable animation
		return 0;
	}

	//2) Next frame
	return current_frame + 1;
}

Position *Animation::getNextFrame(uint8 current_frame)
{
	if (isLastFrame(current_frame) && type != ANIMATION_TYPE_MOVEMENT)
		return 0;
	return getFrame(getNextFrameIndex(current_frame));
}
