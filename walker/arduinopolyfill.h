/*
 * Missing Arduino functions for QT usage
 */

#ifdef IS_QT
	#ifndef ARDUINOPOLYFILL
	#define ARDUINOPOLYFILL

	unsigned long	millis();
	void			delay(int ms);

	#endif // ARDUINOPOLYFILL
#endif // IS_QT
