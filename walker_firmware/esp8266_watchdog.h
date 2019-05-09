#ifndef ESP8266_WATCHDOG_H
#define ESP8266_WATCHDOG_H

#include "arduinopolyfill.h"
#include "types.h"

static void esp8266_delay(unsigned int time = 1)
{
	#ifdef ARDUINO_ESP8266_NODEMCU
		if (time > 1)
		{
			ESP.wdtDisable();
			ESP.wdtEnable(time);
		}
		else
			delay(1);
	#endif
}

#endif // ESP8266_WATCHDOG_H
