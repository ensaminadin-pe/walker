#include "walkerleg.h"
#include "arduinopolyfill.h"
#include "kinematic.h"
#include <stdlib.h>
#include <stdio.h>

WalkerLeg::WalkerLeg()
{
	joints = NULL;
	joint_count = 0;
	current_point = NULL;
}

WalkerLeg::~WalkerLeg()
{
	freeJoints();
}

/**
 * @brief WalkerLeg::freeJoints Free allocated memory
 */
void WalkerLeg::freeJoints()
{
	if (!joints)
		return;
	for (uint8 i = 0; i < joint_count; i++)
		delete joints[i];
	free(joints);
	joint_count = 0;
	joints = NULL;
}

/**
 * @brief WalkerLeg::addJoint Add a new joint
 */
WalkerJoint* WalkerLeg::addJoint(uint8 _driver_board, uint8 _driver_index, uint16 _offset, float _distance, KinematicAxis _rotation_axis, float _angle)
{
	//1) Crate new joint
	WalkerJoint* joint = new WalkerJoint(_driver_board, _driver_index, _offset, _distance, _rotation_axis, _angle);
	if (!joint)
		return NULL;
	joint_count ++;

	//2) Resize joint storage
	joints = (WalkerJoint**)realloc(joints, joint_count * sizeof(WalkerJoint*));
	if (!joints)
	{
		delete(joint);
		return NULL;
	}

	//3) Store new joint
	joints[joint_count - 1] = joint;

	//4) Add joint to leg kinematic
	Fabrik2D* kinematic = getKinematic(joint->getAxis());
	if (!kinematic) // Missing axis
		return joint;
	kinematic->addJoint(joint);

	return joint;
}

/**
 * @brief WalkerLeg::updateJoints Update joint positions
 * @param diff
 */
bool WalkerLeg::updateJoints(unsigned long diff)
{
	//1) Check if we have joints to update at all
	if (!joints || joint_count == 0)
		return false;

	//2) Update every joints and check if all the joints have reached their destinations
	bool movement_done = true;
	for (uint8 i = 0; i < joint_count; i++)
	{
		if (!joints[i]->updateServoPosition(diff))
			movement_done = false;
	}

	return movement_done;
}

/**
 * @brief WalkerLeg::homeJoints Move every joints to the 0° position
 */
void WalkerLeg::homeJoints()
{
	if (!joints || joint_count == 0)
		return;

	for (uint8 i = 0; i < joint_count; i++)
		joints[i]->setPosition(0);
}

/**
 * @brief WalkerLeg::test Test every joints and beep
 */
void WalkerLeg::test()
{
	if (!joints || joint_count == 0)
	{
		/// - TODO - UNHAPPY BEEP
		return;
	}

	for (uint8 i = 0; i < joint_count; i++)
	{
		joints[i]->setPosition(-10);
		delay(150);
		joints[i]->setPosition(10);
		delay(150);
		joints[i]->setPosition(0);
		/// - TODO - HAPPY BEEP, TONE = 400 + (100 * i)
		delay(150);
	}
}

/**
 * @brief WalkerLeg::moveTo Compute the joint angles to reach the target 3D point
 * @param position
 */
void WalkerLeg::moveTo(PositionPoint *position)
{
	if (position == current_point)
		return; //Already there

	//We have a 3d point
	//We have 3 Fabirk2D resolver for each axis
	//Tricky part is, axis X and Y in the Fabrik2D resolvers are NOT
	//the same as the X/Y axis in the 3D point
	//For the 3D point :
	//	X axix run threw the front and back of the walker
	//	Y axis run threw the sides of the walker
	//	Z axis run threw the top and bottom
	//For Fabrik2D resolver on axis :
	//	X : Fabrik axis X = 3DZ & Y = 3DY
	//	Y : Fabrik axis X = 3DZ & Y = 3DX
	//	Z : Fabrik axis X = 3DX & Y = 3DY

	//Pass target point correctly to each Farbik2D resolver
	//Resolver will update the joints
	getKinematic(KINEMATIC_AXIS_X)->reachFor(position->z, position->y);
	getKinematic(KINEMATIC_AXIS_Y)->reachFor(position->z, position->x);
	getKinematic(KINEMATIC_AXIS_Z)->reachFor(position->x, position->y);
	current_point = position;
}

/**
 * @brief WalkerLeg::toggleJoints Togle joints active or inactive
 * @param state
 */
void WalkerLeg::toggleJoints(bool state)
{
	if (!joints || joint_count == 0)
		return;

	for (uint8 i = 0; i < joint_count; i++)
	{
		if (joints[i])
			joints[i]->setRunning(state);
	}
}

/**
 * @brief WalkerLeg::getJoint Get target joint in leg
 * @param index
 * @return
 */
WalkerJoint* WalkerLeg::getJoint(uint8 index)
{
	if (!joints || index > joint_count - 1)
		return NULL;
	return joints[index];
}

/**
 * @brief WalkerLeg::getKinematic Get Fabrik2D kinematic for given axis
 * @param axis
 */
Fabrik2D* WalkerLeg::getKinematic(KinematicAxis axis)
{
	if (axis == KINEMATIC_AXIS_UNDEFINED)
		return NULL;
	return &kinematics[(uint8)axis - 1];
}
