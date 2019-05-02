/*
 * Contains main functions for easy use on QT & Arduino
 * Did it that way because i wanted to test things on QT first
 */

#ifndef MAINFUNCTIONS_H
#define MAINFUNCTIONS_H

#include "arduinopolyfill.h"
#include "wiinunchuck.h"

static unsigned long update_time;
static WiiNunchuck direction_nunchuck;
static WiiNunchuckPacket nunchuck_packet;

static uint16 send_nunchuck_timer;

void main_setup();
void main_loop();
unsigned long diff();

void sendNunchuckUpdate(unsigned long diff);

#endif // MAINFUNCTIONS_H
