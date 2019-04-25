#include "gaitdictionary.h"

GaitDictionary::GaitDictionary()
{
}

Gait *GaitDictionary::getDefaultGait()
{
	static Gait* defaultGait;
	if (!defaultGait)
		defaultGait = getGait(1);
	return defaultGait;
}

Gait* GaitDictionary::getGait(int id)
{
	return 0;
}
