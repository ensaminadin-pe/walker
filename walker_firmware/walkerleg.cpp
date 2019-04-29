#include "walkerleg.h"
#include <stdlib.h>
#include <stdio.h>

WalkerLeg::WalkerLeg(Walker* _parent, uint8 _index)
{
	parent = _parent;
	index = _index;
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
 * @brief WalkerLeg::addJoints Add a number of joint
 * @param count
 */
void WalkerLeg::addJoints(uint8 count)
{
	//1) Clear joints
	freeJoints();

	//2) Add the given number of joints
	while (count-- > 0)
		addJoint();
}

/**
 * @brief WalkerLeg::addJoint Add a new joint
 */
void WalkerLeg::addJoint()
{
	//1) Crate new joint
	WalkerJoint* joint = new WalkerJoint(index, joint_count);
	joint_count++;
	//2) Resize joint storage
	WalkerJoint** _joints = (WalkerJoint**)realloc(joints, joint_count * sizeof(WalkerJoint*));
	if (!_joints)
		return;
	//3) Store new joint
	_joints[joint_count - 1] = joint;
	joints = _joints;
}

WalkerJoint* WalkerLeg::getJoint(uint8 index)
{
	if (!joints || index > joint_count - 1)
		return NULL;
	return joints[index];
}
