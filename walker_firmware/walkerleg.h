#ifndef WALKERLEG_H
#define WALKERLEG_H

#include "walkerjoint.h"
#include "types.h"

class Walker;

class WalkerLeg
{
	public:
		// - Constructor/destructor
		WalkerLeg();
		~WalkerLeg();
		// - Methods
		WalkerJoint* addJoint(uint8 _driver_board, uint8 _driver_index, uint16 _offset, float _distance, KinematicAxis _rotation_axis, float _angle);
		void updateJoints(unsigned long diff);
		void homeJoints();	//Move every joints to 0°
		void test();		//Test leg, move each joints and beep
		/// - TODO - LOAD/GET MOVEMENT POSITIONS
		// - Getters
		// ----------------------------------------
		//WalkerLeg(Walker* _parent, uint8 _index);
		int getJointCount() { return joint_count; }
		WalkerJoint *getJoint(uint8 index);
	private:
		// - Properties
		uint8 joint_count;
		WalkerJoint** joints; //Top to bottom
		// - Methods
		void freeJoints();
};

#endif // WALKERLEG_H
