/** TODO
 - add servo structure/driver
 - add I2C communication with servos
 - handle some kind of command input
	- 2.4Ghz rf modules
	- build the remote
 */

#ifdef IS_QT
	#include "mainfunctions.h"

	int main()
	{
		main_setup();
		while (1)
			main_loop();

		return 0;
	}
#endif // IS_QT
