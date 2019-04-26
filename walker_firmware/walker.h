#ifndef WALKER_H
#define WALKER_H

#include "walkerleg.h"
#include "walkerjoint.h"
#include "oscillator.h"
#include "types.h"
#include "gait.h"
#include "servodriver.h"

class Walker
{
	public:
		//Constructor/destructor
		Walker();
		~Walker();
		//Methods
		// - Getters
		static Walker* instance();
		uint8	getLegCount()	{ return leg_count; }
		uint8	getJointCount()	{ return joint_count; }
		uint16	getTotalCount()	{ return leg_count * joint_count; }
		// - Walker setup
		void setup(uint8 leg_count, uint8 joint_count);
		void addLegs(uint8 count);
		void addLeg();
		void addJoints(uint8 count);
		void init();
		// - Speed
		void setGaitSpeed(float speed);
		float increaseGaitSpeed();
		float decreaseGaitSpeed();
		// - Main
		void update(int diff);
		void setNextGait(Gait* _gait);
		// - Debug
		void print();
	private:
		//Propetries
		Oscillator**	oscillators;	//Sine oscillators for joint movements
		WalkerLeg**		legs;			//Walker legs
		WalkerJoint**	joints;			//Walker joints
		Gait*			gait;			//Active gait configuration
		Gait*			next_gait;		//Next gait

		float			speed_multiplier;	//Oscillator period modifier
		uint8			leg_count;			//Number of legs
		uint8			joint_count;		//Number of joints (servos)

		int16			update_timer;		//Oscillators & Servos update timer
		//Methods
		void freeLegs();
		void reset();
		void updateOscillators();
		void updateServoPositions();
		void clearGait();
		void loadNextGait();
};

#define sWalker Walker::instance()

#endif // WALKER_H
