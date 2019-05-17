#ifndef FABRIK2D_h
#define FABRIK2D_h

#ifndef IS_QT
	#include "Arduino.h"
#else
	#include "arduinopolyfill.h"
#endif
#include "walkerjoint.h"

#define RADIAN_TO_DEGREE_FACTOR 57.2958 //1rad = 57.2958°

class Fabrik2D
{
	/// - Original from https://github.com/henriksod/Fabrik2DArduino
	/// - Base algorithm concept: http://www.andreasaristidou.com/publications/papers/FABRIK.pdf
	/// Some stuff where nor optimised the way i like it, and the whole chain and joint part
	/// is redundant in my project, i can save everything in the WalkerJoint class
	/// I also wanted to be precise up to 0.1mm in the calculations, so mesures are in
	/// 10th of millimeters and not millimeters. Its pretty much just multiplied by 10 and put in a uint16
	/// so the theorical maximum distance between two joints is 6.5535 meter. If you are piloting a
	/// walker that big with this library, i'll be kinda proud but mostly scared.
	public:
		Fabrik2D(float _tolerance = 10.0f);
		~Fabrik2D();
		// - Setup methods
		bool			addJoint(WalkerJoint* joint);
		WalkerJoint*	getJoint(uint8 joint_index);
		// - Main methods
		bool			reachFor(float target_x, float target_y);
		void			setTolerance(float _tolerance);
		// - Debug
		void			printChain();
	private:
		// - Methods
		void	updateJointAngles();
		void	clearChain();
		float	get2dDistance(float x1, float y1, float x2, float y2);
		void	placeNextJoint(WalkerJoint* source, WalkerJoint* target);
		void	relocateJoint(WalkerJoint* joint, float target_x, float target_y, float distance);
		float	computeAngleFor(float x, float y);
		float	radianToDegree(float radian);
		// - Properties
		uint8			joint_count;	//Number of joints
		float			chain_length;	//Total chain lenght
		WalkerJoint**	joint_chain;	//Chain of joints to use for kinematic computation
		KinematicAxis	joints_axis;	//Common axis of joints, defined by the first joint added
		float			tolerance;		//Tolerable distance to reach
};

#endif
