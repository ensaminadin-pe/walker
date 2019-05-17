#include "animation_dictionary_6_3.h"

#include "mainfunctions.h"
#include "kinematic.h"
#include "walker.h"
#include "radio.h"
#include "config.h"
#include "wiinunchuck.h"
#include <stdio.h>

#ifdef IS_QT
	#include "arduinopolyfill.h"
#else
	#include "stdpolyfill.h"
#endif
/*
 Hardware used for default setup
 - MG92B servos (18 for my 6x3)
		https://www.aliexpress.com/item/10pcs-TowerPro-MG92B-Digital-Servo-Motor-Metal-Gear-3-5kg-cm-High-Torque-Double-Bearing-For/32255216023.html
		~5.6€/u, expensive but 3.5kg/cm and very small
 - ESP8266-E12 NodeMCU V3
		~5-6€/u, huge upgrade from an arduino for super low price : 80MHz, 4Mb of flash & 32Kb RAM
				 Regular ATmega328 is 16MHz, 32Kb of flash, 2Kb of RAM :/
 - PCA9685 boards (2 for my walker)
		https://www.aliexpress.com/item/16-Channel-12-bit-PWM-Servo-Driver-I2C-interface-PCA9685-module-for-arduino-or-Raspberry-pi/32718274859.html
		1.4€/u, cheap as dirt
 - Mini 360 DC-DC buck converter
		https://www.aliexpress.com/item/10PCS-LOT-Mini360-DC-DC-Buck-Converter-Step-Down-Module-4-75V-23V-to-1V-17V/32411214887.html
		2.48€/10pcs, wot?, one for each leg of the walker, handle 1.8A of regular output and peaks up to 3A
 - NRF24 regular radio transmitter
		~ 1€/u, range of ~20m with 1 wall max, might get a stronger one
 - Battery
	Samsung INR18650-30Q 3000mAh - 15A
	- TBD https://eu.nkon.nl/rechargeable/18650-size/samsung-inr-18650-30q-3000mah.html
 ----- V2 -----
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

 ----- V3 -----
// Edit :
// - Overall i really want to do this, but for a later incrementation
// - Now everything run on a single ESP8266 board that coast ~5€, this setup will require wayyyy more boards, money and time

Separate muscle from brain
	Add raspberry pi as main brainbox that tell arduinos what to do
	Rewrite most of the program on a raspberry pi in Python
		- Raspberry pi multithread for :
			. RF connection to remote							|	Same library as Arduino
			. global kinematic processing						|	https://github.com/Phylliade/ikpy
			. attachment inversed kinematics (legs and arm)
			. camera and sonar sensors : spatial awareness		|	OpenCV, multiple cameras, depth map, seems hard
		- Standardize kinematics movement
			. Raspberry will compute the inversed kinematics movements and send servo positions to reach for current movement and next one
			. This standard movement packet can be applied for main leg movement or potential attachment movement
			. movement packet schematic
				4	0x50 0x41 0x43 0x4B		(PACK in hexa)
				1	10 = servo angles		(Packet type)
				2	0-65535					(packet size in bytes)
				1	0-255					(number of chains per movement)
				1	0-180					(rotation chain0 angle1)
				1	0-180					(rotation chain0 angle2)
				1	0-180					(rotation chain0 angle3)
				1	255						(chain separator)
				1	0-180					(rotation chain1 angle1)
				1	0-180					(rotation chain1 angle2)
				1	0-180					(rotation chain1 angle3)
				1	0						(chain separator)
				For current 6-3 configuration, a single packet with current positions and next position will weight 54 bytes.
				Classic serial port speed is 54kbit/s, or 6.75Kbytes/s or 112bytes/20ms, wich is our main movement update rate.
				We have a huge margin for communication with this system, movement instruction will be sent every 250/500ms
				We could send the 4/5 next movements and queue them in the controller.
			. controller request schematic 5 bytes
				4	0x50 0x41 0x43 0x4B		(PACK in hexa)
				1	11 = require next movement
		- Attachments
			. leg controller
			. interaction controller (buzzer, led, LCD screen)
			. arm/gun controller
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

unsigned long diff()
{
	unsigned long _diff = millis() - update_time;
	if (_diff < 1)
		return 1;
	return _diff;
}

/**
 * @brief updateMovements This is where you can map the nunchuck datas to actions, i do not know hot ot make it less messy
 * @param time_diff
 */
void updateAction(unsigned long time_diff)
{
	//1) Delay updates
	if (movement_update_time > time_diff)
	{
		movement_update_time -= time_diff;
		return;
	}
	movement_update_time = 100;

	//2) Check if we have remote connectivity
	if (remote_timeout <= time_diff)
	{ //Lost the remote, go back to idle
		return;
	}

	//3) Choose the movement to execute
	if (direction_nunchuck.cPressed() && direction_nunchuck.zPressed())
		;//UNK
	else if (direction_nunchuck.cPressed())
		;//UNK
	else if (JOYSTICK_MOVEMENT & MOVEMENT_UP_HIGH)
		;//MOVE FORWARD
	else if (JOYSTICK_MOVEMENT & MOVEMENT_UP_MID && JOYSTICK_MOVEMENT & MOVEMENT_LEFT_MID)
		;//MOVE FORWARD LEFT
	else if (JOYSTICK_MOVEMENT & MOVEMENT_UP_MID && JOYSTICK_MOVEMENT & MOVEMENT_RIGHT_MID)
		;//MOVE FORWARD RIGHT
	else if (JOYSTICK_MOVEMENT & MOVEMENT_LEFT_HIGH)
	{
		if (direction_nunchuck.zPressed())
			;//STRAFE LEFT
		else
			;//TURN LEFT
	}
	else if (JOYSTICK_MOVEMENT & MOVEMENT_RIGHT_HIGH)
	{
		if (direction_nunchuck.zPressed())
			;//STRAFE RIGHT
		else
			;//TURN RIGHT
	}
	else if (JOYSTICK_MOVEMENT & MOVEMENT_DOWN_HIGH)
		;//MOVE BACK
	else if (JOYSTICK_MOVEMENT & MOVEMENT_DOWN_MID && JOYSTICK_MOVEMENT & MOVEMENT_LEFT_MID)
		;//MOVE BACK LEFT
	else if (JOYSTICK_MOVEMENT & MOVEMENT_DOWN_MID && JOYSTICK_MOVEMENT & MOVEMENT_RIGHT_MID)
		;//MOVE BACK RIGHT
	else
		;//IDLE
}

void main_setup()
{ //Arduino like setup
	//Get frame 2 of 6_3 forwrd walk
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
	// X
	// |     Leg1    front     Leg2
	// |  --02--01--00----03--04--05--
	// |            |      |
	// |     Leg2   |      |   Leg3
	// |  --08--07--06----09--10--11--
	// |_______________________________Y
	//
	//  - Add each leg individually and then add joint to leg from top to bottom
	//  - Each joint must have an adress : driver board index and pin index on the board
	//  - Servos are not totally centered, you can setup a small pulse offset to set the servo at an exact 0 position
	//  - You need to specify the distance between each joints in mm for the inversed kinematic to work
	//    for each joint, give the distance from the middle of the joint to the middle of the next joint,
	//	  for the last joint give the distance to the leg tip
	//  - This doesnt need to be 0.1mm precise, 0.5mm of margin is ok. Ex: round up 25.31 to 25.5
	//  - The setup is a bit tedious, but it allows you to fine tune each joints with an offset and place place
	//    joint on target board at target pin
	/// - SETUP OF A 6_3 walker : 6 legs with 3 joint each
	/// -- LEG 0 - Board 0 indexes 0, 1 & 2
	WalkerLeg* leg0 = sWalker->addLeg();
	leg0->addJoint(0, 0, 0, DIMENTION_JOINT1, KINEMATIC_AXIS_Z, 0.0f);
	leg0->addJoint(0, 1, 0, DIMENTION_JOINT2, KINEMATIC_AXIS_X, 0.0f);
	leg0->addJoint(0, 2, 0, DIMENTION_JOINT3, KINEMATIC_AXIS_X, -90.0f);

	/// -- LEG 1 - Board 0 indexes 3, 4 & 5
	WalkerLeg* leg1 = sWalker->addLeg();
	leg1->addJoint(0, 3, 0, DIMENTION_JOINT1, KINEMATIC_AXIS_Z, 0.0f);
	leg1->addJoint(0, 4, 0, DIMENTION_JOINT2, KINEMATIC_AXIS_X, 0.0f);
	leg1->addJoint(0, 5, 0, DIMENTION_JOINT3, KINEMATIC_AXIS_X, -90.0f);

	/// -- LEG 2 - Board 0 indexes 6, 7 & 8
	WalkerLeg* leg2 = sWalker->addLeg();
	leg2->addJoint(0, 6, 0, DIMENTION_JOINT1, KINEMATIC_AXIS_Z, 0.0f);
	leg2->addJoint(0, 7, 0, DIMENTION_JOINT2, KINEMATIC_AXIS_X, 0.0f);
	leg2->addJoint(0, 8, 0, DIMENTION_JOINT3, KINEMATIC_AXIS_X, -90.0f);

	/// -- LEG 3 - Board 0 indexes 9, 10 & 11
	WalkerLeg* leg3 = sWalker->addLeg();
	leg3->addJoint(0, 9, 0, DIMENTION_JOINT1, KINEMATIC_AXIS_Z, 0.0f);
	leg3->addJoint(0, 10, 0, DIMENTION_JOINT2, KINEMATIC_AXIS_X, 0.0f);
	leg3->addJoint(0, 11, 0, DIMENTION_JOINT3, KINEMATIC_AXIS_X, -90.0f);

	/// -- LEG 4 - Board 0 indexes 12, 13 & 14
	WalkerLeg* leg4 = sWalker->addLeg();
	leg4->addJoint(0, 12, 0, DIMENTION_JOINT1, KINEMATIC_AXIS_Z, 0.0f);
	leg4->addJoint(0, 13, 0, DIMENTION_JOINT2, KINEMATIC_AXIS_X, 0.0f);
	leg4->addJoint(0, 14, 0, DIMENTION_JOINT3, KINEMATIC_AXIS_X, -90.0f);

	/// -- LEG 5 - Board 0 index 15 & board 1 indexes 0 & 1
	WalkerLeg* leg5 = sWalker->addLeg();
	leg5->addJoint(0, 15, 0, DIMENTION_JOINT1, KINEMATIC_AXIS_Z, 0.0f);
	leg5->addJoint(1, 0, 0, DIMENTION_JOINT2, KINEMATIC_AXIS_X, 0.0f);
	leg5->addJoint(1, 1, 0, DIMENTION_JOINT3, KINEMATIC_AXIS_X, -90.0f);

	// --------------------------------------------------------------------------------------------
	//4) Test legs, bip or do some shit to show that the walker is ok
	// - https://github.com/AnonymousAlly/Arduino-Music-Codes
	for (uint8 i = 0; i < sWalker->getLegCount(); i++)
		sWalker->getLeg(i)->test(); //Test move

	// --------------------------------------------------------------------------------------------
	//5) Init timers
	update_time = millis() - 5;
	remote_timeout = RADIO_TIMEOUT;
}

void main_loop()
{ //Arduino like loop
	//1) Update remote control datas
	uint8* radio_packet = sRadio->update(diff());
	if (radio_packet)
	{
		direction_nunchuck.handlePacket((WiiNunchuckPacket*)radio_packet);
		remote_timeout = RADIO_TIMEOUT; //Just heard from the remote, reset timeout
	}

	//2) Update movements
	updateAction(diff());

	//3) Run walker update loop
	sWalker->update(diff());
	update_time = millis();
}
