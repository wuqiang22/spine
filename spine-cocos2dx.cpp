/******************************************************************************
 * Spine Runtimes Software License
 * Version 2.1
 * 
 * Copyright (c) 2013, Esoteric Software
 * All rights reserved.
 * 
 * You are granted a perpetual, non-exclusive, non-sublicensable and
 * non-transferable license to install, execute and perform the Spine Runtimes
 * Software (the "Software") solely for internal use. Without the written
 * permission of Esoteric Software (typically granted by licensing Spine), you
 * may not (a) modify, translate, adapt or otherwise create derivative works,
 * improvements of the Software or develop new applications using the Software
 * or (b) remove, delete, alter or obscure any trademarks or any copyright,
 * trademark, patent or other intellectual property or proprietary rights
 * notices on or in the Software, including any copy thereof. Redistributions
 * in binary or source form must include this license and terms.
 * 
 * THIS SOFTWARE IS PROVIDED BY ESOTERIC SOFTWARE "AS IS" AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO
 * EVENT SHALL ESOTERIC SOFTARE BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *****************************************************************************/

#include <spine/spine-cocos2dx.h>
#include <spine/extension.h>
#include <spine/AtlasAttachmentLoader.h>

USING_NS_CC;

void _spAtlasPage_createTexture (spAtlasPage* self, const char* path) {
	Texture2D* texture = Director::getInstance()->getTextureCache()->addImage(path);
	texture->retain();
	self->rendererObject = texture;
	self->width = texture->getPixelsWide();
	self->height = texture->getPixelsHigh();
}

void _spAtlasPage_setTexture(spAtlasPage* self, void* texture)
{
	if (!texture)
	{
		return;
	}
	cocos2d::Texture2D* _texture = (cocos2d::Texture2D*)texture;
	if (!_texture)
	{
		return;
	}

	_texture->retain();
	self->rendererObject = _texture;
	self->width = _texture->getPixelsWide();
	self->height = _texture->getPixelsHigh();
	spAtlasRegion* region= self->atlas->regions;
	while (region)
	{
		if (region->page == self)
		{
			region->u = region->x / (float)self->width;
			region->v = region->y / (float)self->height;
			if (region->rotate) {
				region->u2 = (region->x + region->height) / (float)self->width;
				region->v2 = (region->y + region->width) / (float)self->height;
			}
			else {
				region->u2 = (region->x + region->width) / (float)self->width;
				region->v2 = (region->y + region->height) / (float)self->height;
			}
		}
		region = region->next;
	}

	
}

void _spAtlasPage_disposeTexture (spAtlasPage* self) {
	((Texture2D*)self->rendererObject)->release();
}

char* _spUtil_readFile (const char* path, int* length) {
	Data data = FileUtils::getInstance()->getDataFromFile(
			FileUtils::getInstance()->fullPathForFilename(path).c_str());
	*length = data.getSize();
	char* bytes = MALLOC(char, *length);
	memcpy(bytes, data.getBytes(), *length);
	return bytes;
}

void _spAtlasPage_createTexture_ext(spAtlasPage* self, void* rendererObject)
{
	Texture2D* texture = static_cast<Texture2D*>(rendererObject);
	if (texture)
	{
		texture->retain();
		self->rendererObject = texture;
		self->width = texture->getPixelsWide();
		self->height = texture->getPixelsHigh();
		self->format = SP_ATLAS_RGBA8888;
		self->minFilter = SP_ATLAS_LINEAR;
		self->magFilter = SP_ATLAS_LINEAR;
		self->uWrap = SP_ATLAS_MIRROREDREPEAT;
		self->vWrap = SP_ATLAS_MIRROREDREPEAT;
	}

}


void _spAtlasRegion_updateSpriteFrame(spAtlasRegion* self, void* spriteFrame)
{
	if (!spriteFrame)
	{
		return;
	}
	cocos2d::SpriteFrame* frame = static_cast<cocos2d::SpriteFrame*>(spriteFrame);
	if (frame)
	{
		const Size& texturePixelSize = frame->getTexture()->getContentSizeInPixels();
		const Rect& pixelRect = frame->getRectInPixels();

		self->rotate = frame->isRotated();
		self->flip = false;
		self->x = frame->getRect().origin.x;
		self->y = frame->getRect().origin.y;
		self->width = frame->getRect().size.width;
		self->height = frame->getRect().size.height;
		self->u = (float)(pixelRect.origin.x) / texturePixelSize.width;
		self->v = (float)(pixelRect.origin.y) / texturePixelSize.height;
		if (frame->isRotated())
		{
			self->u2 = (float)(pixelRect.size.height + pixelRect.origin.x) / texturePixelSize.width;
			self->v2 = (float)(pixelRect.size.width + pixelRect.origin.y) / texturePixelSize.height;
		}
		else
		{
			self->u2 = (float)(pixelRect.size.width + pixelRect.origin.x) / texturePixelSize.width;
			self->v2 = (float)(pixelRect.size.height + pixelRect.origin.y) / texturePixelSize.height;
		}
		self->originalWidth = frame->getOriginalSize().width;
		self->originalHeight = frame->getOriginalSize().height;
		self->offsetX = frame->getOffset().x + (self->originalWidth - frame->getRect().size.width) / 2;
		self->offsetY = frame->getOffset().y + (self->originalHeight - frame->getRect().size.height) / 2;
		self->index = -1;
	}
}

void _spAtlasRegion_updateSprite(spAtlasRegion* self, void* sprite)
{
	if (!sprite)
	{
		return;
	}

	cocos2d::Sprite* spSprite = static_cast<cocos2d::Sprite*>(sprite);
	if (spSprite)
	{
		const Size& texturePixelSize = spSprite->getTexture()->getContentSizeInPixels();
		const Size& spriteSize = spSprite->getContentSize();
		const Rect& spriteRect = spSprite->getTextureRect();
		const Rect& pixelRect = CC_RECT_POINTS_TO_PIXELS(spriteRect);


		self->rotate = false;
		self->flip = false;

		self->x = spriteRect.origin.x;
		self->y = spriteRect.origin.y;
		self->width = spriteRect.size.width;
		self->height = spriteRect.size.height;
		self->u = (float)(pixelRect.origin.x) / texturePixelSize.width;
		self->v = (float)(pixelRect.origin.y) / texturePixelSize.height;

		

		self->u2 = (float)(pixelRect.size.width + pixelRect.origin.x) / texturePixelSize.width;
		self->v2 = (float)(pixelRect.size.height + pixelRect.origin.y) / texturePixelSize.height;

		self->originalWidth = spriteRect.size.width;
		self->originalHeight = spriteRect.size.height;
		self->offsetX = (spriteSize.width - spriteRect.size.width) / 2;
		self->offsetY = (spriteSize.height - spriteRect.size.height) / 2;
		self->index = -1;
	}
}


//统一了上面的两个接口
void _spAtlasRegion_updateSprite_Total(spAtlasRegion* self, void* sprite)
{
	if (!sprite)
	{
		return;
	}

	cocos2d::Sprite* spSprite = static_cast<cocos2d::Sprite*>(sprite);
	if (spSprite)
	{
		const Size& texturePixelSize = spSprite->getTexture()->getContentSizeInPixels();
		const Size& spriteSize = spSprite->getContentSize();
		const Rect& spriteRect = spSprite->getTextureRect();
		const Rect& pixelRect = CC_RECT_POINTS_TO_PIXELS(spriteRect);


		self->rotate = spSprite->isTextureRectRotated();
		self->flip = false;

		self->x = spriteRect.origin.x;
		self->y = spriteRect.origin.y;
		self->width = spriteRect.size.width;
		self->height = spriteRect.size.height;
		self->u = (float)(pixelRect.origin.x) / texturePixelSize.width;
		self->v = (float)(pixelRect.origin.y) / texturePixelSize.height;

		if (spSprite->isTextureRectRotated())
		{
			self->u2 = (float)(pixelRect.size.height + pixelRect.origin.x) / texturePixelSize.width;
			self->v2 = (float)(pixelRect.size.width + pixelRect.origin.y) / texturePixelSize.height;
		}
		else
		{
			self->u2 = (float)(pixelRect.size.width + pixelRect.origin.x) / texturePixelSize.width;
			self->v2 = (float)(pixelRect.size.height + pixelRect.origin.y) / texturePixelSize.height;
		}


		self->originalWidth = spriteRect.size.width;
		self->originalHeight = spriteRect.size.height;
		self->offsetX = spSprite->getOffsetPosition().x;
		self->offsetY = spSprite->getOffsetPosition().y;
		self->index = -1;
	}
}



//废弃
spAttachment* _spSkeleton_createNewAttachmentWithSpriteFrame(spAtlas* atlas, spAttachment* oldAttachment, void* frame, const char* newPageName, const char* newRegionName)
{
	if (!frame)
	{
		return 0;
	}
	const char* oldAttachmentName = oldAttachment->name; 
	cocos2d::SpriteFrame* spriteFrame = static_cast<cocos2d::SpriteFrame*>(frame);
	spAtlasPage* newPage = spAtlas_createNewAtlasPage(atlas, newPageName, spriteFrame->getTexture());
	if (!newPage)
	{
		return 0;
	}
	
	spAtlasRegion* newRegion = spAtlas_createNewAtlasRegionWithSpriteFrame(atlas, newRegionName, spriteFrame);
	if (!newRegion)
	{
		return 0;
	}

	newRegion->page = newPage;

	return spAtlasAttachmentLoader_createAttachmentWidthOldAttachment(oldAttachment, newRegion);
	
}

//1:创建新的spAtlasPage:纹理图
//2:创建新的spAtlasRegion:纹理的描述
//3:创建新的spAttachment:附载点
//4:替换现在的附载点即可
spAttachment* _spSkeleton_createNewAttachmentWithSprite(spAtlas* atlas, spAttachment* oldAttachment, void* sprite, const char* newPageName, const char* newRegionName)
{
	if (!sprite)
	{
		return 0;
	}
	const char* oldAttachmentName = oldAttachment->name;
	cocos2d::Sprite* spSprite = static_cast<cocos2d::Sprite*>(sprite);
	spAtlasPage* newPage = spAtlas_createNewAtlasPage(atlas, newPageName, spSprite->getTexture());
	if (!newPage)
	{
		return 0;
	}

	spAtlasRegion* newRegion = spAtlas_createNewAtlasRegionWithSprite(atlas, newRegionName, spSprite);
	if (!newRegion)
	{
		return 0;
	}

	newRegion->page = newPage;

	return spAtlasAttachmentLoader_createAttachmentWidthOldAttachment(oldAttachment, newRegion);

}


