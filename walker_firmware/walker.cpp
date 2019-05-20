#include "walker.h"
#include "arduinopolyfill.h"
#include "config.h"
#include <stdlib.h>
#include <stdio.h>

Walker::Walker()
{
	legs			= NULL;

	speed_multiplier	= 1.0f;
	leg_count			= 0;
	update_timer		= UPDATE_SPEED;

	next_animation		= NULL;
	current_animation	= NULL;
	next_frame			= NULL;
}

Walker::~Walker()
{
	freeLegs();
}

/* ------------------------------------------- */
/* ---------------   SETUP    ---------------- */
/* ------------------------------------------- */

/**
 * @brief Walker::freeLegs Free allocated memory
 */
void Walker::freeLegs()
{
	if (!legs)
		return;
	for (uint8 i = 0; i < leg_count; i++)
	{
		if (legs[i])
			delete legs[i];
	}
	free(legs);
	leg_count = 0;
	legs = NULL;
}

/**
 * @brief Walker::addLegs Add a number of leg
 * @param count
 */
void Walker::addLegs(uint8 count)
{
	//1) Free legs
	freeLegs();

	//2) Add the number of legs
	while (count-- > 0)
		addLeg();
}

/**
 * @brief Walker::getLeg Get target leg
 * @param index
 * @return
 */
WalkerLeg *Walker::getLeg(uint8 index)
{
	if (!legs || leg_count == 0 || index > leg_count - 1 || !legs[index])
		return NULL;

	return legs[index];
}

/**
 * @brief Walker::addLeg Add a new leg
 */
WalkerLeg* Walker::addLeg()
{
	//1) Create the new leg
	WalkerLeg* leg = new WalkerLeg(leg_count++);

	//2) Resize leg storage
	WalkerLeg** _legs = (WalkerLeg**)realloc(legs, leg_count * sizeof(WalkerLeg*));
	if (!_legs)
	{
		delete leg;
		return NULL;
	}

	//3) Store new leg
	_legs[leg_count - 1] = leg;
	legs = _legs;
	return leg;
}

/**
 * @brief Walker::loadNextFrameFor load next animation frame for
 * @param leg_index
 */
void Walker::loadFrameFor(uint8 frame_index, uint8 leg_index)
{
	//1) Check leg and frame validity
	if (!legs || leg_index > leg_count - 1 || !current_animation || frame_index > current_animation->frame_count - 1)
		return;

	//2) Get point in target frame for target leg
	//current_animation->
}

/* ------------------------------------------- */
/* ------------   MAIN UPDATE   -------------- */
/* ------------------------------------------- */

/**
 * @brief Walker::update Main walker update
 * @param diff
 */
void Walker::update(unsigned long diff)
{
	if (update_timer <= diff)
	{
		//1) Reset timer
		update_timer = UPDATE_SPEED;

		//2) Check legs
		if (!legs || leg_count == 0)
			return;

		//3) Check for movement change
		if (next_animation != NULL)
		{
			//Set new animation as current one
			current_frame = 0;
			current_animation = next_animation;
			next_animation = 0;
			//Load the first frame and the next frame
			loadFrame(current_animation->getFrame(0));
			next_frame = current_animation->getNextFrame(current_frame);
			//Activate legs
			toggleLegsRunning(true);
		}

		//4) Update joint positions
		bool movement_done = true;
		for (uint8 i = 0; i < leg_count; i++)
		{
			if (!legs[i])
				continue;
			if (legs[i]->updateJoints(diff)) //This leg has finished its movement, load the next frame in it
			{
				if (next_frame)
					legs[i]->moveTo(next_frame->getPoint(i));
			}
			else
				movement_done = false;
		}

		//5) Go to next frame if all the legs have finished their current movement
		if (movement_done && current_animation)
		{
			//5.1) Check if animation has a next frame and is loopable
			if (!next_frame)
			{ //Reached end of a non loopable animation
				/// - TBD - Return to an idle position/animation ?
				return;
			}
			//5.2) Go to next frame
			//Since every legs have reached their targeted position, they already loaded their
			//next position, just load the next position list
			current_frame = current_animation->getNextFrameIndex(current_frame);
			next_frame = current_animation->getNextFrame(current_frame);
			toggleLegsRunning(true); //activate legs
		}
	}
	else
		update_timer -= diff;
}

/**
 * @brief Walker::setNextAnimation Set the next animation to be played
 * @param animation
 */
void Walker::setNextAnimation(Animation *animation)
{
	//1) Check if we are not trying to load comething already here
	if (current_animation == animation || next_animation == animation)
		return;

	//2) Set the next animation
	next_animation = animation;
}

/**
 * @brief Walker::loadFrame Load an animation frame into each legs
 * @param position
 */
void Walker::loadFrame(Position *position)
{
	//1) Check walker state
	if (!legs)
		return;

	//2) Move each legs to target frame positions
	for (uint8 i = 0; i < leg_count; i++)
	{
		if (i >= position->getPointCount())
			break; //Frame doesn't have enough points
		if (!legs[i]) //Leg not loaded
			continue;
		legs[i]->moveTo(position->getPoint(i));
	}
}

/**
 * @brief Walker::toggleLegsRunning Toggle every leg update on or off
 * @param state
 */
void Walker::toggleLegsRunning(bool state)
{
	//1) Check walker state
	if (!legs)
		return;

	//2) Toggle every leg state
	for (uint8 i = 0; i < leg_count; i++)
	{
		if (!legs[i]) //Leg not loaded
			continue;
		legs[i]->toggleJoints(state);
	}
}

/* ------------------------------------------- */
/* -----------   SPEED CONTROL   ------------- */
/* ------------------------------------------- */

/* ------------------------------------------- */
/* ------------   STATIC GETTER -------------- */
/* ------------------------------------------- */
/**
 * @brief Walker::instance Static singleton getter
 * @return
 */
Walker* Walker::instance()
{
	//Static getter to allow access everywhere, singleton
	static Walker instance;
	return &instance;
}
