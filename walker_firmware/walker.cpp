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
	WalkerLeg* leg = new WalkerLeg();
	leg_count++;

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
			current_frame = 0;
			current_animation = next_animation;
			/// - TODO - Load first frame in legs
		}

		//4) Update joint positions
		bool movement_done = true;
		for (uint8 i = 0; i < leg_count; i++)
		{
			if (!legs[i])
				continue;
			if (legs[i]->updateJoints(diff))
			{
				/// - TODO - Load next movement on target leg
			}
			else
				movement_done = false;
		}

		//5) Go to next frame if all the legs have finished their current movement
		if (movement_done)
		{
			/// - TODO - Hangle next frame change/loop & movement exit on certain cases
			//current_frame++;
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
