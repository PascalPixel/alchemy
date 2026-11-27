// Sprites, their animations and the picture caches they draw from. Part of field.c.

// A sprite size as one number, width in the high byte and height in the low, to pick its OAM shape
// and size with one switch.
#define SPRITE_SIZE_KEY(width, height) (((width) << 8) + (height))

// Sizes and limits of the field's sprite pictures.
enum
{
    ANIM_SHADOW_TILE_BYTES       = 16 * 8, // 16x8 pixels, 256 colours
    // Priority 2, the field's for its sprites: in front of a map layer at 3, or at 2 since a sprite
    // wins a tie, and behind one at 1.
    ANIM_OAM_FIELD_PRIORITY      = OBJ_PRIORITY(2),
    // A picture cache's tag holds its slot above the animation id's bits.
    PICTURE_CACHE_TAG_SLOT_SHIFT = 12,
    // Rows of SpritePictureCacheSources searched, at most.
    PICTURE_CACHE_SOURCES_MAX    = 256,
    // Picture offsets at the head of a decoded set, at most.
    PICTURE_CACHE_PICTURES_MAX   = 256,
    // The colours of palettes 0 to 13, which syPaletteLoadBasicSpriteColors loads; a variant remaps
    // only these.
    BASIC_SPRITE_COLORS          = 14 * 16
};

#undef fdSpriteAnimationAllocate

// Loads the pictures of animation id number into picture cache slot's buffer: the resource listed
// for it in SpritePictureCacheSources, decoded, its picture offsets turned into pointers, and for
// variant 1-5 its pixel colours (those up to 223, the basic sprite palettes 0 to 13) remapped
// through that variant's conversion table; a variant above 5 uses variant 1's.
Sint32 fdSpritePictureCacheLoad(Uint32 slot, Uint32 *buf, Sint32 number, Uint32 variant)
{
    ANIMATION_METADATA       *metadata;
    ACTOR_SYSTEM_STATE       *system;
    SPR_PICTURE_CACHE        *cache;
    SPR_PICTURE_CACHE_SOURCE *entry;
    Uint32                   *offset;
    Uint8                    *pixel;
    Uint8                    *end;
    Uint8                    *table;
    Uint32                    size;
    Uint32                    count;
    Uint32                    index;
    Uint16                    entryNumber;
    Sint32                    resource;

    if (slot >= SPRITE_PICTURE_CACHE_COUNT)
        return 0;
    system          = RamHeapSlots->actorSystem;
    cache           = &system->pictureCaches[slot];
    metadata        = fdResourceGetMetadataRecordStub(number);
    cache->tag      = (slot << PICTURE_CACHE_TAG_SLOT_SHIFT) | number;
    cache->pictures = buf;

    entry           = SpritePictureCacheSources;
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
        index = variant - 1;
        pixel = (Uint8 *)(offset + 1);
        end   = (Uint8 *)buf + size;
        if (index > 4)
            index = 0;
        table = SpriteColorTables[index];
        for (; pixel < end; pixel++)
        {
            Uint32 c = *pixel;

            if (c < BASIC_SPRITE_COLORS)
            {
                c      = table[c];
                *pixel = c;
            }
        }
    }
    return metadata->width * metadata->height;
}

// Returns the pictures loaded into the sprite picture cache with this tag, or 0.
Sint32 fdSpritePictureCacheFindByTag(Sint32 tag)
{
    ACTOR_SYSTEM_STATE *system = RamHeapSlots->actorSystem;
    Uint32              i;
    SPR_PICTURE_CACHE  *cache;

    i = 0;
    for (cache = system->pictureCaches; i < SPRITE_PICTURE_CACHE_COUNT; i++, cache++)
    {
        if (cache->tag == tag)
            return (Sint32)cache->pictures;
    }
    return 0;
}

// Resets each sprite animation of a sprite from its animation's metadata: its facing kind,
// pictures, scripts and codec, with no picture shown and no script running.
Sint32 fdSpriteResetAnimations(SPR *sprite)
{
    Sint32          index;
    SPR_ANIMATION **slot;

    index = 0;
    if (index < sprite->count)
    {
        slot = sprite->entries;
        for (; index < sprite->count; index++)
        {
            SPR_ANIMATION      *spriteAnimation = *slot++;
            ANIMATION_METADATA *metadata =
                fdResourceGetMetadataRecordStub(spriteAnimation->animationId);
            void  **pictures;
            Uint8 **animation;

            if (metadata->width == 0)
                continue;

            if (index == 0)
            {
                sprite->width   = metadata->width;
                sprite->height  = metadata->height;
                sprite->scale   = metadata->scale << 8;
                sprite->offsetY = metadata->adjustY;
                sprite->offsetX = metadata->adjustX;
            }

            pictures = metadata->pictures;
            if (pictures == 0)
                pictures = (void **)fdSpritePictureCacheFindByTag(spriteAnimation->animationId);

            spriteAnimation->kind = metadata->drawKind;
            animation             = metadata->animation;
            // UB: the pictures are stored through a Uint8 ** view of the field, the scripts' type.
#if FIX_TYPE_PUNNING
            spriteAnimation->pictures = pictures;
#else
            *(Uint8 ***)&spriteAnimation->pictures = (Uint8 **)pictures;
#endif
            spriteAnimation->scripts      = animation;
            spriteAnimation->mode         = metadata->pictureCodec;
            spriteAnimation->picture      = SPRITE_PICTURE_NONE;
            spriteAnimation->script       = 0;
            spriteAnimation->scriptOffset = 0;
        }
    }

    return 0;
}

// Resets a sprite animation from its metadata: its pictures, scripts, codec and drawing kind, with
// no script running and no picture shown.
void fdSpriteAnimationReset(SPR_ANIMATION *spriteAnimation)
{
    void              **pictures;
    Uint8             **scripts;
    ANIMATION_METADATA *metadata;

    if (spriteAnimation != NULL)
    {
        metadata = fdResourceGetMetadataRecordStub(spriteAnimation->animationId);
        if (metadata->width != 0)
        {
            pictures = metadata->pictures;
            if (pictures == 0)
                pictures = (void **)fdSpritePictureCacheFindByTag(spriteAnimation->animationId);
            spriteAnimation->kind = metadata->drawKind;
            syIOWrite32(0, (Uint32 *)&spriteAnimation->script);
            scripts                       = metadata->animation;
            spriteAnimation->pictures     = pictures;
            spriteAnimation->scripts      = scripts;
            spriteAnimation->scriptOffset = 0;
            spriteAnimation->mode         = metadata->pictureCodec;
            spriteAnimation->picture      = SPRITE_PICTURE_NONE;
        }
    }
}

// Adds a sprite animation of animation id to a sprite, in the first of its four slots that is free;
// returns it, 0 when none could be taken, or -1 when all four are in use.
Sint32 fdSpriteAddAnimation(SPR *sprite, Sint32 id)
{
    Sint32              spriteAnimation = (Sint32)sprite->entries[0];
    Sint32              freeIndex       = 0;
    SPR_ANIMATION     **entry;
    ANIMATION_METADATA *metadata;

    if (spriteAnimation != 0)
    {
        entry = &sprite->entries[0];
        do
        {
            freeIndex++;
            if (freeIndex >= SPRITE_ANIMATION_COUNT)
                break;
            entry++;
            spriteAnimation = (Sint32)*entry;
        } while (spriteAnimation != 0);
    }
    if (freeIndex == SPRITE_ANIMATION_COUNT)
        return -1;
    spriteAnimation = (Sint32)fdSpriteAnimationAllocate(id);
    if (spriteAnimation == 0)
        return 0;
    sprite->entries[freeIndex] = (SPR_ANIMATION *)spriteAnimation;
    metadata                   = fdResourceGetMetadataRecordStub(id);
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
    return spriteAnimation;
}

// Removes the sprite animation handle from a sprite and clears it, trimming the sprite's animation
// count when no later sprite animation remains.
void fdSpriteRemoveAnimation(SPR *sprite, Sint32 spriteAnimation)
{
    Sint32 laterAnimations;
    Uint32 index;
    Uint32 i;

    if (sprite != NULL && spriteAnimation != 0)
    {
        fdSpriteAnimationClear((SPR_ANIMATION *)spriteAnimation);
        // Locate the handle, then trim only empty slots at the end of the list.
        for (index = 0; index < SPRITE_ANIMATION_COUNT; index++)
        {
            if (spriteAnimation == (Sint32)sprite->entries[index])
                break;
        }
        if (index != SPRITE_ANIMATION_COUNT)
        {
            sprite->entries[index] = 0;
            laterAnimations        = 0;
            for (i = index + 1; i < SPRITE_ANIMATION_COUNT; i++)
            {
                if (sprite->entries[i] != 0)
                    laterAnimations++;
            }
            if (laterAnimations == 0)
                sprite->count = index;
        }
    }
}

// Removes and clears the sprite animation in a sprite's slot index (0 to 3), trimming the sprite's
// animation count when no later sprite animation remains.
void fdSpriteRemoveAnimationAt(SPR *sprite, Uint32 index)
{
    SPR_ANIMATION *spriteAnimation;
    Sint32         laterAnimations;
    Uint32         i;

    if (sprite != NULL && index < SPRITE_ANIMATION_COUNT)
    {
        spriteAnimation = sprite->entries[index];
        if (spriteAnimation != NULL)
        {
            fdSpriteAnimationClear(spriteAnimation);
            sprite->entries[index] = NULL;
            laterAnimations        = 0;
            // Interior holes stay available; only trailing holes reduce the count.
            for (i = index + 1; i < SPRITE_ANIMATION_COUNT; i++)
            {
                if (sprite->entries[i] != NULL)
                    laterAnimations++;
            }
            if (laterAnimations == 0)
                sprite->count = index;
        }
    }
}

// Starts the indexed animation on a sprite animation when its metadata contains it. Unlike
// fdSpriteStartAnimation it does not mask SPRITE_ANIMATION_KEEP_PLACE off, so with it set the index
// would pass animationCount only in a set of more than 128 animations.
void fdSpriteAnimationStart(SPR_ANIMATION *spriteAnimation, Sint32 animation)
{
    Sint32              keepPlace;
    ANIMATION_METADATA *metadata;
    Sint32              scriptAddr;

    keepPlace = SPRITE_ANIMATION_KEEP_PLACE & animation;
    if (spriteAnimation->scripts != 0)
    {
        metadata = fdResourceGetMetadataRecordStub(spriteAnimation->animationId);
        if (animation < metadata->animationCount)
        {
            scriptAddr              = (Sint32)spriteAnimation->scripts[animation];
            spriteAnimation->kind   = metadata->drawKind;
            spriteAnimation->script = (Uint8 *)scriptAddr;
            spriteAnimation->step   = SPRITE_ANIMATION_NORMAL_STEP;
            if (keepPlace == 0)
            {
                spriteAnimation->scriptOffset = keepPlace;
                spriteAnimation->timer        = keepPlace;
            }
        }
    }
}

// Starts an animation on every sprite animation of a sprite (SPRITE_ANIMATION_KEEP_PLACE keeps each
// one's place and timer), unless the sprite plays it already.
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
            SPR_ANIMATION      *spriteAnimation = sprite->entries[i];
            ANIMATION_METADATA *metadata;
            Sint32              selectedAnimation;

            if (spriteAnimation != 0 && spriteAnimation->scripts != 0)
            {
                metadata = fdResourceGetMetadataRecordStub(spriteAnimation->animationId);
                if (animation < metadata->animationCount)
                {
                    selectedAnimation       = (Sint32)spriteAnimation->scripts[animation];
                    spriteAnimation->kind   = metadata->drawKind;
                    spriteAnimation->script = (Uint8 *)selectedAnimation;
                    spriteAnimation->step   = SPRITE_ANIMATION_NORMAL_STEP;
                    if (keepPlace == 0)
                    {
                        spriteAnimation->scriptOffset = 0;
                        spriteAnimation->timer        = 0;
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
    Sint32 i;

    for (i = 0; i < sprite->count; i++)
    {
        SPR_ANIMATION *spriteAnimation = sprite->entries[i];

        // The sprite animation timer counts sixteenths of a tick.
        if (spriteAnimation != NULL && spriteAnimation->scripts != NULL)
        {
            spriteAnimation->timer = ticks << 4;
        }
    }
    return 0;
}

// Sets the timer step, the animation speed, of each playing sprite animation of a sprite (16 is
// normal speed).
void fdSpriteSetAnimationSpeed(SPR *sprite, Sint32 step)
{
    Sint32 i;

    for (i = 0; i < sprite->count; i++)
    {
        SPR_ANIMATION *spriteAnimation = sprite->entries[i];

        if (spriteAnimation != NULL && spriteAnimation->scripts != NULL)
            spriteAnimation->step = step;
    }
}

// Takes the sprite and sprite animation pools (from EWRAM first on the World Map,
// ACTOR_SYSTEM_WORLD_MAP, and IWRAM first otherwise) and clears them, loads the shadow tiles into
// VRAM cache entry VRAM_CACHE_ENTRY_ACTOR_SHADOW and copies the strided tile copier into RAM.
void fdActorSystemConfigure(Sint32 mode)
{
    void           *sprites;
    void           *spriteAnimations;
    volatile Uint32 zero;

    if (mode == ACTOR_SYSTEM_WORLD_MAP)
    {
        sprites = syHeapSlotAllocateEWRAM(HEAP_SLOT_SPRITES, ANIM_OBJECT_COUNT * sizeof(SPR));
        spriteAnimations = syHeapSlotAllocateEWRAM(HEAP_SLOT_SPRITE_ANIMATIONS,
                                                   ANIM_OBJECT_COUNT * sizeof(SPR_ANIMATION));
    }
    else
    {
        sprites          = syHeapSlotAllocate(HEAP_SLOT_SPRITES, ANIM_OBJECT_COUNT * sizeof(SPR));
        spriteAnimations = syHeapSlotAllocate(HEAP_SLOT_SPRITE_ANIMATIONS,
                                              ANIM_OBJECT_COUNT * sizeof(SPR_ANIMATION));
    }
    syPaletteLoadBasicSpriteColors();
    DMA_FILL32_CELL(zero, 0, sprites, ANIM_OBJECT_COUNT * sizeof(SPR));
    DMA_FILL32_CELL(zero, 0, spriteAnimations, ANIM_OBJECT_COUNT * sizeof(SPR_ANIMATION));
    syVRAMCacheLoad(VRAM_CACHE_ENTRY_ACTOR_SHADOW, ANIM_SHADOW_TILE_BYTES, ActorShadowTiles);
    RAM_CODE_LOAD_SLOT(
        HEAP_SLOT_PICTURE_UPLOADER, fdRenderCopyStridedCode, fdRenderCopyStridedCodeSize);
}

// Frees the actors' shadow tiles (VRAM_CACHE_ENTRY_ACTOR_SHADOW).
void fdActorShadowFreeTiles(void)
{
    syVRAMCacheFreeEntry(VRAM_CACHE_ENTRY_ACTOR_SHADOW);
}

// Loads the actors' shadow tiles into VRAM_CACHE_ENTRY_ACTOR_SHADOW.
void fdActorShadowLoadTiles(void)
{
    const Uint8 *data = ActorShadowTiles;
    Sint32       size = ANIM_SHADOW_TILE_BYTES;

    syVRAMCacheLoad(VRAM_CACHE_ENTRY_ACTOR_SHADOW, size, data);
}

// Takes a free sprite animation and starts it on animation id's first animation.
SPR_ANIMATION *fdSpriteAnimationAllocate(Sint32 id)
{
    ANIMATION_METADATA *metadata;
    SPR_ANIMATION      *entry;
    SPR_ANIMATION      *found;
    SPR_ANIMATION      *spriteAnimation;
    Sint32              i;
    void              **pictures;
    Uint8             **animation;

    found           = NULL;
    metadata        = fdResourceGetMetadataRecordStub(id);
    entry           = RamHeapSlots->spriteAnimations;
    spriteAnimation = NULL;

    if (metadata->width != 0)
    {
        for (i = 0; i < ANIM_OBJECT_COUNT; i++, entry++)
        {
            if (entry->kind == 0)
            {
                found = entry;
                break;
            }
        }
        if (found != NULL)
        {
            // UB: the word view aliases a pointer field.
#if FIX_TYPE_PUNNING
            pictures = metadata->pictures;
#else
            pictures = (void **)*(Sint32 *)&metadata->pictures;
#endif
            spriteAnimation              = found;
            spriteAnimation->animationId = id;
            if (pictures == 0)
                pictures = (void **)fdSpritePictureCacheFindByTag(id);

            animation                     = metadata->animation;
            spriteAnimation->scripts      = animation;
            spriteAnimation->pictures     = pictures;
            spriteAnimation->mode         = metadata->pictureCodec;
            spriteAnimation->picture      = SPRITE_PICTURE_NONE;
            spriteAnimation->script       = animation[0];
            spriteAnimation->scriptOffset = 0;
            spriteAnimation->kind         = metadata->drawKind;
            spriteAnimation->colorMode    = 0;
        }
    }
    return spriteAnimation;
}

// Zeroes one sprite animation (SPR_ANIMATION); does nothing for NULL. Zeroing kind frees the sprite
// animation for fdSpriteAnimationAllocate.
void fdSpriteAnimationClear(void *record)
{
    if (record != 0)
    {
        DMA_FILL32(0, record, sizeof(SPR_ANIMATION));
    }
}

// Creating and releasing animated sprites, and measuring animation lengths.

// Creates a sprite for animation id, taking one of the 64 free sprites and a VRAM cache entry for
// its tiles.
struct RESOURCE_OBJECT *fdSpriteCreate(Sint32 id)
{
    ANIMATION_METADATA *metadata;
    SPR                *sprite;
    SPR                *freeSprite;
    Uint32             *attribute;
    Sint32              slot;
    Sint32              tile;
    Sint32              i;
    Uint32              shape;

    freeSprite = NULL;
    metadata   = fdResourceGetMetadataRecordStub(id);
    slot       = syVRAMCacheFindFreeEntry();
    sprite     = RamHeapSlots->sprites;
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
    freeSprite->rotation = 0;
    freeSprite->flags    = SPRITE_FLAG_SHADOW;
    freeSprite->slot     = slot;

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

    // Part 0 is the image; part 1 is its shadow, from the shared shadow tiles.
    attribute    = (Uint32 *)&freeSprite->image;
    *attribute++ = 0;
    *attribute++ = shape | OBJ_256_COLORS;
    *attribute   = tile | ANIM_OAM_FIELD_PRIORITY;
    attribute    = (Uint32 *)&freeSprite->shadow;
    *attribute++ = 0;
    *attribute++ = OBJ_SIZE_16X8 | OBJ_256_COLORS;
    *attribute =
        (VramCacheEntries[VRAM_CACHE_ENTRY_ACTOR_SHADOW].offset >> 5) | ANIM_OAM_FIELD_PRIORITY;
    // DEAD: a test of the call's result for -1 that does nothing either way; the ROM keeps the -1
    // it builds after the call.
    if (fdSpriteAddAnimation(sprite, id) == -1)
    {
        // FAKEMATCH(dead): a store of the byte it reloads.
        freeSprite->dirty = freeSprite->dirty;
    }
    return (struct RESOURCE_OBJECT *)freeSprite;
}

// Releases a sprite: gives back its VRAM cache entry unless it keeps it (retainCacheEntry), clears
// its four sprite animations and zeroes it.
void fdSpriteDestroy(SPR *sprite)
{
    volatile Uint32 zero;
    Sint32          n;
    SPR_ANIMATION **entry;

    if (sprite)
    {
        if (!sprite->retainCacheEntry)
            syVRAMCacheFreeEntry(sprite->slot);
        entry = sprite->entries;
        for (n = 0; n < SPRITE_ANIMATION_COUNT; n++)
        {
            fdSpriteAnimationClear(*entry++);
        }
        DMA_FILL32_CELL(zero, 0, sprite, sizeof(*sprite));
    }
}

// Returns how many ticks the first timedCount timed steps of animation number animation of sprite
// id last. Commands are two bytes (command, value); ANIMCMD_PLAY, ANIMCMD_HOLD, ANIMCMD_JUMP and
// ANIMCMD_END end the count, and the values of ANIMCMD_WAIT, ANIMCMD_BLANK and the pictures below
// ANIMCMD_END count.
Sint32 fdSpriteAnimationSumCommandLengths(Sint32 id, Uint32 animation, Sint32 timedCount)
{
    ANIMATION_METADATA *metadata;
    Uint8              *script;
    Uint8               command;
    Uint8               duration;
    Sint32              durationSum = 0;

    metadata                        = fdResourceGetMetadataRecordStub(id);
    if (animation >= metadata->animationCount)
    {
        return 0;
    }
    script = ((Uint8 **)metadata->animation)[animation];
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
