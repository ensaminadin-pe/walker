#ifndef WALKER_H
#define WALKER_H

#include "walkerleg.h"
#include "walkerjoint.h"
#include "types.h"
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
		uint8	getLegCount()			{ return leg_count; }
		// - Walker setup
		WalkerLeg* addLeg();
		void addLegs(uint8 count);
		WalkerLeg* getLeg(uint8 index);
		// - Speed
		/// - TODO - SPEED MODIFIER
		// - Main
		void update(unsigned long diff);
		/// - TODO - FUNCTION TO SET NEXT MOVEMENT
	private:
		//Properties
		WalkerLeg**		legs;				//Walker legs
		uint8			leg_count;			//Number of legs
		int16			update_timer;		//Main update timer
		float			speed_multiplier;	/// - WIP - Movement speed modifier
		//Methods
		void reset();
		void freeLegs();
};

#define sWalker Walker::instance()

#endif // WALKER_H
