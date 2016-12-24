#include "SpineCacheAnimation.h"
#include "async/AsyncFileUtils.h"
#include <spine/extension.h>

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



spSkeletonData* SpineCacheFactory::getCacheSkeletonData(const std::string& skeletonDataFile)
{
	const auto& iter = skeletonDataCaches.find(skeletonDataFile);
	if (iter == skeletonDataCaches.end())
	{

		return nullptr;
	}
	return iter->second->skeletonData;
}

spSkeletonData* SpineCacheFactory::getOrCreateCacheSkeletonData(const std::string& skeletonDataFile, const std::string& atlasFile, float scale)
{
	if (!getCacheSkeletonData(skeletonDataFile))
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

void SpineCacheFactory::loadspAtlasAsync(const std::string& skeletonDataFile, const std::string& atlasFile, std::function<void(spAtlas*)> callback)
{
	std::string key = skeletonDataFile + atlasFile;
	
	game::AsyncFileUtils::getInstance()->loadDataAsync(atlasFile, [=](const cocos2d::Data& data){

		AsyncStruct* asyncStruct = asyncStructs[key];
		if (!asyncStruct)
		{
			cocos2d::log("there is no asyncStruct for skeletonPath = %s,atlasPath = %s", skeletonDataFile.c_str(), atlasFile.c_str());
			return;
		}

		if (data.getSize() <= 0 || !data.getBytes())
		{
			asyncStruct->notifyAll(nullptr);
			
			return;
		}

		spSkeletonData* skeletonData = getCacheSkeletonData(skeletonDataFile);
		if (skeletonData)
		{
			asyncStruct->notifyAll(skeletonData);
			return;
		}
		

		const char* bytes = (const char*)data.getBytes();
		size_t len = data.getSize();

		int dirLength;
		char *dir;
		int length;

		spAtlas* atlas = 0;

		/* Get directory from atlas path. */
		const char* lastForwardSlash = strrchr(atlasFile.c_str(), '/');
		const char* lastBackwardSlash = strrchr(atlasFile.c_str(), '\\');
		const char* lastSlash = lastForwardSlash > lastBackwardSlash ? lastForwardSlash : lastBackwardSlash;
		if (lastSlash == atlasFile) lastSlash++; /* Never drop starting slash. */
		dirLength = (int)(lastSlash ? lastSlash - atlasFile.c_str() : 0);
		dir = MALLOC(char, dirLength + 1);
		memcpy(dir, atlasFile.c_str(), dirLength);
		dir[dirLength] = '\0';
		atlas = spAtlas_create(bytes, len, dir, 0, 0);

		do{
			if (!atlas)
			{
				asyncStruct->notifyAll(nullptr);
				break;
			}
			spAtlasPage* page = atlas->pages;
			bool isLoadFail = false;
			while (page)
			{
				++asyncStruct->textureLoadCount;
				int dirLength = (int)strlen(dir);
				int needsSlash = dirLength > 0 && dir[dirLength - 1] != '/' && dir[dirLength - 1] != '\\';
				char* path = MALLOC(char, dirLength + needsSlash + strlen(page->name) + 1);
				memcpy(path, dir, dirLength);
				if (needsSlash) path[dirLength] = '/';
				strcpy(path + dirLength + needsSlash, page->name);

				cocos2d::Director::getInstance()->getTextureCache()->addImageAsync(path, [=,&isLoadFail](cocos2d::Texture2D* texture) {
					if (isLoadFail)
					{
						return;
					}
					AsyncStruct* newasyncStruct = asyncStructs[key];
					if (!texture)
					{
						if (newasyncStruct)
						{
							newasyncStruct->notifyAll(nullptr);
						}
						isLoadFail = true;
						return;
					}
					++newasyncStruct->currentLoadCount;
					_spAtlasPage_setTexture(page, texture);
					if (newasyncStruct->currentLoadCount >= newasyncStruct->textureLoadCount)
					{
						cocos2d::log("all texture has loaded");
						if (callback)
						{
							callback(atlas);
						}
					}
				});
				FREE(path);

				page = page->next;
			}

		} while (0);
		FREE(dir);
	});
}

void SpineCacheFactory::loadSkeletonDataAsync(const std::string& skeletonDataFile, const std::string& atlasFile, spAtlas* atlas, float scale,std::function<void(spSkeletonData*)> callback)
{
	CCASSERT(atlas, "atlas is nullptr");
	game::AsyncFileUtils::getInstance()->loadDataAsync(skeletonDataFile, [=](const cocos2d::Data& data){
		std::string key = skeletonDataFile + atlasFile;
		AsyncStruct* asyncStruct = asyncStructs[key];
		if (!asyncStruct)
		{
			return;
		}
		if (!data.getBytes() || data.getSize() <= 0)
		{
			asyncStruct->notifyAll(nullptr);
			cocos2d::log("load skeleton data fail for path = %s", skeletonDataFile.c_str());
			return;
		}

		spSkeletonJson* json = spSkeletonJson_create(atlas);
		json->scale = scale;
		spSkeletonData* skeletonData =  spSkeletonJson_readSkeletonData(json, (const char*)data.getBytes());
		CCASSERT(skeletonData, json->error ? json->error : "Error reading skeleton data file.");
		spSkeletonJson_dispose(json);
		if (callback)
		{
			callback(skeletonData);
		}
	});
}

void SpineCacheFactory::getCacheSkeletonDataAsync(const std::string& skeletonDataFile, const std::string& atlasFile, float scale, std::function<void(spSkeletonData*)> callback)
{
	spSkeletonData* skeletonData = getCacheSkeletonData(skeletonDataFile);
	if (skeletonData)
	{
		if (callback){ callback(skeletonData); }
		return;
	}
	std::string key = skeletonDataFile + atlasFile;
	AsyncStruct* asyncStruct = asyncStructs[key];
	bool exist = true;
	if (!asyncStruct)
	{
		exist = false;
		asyncStruct = new AsyncStruct(skeletonDataFile, atlasFile);
		asyncStructs[key] = asyncStruct;
	}
	asyncStruct->emplace_back(callback);
	if (exist)
	{
		return;
	}

	loadspAtlasAsync(skeletonDataFile, atlasFile, [=](spAtlas* atlas){
	
		if (!atlas)
		{
			spSkeletonData* data = getCacheSkeletonData(skeletonDataFile);
			asyncStruct->notifyAll(data);
			return;
		}



		loadSkeletonDataAsync(skeletonDataFile, atlasFile, atlas, scale, [=](spSkeletonData* skeletonData){
			asyncStruct->notifyAll(skeletonData);
		});
	});

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

