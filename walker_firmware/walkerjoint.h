#ifndef WALKERJOINT_H
#define WALKERJOINT_H

#include "types.h"
#include "kinematic.h"

class WalkerJoint
{
	public:
		WalkerJoint(uint8 _driver_board = 255, uint8 _driver_index = 255, uint16 _offset = 0, float _distance = 0.0f, KinematicAxis _rotation_axis = KINEMATIC_AXIS_UNDEFINED, float _angle = 0.0f);
		~WalkerJoint();
		// - Main udpate
		bool			updateServoPosition(unsigned long diff);
		// - Actions methods
		void			setTargetPosition(float position) { target_position = position; }
		void			setPosition(float position);
		void			toggle(uint8 toggle = 2);
		// - Getter & setters
		bool			isMovementDone()	{ return movement_done; }
		float			getKinematicDistance() { return kinematic_distance; }
		KinematicAxis	getAxis()			{ return rotation_axis; }
		float			getBaseAngle()		{ return base_angle; }
		float			getComputedAngle()	{ return computed_angle; }
		void			setComputedAngle(float angle) { computed_angle = angle; }
		void			setKinematicPosition(float x, float y);
		float			getPlaneX()			{ return plane_x; }
		void			setPlaneX(float x)	{ plane_x = x; }
		float			getPlaneY()			{ return plane_y; }
		void			setPlaneY(float y)	{ plane_y = y; }
	private:
		// - Config properties
		uint8			driver_board;		//Driver board index
		uint8			driver_index;		//Index on the driver board
		uint16			offset;				//Pulse width offset
		float			distance;			//Distance to next joint or tip
		// - Kinematic properties
		float			kinematic_distance;	//Rounded distance in tenth of millimeter for the inversed kinematic processing
		KinematicAxis	rotation_axis;		//On wich axis does this joint rotate on default position
		float			base_angle;			//Angle relative to the previous joint on initialisation
		float			computed_angle;		//Angle computed by the inversed kinematic algorythm
		float			plane_x;			//Position x in the plane used for kinematic computation
		float			plane_y;			//Position y in the plane used for kinematic computation
		// - Running properties
		bool			running;			//Can update position
		float			current_position;	//Current servo angle
		float			target_position;	//Target servo angle
		bool			movement_done;		//Target movement position is reached
		// - Methods
		void			setServoPosition(float angle);
};

#endif // WALKERJOINT_H
