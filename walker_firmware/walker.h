#ifndef WALKER_H
#define WALKER_H

#include "walkerleg.h"
#include "walkerjoint.h"
#include "types.h"
#include "servodriver.h"
#include "animation.h"

class Walker
{
	public:
		//Constructor/destructor
		Walker();
		~Walker();
		//Methods
		// - Getters
		static Walker*	instance();
		uint8			getLegCount()			{ return leg_count; }
		// - Walker setup
		WalkerLeg*		addLeg();
		void			addLegs(uint8 count);
		WalkerLeg*		getLeg(uint8 index);
		// - Speed
		/// - TODO - SPEED MODIFIER
		// - Main
		void			update(unsigned long diff);
		void			setNextAnimation(Animation* animation);
		/// - TODO - FUNCTION TO SET NEXT MOVEMENT
	private:
		//Properties
		// - Main properties
		WalkerLeg**		legs;				//Walker legs
		uint8			leg_count;			//Number of legs
		int16			update_timer;		//Main update timer
		float			speed_multiplier;	/// - WIP - Movement speed modifier
		// - Animation properties
		uint8			current_frame;		//Current frame index in animation
		Animation*		current_animation;	//Running animation
		Animation*		next_animation;		//Next animation
		// - Methods
		void reset();
		void freeLegs();
		void loadFrameFor(uint8 frame_index, uint8 leg_index);
};

#define sWalker Walker::instance()

#endif // WALKER_H
