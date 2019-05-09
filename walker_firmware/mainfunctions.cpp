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
Alternative to gait : animations, list of servos angles
	Need to find a way to build a static version of this
	Ex: a raw file with everything in binary that is loaded when needed
	Might need a tool to test the movements frame per frame too
	Might need a format to store animation dictionaries
	Animation
		uint8	leg_count
		uint8	joint_count
		uint8	frame_count
		uint8	type			- 0 = fixed position, 1 = movement, 2 = emote
		uint16	duration		- animation duration in ms
		Frame	frames[]		- animation frames
	Frame
		uint16		option			- flag or something, do an action
		uint16		option_value	- option value, ex if flag = 1, option value will be an id of sound to play or something like that
		uint8[X]	angle			- 0-180

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
	/// Setup your remote here
	sRadio->setup(RADIO_PIN, RADIO_ADRESS, RADIO_RECEIVE); //Radio data on pin 10, can be anything
	sRadio->setListenSize(sizeof(WiiNunchuckPacket));
	/// Setup your walker here
	//1) Setup leg & joint counts
	sWalker->setup(4, 2);
	printf("Leg count : %i\n", sWalker->getLegCount());
	printf("Joint count : %i\n", sWalker->getJointCount());
	//2) Init Oscillators & servos
	sWalker->init();
	//3) Setup servo pulse range
	//  This is the tricky part, you need to find the 0 and 180° pulse width yourself
	//	Its a number between 0 and 4095 and, for my sg90s it's 145 and 515 at 60Hz
	sServoDriver->setFrequency(SERVO_UPDATE_FREQUENCY); //60Hz
	sServoDriver->setupAngleRange(-90.0f, 90.0f);
	sServoDriver->setupPulseRange(145, 515); //0° and 180°
	printf("Walker is init !\n");
	//4) Setup default Gait for number of legs and joint
	//   default should be stand position for calibration
	sWalker->setNextGait(1);
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
