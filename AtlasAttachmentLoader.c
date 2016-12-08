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

#include <spine/AtlasAttachmentLoader.h>
#include <spine/extension.h>

spAttachment* _spAtlasAttachmentLoader_newAttachment (spAttachmentLoader* loader, spSkin* skin, spAttachmentType type,
		const char* name, const char* path) {
	spAtlasAttachmentLoader* self = SUB_CAST(spAtlasAttachmentLoader, loader);
	switch (type) {
	case SP_ATTACHMENT_REGION: {
		spRegionAttachment* attachment;
		spAtlasRegion* region = spAtlas_findRegion(self->atlas, path);
		if (!region) {
			_spAttachmentLoader_setError(loader, "Region not found: ", path);
			return 0;
		}
		attachment = spRegionAttachment_create(name);
		attachment->rendererObject = region;
		spRegionAttachment_setUVs(attachment, region->u, region->v, region->u2, region->v2, region->rotate);
		attachment->regionOffsetX = region->offsetX;
		attachment->regionOffsetY = region->offsetY;
		attachment->regionWidth = region->width;
		attachment->regionHeight = region->height;
		attachment->regionOriginalWidth = region->originalWidth;
		attachment->regionOriginalHeight = region->originalHeight;
		return SUPER(attachment);
	}
	case SP_ATTACHMENT_MESH: {
		spMeshAttachment* attachment;
		spAtlasRegion* region = spAtlas_findRegion(self->atlas, path);
		if (!region) {
			_spAttachmentLoader_setError(loader, "Region not found: ", path);
			return 0;
		}
		attachment = spMeshAttachment_create(name);
		attachment->rendererObject = region;
		attachment->regionU = region->u;
		attachment->regionV = region->v;
		attachment->regionU2 = region->u2;
		attachment->regionV2 = region->v2;
		attachment->regionRotate = region->rotate;
		attachment->regionOffsetX = region->offsetX;
		attachment->regionOffsetY = region->offsetY;
		attachment->regionWidth = region->width;
		attachment->regionHeight = region->height;
		attachment->regionOriginalWidth = region->originalWidth;
		attachment->regionOriginalHeight = region->originalHeight;
		return SUPER(attachment);
	}
	case SP_ATTACHMENT_SKINNED_MESH: {
		spSkinnedMeshAttachment* attachment;
		spAtlasRegion* region = spAtlas_findRegion(self->atlas, path);
		if (!region) {
			_spAttachmentLoader_setError(loader, "Region not found: ", path);
			return 0;
		}
		attachment = spSkinnedMeshAttachment_create(name);
		attachment->rendererObject = region;
		attachment->regionU = region->u;
		attachment->regionV = region->v;
		attachment->regionU2 = region->u2;
		attachment->regionV2 = region->v2;
		attachment->regionRotate = region->rotate;
		attachment->regionOffsetX = region->offsetX;
		attachment->regionOffsetY = region->offsetY;
		attachment->regionWidth = region->width;
		attachment->regionHeight = region->height;
		attachment->regionOriginalWidth = region->originalWidth;
		attachment->regionOriginalHeight = region->originalHeight;
		return SUPER(attachment);
	}
	case SP_ATTACHMENT_BOUNDING_BOX:
		return SUPER(spBoundingBoxAttachment_create(name));
	default:
		_spAttachmentLoader_setUnknownTypeError(loader, type);
		return 0;
	}
}

spAtlasAttachmentLoader* spAtlasAttachmentLoader_create (spAtlas* atlas) {
	spAtlasAttachmentLoader* self = NEW(spAtlasAttachmentLoader);
	_spAttachmentLoader_init(SUPER(self), _spAttachmentLoader_deinit, _spAtlasAttachmentLoader_newAttachment);
	self->atlas = atlas;
	return self;
}

spAttachment* spAtlasAttachmentLoader_createAttachmentWidthOldAttachment(spAttachment* oldAttachment, spAtlasRegion* newRegion)
{
	const char* oldAttachmentName = oldAttachment->name;
	spAttachmentType type = oldAttachment->type;
	switch (type)
	{
	case SP_ATTACHMENT_REGION:
	{
		spRegionAttachment* attachment;
		spAtlasRegion* region = newRegion;
		attachment = spRegionAttachment_create(oldAttachmentName);
		attachment->rendererObject = region;
		spRegionAttachment_setUVs(attachment, region->u, region->v, region->u2, region->v2, region->rotate);
		attachment->regionOffsetX = region->offsetX;
		attachment->regionOffsetY = region->offsetY;
		attachment->regionWidth = region->width;
		attachment->regionHeight = region->height;
		attachment->regionOriginalWidth = region->originalWidth;
		attachment->regionOriginalHeight = region->originalHeight;

		spRegionAttachment* oldRegionAttachment = SUB_CAST(spRegionAttachment, oldAttachment);
		attachment->x = oldRegionAttachment->x;
		attachment->y = oldRegionAttachment->y;
		attachment->scaleX = oldRegionAttachment->scaleX;
		attachment->scaleY = oldRegionAttachment->scaleY;
		attachment->width = oldRegionAttachment->width;
		attachment->height = oldRegionAttachment->height;
		attachment->r = oldRegionAttachment->r;
		attachment->g = oldRegionAttachment->g;
		attachment->b = oldRegionAttachment->b;
		attachment->a = oldRegionAttachment->a;
		attachment->rotation = oldRegionAttachment->rotation;
		spRegionAttachment_updateOffset(attachment);


		return SUPER(attachment);
	}
	case SP_ATTACHMENT_MESH:
	{
		spMeshAttachment* attachment;
		spAtlasRegion* region = newRegion;
		attachment = spMeshAttachment_create(oldAttachmentName);
		attachment->rendererObject = region;
		attachment->regionU = region->u;
		attachment->regionV = region->v;
		attachment->regionU2 = region->u2;
		attachment->regionV2 = region->v2;
		attachment->regionRotate = region->rotate;
		attachment->regionOffsetX = region->offsetX;
		attachment->regionOffsetY = region->offsetY;
		attachment->regionWidth = region->width;
		attachment->regionHeight = region->height;
		attachment->regionOriginalWidth = region->originalWidth;
		attachment->regionOriginalHeight = region->originalHeight;

		spMeshAttachment* oldMeshAttachment = SUB_CAST(spMeshAttachment, oldAttachment);
		attachment->verticesCount = oldMeshAttachment->verticesCount;
		attachment->vertices = MALLOC(float, attachment->verticesCount);
		for (int i = 0; i < attachment->verticesCount; i++)
			attachment->vertices[i] = oldMeshAttachment->vertices[i];

		attachment->trianglesCount = oldMeshAttachment->trianglesCount;
		attachment->triangles = MALLOC(int, attachment->trianglesCount);
		for (int i = 0; i < attachment->trianglesCount; i++)
			attachment->triangles[i] = oldMeshAttachment->triangles[i];

		attachment->regionUvsCount = oldMeshAttachment->regionUvsCount;
		attachment->regionUVs = MALLOC(float, oldMeshAttachment->regionUvsCount);
		for (int i = 0; i < attachment->trianglesCount; i++)
			attachment->regionUVs[i] = oldMeshAttachment->regionUVs[i];
		spMeshAttachment_updateUVs(attachment);

		attachment->r = oldMeshAttachment->r;
		attachment->b = oldMeshAttachment->b;
		attachment->g = oldMeshAttachment->g;
		attachment->a = oldMeshAttachment->a;
		attachment->hullLength = oldMeshAttachment->hullLength;

		attachment->edgesCount = oldMeshAttachment->edgesCount;
		attachment->edges = MALLOC(int, oldMeshAttachment->edgesCount);
		for (int i = 0; i < attachment->edgesCount; i++)
			attachment->edges[i] = oldMeshAttachment->edges[i];
		attachment->width = oldMeshAttachment->width;
		attachment->height = oldMeshAttachment->height;
		return SUPER(attachment);
	}
	case SP_ATTACHMENT_SKINNED_MESH:
	{
		spSkinnedMeshAttachment* attachment;
		spAtlasRegion* region = newRegion;
		attachment = spSkinnedMeshAttachment_create(oldAttachmentName);
		attachment->rendererObject = region;
		attachment->regionU = region->u;
		attachment->regionV = region->v;
		attachment->regionU2 = region->u2;
		attachment->regionV2 = region->v2;
		attachment->regionRotate = region->rotate;
		attachment->regionOffsetX = region->offsetX;
		attachment->regionOffsetY = region->offsetY;
		attachment->regionWidth = region->width;
		attachment->regionHeight = region->height;
		attachment->regionOriginalWidth = region->originalWidth;
		attachment->regionOriginalHeight = region->originalHeight;

		spSkinnedMeshAttachment* oldSkinMeshAttachment = SUB_CAST(spSkinnedMeshAttachment, oldAttachment);
		attachment->uvsCount = oldSkinMeshAttachment->uvsCount;
		attachment->regionUVs = MALLOC(float, attachment->uvsCount);
		for (int i = 0; i < attachment->uvsCount; i++)
		{
			attachment->regionUVs[i] = oldSkinMeshAttachment->regionUVs[i];
		}
		attachment->bonesCount = oldSkinMeshAttachment->bonesCount;
		attachment->weightsCount = oldSkinMeshAttachment->weightsCount;

		attachment->bones = MALLOC(int, attachment->bonesCount);
		attachment->weights = MALLOC(float, attachment->weightsCount);
		for (int i = 0; i < attachment->bonesCount; i++)
		{
			attachment->bones[i] = oldSkinMeshAttachment->bones[i];
		}
		for (int i = 0; i < attachment->weightsCount; i++)
		{
			attachment->weights[i] = oldSkinMeshAttachment->weights[i];
		}

		attachment->trianglesCount = oldSkinMeshAttachment->trianglesCount;
		attachment->triangles = MALLOC(int, attachment->trianglesCount);
		for (int i = 0; i < attachment->trianglesCount; i++)
		{
			attachment->triangles[i] = oldSkinMeshAttachment->triangles[i];
		}
		spSkinnedMeshAttachment_updateUVs(attachment);
		attachment->r = oldSkinMeshAttachment->r;
		attachment->g = oldSkinMeshAttachment->g;
		attachment->b = oldSkinMeshAttachment->b;
		attachment->a = oldSkinMeshAttachment->a;

		attachment->hullLength = oldSkinMeshAttachment->hullLength;

		attachment->edgesCount = oldSkinMeshAttachment->edgesCount;
		attachment->edges = MALLOC(int, attachment->edgesCount);
		for (int i = 0; i < attachment->edgesCount; i++)
		{
			attachment->edges[i] = oldSkinMeshAttachment->edges[i];
		}

		attachment->width = oldSkinMeshAttachment->width;
		attachment->height = oldSkinMeshAttachment->height;


		return SUPER(attachment);
	}
	case SP_ATTACHMENT_BOUNDING_BOX:
	{
		spBoundingBoxAttachment* attachment = spBoundingBoxAttachment_create(oldAttachmentName);
		spBoundingBoxAttachment* oldBoundingBoxAttachment = SUB_CAST(spBoundingBoxAttachment, oldAttachment);
		attachment->verticesCount = oldBoundingBoxAttachment->verticesCount;
		attachment->vertices = MALLOC(float, attachment->verticesCount);
		for (int i = 0; i < attachment->verticesCount; i++)
		{
			attachment->vertices[i] = oldBoundingBoxAttachment->vertices[i];
		}

		return SUPER(attachment);
	}
	default:
		return 0;
	}
}