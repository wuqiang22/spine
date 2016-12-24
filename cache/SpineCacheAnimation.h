#ifndef __SPINE_CACHE_ANIMATION_H__
#define __SPINE_CACHE_ANIMATION_H__

#include "spine/spine-cocos2dx.h"

#include "spine/SkeletonAnimation.h"
#include <unordered_map>
#include <vector>

class SpineCacheFactory
{
public:

	struct AsyncStruct{
	public:
		AsyncStruct(const std::string& _skeletonPath, const std::string& _atlasFile) :skeletonDataFile(_skeletonPath), atlasFile(_atlasFile), key(skeletonDataFile + atlasFile), textureLoadCount(0), currentLoadCount(0)
		{}
		std::string skeletonDataFile;
		std::string atlasFile;
		std::string key;

		int currentLoadCount;
		int textureLoadCount;

		std::vector<std::function<void(spSkeletonData*)>> callbacks;

		void emplace_back(std::function<void(spSkeletonData*)> callback)
		{
			if (callback)
			{
				callbacks.emplace_back(callback);
			}
		}

		void notifyAll(spSkeletonData* data)
		{
			for (auto& callback : callbacks)
			{
				callback(data);
			}
			callbacks.clear();
		}
	};

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

	spSkeletonData* getCacheSkeletonData(const std::string& skeletonDataFile);
	spSkeletonData* getOrCreateCacheSkeletonData(const std::string& skeletonDataFile, const std::string& atlasFile, float scale = 1);
	
	void loadSkeletonDataAsync(const std::string& skeletonDataFile, const std::string& atlasFile, spAtlas* atlas,float scale = 1,std::function<void(spSkeletonData*)> callback = nullptr);
	void loadspAtlasAsync(const std::string& skeletonDataFile, const std::string& atlasFile, std::function<void(spAtlas*)> callback = nullptr);
	void getCacheSkeletonDataAsync(const std::string& skeletonDataFile, const std::string& atlasFile, float scale = 1, std::function<void(spSkeletonData*)> callback = nullptr);

	void returnCacheSkeletonData(const std::string& skeletonDataFile);
	
private:
	spSkeletonData* createspSkeletonData(const std::string& skeletonDataFile, const std::string& atlasFile, float scale = 1);
private:
	std::unordered_map<std::string, spSkeletonDataCache*> skeletonDataCaches;
	std::unordered_map<std::string, AsyncStruct*> asyncStructs;
};

class SpineCacheAnimation : public spine::SkeletonAnimation{
public:

	SpineCacheAnimation(spSkeletonData* skeletonData);
	virtual ~SpineCacheAnimation();
protected:
	std::string skeletonDataFile;
};


#endif