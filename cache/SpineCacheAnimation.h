#ifndef __SPINE_CACHE_ANIMATION_H__
#define __SPINE_CACHE_ANIMATION_H__

#include "spine/spine-cocos2dx.h"
#include "spine/SkeletonAnimation.h"
#include <unordered_map>

class SpineCacheFactory
{
public:


	struct spSkeletonDataCache{
		spSkeletonDataCache(const std::string& path, spSkeletonData* object) :skeletonDataFile(path), skeletonData(object),refrenceCount(0){}
		const std::string skeletonDataFile;
		spSkeletonData* skeletonData;
		size_t refrenceCount;

		void retain()
		{
			++refrenceCount;
		}
		void release()
		{
			--refrenceCount;
		}
	};

	static SpineCacheFactory* instance;
	static SpineCacheFactory* getInstance();
	static void destoryInstance();

	SpineCacheFactory(){};
	virtual ~SpineCacheFactory();

	spSkeletonData* getCacheSkeletonData(const std::string& skeletonDataFile, const std::string& atlasFile,float scale = 1);

	void returnCacheSkeletonData(const std::string& skeletonDataFile);
	
private:
	spSkeletonData* createspSkeletonData(const std::string& skeletonDataFile, const std::string& atlasFile, float scale = 1);
private:
	std::unordered_map<std::string, spSkeletonDataCache*> skeletonDataCaches;
};

class SpineCacheAnimation : public spine::SkeletonAnimation{
public:

	SpineCacheAnimation(spSkeletonData* skeletonData);
	virtual ~SpineCacheAnimation();
protected:
	std::string skeletonDataFile;
};


#endif