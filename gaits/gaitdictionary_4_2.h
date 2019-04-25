#ifndef GAITDICTIONARY_4_2_H
#define GAITDICTIONARY_4_2_H

#include "../gaitdictionary.h"

class GaitDictionary_4_2: public GaitDictionary
{
	public:
		Gait* getGait(uint8 id);
		Gait* getDefaultGait();
};

#endif // GAITDICTIONARY_4_2_H
