#include "walkerjoint.h"
#include "walker.h"
#include "servodriver.h"

WalkerJoint::WalkerJoint(uint8 _driver_board, uint8 _driver_index, uint16 _offset, float _distance)
{
	driver_board = _driver_board;
	driver_index = _driver_index;
	offset = _offset;
	distance = _distance;
	current_position = 0;
	target_position = 0;
	running = false;
	movement_done = true;
}

WalkerJoint::~WalkerJoint()
{

}

/**
 * @brief WalkerJoint::updateServoPosition Update joint's servo current position
 * @param diff
 * @return
 */
bool WalkerJoint::updateServoPosition(unsigned long diff)
{
	//1) Stop update if reached the target position
	if (!running)
		return movement_done;

	//2) Calculate current position
	/// - TODO - UPDATE current_position WITH CORRECT MOVEMENT RATE

	//3) Update servo position
	setServoPosition(current_position);

	//4) Check if reached target position
	movement_done = (current_position == target_position);
	if (movement_done)
		running = false;
	return movement_done;
}

/**
 * @brief WalkerJoint::setPosition Set current position and target position
 * @param position
 * @param apply
 */
void WalkerJoint::setPosition(float position)
{
	current_position = position;
	target_position = position;
	setServoPosition(current_position);
}

/**
 * @brief WalkerJoint::toggle Toggle running state
 * @param toggle
 */
void WalkerJoint::toggle(uint8 toggle)
{
	if (toggle == 2)
		running = !running; //Nothing specified, just toggle it
	else
		running = (toggle == 1); //Something specified, set to true or false
}

/**
 * @brief WalkerJoint::setServoPosition Set joint servo position
 * @param angle
 */
void WalkerJoint::setServoPosition(float angle)
{
	sServoDriver->setServo(driver_board, driver_index, angle, offset);
}
