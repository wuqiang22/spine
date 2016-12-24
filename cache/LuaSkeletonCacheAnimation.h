#ifndef LUASKELETONANIMATIONCACHE_H_
#define LUASKELETONANIMATIONCACHE_H_
#include "spine/spine-cocos2dx.h"
#include "spine/SpineCacheAnimation.h"

class LuaSkeletonCacheAnimation : public SpineCacheAnimation {
public:
	static LuaSkeletonCacheAnimation* create(const char* skeletonDataFile, const char* atlasFile, float scale = 1);

	static void createAsync(const char* skeletonDataFile, const char* atlasFile, float scale = 1, std::function<void(LuaSkeletonCacheAnimation*)> callback = nullptr);
	LuaSkeletonCacheAnimation(spSkeletonData* skeletonData);

	virtual ~LuaSkeletonCacheAnimation();
};

#endif /* LUASKELETONANIMATIONCACHE_H_ */