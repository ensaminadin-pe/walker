#include "walkerjoint.h"
#include "walker.h"
#include "servodriver.h"
#include "config.h"
#include <stdio.h>

WalkerJoint::WalkerJoint(uint8 _driver_board, uint8 _driver_index, bool _is_mirror, uint16 _offset, float _distance, KinematicAxis _rotation_axis, float _angle)
{
	// Config
	driver_board = _driver_board;
	driver_index = _driver_index;
	offset = _offset;
	distance = _distance;
	is_mirror = _is_mirror;
	// Kinematics
	kinematic_distance = (float)((uint16)(distance * 10.0f)); //mm to rounded 10th of mm
	rotation_axis = _rotation_axis;
	base_angle = _angle;
	computed_angle = base_angle;
	plane_x = 0.0f;
	plane_y = 0.0f;
	// Running
	running = false;
	current_position = 0;
	target_position = 0;
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

	//2) Check that we are within tolerance range
	/// - TODO - Define a better tolerance
	float position_diff = current_position - target_position;
	if (position_diff < 0)
		position_diff *= -1;
	if (position_diff < (SERVO_DEFAULT_MOVEMENT * 2))
		current_position = target_position;

	//3) Update current position
	/// - TODO - UPDATE current_position WITH CORRECT MOVEMENT RATE
	//if (current_position != 0.0f && target_position != 0.0f)
	//	printf("%f UPDATE SERVO POSITION FROM %f TO %f WITH DIFF %f\n", kinematic_distance, current_position, target_position, position_diff);
	if (current_position > target_position)
		current_position -= SERVO_DEFAULT_MOVEMENT;
	else if (current_position < target_position)
		current_position += SERVO_DEFAULT_MOVEMENT;

	//4) Update servo position if there is something to update
	setServoPosition(current_position);

	//5) Check if reached target position
	movement_done = (current_position == target_position);
	if (movement_done)
		running = false;
	return movement_done;
}

/**
 * @brief WalkerJoint::setTargetPosition Set target joint position to reach
 * @param position
 */
void WalkerJoint::setTargetPosition(float position)
{
	target_position = is_mirror ? -position : position;
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
 * @brief WalkerJoint::setKinematicPosition Set position X/Y in the plane used by the kinematic algorithm
 * @param x
 * @param y
 */
void WalkerJoint::setKinematicPosition(float x, float y)
{
	plane_x = x;
	plane_y = y;
}

/**
 * @brief WalkerJoint::setServoPosition Set joint servo position
 * @param angle
 */
void WalkerJoint::setServoPosition(float angle)
{
	sServoDriver->setServo(driver_board, driver_index, angle, offset);
}
