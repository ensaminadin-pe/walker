#ifndef BEEPER_H
#define BEEPER_H

#include "types.h"

class Beeper
{
	public:
		Beeper();
		static Beeper* instance();
		void update(unsigned long diff);
		void beep(uint16 frequency, uint16 note_lenght, uint16 _off_timer);
		/// - TODO - SING QUEUE
		///		? NOTE STRUCT
		///		? HOW TO STORE SONGS
	private:
		bool	is_on;
		uint16	off_timer;
};

#define sBeeper Beeper::instance()

#endif // BEEPER_H
