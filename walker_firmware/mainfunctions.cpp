#include "mainfunctions.h"
#include "walker.h"
#include "gaitdictionarymgr.h"
#include "radio.h"
#include "config.h"
#include <stdio.h>
#include "wiinunchuck.h"

#ifdef IS_QT
	#include "arduinopolyfill.h"
#else
	#include "stdpolyfill.h"
#endif
/*
Implement inversed kinematics
Fuck gaits
-https://github.com/henriksod/Fabrik2DArduino

Change robot setup :
	leg count
	joint1 distance = 0
	jointX distance = from joint X - 1
	...

Movements are list of X/Y/Z positions for each legs
Movement struct:
	uint8		frame_count
	uint8		type			- 0 = fixed position, 1 = movement, 2 = emote
	Position	positions[]		- leg tip positions
	Position
		uint16		option			- flag or something, do an action
		uint16		option_value	- option value, ex if flag = 1, option value will be an id of sound to play or something like that
		Point		points[]		- one for each leg
		uint16		forced_delay	- if not 0, this is the time in ms spent to execute this position, allow to bypass user modified speed for emotes and whatnot
	Point
		uint16		x | - X & Y used in inversed kinematics
		uint16		y |
		uint16		z   - Z used to calculate top view rotation

Rewrite servo movement
	On startup, check pulse range
	Build array of 180 uint16 (360 bytes of memory), each contain the correct pulse width for
	the target angle, angle is array key
	This way we calculate only once the target pulse width for an angle, want to move to -12° ? Check the (90 - 12) position in the array and voila

Rewrite movement update
	Movement* movement;		//Loaded movement
	uint8 movement_frame;	//Current frame in the loaded movement
	uint8 servo_rotation;	//This determine the movement speed, maximum servo angle change
	Leg legs[]
		bool done;
		Joint joints[]
			uint8 current_position;
			uint8 target_position;

	- Reversed kinematics will calculate the target servo angle once on load
	- In our update loop we will change the current servo position by a variable value X
	- Changing X will increase/decrease the robot movement speed without seamlessly
	- Servo position updates are every 20ms (60Hz) but the amplitude of rotation will change

	update current position to reach target position
	cannot move more than X° per update, maximum is servo's top tolerance, X is variable
	when a leg reach its target position toggle it to done, (load next movement?)
	when all legs are done, load next positions to reach in each legs and reset done
	if no new Position, load default Position stance
	if Movement struct is repetable, reset and load first Position
------------------------------------------------------------------------------------------------
Unit infos
	. radio connection status : (red)cannot connect module, (blue)radio off, (orange)connection lost, (green)connection on
	. battery level (how to read ?)

Radio connectivity
	Requirements :
		. timeout detection
		. receiver reset
	Setup	=	set wich pins are used
	Link	=	clear current link
				open new link
	Update	=	check if SPI bus is ok
					link if not, cannot link = nothing to do
				reset timeout of 1 sec
				read packet and update remote datas

Servo control boards
	Requirements :
		. easy access to 2 default positions :
			- folded
			- idle

Hardware updgrade
	. Easy swap 7.4v Li ion battery
	. allow voltage input via 9/12 ? volt DC jack input, bypass battery
	. remote port : put remote on the body, charge remote and keep reading datas
	. rotating turret (360°? 180°?) with easy attachment snap on
	. bb gun attachment
 */

/// - This is the core of the program
/// 1) Hardware setup for Arduino
///		1.1) PCA9685 board
///			Connect the PCA9685 board like so :
///				SCL to arduino's A5
///				SDA to arduino's A4
///				VCC to arduino's +3.3v or +5v or any other power source
///				GND to arduino's GND or other source GND
///				V+	to another power source, this is the servo's power input,
///					the arduino is too weak to power the properly and this can damage it
///
///			The PCA9685 is a chip that can handle up to 16 PWM signals.
///			You can tell it to output a specific PWM signal on a target pin
///			with a very small instruction using a communication protocol called I2C.
///			Arduinos have an I2C bus on the A5 and A4 pins, you cannot move it to other pins,
///			from what i understand it's a feature built into the controller itself.
///			If you are using another board, you need to check the correct documentation to find what pins to use.
///		1.2) Radio receiver
///			Later

//static uint32 update_time;

unsigned long diff()
{
	unsigned long _diff = millis() - update_time;
	if (_diff < 1)
		return 1;
	return _diff;
}

void update_movement()
{
	unsigned int _movement = 0;
	if (direction_nunchuck.getJoystickX() > 0)
		_movement += map_movement_value(MOVEMENT_RIGHT_LOW, direction_nunchuck.getJoystickX());
	else
		_movement += map_movement_value(MOVEMENT_LEFT_LOW, direction_nunchuck.getJoystickX());
	if (direction_nunchuck.getJoystickY() > 0)
		_movement += map_movement_value(MOVEMENT_UP_LOW, direction_nunchuck.getJoystickY());
	else
		_movement += map_movement_value(MOVEMENT_DOWN_LOW, direction_nunchuck.getJoystickY());
	movement = _movement;
}

void update_gait(unsigned long time_diff)
{
	if (gait_update_time > time_diff)
	{
		gait_update_time -= time_diff;
		return;
	}
	gait_update_time = 100;

	if (remote_timeout <= time_diff)
	{ //Lost the remote, go back to idle
		sWalker->setNextGait(1);
		return;
	}

	//Choose the gait to run
	if (direction_nunchuck.cPressed() && direction_nunchuck.zPressed())
		sWalker->setNextGait(2);
	else if (direction_nunchuck.cPressed())
		sWalker->setNextGait(2110);
	else if (direction_nunchuck.zPressed())
	{
		if (movement & MOVEMENT_UP_HIGH)
			sWalker->setNextGait(1100);
		else if (movement & MOVEMENT_LEFT_HIGH)
			sWalker->setNextGait(1110);
		else if (movement & MOVEMENT_DOWN_HIGH)
			sWalker->setNextGait(1101);
		else if (movement & MOVEMENT_RIGHT_HIGH)
			sWalker->setNextGait(1111);
		else
			sWalker->setNextGait(1);
	}
	else if (movement & MOVEMENT_UP_HIGH)
		sWalker->setNextGait(1100);
	else if (movement & MOVEMENT_LEFT_HIGH)
		sWalker->setNextGait(1001);
	else if (movement & MOVEMENT_DOWN_HIGH)
		sWalker->setNextGait(1101);
	else if (movement & MOVEMENT_RIGHT_HIGH)
		sWalker->setNextGait(1002);
	else
		sWalker->setNextGait(1);
}

unsigned int map_movement_value(unsigned int base, signed short joystick_value)
{
	if (joystick_value < 0)
		joystick_value *= -1;
	if (joystick_value < MOVEMENT_LOW_THRESHOLD)
		return 0;
	if (joystick_value >= MOVEMENT_LOW_THRESHOLD && joystick_value < MOVEMENT_MID_THRESHOLD)
		return base;
	if (joystick_value >= MOVEMENT_MID_THRESHOLD && joystick_value < MOVEMENT_HIGH_THRESHOLD)
		return base * 2;
	if (joystick_value >= MOVEMENT_HIGH_THRESHOLD)
		return base * 4;
}

void main_setup()
{ //Arduino like setup
	/// - Setup your configurations in the config.h file
	//1) Setup remote
	sRadio->setup(RADIO_PIN, RADIO_ADRESS, RADIO_RECEIVE); //Radio data on pin 10, can be anything
	sRadio->setListenSize(sizeof(WiiNunchuckPacket));
	// --------------------------------------------------------------------------------------------
	//2) Setup servo drivers
	//  This is the tricky part, you need to find the 0 and 180° pulse width yourself
	//	Its a number between 0 and 4095 and
	//  - for my sg90s it's 145 and 515 at 60Hz
	//  - For my MG92B it's 345 and 995 at 120Hz for -60° to 60° :/
	sServoDriver->setupDrivers(DRIVER_BOARD_COUNT, DRIVER_BOARD_FREQUENCY);
	sServoDriver->setupPulseRange(SERVO_MIN_PULSE, SERVO_MAX_PULSE, SERVO_ANGLE_GRANULARITY);
	sServoDriver->setupAngleRange(SERVO_MIN_ANGLE, SERVO_MAX_ANGLE);
	// --------------------------------------------------------------------------------------------
	//3) Setup legs
	// Legs are init left to right, top to bottom :
	//
	//    Leg1    front     Leg2
	// --02--01--00----03--04--05--
	//           |      |
	//    Leg2   |      |   Leg3
	// --08--07--06----09--10--11--
	//
	// Leg is specified by :
	//  - index and number of joints
	//  - each joint must have an adress : board & index
	//  - servos are not totally centered, you can setup a small pulse offset to set the servo at an exact 0 position
	//  - you need to specify the distance between each joints in mm for the inversed kinematic to work
	//    for each joint, give the distance from the middle of the joint to the middle of the next joint,
	//	  for the last joint give the distance to the leg tip
	//  - This doesnt need to be 0.1mm precise, 0.3/4mm of margin is ok. Ex: round up 25.31 to 25.5
	///	uint8 leg_index = sWalker->addLeg();
	/// //Add joints from top to bottom
	/// WalkerLeg* leg = sWalker->getLeg(leg_index);
	/// leg->addJoint(0, 0, 0, 25.0f); //Top joint : board 0, board index 0, offset 0, dimention to next 25.0f
	/// leg->addJoint(0, 1, -2, 41.5f); //Top joint : board 0, board index 1, offset -2, dimention to next 41.5f
	/// leg->addJoint(0, 2, 5, 89.0f); //Top joint : board 0, board index 2, offset 5, dimention to next 89.0f
	// --------------------------------------------------------------------------------------------
	//4) Load default position
	// - https://github.com/henriksod/Fabrik2DArduino
	/// sWalker->setPosition(sPositionsDictionary->getDefault());
	/// sWalker->initPosition();
	// --------------------------------------------------------------------------------------------
	//5) Test legs, bip or do some shit to show that the walker is ok
	// - https://github.com/AnonymousAlly/Arduino-Music-Codes
	/// for (uint8 i = 0; i < sWalker->getLegCount(); i++)
	/// {
	///		sWalker->getLeg(i)->test(); //Test move
	///		delay(200);
	/// }
}

void main_loop()
{ //Arduino like loop
	//1) Setup loop diff time
	if (update_time == 0)
	{
		update_time = millis();
		return;
	}

	//2) Add control logic here
	//   - sensor detection and whatnot
	uint8* radio_packet = sRadio->update(diff());
	if (radio_packet && direction_nunchuck.handlePacket((WiiNunchuckPacket*)radio_packet))
	{ // Has a radio update & packet was correct
		direction_nunchuck.print();
		update_movement();
		//movement = 4;
		remote_timeout = 2000;
	}

	//3) Update gaits with remote infos
	update_gait(diff());

	//4) Run walker update loop
	sWalker->update(diff());
	update_time = millis();
}
