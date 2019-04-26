#define IS_ARDUINO
#include <Arduino.h>
#include <Servo.h>
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

/// - This is the servo calibration & pulse discovery program
///
/// This will help you find out how the right values to feed the PCA9685.
/// I'll assume you are using an Arduino, but it can work on any board
/// You just need to locate the I2C pins and a PWM capable pin
///
/// Before anything else, you need to install the adafruit PCA9685 library
///	You can find it in the library manager in the Arduino IDE, more infos on
///	https://github.com/adafruit/Adafruit-PWM-Servo-Driver-Library
///
/// - Step 1 : Zero the servo
///		Firt of all, you need to zero your servo.
///
///		Servos have 3 wires, left to right it's commonly brown, red and orange
///		Your colors may vary, but often the darkest is GND,
///		the middle is VCC and the last is data
///		A servo is controlled via a Pulsed Width Modulation (PWM) signal on a data pin
///		To cut it short, a specific signal will tell the servo to rotate at a specific angle
///
///		Here we will just set our servo to 180°, to calibrate it.
///		You just need to connect your servos's VCC to the arduino's +5V,
///		servos's GND to arduino's GND and servo's data to arduino's pin 9
///
///		Once you've connected the servos and powered the board, it should spin to the 180°
///		position, now put a servo arm on the gear parallel to the servo's body to remember
///		the position.
///
/// - Step 2 : Find the good pulse width
///		Now we'll play with the PCA9685
///		You can learn more about it here :
///		https://learn.adafruit.com/16-channel-pwm-servo-driver/hooking-it-up
///
///		Connect the PCA9685 board like so :
///			SCL to arduino's A5
///			SDA to arduino's A4
///			VCC to arduino's +3.3v
///			GND to arduino's GND or other source GND
///			V+	to arduino's +5v, DO NOT DO THIS LATER, you should use another power source
///								  but it will be okay for a quick and dirty debug
///		Now connect your servo to the PCA9685 board on the pins closest to the already
///		connected pins, this is the servo 0 on the board, the other side is the servo 15
///
///		And here is the tricky part, you need to find the right value between 0 and 4095 to
///		tell the PCA9685 to turn your sero at 180°.
///		The way to do it is dirty, just set a value and connect the PCA9685 V+, if the servo
///		reach the correct position, you won, if not, try another value.
///		For me, 180 was 515 and 0 was 145 so try low and go up
///		If the servo turn past its maximum and makes a crunching noise, remove the V+ wire immediatly,
///		you can damage your servo this way
///		Of course, when you found 180, you'll need to find 0

#define PCA9685_FREQUENCY 60 //60Hz, one update every 20ms

//Our 180° calibration
Servo test_servo;
//Servo driver
Adafruit_PWMServoDriver servoDriver;

void setup()
{
	//Step1 - Set servo to 180°
	test_servo.attach(9); //Default calibration on pin 9, change it to another PWM pin if needed
	test_servo.write(180); //Go to 180°
	//Step2 - Find correct pulse width for the PCA9685
	servoDriver.begin();
	servoDriver.setPWMFreq(PCA9685_FREQUENCY);
	delay(10); //Wait a beat for the PCA9685 to init properly

	int pulse_width = 515; //Change this to try an angle
	servoDriver.setPWM(0, 0, pulse_width);
}

void loop()
{
	delay(1500);
}
