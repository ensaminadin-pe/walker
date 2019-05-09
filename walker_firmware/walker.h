#ifndef WALKER_H
#define WALKER_H

#include "walkerleg.h"
#include "walkerjoint.h"
#include "oscillator.h"
#include "types.h"
#include "gait.h"
#include "servodriver.h"

#define GAIT_POSITION_TO_FILE

#ifdef IS_QT
	#ifdef GAIT_POSITION_TO_FILE
		#define OUTPUT_POSITION_TO_FILE
		#include <stdio.h>
	#endif
#endif

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
		float* getPeriodModifier() { return &speed_multiplier; }
		float increaseGaitSpeed();
		float decreaseGaitSpeed();
		// - Main
		void update(unsigned int diff);
		void setNextGait(uint16 gait_id);
		// - Debug
		void print();
	private:
		//Properties
		WalkerLeg**		legs;			//Walker legs
		Gait*			gait;			//Active gait configuration
		uint16			next_gait_id;	//Next gait
		#ifdef OUTPUT_POSITION_TO_FILE
			FILE*		outputFile;
		#endif

		float			speed_multiplier;	//Oscillator period modifier
		uint8			leg_count;			//Number of legs
		uint8			joint_count;		//Number of joints (servos)

		int16			update_timer;		//Oscillators & Servos update timer
		//Methods
		void freeLegs();
		void reset();
		void updateServoPositions();
		void clearGait();
		void loadNextGait();
};

#define sWalker Walker::instance()

#endif // WALKER_H
