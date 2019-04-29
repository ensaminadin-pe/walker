#ifndef WALKERLEG_H
#define WALKERLEG_H

#include "walkerjoint.h"
#include "types.h"

class Walker;

class WalkerLeg
{
	public:
		WalkerLeg(Walker* _parent, uint8 _index);
		~WalkerLeg();
		void freeJoints();
		void addJoints(uint8 count);
		void addJoint();
		int getJointCount() { return joint_count; }
		WalkerJoint *getJoint(uint8 index);
	private:
		//Properties
		Walker* parent;
		uint8 index;
		uint8 joint_count;
		WalkerJoint** joints; //Top to bottom
};

#endif // WALKERLEG_H
