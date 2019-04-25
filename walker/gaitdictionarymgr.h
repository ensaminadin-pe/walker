#ifndef GAITDICTIONARYMGR_H
#define GAITDICTIONARYMGR_H

#include "gaitdictionary.h"
#include "types.h"

class GaitDictionaryMgr
{
	public:
		GaitDictionaryMgr();
		//Methods
		static GaitDictionaryMgr* instance();
		static GaitDictionary* getDictionary();
		GaitDictionary* getDictionaryFor(uint8 leg_count, uint8 joint_count);
};

#define sGaitDictionaryMgr GaitDictionaryMgr::instance()
#define sGaitDictionary GaitDictionaryMgr::getDictionary()

#endif // GAITDICTIONARYMGR_H
