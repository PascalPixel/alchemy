// Sprites, their animations and the picture caches they draw from. Part of field.c.

#include "field_internal.h"

// A sprite size as one number, width in the high byte and height in the low, to pick its OAM shape
// and size with one switch.
#define SPRITE_SIZE_KEY(width, height) (((width) << 8) + (height))

// The field sprites' shadow size, OAM priority, picture cache limits and basic colours.
enum
{
	ANIM_SHADOW_TILE_BYTES     = 16 * 8, // 16x8 pixels, 256 colours
	// Priority 2, the field's for its sprites: in front of a map layer at 3, or at 2 since a sprite
	// wins a tie, and behind one at 1.
	ANIM_OAM_FIELD_PRIORITY    = OBJ_PRIORITY(2),
	// Rows of SpritePictureCacheSources searched, at most.
	PICTURE_CACHE_SOURCES_MAX  = 256,
	// Picture offsets at the head of a decoded set, at most.
	PICTURE_CACHE_PICTURES_MAX = 256,
	// The colours of palettes 0 to 13, which syPaletteLoadBasicSpriteColors loads; a variant remaps
	// only these.
	BASIC_SPRITE_COLORS        = 14 * PALETTE_COLORS
};

// Loads the pictures of animation `number` into picture cache `slot`, at `buf`: the resource
// SpritePictureCacheSources lists for it, decoded, its picture offsets turned into pointers, and
// for variant 1 to 5 its colours of the basic sprite palettes recoloured through that variant's
// table (a variant above 5 takes variant 1's). Returns the size of one picture in pixels, or 0 when
// nothing was loaded.
Sint32 fdSpritePictureCacheLoad(Uint32 slot, Uint32 *buf, Sint32 number, Uint32 variant)
{
	ANIMATION_METADATA       *metadata;
	ACTOR_SYSTEM_STATE       *system;
	SPR_PICTURE_CACHE        *record;
	SPR_PICTURE_CACHE_SOURCE *entry;
	Uint32                   *offset;
	Uint8                    *pixels;
	Uint8                    *end;
	Uint8                    *colorMap;
	Uint32                    size;
	Uint32                    count;
	Uint32                    index;
	Uint16                    entryNumber;
	Sint32                    resource;

	if (slot >= RESOURCE_SLOT_COUNT)
		return 0;
	system           = ActorSystemStateWork;
	record           = &system->pictureCaches[slot];
	metadata         = syResourceGetMetadataRecordStub(number);
	record->tag      = (slot << PICTURE_CACHE_TAG_SLOT_SHIFT) | number;
	record->pictures = buf;

	// Resolve the animation number to its packed resource.
	entry            = SpritePictureCacheSources;
	for (count = 0; count < PICTURE_CACHE_SOURCES_MAX; count++)
	{
		Sint16 *numbers = &entry->number;

		resource        = entry->resource;
		entryNumber     = *numbers;
		entry++;
		if (entryNumber == 0)
			return 0;
		if (entryNumber == number)
			break;
	}
	size   = syDecompressTaggedLZ(syResourceGetTableEntry(resource), buf);

	// Turn the picture-offset directory into pointers before the pixel payload.
	offset = buf;
	for (count = 0; count < PICTURE_CACHE_PICTURES_MAX; count++)
	{
		if (*offset == 0)
			break;
		*offset += (Uint32)buf;
		offset++;
	}
	if (variant != 0)
	{
		index  = variant - 1;
		pixels = (Uint8 *)(offset + 1);
		end    = (Uint8 *)buf + size;
		if (index > 4)
			index = 0;
		// Colours of palettes 14 and 15 are kept; only the basic palettes' change.
		colorMap = (Uint8 *)SpriteColorTables[index];
		for (; pixels < end; pixels++)
		{
			Uint32 colorIndex = *pixels;

			if (colorIndex < BASIC_SPRITE_COLORS)
			{
				colorIndex = colorMap[colorIndex];
				*pixels    = colorIndex;
			}
		}
	}
	return metadata->width * metadata->height;
}

// Returns the buffer loaded into the sprite picture cache with this tag, or 0.
Sint32 fdSpritePictureCacheFindByTag(Sint32 tag)
{
	Uint32             i;
	SPR_PICTURE_CACHE *slot = ActorSystemStateWork->pictureCaches;

	for (i = 0; i < RESOURCE_SLOT_COUNT; i++, slot++)
	{
		if (slot->tag == tag)
			return (Sint32)slot->pictures;
	}
	return 0;
}

// Resets each sprite animation of a sprite from its animation's metadata: its facing kind,
// pictures, scripts and codec, with no picture shown and no script running.
Sint32 fdSpriteResetAnimations(SPR *sprite)
{
	Sint32 i;

	for (i = 0; i < sprite->count; i++)
	{
		SPR_ANIMATION      *layer    = sprite->entries[i];
		ANIMATION_METADATA *metadata = syResourceGetMetadataRecordStub(layer->animationId);
		void              **pictures;

		if (metadata->width == 0)
			continue;

		if (i == 0)
		{
			// The first sprite animation supplies the dimensions and origin of the whole sprite.
			sprite->width   = metadata->width;
			sprite->height  = metadata->height;
			sprite->scale   = metadata->scale << 8;
			sprite->offsetY = metadata->adjustY;
			sprite->offsetX = metadata->adjustX;
		}

		// An animation whose metadata has no pictures takes them from the resource slot loaded
		// under its id (fdSpritePictureCacheLoad).
		pictures = metadata->pictures;
		if (pictures == 0)
			pictures = (void **)fdSpritePictureCacheFindByTag(layer->animationId);

		layer->kind         = metadata->drawKind;
		layer->pictures     = pictures;
		layer->scripts      = metadata->animation;
		layer->mode         = metadata->pictureCodec;
		// SPRITE_PICTURE_NONE and no script: fdSpriteComposePicture skips the sprite animation
		// until an animation is selected on it.
		layer->picture      = SPRITE_PICTURE_NONE;
		layer->script       = 0;
		layer->scriptOffset = 0;
	}

	return 0;
}

// Resets one sprite animation from its animation's metadata, as fdSpriteResetAnimations does for
// each sprite animation.
void fdSpriteAnimationReset(SPR_ANIMATION *layer)
{
	void              **pictures;
	ANIMATION_METADATA *metadata;

	if (layer != NULL)
	{
		metadata = syResourceGetMetadataRecordStub(layer->animationId);
		if (metadata->width != 0)
		{
			// Keep the sprite animation inactive until an animation script is selected.
			pictures = metadata->pictures;
			if (pictures == 0)
				pictures = (void **)fdSpritePictureCacheFindByTag(layer->animationId);
			layer->kind         = metadata->drawKind;
			layer->scripts      = metadata->animation;
			layer->pictures     = pictures;
			layer->mode         = metadata->pictureCodec;
			layer->picture      = SPRITE_PICTURE_NONE;
			layer->script       = NULL;
			layer->scriptOffset = 0;
		}
	}
}

// Adds a sprite animation of animation `id` to a sprite, in the first of its four slots that is
// free. Returns the new sprite animation, 0 when none is free, or -1 when the sprite's four slots
// are taken.
Sint32 fdSpriteAddAnimation(SPR *sprite, Sint32 id)
{
	Sint32              layerHandle = (Sint32)sprite->entries[0];
	Sint32              freeIndex   = 0;
	SPR_ANIMATION     **entry;
	ANIMATION_METADATA *metadata;

	// Find an unused sprite animation slot before reserving a sprite animation.
	if (layerHandle != 0)
	{
		entry = &sprite->entries[0];
		do
		{
			freeIndex++;
			if (freeIndex >= ANIM_LAYER_COUNT)
				break;
			entry++;
			layerHandle = (Sint32)*entry;
		} while (layerHandle != 0);
	}
	if (freeIndex == ANIM_LAYER_COUNT)
		return -1;
	layerHandle = (Sint32)fdSpriteAnimationAllocate(id);
	if (layerHandle == 0)
		return 0;
	sprite->entries[freeIndex] = (SPR_ANIMATION *)layerHandle;
	metadata                   = syResourceGetMetadataRecordStub(id);
	// Only the first registered sprite animation establishes the sprite's geometry.
	if (sprite->count == 0)
	{
		sprite->width   = metadata->width;
		sprite->height  = metadata->height;
		sprite->scale   = metadata->scale << 8;
		sprite->offsetY = metadata->adjustY;
		sprite->offsetX = metadata->adjustX;
	}
	if (freeIndex == sprite->count)
		sprite->count = freeIndex + 1;
	return layerHandle;
}

// Removes the sprite animation handle from a sprite and clears it, trimming the sprite's animation
// count when no later sprite animation remains.
void fdSpriteRemoveAnimation(SPR *sprite, Sint32 layer)
{
	Sint32 laterLayers;
	Uint32 index;
	Uint32 i;

	if (sprite != NULL && layer != 0)
	{
		fdSpriteAnimationClear((SPR_ANIMATION *)layer);
		// Locate the handle, then trim only empty slots at the end of the list.
		for (index = 0; index < ANIM_LAYER_COUNT; index++)
		{
			if (layer == (Sint32)sprite->entries[index])
				break;
		}
		if (index != ANIM_LAYER_COUNT)
		{
			sprite->entries[index] = 0;
			laterLayers            = 0;
			for (i = index + 1; i < ANIM_LAYER_COUNT; i++)
			{
				if (sprite->entries[i] != 0)
					laterLayers++;
			}
			if (laterLayers == 0)
				sprite->count = index;
		}
	}
}

// Removes and clears a sprite's sprite animation `index` (0 to 3), trimming the sprite's animation
// count when no later sprite animation remains.
void fdSpriteRemoveAnimationAt(SPR *sprite, Uint32 index)
{
	SPR_ANIMATION *layer;
	Sint32         laterLayers;
	Uint32         i;

	if (sprite != NULL && index < ANIM_LAYER_COUNT)
	{
		layer = sprite->entries[index];
		if (layer != NULL)
		{
			fdSpriteAnimationClear(layer);
			sprite->entries[index] = NULL;
			laterLayers            = 0;
			// Interior holes stay available; only trailing holes reduce the count.
			for (i = index + 1; i < ANIM_LAYER_COUNT; i++)
			{
				if (sprite->entries[i] != NULL)
					laterLayers++;
			}
			if (laterLayers == 0)
				sprite->count = index;
		}
	}
}

// Starts animation `animation` on one sprite animation, when its set has that many;
// SPRITE_ANIMATION_KEEP_PLACE keeps its place and timer.
void fdSpriteAnimationStart(SPR_ANIMATION *layer, Sint32 animation)
{
	Sint32              keepPlace;
	ANIMATION_METADATA *metadata;
	Sint32              scriptAddr;

	// Unlike fdSpriteStartAnimation it leaves SPRITE_ANIMATION_KEEP_PLACE in the index, so with
	// that bit set the animation starts only in a set of more than 128 animations.
	keepPlace = SPRITE_ANIMATION_KEEP_PLACE & animation;
	if (layer->scripts != 0)
	{
		metadata = syResourceGetMetadataRecordStub(layer->animationId);
		if (animation < metadata->animationCount)
		{
			scriptAddr    = (Sint32)layer->scripts[animation];
			layer->kind   = metadata->drawKind;
			layer->script = (Uint8 *)scriptAddr;
			layer->step   = SPRITE_ANIMATION_NORMAL_STEP;
			if (keepPlace == 0)
			{
				layer->scriptOffset = keepPlace;
				layer->timer        = keepPlace;
			}
		}
	}
}

// Starts `animation` (its SPRITE_ANIMATION_INDEX_MASK bits) on every sprite animation of a sprite,
// unless the sprite plays it already; SPRITE_ANIMATION_KEEP_PLACE keeps each one's place and timer.
Sint32 fdSpriteStartAnimation(SPR *sprite, Sint32 animation)
{
	Sint32 keepPlace;
	Sint32 i;

	keepPlace  = animation & SPRITE_ANIMATION_KEEP_PLACE;
	animation &= SPRITE_ANIMATION_INDEX_MASK;

	if (sprite->lastNo != animation)
	{
		i = 0;
		goto test_entry;
	entry_loop:
		// Only the first sprite animation supplies the sprite's offsets.
		{
			SPR_ANIMATION      *layer = sprite->entries[i];
			ANIMATION_METADATA *metadata;
			Sint32              selectedAnimation;

			if (layer != 0 && layer->scripts != 0)
			{
				metadata = syResourceGetMetadataRecordStub(layer->animationId);
				if (animation < metadata->animationCount)
				{
					selectedAnimation = (Sint32)layer->scripts[animation];
					layer->kind       = metadata->drawKind;
					layer->script     = (Uint8 *)selectedAnimation;
					layer->step       = SPRITE_ANIMATION_NORMAL_STEP;
					if (keepPlace == 0)
					{
						layer->scriptOffset = 0;
						layer->timer        = 0;
					}
					if (i == 0)
					{
						sprite->offsetY = metadata->adjustY;
						sprite->offsetX = metadata->adjustX;
					}
				}
			}
		}
		i++;
	test_entry:
		if (i < sprite->count)
			goto entry_loop;
		sprite->lastNo = animation;
	}
	return 0;
}

// Sets each playing sprite animation's timer to the given duration in ticks. Returns 0.
Sint32 fdSpriteSetAnimationTimers(SPR *sprite, Sint32 ticks)
{
	Uint8           count = sprite->count;
	SPR_ANIMATION **layerList;
	Sint32          remaining;

	if (count != 0)
	{
		// The sprite animation timer counts sixteenths of a tick.
		ticks     <<= 4;
		layerList   = sprite->entries;
		remaining   = count;
		do
		{
			SPR_ANIMATION *layer = *layerList++;
			if (layer != 0 && layer->scripts != 0)
				layer->timer = ticks;
			remaining--;
		} while (remaining != 0);
	}
	return 0;
}

// Sets the timer step, the animation speed, of each playing sprite animation of a sprite (16 is
// normal speed).
void fdSpriteSetAnimationSpeed(SPR *sprite, Sint32 step)
{
	Uint8 count = sprite->count;
	if (count != 0)
	{
		SPR_ANIMATION **layerList = sprite->entries;
		Sint32          remaining = count;
		do
		{
			SPR_ANIMATION *layer = *layerList++;
			if (layer != 0 && layer->scripts != 0)
				layer->step = step;
			remaining--;
		} while (remaining != 0);
	}
}

// Allocates and zeroes the 64 sprites and 64 sprite animations (HEAP_SLOT_SPRITES and
// HEAP_SLOT_SPRITE_ANIMATIONS, from EWRAM first in ACTOR_SYSTEM_WORLD_MAP and IWRAM first
// otherwise), loads the basic sprite palettes, loads the shadow tiles into
// VRAM_CACHE_ENTRY_ACTOR_SHADOW and puts the sprite tile uploader in RAM.
void fdActorSystemConfigure(Sint32 mode)
{
	void           *sprites;
	void           *layers;
	volatile Uint32 zero;

	// On the world map (ACTOR_SYSTEM_WORLD_MAP) the two pools are taken from EWRAM first; elsewhere
	// from IWRAM first, though a battle has already taken both from EWRAM.
	if (mode == ACTOR_SYSTEM_WORLD_MAP)
	{
		sprites = syHeapSlotAllocateEWRAM(HEAP_SLOT_SPRITES, ANIM_OBJECT_COUNT * sizeof(SPR));
		layers  = syHeapSlotAllocateEWRAM(HEAP_SLOT_SPRITE_ANIMATIONS,
                                         ANIM_OBJECT_COUNT * sizeof(SPR_ANIMATION));
	}
	else
	{
		sprites = syHeapSlotAllocate(HEAP_SLOT_SPRITES, ANIM_OBJECT_COUNT * sizeof(SPR));
		layers  = syHeapSlotAllocate(HEAP_SLOT_SPRITE_ANIMATIONS,
                                    ANIM_OBJECT_COUNT * sizeof(SPR_ANIMATION));
	}
	// Load the sprites' basic colours, clear both pools, then load the shared shadow (128 bytes:
	// the two 8x8 tiles of a 16x8, 256-colour sprite) and the tile uploader.
	syPaletteLoadBasicSpriteColors();
	DMA_FILL32_CELL(zero, 0, sprites, ANIM_OBJECT_COUNT * sizeof(SPR));
	DMA_FILL32_CELL(zero, 0, layers, ANIM_OBJECT_COUNT * sizeof(SPR_ANIMATION));
	syVRAMCacheLoad(VRAM_CACHE_ENTRY_ACTOR_SHADOW, ANIM_SHADOW_TILE_BYTES, ActorShadowTiles);
	RAM_CODE_LOAD_SLOT(HEAP_SLOT_PICTURE_UPLOADER, fdTileCopyStrided, fdTileCopyStridedCodeSize);
}

// Takes a free sprite animation and starts it on the first animation of animation `id`; NULL when
// none is free.
SPR_ANIMATION *fdSpriteAnimationAllocate(Sint32 id)
{
	ANIMATION_METADATA *metadata;
	SPR_ANIMATION      *candidate;
	SPR_ANIMATION      *found;
	SPR_ANIMATION      *layer;
	Sint32              i;
	void              **pictures;
	Uint8             **scripts;

	found     = NULL;
	metadata  = syResourceGetMetadataRecordStub(id);
	candidate = SpriteAnimations[0];
	layer     = NULL;

	if (metadata->width != 0)
	{
		// A zero facing kind marks a free sprite animation.
		for (i = 0; i < ANIM_OBJECT_COUNT; i++, candidate++)
		{
			if (candidate->kind == 0)
			{
				found = candidate;
				break;
			}
		}
		if (found != NULL)
		{
			// Bind the metadata and start its first animation at command zero. UB: the word view
			// aliases a pointer field.
#if FIX_TYPE_PUNNING
			pictures = metadata->pictures;
#else
			pictures = (void **)*(Sint32 *)&metadata->pictures;
#endif
			layer              = found;
			layer->animationId = id;
			if (pictures == 0)
				pictures = (void **)fdSpritePictureCacheFindByTag(id);

			scripts             = metadata->animation;
			layer->scripts      = scripts;
			layer->pictures     = pictures;
			layer->mode         = metadata->pictureCodec;
			layer->picture      = SPRITE_PICTURE_NONE;
			layer->script       = scripts[0];
			layer->scriptOffset = 0;
			layer->kind         = metadata->drawKind;
			layer->colorMode    = 0;
		}
	}
	return layer;
}

// Zeroes one sprite animation (SPR_ANIMATION); does nothing for NULL. Zeroing kind frees the sprite
// animation for fdSpriteAnimationAllocate.
void fdSpriteAnimationClear(SPR_ANIMATION *record)
{
	if (record != 0)
	{
		DMA_FILL32(0, record, sizeof(*record));
	}
}

// Creating and releasing animated sprites, and measuring animation lengths.

// Creates a sprite for animation `id`, taking one of the 64 free sprites and a VRAM cache entry for
// its tiles.
SPR *fdSpriteCreate(Sint32 id)
{
	ANIMATION_METADATA *metadata;
	SPR                *sprite;
	SPR                *freeSprite;
#if FIX_TYPE_PUNNING
	Uint32 attributes[2 * sizeof(ANIMATION_SPR_PART) / sizeof(Uint32)];
#else
	Uint32 *attribute;
#endif
	Sint32 slot;
	Sint32 tile;
	Sint32 i;
	Uint32 shape;

	freeSprite = NULL;
	metadata   = syResourceGetMetadataRecordStub(id);
	slot       = syVRAMCacheFindFreeEntry();
	sprite     = SpritesWork;
	if (metadata->width == 0)
		return NULL;
	// A width of 0 marks a free sprite.
	for (i = 0; i < ANIM_OBJECT_COUNT; i++, sprite++)
	{
		if (sprite->width == 0)
		{
			freeSprite = sprite;
			break;
		}
	}
	if (freeSprite == NULL)
		return NULL;
	if (slot == VRAM_CACHE_ENTRY_COUNT)
		return NULL;
	// Claim the cache entry with no VRAM yet (size 0): the first fdSpriteComposePicture that draws
	// the sprite gives it width * height bytes.
	tile = syVRAMCacheLoad(slot, 0, 0);
	if (tile == 0)
		return NULL;
	freeSprite->slot     = slot;
	freeSprite->rotation = 0;
	freeSprite->flags    = SPRITE_FLAG_SHADOW;

	// The OAM shape and size bits (attributes 0 and 1) for the image's width and height in pixels.
	switch ((Uint32)SPRITE_SIZE_KEY(metadata->width, metadata->height))
	{
		case SPRITE_SIZE_KEY(8, 8)  : shape = OBJ_SIZE_8X8; break;
		case SPRITE_SIZE_KEY(8, 16) : shape = OBJ_SIZE_8X16; break;
		case SPRITE_SIZE_KEY(16, 8) : shape = OBJ_SIZE_16X8; break;
		case SPRITE_SIZE_KEY(16, 16): shape = OBJ_SIZE_16X16; break;
		case SPRITE_SIZE_KEY(16, 32): shape = OBJ_SIZE_16X32; break;
		case SPRITE_SIZE_KEY(32, 16): shape = OBJ_SIZE_32X16; break;
		case SPRITE_SIZE_KEY(32, 32): shape = OBJ_SIZE_32X32; break;
		case SPRITE_SIZE_KEY(32, 64): shape = OBJ_SIZE_32X64; break;
		case SPRITE_SIZE_KEY(64, 32): shape = OBJ_SIZE_64X32; break;
		case SPRITE_SIZE_KEY(64, 64): shape = OBJ_SIZE_64X64; break;
		default                     : shape = 0; break;
	}

		// Part 0 is the image; part 1 is its shadow, from the shared shadow tiles. UB: the word
		// cursor aliases the sprite and crosses its members. UBFIX copies the same representation
		// from an actual word array.
#if FIX_TYPE_PUNNING
	attributes[0] = 0;
	attributes[1] = shape | OBJ_256_COLORS;
	attributes[2] = tile | ANIM_OAM_FIELD_PRIORITY;
	attributes[3] = 0;
	attributes[4] = OBJ_SIZE_16X8 | OBJ_256_COLORS;
	attributes[5] =
		(VramCacheEntries[VRAM_CACHE_ENTRY_ACTOR_SHADOW].offset >> 5) | ANIM_OAM_FIELD_PRIORITY;
	memcpy(freeSprite, attributes, sizeof(attributes));
#else
	attribute    = (Uint32 *)freeSprite;
	*attribute++ = 0;
	*attribute++ = shape | OBJ_256_COLORS;
	*attribute++ = tile | ANIM_OAM_FIELD_PRIORITY;
	*attribute++ = 0;
	*attribute++ = OBJ_SIZE_16X8 | OBJ_256_COLORS;
	*attribute =
		(VramCacheEntries[VRAM_CACHE_ENTRY_ACTOR_SHADOW].offset >> 5) | ANIM_OAM_FIELD_PRIORITY;
#endif
	// DEAD: a test of the call's result for -1 that does nothing either way; the ROM keeps the -1
	// it builds after the call.
	if (fdSpriteAddAnimation(sprite, id) == -1)
	{
		// FAKEMATCH(dead): a store of the byte it reloads.
		freeSprite->dirty = freeSprite->dirty;
	}
	return freeSprite;
}

// Releases a sprite: its VRAM cache entry (unless retainCacheEntry is set), its four sprite
// animations, and then zeroes its 56 bytes.
void fdSpriteDestroy(SPR *sprite)
{
	Sint32 i;

	if (sprite)
	{
		// Release its VRAM cache entry and sprite animations before clearing the sprite itself.
		if (!sprite->retainCacheEntry)
			syVRAMCacheFreeEntry(sprite->slot);
		for (i = 0; i < ANIM_LAYER_COUNT; i++)
			fdSpriteAnimationClear(sprite->entries[i]);
		DMA_FILL32(0, sprite, sizeof(*sprite));
	}
}

// Returns how long, in ticks, the first `timedCount` timed commands of animation `animation` of
// animation `id` last: the sum of the arguments of its picture, wait and blank commands, up to
// timedCount of them or the first play, hold, jump or end command.
Sint32 fdSpriteAnimationSumCommandLengths(Sint32 id, Uint32 animation, Sint32 timedCount)
{
	ANIMATION_METADATA *metadata;
	Uint8              *script;
	Uint8               command;
	Uint8               duration;
	Sint32              durationSum = 0;

	metadata                        = syResourceGetMetadataRecordStub(id);
	if (animation >= metadata->animationCount)
	{
		return 0;
	}
	script = ((Uint8 **)metadata->animation)[animation];
	// Only timed commands count; control-flow commands terminate this scan.
	for (;;)
	{
		command   = script[0];
		duration  = script[1];
		script   += 2;
		if (command == ANIMCMD_PLAY || command == ANIMCMD_HOLD || command == ANIMCMD_JUMP
		    || command == ANIMCMD_END)
		{
			break;
		}
		if (command == ANIMCMD_WAIT || command == ANIMCMD_BLANK || command < ANIMCMD_END)
		{
			durationSum += duration;
			timedCount--;
			if (timedCount == 0)
			{
				break;
			}
		}
	}
	return durationSum;
}
