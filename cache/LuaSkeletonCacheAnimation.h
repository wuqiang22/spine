#ifndef LUASKELETONANIMATIONCACHE_H_
#define LUASKELETONANIMATIONCACHE_H_
#include "spine/spine-cocos2dx.h"
#include "spine/SpineCacheAnimation.h"

class LuaSkeletonCacheAnimation : public SpineCacheAnimation {
public:
	static LuaSkeletonCacheAnimation* createWithFile(const char* skeletonDataFile, const char* atlasFile, float scale = 1);

	LuaSkeletonCacheAnimation(spSkeletonData* skeletonData);

	virtual ~LuaSkeletonCacheAnimation();
};

#endif /* LUASKELETONANIMATIONCACHE_H_ */