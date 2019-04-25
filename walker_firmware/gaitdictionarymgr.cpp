#include "gaitdictionarymgr.h"
#include "gaits/gaitdictionary_4_2.h"
#include "walker.h"

GaitDictionaryMgr::GaitDictionaryMgr()
{

}

/**
 * @brief Walker::instance Static singleton getter
 * @return
 */
GaitDictionaryMgr* GaitDictionaryMgr::instance()
{
	//Static getter to allow access everywhere, singleton
	static GaitDictionaryMgr instance;
	return &instance;
}

/**
 * @brief GaitDictionaryMgr::getDictionary Get correct gait dictionary for current walker configuration
 * @return
 */
GaitDictionary *GaitDictionaryMgr::getDictionary()
{
	static GaitDictionary* loadedDictionary;
	static uint8 leg_count;
	static uint8 joint_count;

	if (loadedDictionary && sWalker->getLegCount() == leg_count && sWalker->getJointCount() == joint_count)
		return loadedDictionary;

	leg_count = sWalker->getLegCount();
	joint_count = sWalker->getJointCount();
	loadedDictionary = sGaitDictionaryMgr->getDictionaryFor(sWalker->getLegCount(), sWalker->getJointCount());
	return loadedDictionary;
}

/**
 * @brief GaitDictionaryMgr::getDictionary Get correct gait dictionary from leg and joint count
 * @param leg_count
 * @param joint_count
 * @return
 */
GaitDictionary* GaitDictionaryMgr::getDictionaryFor(uint8 leg_count, uint8 joint_count)
{
	if (leg_count == 4)
	{
		if (joint_count == 2)
		{
			static GaitDictionary_4_2 dictionary_4_2;
			return &dictionary_4_2;
		}
	}

	return 0;
}
