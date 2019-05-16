#include "config.h"
#include "beeper.h"
#include "arduinopolyfill.h"
#ifndef IS_QT
	#include "Arduino.h"
#endif

Beeper::Beeper()
{
	off_timer = 0;
	is_on = false;
}

/**
 * @brief Beeper::update Update beeper turn off
 * @param diff
 */
void Beeper::update(unsigned long diff)
{
	if (!is_on)
		return;

	if (off_timer <= diff)
	{ // Turn off beeper until another note call is made
		noTone(BEEPER_PIN);
		off_timer = 0;
		is_on = false;
	}
	else
		off_timer -= diff;
}

/**
 * @brief Beeper::beep Setup a beep note for a duration
 * @param frequency
 * @param _note_lenght
 * @param _off_timer
 */
void Beeper::beep(uint16 frequency, uint16 note_lenght, uint16 _off_timer)
{
	tone(BEEPER_PIN, frequency, note_lenght);
	is_on = true;
	off_timer = _off_timer;
}

/**
 * @brief Beeper::instance Static singleton getter
 * @return
 */
Beeper* Beeper::instance()
{
	//Static getter to allow access everywhere, singleton
	static Beeper instance;
	return &instance;
}
