/* Sprites, their animations and the picture caches they draw from. Part of
   field.c. */

/* A sprite size as one number, width in the high byte and height in the low, to
   pick its OAM shape and size with one switch. */
#define SPRITE_SIZE_KEY(width, height) (((width) << 8) + (height))

/* Sizes and limits of the field's sprite pictures. */
enum {
	ANIM_SHADOW_TILE_BYTES = 16 * 8, /* 16x8 pixels, 256 colours */
	/* Priority 2, the field's for its sprites: in front of a map layer at 3, or
	   at 2 since a sprite wins a tie, and behind one at 1. */
	ANIM_OAM_FIELD_PRIORITY = OBJ_PRIORITY(2),
	/* A picture cache's tag holds its slot above the animation id's bits. */
	PICTURE_CACHE_TAG_SLOT_SHIFT = 12,
	/* Rows of SpritePictureCache_Sources searched, at most. */
	PICTURE_CACHE_SOURCES_MAX = 256,
	/* Picture offsets at the head of a decoded set, at most. */
	PICTURE_CACHE_PICTURES_MAX = 256,
	/* The colours of palettes 0 to 13, which syPaletteLoadBasicSprCols loads; a
	   variant remaps only these. */
	BASIC_SPRITE_COLORS = 14 * 16
};

#undef fdSprAnimationAlloc

/* Loads the pictures of animation id number into picture cache slot's buffer:
   the resource listed for it in SpritePictureCache_Sources, decoded, its
   picture offsets turned into pointers, and for variant 1-5 its pixel colours
   (those up to 223, the basic sprite palettes 0 to 13) remapped through that
   variant's conversion table; a variant above 5 uses variant 1's. */
Sint32 fdSprPictureCacheLoad(
	Uint32 slot, Uint32 *buf, Sint32 num, Uint32 variant)
{
	ANIMATION_METADATA		 *metadata;
	ACTOR_SYSTEM_STATE		 *system;
	SPR_PICTURE_CACHE		 *cache;
	SPR_PICTURE_CACHE_SOURCE *entry;
	Uint32					 *ofs;
	Uint8					 *pixel;
	Uint8					 *end;
	Uint8					 *tbl;
	Uint32					  size;
	Uint32					  cnt;
	Uint32					  idx;
	Uint16					  entryNum;
	Sint32					  resource;

	if(slot >= SPRITE_PICTURE_CACHE_COUNT)
		return 0;
	system = Ram_HeapSlots->actorSystem;
	cache = &system->pictureCaches[slot];
	metadata = fdResourceGetMetadataRecordStub(num);
	cache->tag = (slot << PICTURE_CACHE_TAG_SLOT_SHIFT) | num;
	cache->pictures = buf;

	entry = SpritePictureCache_Sources;
	for(cnt = 0; cnt < PICTURE_CACHE_SOURCES_MAX; cnt++) {
		Sint16 *numbers = &entry->num;

		resource = entry->resource;
		entryNum = *numbers;
		entry++;
		if(entryNum == 0)
			return 0;
		if(entryNum == num)
			break;
	}
	size = syDecompressTaggedLZ(syResourceGetTableEntry(resource), buf);

	ofs = buf;
	for(cnt = 0; cnt < PICTURE_CACHE_PICTURES_MAX; cnt++) {
		if(*ofs == 0)
			break;
		*ofs += (Uint32)buf;
		ofs++;
	}
	if(variant != 0) {
		idx = variant - 1;
		pixel = (Uint8 *)(ofs + 1);
		end = (Uint8 *)buf + size;
		if(idx > 4)
			idx = 0;
		tbl = SpriteColorTables[idx];
		for(; pixel < end; pixel++) {
			Uint32 c = *pixel;

			if(c < BASIC_SPRITE_COLORS) {
				c = tbl[c];
				*pixel = c;
			}
		}
	}
	return metadata->width * metadata->height;
}

/* Returns the pictures loaded into the sprite picture cache with this tag, or
   0. */
Sint32 fdSprPictureCacheFindByTag(Sint32 tag)
{
	ACTOR_SYSTEM_STATE *system = Ram_HeapSlots->actorSystem;
	Uint32				i;
	SPR_PICTURE_CACHE  *cache;

	i = 0;
	for(cache = system->pictureCaches; i < SPRITE_PICTURE_CACHE_COUNT;
		i++, cache++) {
		if(cache->tag == tag)
			return (Sint32)cache->pictures;
	}
	return 0;
}

/* Resets each sprite animation of a sprite from its animation's metadata: its
   facing kind, pictures, scripts and codec, with no picture shown and no script
   running. */
Sint32 fdSprResetAnimations(SPR *spr)
{
	Sint32			idx;
	SPR_ANIMATION **slot;

	idx = 0;
	if(idx < spr->cnt) {
		slot = spr->entries;
		for(; idx < spr->cnt; idx++) {
			SPR_ANIMATION	   *sprAnm = *slot++;
			ANIMATION_METADATA *metadata =
				fdResourceGetMetadataRecordStub(sprAnm->anmId);
			void  **pictures;
			Uint8 **anm;

			if(metadata->width == 0)
				continue;

			if(idx == 0) {
				spr->width = metadata->width;
				spr->height = metadata->height;
				spr->scale = metadata->scale << 8;
				spr->ofsY = metadata->adjustY;
				spr->ofsX = metadata->adjustX;
			}

			pictures = metadata->pictures;
			if(pictures == 0)
				pictures = (void **)fdSprPictureCacheFindByTag(sprAnm->anmId);

			sprAnm->kind = metadata->drawKind;
			anm = metadata->anm;
			/* UB: the pictures are stored through a Uint8 ** view of the field,
			   the scripts' type. */
#if FIX_TYPE_PUNNING
			sprAnm->pictures = pictures;
#else
			*(Uint8 ***)&sprAnm->pictures = (Uint8 **)pictures;
#endif
			sprAnm->scripts = anm;
			sprAnm->mode = metadata->pictureCodec;
			sprAnm->picture = SPRITE_PICTURE_NONE;
			sprAnm->script = 0;
			sprAnm->scriptOfs = 0;
		}
	}

	return 0;
}

/* Resets a sprite animation from its metadata: its pictures, scripts, codec and
   drawing kind, with no script running and no picture shown. */
void fdSprAnimationReset(SPR_ANIMATION *sprAnm)
{
	void			  **pictures;
	Uint8			  **scripts;
	ANIMATION_METADATA *metadata;

	if(sprAnm != NULL) {
		metadata = fdResourceGetMetadataRecordStub(sprAnm->anmId);
		if(metadata->width != 0) {
			pictures = metadata->pictures;
			if(pictures == 0)
				pictures = (void **)fdSprPictureCacheFindByTag(sprAnm->anmId);
			sprAnm->kind = metadata->drawKind;
			syIOWrite32(0, (Uint32 *)&sprAnm->script);
			scripts = metadata->anm;
			sprAnm->pictures = pictures;
			sprAnm->scripts = scripts;
			sprAnm->scriptOfs = 0;
			sprAnm->mode = metadata->pictureCodec;
			sprAnm->picture = SPRITE_PICTURE_NONE;
		}
	}
}

/* Adds a sprite animation of animation id to a sprite, in the first of its four
   slots that is free; returns it, 0 when none could be taken, or -1 when all
   four are in use. */
Sint32 fdSprAddAnimation(SPR *spr, Sint32 id)
{
	Sint32				sprAnm = (Sint32)spr->entries[0];
	Sint32				freeIdx = 0;
	SPR_ANIMATION	  **entry;
	ANIMATION_METADATA *metadata;

	if(sprAnm != 0) {
		entry = &spr->entries[0];
		do {
			freeIdx++;
			if(freeIdx >= SPRITE_ANIMATION_COUNT)
				break;
			entry++;
			sprAnm = (Sint32)*entry;
		} while(sprAnm != 0);
	}
	if(freeIdx == SPRITE_ANIMATION_COUNT)
		return -1;
	sprAnm = (Sint32)fdSprAnimationAlloc(id);
	if(sprAnm == 0)
		return 0;
	spr->entries[freeIdx] = (SPR_ANIMATION *)sprAnm;
	metadata = fdResourceGetMetadataRecordStub(id);
	/* Only the first registered sprite animation establishes the sprite's
	   geometry. */
	if(spr->cnt == 0) {
		spr->width = metadata->width;
		spr->height = metadata->height;
		spr->scale = metadata->scale << 8;
		spr->ofsY = metadata->adjustY;
		spr->ofsX = metadata->adjustX;
	}
	if(freeIdx == spr->cnt)
		spr->cnt = freeIdx + 1;
	return sprAnm;
}

/* Removes the sprite animation handle from a sprite and clears it, trimming the
   sprite's animation count when no later sprite animation remains. */
void fdSprRemoveAnimation(SPR *spr, Sint32 sprAnm)
{
	Sint32 laterAnms;
	Uint32 idx;
	Uint32 i;

	if(spr != NULL && sprAnm != 0) {
		fdSprAnimationClear((SPR_ANIMATION *)sprAnm);
		/* Locate the handle, then trim only empty slots at the end of the
		   list. */
		for(idx = 0; idx < SPRITE_ANIMATION_COUNT; idx++) {
			if(sprAnm == (Sint32)spr->entries[idx])
				break;
		}
		if(idx != SPRITE_ANIMATION_COUNT) {
			spr->entries[idx] = 0;
			laterAnms = 0;
			for(i = idx + 1; i < SPRITE_ANIMATION_COUNT; i++) {
				if(spr->entries[i] != 0)
					laterAnms++;
			}
			if(laterAnms == 0)
				spr->cnt = idx;
		}
	}
}

/* Removes and clears the sprite animation in a sprite's slot index (0 to 3),
   trimming the sprite's animation count when no later sprite animation
   remains. */
void fdSprRemoveAnimationAt(SPR *spr, Uint32 idx)
{
	SPR_ANIMATION *sprAnm;
	Sint32		   laterAnms;
	Uint32		   i;

	if(spr != NULL && idx < SPRITE_ANIMATION_COUNT) {
		sprAnm = spr->entries[idx];
		if(sprAnm != NULL) {
			fdSprAnimationClear(sprAnm);
			spr->entries[idx] = NULL;
			laterAnms = 0;
			/* Interior holes stay available; only trailing holes reduce the
			   count. */
			for(i = idx + 1; i < SPRITE_ANIMATION_COUNT; i++) {
				if(spr->entries[i] != NULL)
					laterAnms++;
			}
			if(laterAnms == 0)
				spr->cnt = idx;
		}
	}
}

/* Starts the indexed animation on a sprite animation when its metadata contains
   it. Unlike fdSprStartAnimation it does not mask SPRITE_ANIMATION_KEEP_PLACE
   off, so with it set the index would pass anmCnt only in a set of more than
   128 animations. */
void fdSprAnimationStart(SPR_ANIMATION *sprAnm, Sint32 anm)
{
	Sint32				keepPlace;
	ANIMATION_METADATA *metadata;
	Sint32				scriptAdr;

	keepPlace = SPRITE_ANIMATION_KEEP_PLACE & anm;
	if(sprAnm->scripts != 0) {
		metadata = fdResourceGetMetadataRecordStub(sprAnm->anmId);
		if(anm < metadata->anmCnt) {
			scriptAdr = (Sint32)sprAnm->scripts[anm];
			sprAnm->kind = metadata->drawKind;
			sprAnm->script = (Uint8 *)scriptAdr;
			sprAnm->step = SPRITE_ANIMATION_NORMAL_STEP;
			if(keepPlace == 0) {
				sprAnm->scriptOfs = keepPlace;
				sprAnm->tmr = keepPlace;
			}
		}
	}
}

/* Starts an animation on every sprite animation of a sprite
   (SPRITE_ANIMATION_KEEP_PLACE keeps each one's place and timer), unless the
   sprite plays it already. */
Sint32 fdSprStartAnimation(SPR *spr, Sint32 anm)
{
	Sint32 keepPlace;
	Sint32 i;

	keepPlace = anm & SPRITE_ANIMATION_KEEP_PLACE;
	anm &= SPRITE_ANIMATION_INDEX_MASK;

	if(spr->lastNo != anm) {
		i = 0;
		goto test_entry;
	entry_loop:
		/* Only the first sprite animation supplies the sprite's offsets. */
		{
			SPR_ANIMATION	   *sprAnm = spr->entries[i];
			ANIMATION_METADATA *metadata;
			Sint32				selectedAnm;

			if(sprAnm != 0 && sprAnm->scripts != 0) {
				metadata = fdResourceGetMetadataRecordStub(sprAnm->anmId);
				if(anm < metadata->anmCnt) {
					selectedAnm = (Sint32)sprAnm->scripts[anm];
					sprAnm->kind = metadata->drawKind;
					sprAnm->script = (Uint8 *)selectedAnm;
					sprAnm->step = SPRITE_ANIMATION_NORMAL_STEP;
					if(keepPlace == 0) {
						sprAnm->scriptOfs = 0;
						sprAnm->tmr = 0;
					}
					if(i == 0) {
						spr->ofsY = metadata->adjustY;
						spr->ofsX = metadata->adjustX;
					}
				}
			}
		}
		i++;
	test_entry:
		if(i < spr->cnt)
			goto entry_loop;
		spr->lastNo = anm;
	}
	return 0;
}

/* Sets each playing sprite animation's timer to the given duration in ticks.
   Returns 0. */
Sint32 fdSprSetAnimationTimers(SPR *spr, Sint32 ticks)
{
	Sint32 i;

	for(i = 0; i < spr->cnt; i++) {
		SPR_ANIMATION *sprAnm = spr->entries[i];

		/* The sprite animation timer counts sixteenths of a tick. */
		if(sprAnm != NULL && sprAnm->scripts != NULL) {
			sprAnm->tmr = ticks << 4;
		}
	}
	return 0;
}

/* Sets the timer step, the animation speed, of each playing sprite animation of
   a sprite (16 is normal speed). */
void fdSprSetAnimationSpeed(SPR *spr, Sint32 step)
{
	Sint32 i;

	for(i = 0; i < spr->cnt; i++) {
		SPR_ANIMATION *sprAnm = spr->entries[i];

		if(sprAnm != NULL && sprAnm->scripts != NULL)
			sprAnm->step = step;
	}
}

/* Takes the sprite and sprite animation pools (from EWRAM first on the World
   Map, ACTOR_SYSTEM_WORLD_MAP, and IWRAM first otherwise) and clears them,
   loads the shadow tiles into VRAM cache entry VRAM_CACHE_ENTRY_ACTOR_SHADOW
   and copies the strided tile copier into RAM. */
void fdActorSystemConfigure(Sint32 mode)
{
	void		   *sprs;
	void		   *sprAnms;
	volatile Uint32 zero;

	if(mode == ACTOR_SYSTEM_WORLD_MAP) {
		sprs = syHeapSlotAllocEWRAM(
			HEAP_SLOT_SPRITES, ANIM_OBJECT_COUNT * sizeof(SPR));
		sprAnms = syHeapSlotAllocEWRAM(HEAP_SLOT_SPRITE_ANIMATIONS,
			ANIM_OBJECT_COUNT * sizeof(SPR_ANIMATION));
	} else {
		sprs =
			syHeapSlotAlloc(HEAP_SLOT_SPRITES, ANIM_OBJECT_COUNT * sizeof(SPR));
		sprAnms = syHeapSlotAlloc(HEAP_SLOT_SPRITE_ANIMATIONS,
			ANIM_OBJECT_COUNT * sizeof(SPR_ANIMATION));
	}
	syPaletteLoadBasicSprCols();
	DMA_FILL32_CELL(zero, 0, sprs, ANIM_OBJECT_COUNT * sizeof(SPR));
	DMA_FILL32_CELL(
		zero, 0, sprAnms, ANIM_OBJECT_COUNT * sizeof(SPR_ANIMATION));
	syVRAMCacheLoad(VRAM_CACHE_ENTRY_ACTOR_SHADOW, ANIM_SHADOW_TILE_BYTES,
		Actor_ShadowTiles);
	RAM_CODE_LOAD_SLOT(HEAP_SLOT_PICTURE_UPLOADER, fdRenderCopyStridedCode,
		fdRenderCopyStridedCodeSize);
}

/* Frees the actors' shadow tiles (VRAM_CACHE_ENTRY_ACTOR_SHADOW). */
void fdActorShadowFreeTiles(void)
{
	syVRAMCacheFreeEntry(VRAM_CACHE_ENTRY_ACTOR_SHADOW);
}

/* Loads the actors' shadow tiles into VRAM_CACHE_ENTRY_ACTOR_SHADOW. */
void fdActorShadowLoadTiles(void)
{
	const Uint8 *data = Actor_ShadowTiles;
	Sint32		 size = ANIM_SHADOW_TILE_BYTES;

	syVRAMCacheLoad(VRAM_CACHE_ENTRY_ACTOR_SHADOW, size, data);
}

/* Takes a free sprite animation and starts it on animation id's first
   animation. */
SPR_ANIMATION *fdSprAnimationAlloc(Sint32 id)
{
	ANIMATION_METADATA *metadata;
	SPR_ANIMATION	   *entry;
	SPR_ANIMATION	   *found;
	SPR_ANIMATION	   *sprAnm;
	Sint32				i;
	void			  **pictures;
	Uint8			  **anm;

	found = NULL;
	metadata = fdResourceGetMetadataRecordStub(id);
	entry = Ram_HeapSlots->sprAnms;
	sprAnm = NULL;

	if(metadata->width != 0) {
		for(i = 0; i < ANIM_OBJECT_COUNT; i++, entry++) {
			if(entry->kind == 0) {
				found = entry;
				break;
			}
		}
		if(found != NULL) {
			/* UB: the word view aliases a pointer field. */
#if FIX_TYPE_PUNNING
			pictures = metadata->pictures;
#else
			pictures = (void **)*(Sint32 *)&metadata->pictures;
#endif
			sprAnm = found;
			sprAnm->anmId = id;
			if(pictures == 0)
				pictures = (void **)fdSprPictureCacheFindByTag(id);

			anm = metadata->anm;
			sprAnm->scripts = anm;
			sprAnm->pictures = pictures;
			sprAnm->mode = metadata->pictureCodec;
			sprAnm->picture = SPRITE_PICTURE_NONE;
			sprAnm->script = anm[0];
			sprAnm->scriptOfs = 0;
			sprAnm->kind = metadata->drawKind;
			sprAnm->colMode = 0;
		}
	}
	return sprAnm;
}

/* Zeroes one sprite animation (SPR_ANIMATION); does nothing for NULL. Zeroing
   kind frees the sprite animation for fdSprAnimationAlloc. */
void fdSprAnimationClear(void *record)
{
	if(record != 0) {
		DMA_FILL32(0, record, sizeof(SPR_ANIMATION));
	}
}

/* Creating and releasing animated sprites, and measuring animation lengths. */

/* Creates a sprite for animation id, taking one of the 64 free sprites and a
   VRAM cache entry for its tiles. */
struct RESOURCE_OBJECT *fdSprCreate(Sint32 id)
{
	ANIMATION_METADATA *metadata;
	SPR				   *spr;
	SPR				   *freeSpr;
	Uint32			   *attribute;
	Sint32				slot;
	Sint32				tile;
	Sint32				i;
	Uint32				shape;

	freeSpr = NULL;
	metadata = fdResourceGetMetadataRecordStub(id);
	slot = syVRAMCacheFindFreeEntry();
	spr = Ram_HeapSlots->sprs;
	if(metadata->width == 0)
		return NULL;
	/* A width of 0 marks a free sprite. */
	for(i = 0; i < ANIM_OBJECT_COUNT; i++, spr++) {
		if(spr->width == 0) {
			freeSpr = spr;
			break;
		}
	}
	if(freeSpr == NULL)
		return NULL;
	if(slot == VRAM_CACHE_ENTRY_COUNT)
		return NULL;
	/* Claim the cache entry with no VRAM yet (size 0): the first
	   fdSprComposePicture that draws the sprite gives it width * height
	   bytes. */
	tile = syVRAMCacheLoad(slot, 0, 0);
	if(tile == 0)
		return NULL;
	freeSpr->rotation = 0;
	freeSpr->flg = SPRITE_FLAG_SHADOW;
	freeSpr->slot = slot;

	/* The OAM shape and size bits (attributes 0 and 1) for the image's width
	   and height in pixels. */
	switch((Uint32)SPRITE_SIZE_KEY(metadata->width, metadata->height)) {
	case SPRITE_SIZE_KEY(8, 8):
		shape = OBJ_SIZE_8X8;
		break;
	case SPRITE_SIZE_KEY(8, 16):
		shape = OBJ_SIZE_8X16;
		break;
	case SPRITE_SIZE_KEY(16, 8):
		shape = OBJ_SIZE_16X8;
		break;
	case SPRITE_SIZE_KEY(16, 16):
		shape = OBJ_SIZE_16X16;
		break;
	case SPRITE_SIZE_KEY(16, 32):
		shape = OBJ_SIZE_16X32;
		break;
	case SPRITE_SIZE_KEY(32, 16):
		shape = OBJ_SIZE_32X16;
		break;
	case SPRITE_SIZE_KEY(32, 32):
		shape = OBJ_SIZE_32X32;
		break;
	case SPRITE_SIZE_KEY(32, 64):
		shape = OBJ_SIZE_32X64;
		break;
	case SPRITE_SIZE_KEY(64, 32):
		shape = OBJ_SIZE_64X32;
		break;
	case SPRITE_SIZE_KEY(64, 64):
		shape = OBJ_SIZE_64X64;
		break;
	default:
		shape = 0;
		break;
	}

	/* Part 0 is the image; part 1 is its shadow, from the shared shadow
	   tiles. */
	attribute = (Uint32 *)&freeSpr->image;
	*attribute++ = 0;
	*attribute++ = shape | OBJ_256_COLORS;
	*attribute = tile | ANIM_OAM_FIELD_PRIORITY;
	attribute = (Uint32 *)&freeSpr->shadow;
	*attribute++ = 0;
	*attribute++ = OBJ_SIZE_16X8 | OBJ_256_COLORS;
	*attribute = (VramCacheEntries[VRAM_CACHE_ENTRY_ACTOR_SHADOW].ofs >> 5) |
		ANIM_OAM_FIELD_PRIORITY;
	/* DEAD: a test of the call's result for -1 that does nothing either way;
	   the ROM keeps the -1 it builds after the call. */
	if(fdSprAddAnimation(spr, id) == -1) {
		/* FAKEMATCH(dead): a store of the byte it reloads. */
		freeSpr->dirty = freeSpr->dirty;
	}
	return (struct RESOURCE_OBJECT *)freeSpr;
}

/* Releases a sprite: gives back its VRAM cache entry unless it keeps it
   (retainCacheEntry), clears its four sprite animations and zeroes it. */
void fdSprDestroy(SPR *spr)
{
	volatile Uint32 zero;
	Sint32			n;
	SPR_ANIMATION **entry;

	if(spr) {
		if(!spr->retainCacheEntry)
			syVRAMCacheFreeEntry(spr->slot);
		entry = spr->entries;
		for(n = 0; n < SPRITE_ANIMATION_COUNT; n++) {
			fdSprAnimationClear(*entry++);
		}
		DMA_FILL32_CELL(zero, 0, spr, sizeof(*spr));
	}
}

/* Returns how many ticks the first timedCnt timed steps of animation number
   animation of sprite id last. Commands are two bytes (command, value);
   ANIMCMD_PLAY, ANIMCMD_HOLD, ANIMCMD_JUMP and ANIMCMD_END end the count, and
   the values of ANIMCMD_WAIT, ANIMCMD_BLANK and the pictures below ANIMCMD_END
   count. */
Sint32 fdSprAnimationSumCommandLengths(Sint32 id, Uint32 anm, Sint32 timedCnt)
{
	ANIMATION_METADATA *metadata;
	Uint8			   *script;
	Uint8				command;
	Uint8				duration;
	Sint32				durationSum = 0;

	metadata = fdResourceGetMetadataRecordStub(id);
	if(anm >= metadata->anmCnt) {
		return 0;
	}
	script = ((Uint8 **)metadata->anm)[anm];
	for(;;) {
		command = script[0];
		duration = script[1];
		script += 2;
		if(command == ANIMCMD_PLAY || command == ANIMCMD_HOLD ||
			command == ANIMCMD_JUMP || command == ANIMCMD_END) {
			break;
		}
		if(command == ANIMCMD_WAIT || command == ANIMCMD_BLANK ||
			command < ANIMCMD_END) {
			durationSum += duration;
			timedCnt--;
			if(timedCnt == 0) {
				break;
			}
		}
	}
	return durationSum;
}
