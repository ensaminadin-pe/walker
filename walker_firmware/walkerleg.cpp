#include "walkerleg.h"
#include "arduinopolyfill.h"
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
WalkerJoint* WalkerLeg::addJoint(uint8 _driver_board, uint8 _driver_index, uint16 _offset, float _distance)
{
	//1) Crate new joint
	WalkerJoint* joint = new WalkerJoint(_driver_board, _driver_index, _offset, _distance);
	joint_count++;

	//2) Resize joint storage
	WalkerJoint** _joints = (WalkerJoint**)realloc(joints, joint_count * sizeof(WalkerJoint*));
	if (!_joints)
	{
		delete(joint);
		return NULL;
	}

	//3) Store new joint
	_joints[joint_count - 1] = joint;
	joints = _joints;
	return joint;
}

/**
 * @brief WalkerLeg::updateJoints Update joint positions
 * @param diff
 */
void WalkerLeg::updateJoints(unsigned long diff)
{
	if (!joints || joint_count == 0)
		return;

	for (uint8 i = 0; i < joint_count; i++)
	{
		if (joints[i]->updateServoPosition(diff))
		{ //Movement is done, load next target position
			/// - TODO - LOAD NEXT TARGET POSITION
			//float target_position = ?;
			//joints[i]->setTargetPosition(target_position);
		}
	}
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
