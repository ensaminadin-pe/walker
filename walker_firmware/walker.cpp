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
		/// - TODO - CHECK AND LOAD NEXT MOVEMENT

		//4) Update joint positions
		for (uint8 i = 0; i < leg_count; i++)
		{
			if (!legs[i])
				continue;
			legs[i]->updateJoints(diff);
		}
	}
	else
		update_timer -= diff;
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
