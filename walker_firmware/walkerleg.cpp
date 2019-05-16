#include "walkerleg.h"
#include "arduinopolyfill.h"
#include "kinematic.h"
#include <stdlib.h>
#include <stdio.h>

WalkerLeg::WalkerLeg()
{
	joints = NULL;
	joint_count = 0;
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
