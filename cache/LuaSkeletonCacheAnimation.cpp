#include "spine/LuaSkeletonCacheAnimation.h"
#include "cocos2d.h"
#include "cocos2d/LuaScriptHandlerMgr.h"
#include "CCLuaStack.h"
#include "CCLuaEngine.h"

using namespace spine;
USING_NS_CC;

LuaSkeletonCacheAnimation::LuaSkeletonCacheAnimation(spSkeletonData* skeletonData)
	: SpineCacheAnimation(skeletonData)
{

}


LuaSkeletonCacheAnimation::~LuaSkeletonCacheAnimation()
{
	ScriptHandlerMgr::getInstance()->removeObjectAllHandlers((void*)this);
}

LuaSkeletonCacheAnimation* LuaSkeletonCacheAnimation::createWithFile(const char* skeletonDataFile, const char* atlasFile, float scale)
{
	spSkeletonData* skeletonData = SpineCacheFactory::getInstance()->getCacheSkeletonData(skeletonDataFile, atlasFile, scale);
	if (!skeletonData)
	{
		return nullptr;
	}
	LuaSkeletonCacheAnimation* animation = new LuaSkeletonCacheAnimation(skeletonData);
	animation->skeletonDataFile = skeletonDataFile;

	animation->autorelease();
	return animation;
}
