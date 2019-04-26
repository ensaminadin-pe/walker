#ifndef GAITDICTIONARY_4_2_H
#define GAITDICTIONARY_4_2_H

#include "gaitdictionary.h"

class GaitDictionary_4_2: public GaitDictionary
{
	public:
		GaitDictionary_4_2(void);
		~GaitDictionary_4_2() override;
		Gait* getDefaultGait() override;
		Gait* getGait(uint8 id) override;
};

#endif // GAITDICTIONARY_4_2_H
