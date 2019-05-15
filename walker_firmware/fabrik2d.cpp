/**********************************************************************************************
 * FABRIK 2D inverse kinematics solver - Version 1.0.0
 * by Henrik Söderlund <henrik.a.soderlund@gmail.com>
 *
 * Copyright (C) 2018 Henrik Söderlund
 *
 *     This program is free software: you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation, either version 3 of the License, or
 *     (at your option) any later version.
 *
 *     This program is distributed in the hope that it will be useful,
 *     but WITHOUT ANY WARRANTY; without even the implied warranty of
 *     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *     GNU General Public License for more details.
 *
 *     You should have received a copy of the GNU General Public License
 *     along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 **********************************************************************************************/

#ifndef IS_QT
	#include "Arduino.h"
#else
	#include "arduinopolyfill.h"
	#include <math.h>
#endif
#include "fabrik2d.h"
#include "kinematic.h"
#include <stdlib.h>

Fabrik2D::Fabrik2D(float _tolerance)
{
	tolerance = _tolerance; //1mm
	joint_count = 0;
	chain_length = 0;
	joint_chain = NULL;
	joints_axis = KINEMATIC_AXIS_UNDEFINED;
}

Fabrik2D::~Fabrik2D()
{

}

/**
 * @brief Fabrik2D::addJoint Add a joint to the kinematic chain
 * @param joint
 * @return
 */
bool Fabrik2D::addJoint(WalkerJoint *joint)
{
	//1) Check that the chain exist
	if (!joint_chain)
	{ //Create chain and last point
		joint_chain = (WalkerJoint**)malloc(sizeof(WalkerJoint*));
		if (!joint_chain) // Could not allocate memory, error
			return false;
		WalkerJoint* last_joint = new WalkerJoint();
		if (!last_joint)
			return false;
		joint_chain[0] = last_joint;
		joint_count = 1;
	}

	//2) Check that joint can be added
	if (joints_axis != KINEMATIC_AXIS_UNDEFINED && joint->getAxis() != joints_axis)
		return false; //Cannot add a joint that doesnt rotate on the same axis as the others, this is 2D Fabrik
	joints_axis = joint->getAxis();

	//3) Resize joint chain
	uint8 joint_index = joint_count - 1;
	joint_chain = (WalkerJoint**)realloc(joint_chain, ++joint_count * sizeof(WalkerJoint*));
	if (!joint_chain)
		return false;

	//4) Store new joint in chain at correct spot
	joint_chain[joint_count - 1] = joint_chain[joint_index]; //Last joint is currently at the new joint position, move it
	joint_chain[joint_index] = joint; //Set new joint at its correct position : one before last

	//5) Rebuild joint position for new joint and last joint
	//4.1) Get previous joint if any
	if (joint_index > 0) //Origin is at 0;0, no need to check it
	{
		placeNextJoint(joint_chain[joint_index - 1], joint); //Update positon x;y of our new joint
		placeNextJoint(joint_chain[joint_index], joint_chain[joint_index + 1]); //Update positon x;y of last joint
	}

	//6) Increase chain lenght
	chain_length += joint->getKinematicDistance();

	return true;
}

/**
 * @brief Fabrik2D::getJoint Get loaded joint at target index
 * @param joint_index
 * @return
 */
WalkerJoint *Fabrik2D::getJoint(uint8 joint_index)
{
	if (!joint_chain || joint_index < joint_count - 1 || !joint_chain[joint_count])
		return NULL;
	return joint_chain[joint_count];
}

/**
 * @brief Fabrik2D::reachFor Main resolution function
 * @param target_x
 * @param target_y
 * @return
 */
bool Fabrik2D::reachFor(float target_x, float target_y)
{
	///The basics of FABRIK
	/// http://www.andreasaristidou.com/publications/papers/FABRIK.pdf
	///  1) Check that the target point iw reachable by checking the full length of the chain
	///		If not, our program will move every joint points on a straight line toward the target
	///  2) If the distance can be reached, we need to recalculate the point positions of each joints on the 2D plane
	///     This is done in two stages :
	///		2.1) Backward search
	///			Start by moving the last point (n) of the chain on the target point itself.
	///			Then we need to find a line that passes threw n and n-1, and move n-1 on that line
	///			to the predefined joint distance in n-1, this will be the new positions of n-1.
	///			We repeat this process between n-1 and n-2, and so on until we reach the first point.
	///			But at this point, the first point of the chain is not at the origin anymore, so we need stage 2.
	///		2.2) Forward search
	///			Now we do exactly the same thing as before, but forward
	///			Move the first point of the chain (n) to the origin (0, 0).
	///			move n+1 on a line between n and n+1 to match the predefined distance between n and n+1.
	///			Do that for every points on your chain.
	///			Now our endpoint should be very close of the target point, but maybe a little bit too off,
	///			we can repeat the whole procedure to put it even closer, but reaching the point exactly might
	///			be a little bit too hard for the program, so add a little tolerance.
	///  3) Now you have a series of points on a 2d plane, you can calculate the angles between
	///     the differents vectors, giving you the angles of the joints
	if (joint_count < 2) //Nothing to do with that
		return false;

	//1) Check whether the target is within reach
	if (chain_length < get2dDistance(0, 0, target_x, target_y))
	{ // The target is unreachable, move every joint position in a straight line towards it
		for (uint8 i = 0; i < joint_count - 2; i++)
			relocateJoint(joint_chain[i], target_x, target_y, joint_chain[i]->getKinematicDistance());
	   return false;
	}

	//2) Target is reachable
	for (uint8 iteration = 0; iteration < 10; iteration++)
	{ //Execute the algorithm a maximum of 10 times
		//2.1) Stage 1 : Backward search - move points from last to first
		// - Move last point on the target point
		joint_chain[joint_count - 1]->setKinematicPosition(target_x, target_y);
		// - Update every joints, starting from the one before last
		// - Relocate each joints to a point between their current location and their next joint location
		for (int i = joint_count - 2; i >= 0; i--)
			relocateJoint(joint_chain[i], joint_chain[i + 1]->getPlaneX(), joint_chain[i + 1]->getPlaneY(), joint_chain[i]->getKinematicDistance());

		//2.2) Stage 2 : Forward search - move points from first to last
		// - Move first point to the origin point
		joint_chain[0]->setKinematicPosition(0.0f, 0.0f);
		// - Update every joints, starting from the second
		// - Relocate each joints to a point between their current location and their previous joint location
		for (int i = 1; i < joint_count; i++)
			relocateJoint(joint_chain[i], joint_chain[i - 1]->getPlaneX(), joint_chain[i - 1]->getPlaneY(), joint_chain[i - 1]->getKinematicDistance());

		//2.3) Check distance between last point and target point
		if (get2dDistance(joint_chain[joint_count - 1]->getPlaneX(), joint_chain[joint_count - 1]->getPlaneY(), target_x, target_y) <= tolerance)
			break; //Last point is withing tolerable range of the target point, stop looking
		else
			tolerance += (tolerance / 4); //Not close enought, increase tolerance a little
	}

	//3) Update joint angles in the chains
	updateJointAngles();

	return true;
}

/**
 * @brief Fabrik2D::setTolerance Change the tolerance
 * @param _tolerance
 */
void Fabrik2D::setTolerance(float _tolerance)
{
	if (_tolerance < 0.0f)
		_tolerance = 0.0f; //Not very tolerant of you
	tolerance = _tolerance;
}

/**
 * @brief Fabrik2D::updateJointAngles Update target angles in joints using current chain coordinates
 */
void Fabrik2D::updateJointAngles()
{
	if (joint_count < 2)
		return;

	//1) Update first joint angle
	float found_angle = atan2(joint_chain[1]->getPlaneX(), joint_chain[1]->getPlaneY());
	joint_chain[0]->setComputedAngle(found_angle);

	//2) Update every other joint angles, starting with the second joint and skipping the last tip placeholder joint
	for (uint8 i = 1; i < joint_count - 1; i++)
	{
		found_angle = atan2(
			joint_chain[i + 1]->getPlaneY() - joint_chain[i]->getPlaneY(),
			joint_chain[i + 1]->getPlaneX() - joint_chain[i]->getPlaneX()
		) - found_angle;
		joint_chain[i]->setComputedAngle(found_angle);
	}
}

/**
 * @brief Fabrik2D::clearChain Cleanup our chain in memory
 */
void Fabrik2D::clearChain()
{
	if (!joint_chain)
		return;

	//Just free the chain, the joints are freed elsewhere
	free(joint_chain);
	joint_chain = NULL;
}

/**
 * @brief Fabrik2D::get2dDistance Get distance between two points
 * @param x1
 * @param y1
 * @param x2
 * @param y2
 * @return
 */
float Fabrik2D::get2dDistance(float x1, float y1, float x2, float y2)
{
	float x_diff = x2 - x1;
	float y_diff = y2 - y1;
	return sqrt((x_diff * x_diff) + (y_diff * y_diff));
}

/**
 * @brief Fabrik2D::placeNextJoint Set default position of target joint from source joint using predefined distance and source position
 * @param source
 * @param target
 */
void Fabrik2D::placeNextJoint(WalkerJoint *source, WalkerJoint *target)
{
	target->setPlaneX(source->getKinematicDistance() * cos(source->getBaseAngle()));
	target->setPlaneY(source->getKinematicDistance() * sin(source->getBaseAngle()));
}

/**
 * @brief Fabrik2D::replaceJoint Move given joint position from its current position to the given distance on a line between its current position and the gien x/y coordinates
 * @param joint
 * @param target_x
 * @param target_y
 * @param distance
 */
void Fabrik2D::relocateJoint(WalkerJoint *joint, float target_x, float target_y, float distance)
{
	//1) Get distance between joint and point
	float current_distance = get2dDistance(joint->getPlaneX(), joint->getPlaneY(), target_x, target_y);

	//2) Get distance ration between current distance and required new distance
	float distance_ratio = distance / current_distance;

	//3) Use the ratio to relocate joint point
	joint->setPlaneX(((1.0f - distance_ratio) * joint->getPlaneX()) + (distance_ratio * target_x));
	joint->setPlaneY(((1.0f - distance_ratio) * joint->getPlaneY()) + (distance_ratio * target_y));
}
