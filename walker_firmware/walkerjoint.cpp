#include "walkerjoint.h"
#include "walker.h"
#include "servodriver.h"

WalkerJoint::WalkerJoint(uint8 _leg_index, uint8 _joint_index)
{
	oscillator.setPeriodModifier(sWalker->getPeriodModifier());
	leg_index = _leg_index;
	joint_index = _joint_index;
	servo_driver_index = (sWalker->getJointCount() * leg_index) + joint_index;
}

WalkerJoint::~WalkerJoint()
{

}

void WalkerJoint::reset()
{
	stopOscillator();
}

void WalkerJoint::updateServoPosition()
{
	sServoDriver->setServo(servo_driver_index, oscillator.refresh());
}

void WalkerJoint::setupOscillator(uint16 period, int8 amplitude, int8 offset, uint16 phase)
{
	oscillator.setPeriod(period);
	oscillator.setAmplitude(amplitude);
	oscillator.setOffset(offset);
	oscillator.setPhase(phase);
}

void WalkerJoint::loadGait(Gait *gait)
{
	setupOscillator(gait->getPeriod(servo_driver_index), gait->getAmplitude(servo_driver_index), gait->getOffset(servo_driver_index), gait->getPhase(servo_driver_index));
}
