/** TODO
 - handle some kind of command input
	- 2.4Ghz rf modules
	- build the remote
 */

#include <stdio.h>

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
