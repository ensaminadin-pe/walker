#include "mainfunctions.h"
#include "walker.h"
#include "gaitdictionarymgr.h"
#include <stdio.h>

#ifdef IS_QT
	#include "arduinopolyfill.h"
#else
	#include "stdpolyfill.h"
#endif

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
///		1.2) Wireless receiver
///			Later

unsigned long update_time;

void main_setup()
{ //Arduino like setup
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
	sServoDriver->setFrequency(60); //60Hz
	sServoDriver->setupPulseRange(145, 515); //0° and 180°
	printf("Walker is init !\n");
	//4) Setup default Gait for number of legs and joint
	//   default should be stand position for calibration
	//sWalker->setNextGait(sGaitDictionary->getDefaultGait());
}

void main_loop()
{ //Arduino like loop


	sServoDriver->setServo(0, 90);
	delay(2000);
	sServoDriver->setServo(0, 0);
	delay(2000);
	sServoDriver->setServo(0, -45);
	delay(2000);
	sServoDriver->setServo(0, 0);
	delay(2000);
	return;

	//1) Setup loop diff time
	if (update_time == 0)
	{
		update_time = millis();
		return;
	}

	//2) Calculate loop diff time
	int diff = millis() - update_time;
	if (diff <= 1) //Less than 1ms, delay update, we need at least 1ms of diff
	{
		delay(1);
		diff = 1;
	}
	update_time = millis();

	//3) Add control logic here
	//   - remote control actions to set next gait
	//   - sensor detection and whatnot

	//4) Run walker update loop
	sWalker->update(diff);
}
