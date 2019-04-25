#include "mainfunctions.h"
#include "walker.h"
#include "gaitdictionarymgr.h"
#include <stdio.h>

#ifdef IS_QT
	#include "arduinopolyfill.h"
#else
	#include "stdpolyfill.h"
#endif

unsigned long update_time;

void main_setup()
{ //Arduino like setup
	//1) Setup leg & joint counts
	sWalker->setup(4, 2);
	printf("Leg count : %i\n", sWalker->getLegCount());
	printf("Joint count : %i\n", sWalker->getJointCount());
	//2) Init Oscillators & servos
	sWalker->init();
	printf("Walker is init !\n");
	//3) Setup default Gait for number of legs and joint
	//   default should be stand position for calibration
	Gait* gait = sGaitDictionary->getDefaultGait();
	sWalker->setNextGait(gait);
}

void main_loop()
{ //Arduino like loop
	//1) Setup loop diff time
	if (update_time == 0)
	{
		update_time = millis();
		return;
	}

	//2) Calculate loop diff time
	int diff = millis() - update_time;
	if (diff <= 1) //Less than 1ms, delay update, we need at least 1ms of diff
	{
		delay(1);
		diff = 1;
	}
	update_time = millis();

	//3) Add control logic here
	//   - remote control actions to set next gait
	//   - sensor detection and whatnot

	//4) Run walker update loop
	sWalker->update(diff);
}
