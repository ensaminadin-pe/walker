#ifndef WALKERJOINT_H
#define WALKERJOINT_H

#include "types.h"

class WalkerJoint
{
	public:
		WalkerJoint();
		~WalkerJoint();
		void reset();
	private:
		uint8 angle;
};

#endif // WALKERJOINT_H
