#ifndef WALKERLEG_H
#define WALKERLEG_H

#include "walkerjoint.h"
#include "types.h"
#include "fabrik2d.h"
#include "animation.h"

class Walker;

class WalkerLeg
{
	public:
		//Methods
		// - Constructor/destructor
		WalkerLeg();
		~WalkerLeg();
		// - Main methods
		WalkerJoint*	addJoint(uint8 _driver_board, uint8 _driver_index, uint16 _offset, float _distance, KinematicAxis _rotation_axis, float _angle);
		bool			updateJoints(unsigned long diff);
		void			homeJoints();	//Move every joints to 0°
		void			test();			//Test leg, move each joints and beep
		void			moveTo(PositionPoint* position, bool apply = false);
		void			toggleJoints(bool state);
		// - Getters
		// ----------------------------------------
		int				getJointCount() { return joint_count; }
		WalkerJoint*	getJoint(uint8 index);
	private:
		// - Properties
		uint8			joint_count;
		WalkerJoint**	joints;			//Top to bottom
		Fabrik2D		kinematics[3];	//0 = X, 1 = Y, 2 = Z
		PositionPoint*	current_point;
		// - Methods
		void			freeJoints();
		Fabrik2D*		getKinematic(KinematicAxis axis);
};

#endif // WALKERLEG_H
