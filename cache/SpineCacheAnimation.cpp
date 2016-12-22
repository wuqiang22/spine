#include "SpineCacheAnimation.h"

SpineCacheFactory* SpineCacheFactory::instance = nullptr;

SpineCacheFactory* SpineCacheFactory::getInstance()
{
	if (!instance)
	{
		instance = new SpineCacheFactory();
	}
	return instance;
}

void SpineCacheFactory::destoryInstance()
{
	if (instance)
	{
		delete instance;
		instance = nullptr;
	}
}

SpineCacheFactory::~SpineCacheFactory()
{
	for (auto& iter : skeletonDataCaches)
	{
		if (iter.second->skeletonData)
		{
			spSkeletonData_dispose(iter.second->skeletonData);
		}
		delete iter.second;
	}
	skeletonDataCaches.clear();
}


spSkeletonData* SpineCacheFactory::getCacheSkeletonData(const std::string& skeletonDataFile, const std::string& atlasFile,float scale)
{
	const auto& iter = skeletonDataCaches.find(skeletonDataFile);
	if (iter == skeletonDataCaches.end())
	{
		
		spSkeletonData* skeletonData = createspSkeletonData(skeletonDataFile, atlasFile, scale);
		if (!skeletonData)
		{
			return nullptr;
		}
		
		spSkeletonDataCache* newCahche = new spSkeletonDataCache(skeletonDataFile, skeletonData);
		skeletonDataCaches.emplace(std::make_pair(skeletonDataFile, newCahche));
		
	}

	do{
		spSkeletonDataCache* cache = skeletonDataCaches[skeletonDataFile];
		if (!cache){
			break;
		}
		spSkeletonData* skeletonData = cache->skeletonData;
		if (!skeletonData)
		{
			break;
		}
		cache->retain();
		return skeletonData;

	} while (0);

	return nullptr;
}



spSkeletonData* SpineCacheFactory::createspSkeletonData(const std::string& skeletonDataFile, const std::string& atlasFile, float scale)
{
	spAtlas* atlas = spAtlas_createFromFile(atlasFile.c_str(), 0);
	if (!atlas)
	{
		cocos2d::log("there is no atlas data in cache create skeleton data fail");
		return nullptr;
	}

	spSkeletonJson* json = spSkeletonJson_create(atlas);
	json->scale = scale;
	spSkeletonData* skeletonData = spSkeletonJson_readSkeletonDataFile(json, skeletonDataFile.c_str());
	CCASSERT(skeletonData, json->error ? json->error : "Error reading skeleton data file.");
	spSkeletonJson_dispose(json);
	if (!skeletonData)
	{
		if (atlas)
		{
			spAtlas_dispose(atlas);
		}
	}
	return skeletonData;
}



void SpineCacheFactory::returnCacheSkeletonData(const std::string& skeletonDataFile)
{
	const auto& iter = skeletonDataCaches.find(skeletonDataFile);
	if (iter == skeletonDataCaches.end())
	{
		cocos2d::log("there is no cache skeleton data fro file = %s", skeletonDataFile.c_str());
		return;
	}


	do{
		spSkeletonDataCache* cache = skeletonDataCaches[skeletonDataFile];
		if (!cache){
			break;
		}
		cache->release();

		if (cache->refrenceCount == 0)
		{
			spSkeletonData* skeletonData = cache->skeletonData;
			if (skeletonData)
			{
				spSkeletonData_dispose(skeletonData);
			}
			delete cache;
			skeletonDataCaches.erase(skeletonDataFile);
		}

	} while (0);
}


SpineCacheAnimation::SpineCacheAnimation(spSkeletonData* skeletonData)
	:spine::SkeletonAnimation(skeletonData)
{

}

SpineCacheAnimation::~SpineCacheAnimation()
{
	SpineCacheFactory::getInstance()->returnCacheSkeletonData(skeletonDataFile);
}