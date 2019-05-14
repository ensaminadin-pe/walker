/*
 * Contains main functions for easy use on QT & Arduino
 * Did it that way because i wanted to test things on QT first
 */

#ifndef MAINFUNCTIONS_H
#define MAINFUNCTIONS_H

#include "arduinopolyfill.h"
#include "wiinunchuck.h"

#define MOVEMENT_UP_LOW		1
#define MOVEMENT_UP_MID		2
#define MOVEMENT_UP_HIGH	4
#define MOVEMENT_DOWN_LOW	8
#define MOVEMENT_DOWN_MID	16
#define MOVEMENT_DOWN_HIGH	32
#define MOVEMENT_LEFT_LOW	64
#define MOVEMENT_LEFT_MID	128
#define MOVEMENT_LEFT_HIGH	256
#define MOVEMENT_RIGHT_LOW	512
#define MOVEMENT_RIGHT_MID	1024
#define MOVEMENT_RIGHT_HIGH	2048

#define MOVEMENT_LOW_THRESHOLD	20
#define MOVEMENT_MID_THRESHOLD	60
#define MOVEMENT_HIGH_THRESHOLD	80

static unsigned long	update_time;
static unsigned long	movement_update_time;
static unsigned long	remote_timeout;
static unsigned int		movement;
static WiiNunchuck		direction_nunchuck;

void	main_setup();
void	main_loop();
void	handleMovement();
void	updateMovements(unsigned long time_diff);

unsigned long	diff();
unsigned int	mapMovementValue(unsigned int base, signed short joystick_value);

#endif // MAINFUNCTIONS_H
