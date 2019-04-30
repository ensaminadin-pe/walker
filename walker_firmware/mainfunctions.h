/*
 * Contains main functions for easy use on QT & Arduino
 * Did it that way because i wanted to test things on QT first
 */

#ifndef MAINFUNCTIONS_H
#define MAINFUNCTIONS_H

#include "types.h"
#include "arduinopolyfill.h"
#include "wiinunchuck.h"

static uint32 update_time;
static WiiNunchuck direction_nunchuk;

void main_setup();
void main_loop();
uint32 diff();

#endif // MAINFUNCTIONS_H
