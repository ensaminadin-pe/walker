#ifndef GAITDICTIONARY_H
#define GAITDICTIONARY_H

#include "gait.h"

class GaitDictionary
{
	public:
		GaitDictionary();
		Gait* getDefaultGait();
		Gait* getGait(int id);
};

#endif // GAITDICTIONARY_H
