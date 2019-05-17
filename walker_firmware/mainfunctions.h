/*
 * Contains main functions for easy use on QT & Arduino
 * Did it that way because i wanted to test things on QT first
 */

#ifndef MAINFUNCTIONS_H
#define MAINFUNCTIONS_H

#include "arduinopolyfill.h"
#include "wiinunchuck.h"

#define JOYSTICK_MOVEMENT direction_nunchuck.getJoystickMovement()

static unsigned long	update_time;
static WiiNunchuck		direction_nunchuck;
unsigned long			movement_update_time;
unsigned long			remote_timeout;

void	main_setup();
void	main_loop();

unsigned long	diff();
void			updateAction(unsigned long time_diff);

#endif // MAINFUNCTIONS_H
