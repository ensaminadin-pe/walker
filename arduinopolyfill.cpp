#ifdef IS_QT
	#include "arduinopolyfill.h"
	#include <sys/time.h>
	#include <unistd.h>

	unsigned long millis()
	{
		struct timeval tp;
		gettimeofday(&tp, NULL);
		return tp.tv_sec * 1000 + tp.tv_usec / 1000;
	}

	void delay(int ms)
	{
		usleep(ms * 1000);
	}
#endif
