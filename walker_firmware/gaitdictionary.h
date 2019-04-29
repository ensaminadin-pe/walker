#ifndef GAITDICTIONARY_H
#define GAITDICTIONARY_H

#include "gait.h"
#include "types.h"

class GaitDictionary
{
	public:
		GaitDictionary();
		virtual ~GaitDictionary();
		virtual Gait* getDefaultGait() = 0;
		virtual Gait* getGait(uint16 id) = 0;
};

#endif // GAITDICTIONARY_H
