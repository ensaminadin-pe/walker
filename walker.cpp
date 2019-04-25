#include "walker.h"
#include "arduinopolyfill.h"
#include "config.h"
#include <stdlib.h>
#include <stdio.h>

/***
 * Walker
 *
 * Leg & joint placement & indexes
 * Example for a 6_2 (6legs & 2 joints per leg)
 *
 *  j01-j00-l0##l1-j02-j03
 *	        #    #
 *  j05-j04-l2  l3-j06-j07
 *          #    #
 *  j09-j08-l4##l5-j10-j10
 *
 ***/
Walker::Walker()
{
	legs		= NULL;
	oscillators	= NULL;
	joints		= NULL;
	gait		= NULL;
	next_gait	= NULL;

	speed_multiplier	= 1.0f;
	leg_count			= 0;
	joint_count			= 0;
	update_timer		= UPDATE_SPEED;
}

Walker::~Walker()
{
	reset();
	freeLegs();
}

/* ------------------------------------------- */
/* ---------------   SETUP    ---------------- */
/* ------------------------------------------- */

/**
 * @brief Walker::setup Setup a nomber of leg with a number of joints
 * @param leg_count
 * @param joint_count
 */
void Walker::setup(uint8 leg_count, uint8 joint_count)
{
	addLegs(leg_count);
	addJoints(joint_count);
}

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
 * @brief Walker::addLeg Add a new leg
 */
void Walker::addLeg()
{
	//1) Create the new leg
	WalkerLeg* leg = new WalkerLeg(this);
	leg_count++;
	//2) Resize leg storage
	WalkerLeg** _legs = (WalkerLeg**)realloc(legs, leg_count * sizeof(WalkerLeg*));
	if (!_legs)
		return;
	//3) Store new leg
	_legs[leg_count - 1] = leg;
	legs = _legs;
}

/**
 * @brief Walker::addJoints Add given number of joint to each legs
 * @param count
 */
void Walker::addJoints(uint8 count)
{
	if (!legs)
		return;
	for (uint8 i = 0; i < leg_count; i++)
	{
		if (!legs[i])
			continue;
		legs[i]->freeJoints();
		legs[i]->addJoints(count);
	}
	joint_count = count;
}

/**
 * @brief Walker::init Init the walker's Oscillators and Servos
 */
void Walker::init()
{
	//1) Clear previous running config
	reset();

	//2) Look at the first leg to get the joint number
	if (leg_count == 0 || !legs || !legs[0])
		return; //No legs, nothing to do
	joint_count = legs[0]->getJointCount();

	//3) Reference joints & Init Oscillators
	int index = 0;
	joints = (WalkerJoint**)malloc((getTotalCount()) * sizeof(WalkerJoint*));
	oscillators = (Oscillator**)malloc((getTotalCount()) * sizeof(Oscillator*));
	if (!joints || !oscillators)
		return;

	for (uint8 i = 0; i < leg_count; i++)
	{
		for (uint8 j = 0; j < joint_count; j++)
		{
			if (!legs[i] || !legs[i]->getJoint(j))
				continue;
			joints[index] = legs[i]->getJoint(j);
			oscillators[index] = new Oscillator();
			index++;
		}
	}
}

/**
 * @brief Walker::reset Reset current initialization
 */
void Walker::reset()
{
	//1) Delete & free oscillators
	if (oscillators)
	{
		for (uint16 i = 0; i < (getTotalCount()); i++)
		{
			if (oscillators[i])
				delete oscillators[i];
		}
		free(oscillators);
		oscillators = NULL;
	}

	//2) Free joints list
	if (joints)
	{
		free(joints);
		joints = NULL;
	}
}

/* ------------------------------------------- */
/* ------------   MAIN UPDATE   -------------- */
/* ------------------------------------------- */

/**
 * @brief Walker::update Main walker update
 * @param diff
 */
void Walker::update(int diff)
{
	if (update_timer <= 0)
	{
		//1) Check for gait change
		if (next_gait)
		{
			clearGait();
			loadNextGait();
		}
		//2) Update Oscillators & servos
		updateServoPositions();
		//3) Reset timer
		update_timer = UPDATE_SPEED;
	}
	else
		update_timer -= diff;
}

/* ------------------------------------------- */
/* ------------   GAIT UPDATE   -------------- */
/* ------------------------------------------- */

/**
 * @brief Walker::updateServoPositions Update oscillators and set servos to target positions
 */
void Walker::updateServoPositions()
{
	if (!oscillators)
		return;

	for (uint16 i = 0; i < (getTotalCount()); i++)
	{
		if (oscillators[i])
			oscillators[i]->refresh();
	}

}

/**
 * @brief Walker::clearGait Clear current gait and halt oscillators
 */
void Walker::clearGait()
{
	if (gait)
		delete gait;
	if (oscillators)
	{
		for (uint16 i = 0; i < (getTotalCount()); i++)
		{
			if (oscillators[i])
				oscillators[i]->stop();
		}
	}
}

/**
 * @brief Walker::loadNextGait Load next gait if any is presented
 */
void Walker::loadNextGait()
{
	//1) Check if a gait is provided and clear current gait if needed
	if (!next_gait)
		return;
	if (gait)
		clearGait();
	gait = next_gait;
	next_gait = NULL;

	//2) Setup oscillators
	if (!oscillators)
		return;
	unsigned long time = millis();
	for (uint16 i = 0; i < (getTotalCount()); i++)
	{
		if (!oscillators[i])
			continue;

		oscillators[i]->setPeriodModifier(&speed_multiplier);
		oscillators[i]->setPeriod(gait->getPeriod(i));
		oscillators[i]->setAmplitude(gait->getAmplitude(i));
		oscillators[i]->setOffset(gait->getOffset(i));
		oscillators[i]->setPhase(gait->getPhase(i));
		oscillators[i]->start();
		oscillators[i]->setTime(time);
	}
}

/**
 * @brief Walker::setNextGait Call for a Gait change
 * @param gait
 */
void Walker::setNextGait(Gait *_gait)
{
	//1) Check gait validity
	if (
		(gait && gait->getId() == _gait->getId()) || //Cannot set the same gait twice
		(_gait->getLegCount() != getLegCount() || _gait->getJointCount() != getJointCount()) //Gait doesn't fit
	)
	{ // clear and skip assignation
		delete _gait;
		return;
	}

	//2) Already chose something, trash it before setting the new thing
	if (this->next_gait)
		delete this->next_gait;

	//3) Save provided gait for next update
	this->next_gait = _gait;
}

/* ------------------------------------------- */
/* -----------   SPEED CONTROL   ------------- */
/* ------------------------------------------- */

void Walker::setGaitSpeed(float speed)
{
	if (speed > MAX_GAIT_SPEED)
		speed = MAX_GAIT_SPEED;
	if (speed < MIN_GAIT_SPEED)
		speed = MIN_GAIT_SPEED;

	speed_multiplier = speed;
}

float Walker::increaseGaitSpeed()
{
	if ((speed_multiplier + GAIT_SPEED_INCREMENT) >= MAX_GAIT_SPEED)
		speed_multiplier = MAX_GAIT_SPEED;
	else
		speed_multiplier += GAIT_SPEED_INCREMENT;

	return speed_multiplier;
}

float Walker::decreaseGaitSpeed()
{
	if ((speed_multiplier - GAIT_SPEED_INCREMENT) <= MIN_GAIT_SPEED)
		speed_multiplier = MIN_GAIT_SPEED;
	else
		speed_multiplier -= GAIT_SPEED_INCREMENT;

	return speed_multiplier;
}

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

/*
void Walker::print()
{
	int left_padding = 4 + joint_count + (joint_count * 2);
	int servo_index = 0;

	printf("Loaded walker\n");
	printf("Leg count : %i \n", leg_count);
	printf("Servo per leg : %i \n\n", joint_count);

	for (int i = 0; i < leg_count; i++)
	{
		bool is_left = (i % 2 == 0);
		//LEFT
		if (is_left)
		{
			printf("[l%i]", i);
			for (int j = joint_count; j > 0; j--)
				printf("%i--", servo_index++);
			//MIDDLE
			printf("####");
		}
		//RIGHT
		if (!is_left)
		{
			for (int j = 0; j < joint_count; j++)
				printf("%i--", servo_index++);
			printf("[l%i]\n", i);

			if (i == leg_count - 1)
				break;
			//SPACER
			for (int k = 0; k < left_padding; k++)
				printf(" ");
			printf("####\n");
		}
	}
}
*/
