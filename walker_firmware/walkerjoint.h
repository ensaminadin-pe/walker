#ifndef WALKERJOINT_H
#define WALKERJOINT_H

#include "types.h"

class WalkerJoint
{
	public:
		WalkerJoint(uint8 _driver_board, uint8 _driver_index, uint16 _offset, float _distance);
		~WalkerJoint();
		bool updateServoPosition(unsigned long diff);
		bool isMovementDone() { return movement_done; }
		void setTargetPosition(float position) { target_position = position; }
		void setPosition(float position);
		void toggle(uint8 toggle = 2);
	private:
		// - Config properties
		uint8	driver_board;		//Driver board index
		uint8	driver_index;		//Index on the driver board
		uint16	offset;				//Pulse width offset
		float	distance;			//Distance to next joint or leg tip
		// - Running properties
		bool	running;			//Can update position
		float	current_position;	//Current servo angle
		float	target_position;	//Target servo angle
		bool	movement_done;		//Target movement position is reached
		// - Methods
		void setServoPosition(float angle);
};

#endif // WALKERJOINT_H
