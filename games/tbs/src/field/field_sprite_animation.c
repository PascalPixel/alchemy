/// @file
/// Sprites, their animations and the picture caches they draw from.
// Part of field.c.

#include "field_internal.h"

/// A sprite size as one number, width in the high byte and height in the low,
/// to pick its OAM shape and size with one switch.
#define SPRITE_SIZE_KEY(width, height) (((width) << 8) + (height))

/// The field sprites' shadow size, OAM priority, picture cache limits and
/// basic colours.
enum {
  ANIM_SHADOW_TILE_BYTES = 16 * 8, ///< 16x8 pixels, 256 colours
  /// Priority 2, the field's for its sprites: in front of a map layer at 3, or
  /// at 2 since a sprite wins a tie, and behind one at 1.
  ANIM_OAM_FIELD_PRIORITY = OBJ_PRIORITY(2),
  /// Rows of gSpritePictureCache_Sources searched, at most.
  PICTURE_CACHE_SOURCES_MAX = 256,
  /// Picture offsets at the head of a decoded set, at most.
  PICTURE_CACHE_PICTURES_MAX = 256,
  /// The colours of palettes 0 to 13, which Palette_LoadBasicSpriteColors
  /// loads; a variant remaps only these.
  BASIC_SPRITE_COLORS = 14 * PALETTE_COLORS
};

/// Loads the pictures of animation `number` into picture cache `slot`, at
/// `buffer`: the resource gSpritePictureCache_Sources lists for it, decoded,
/// its picture offsets turned into pointers, and for variant 1 to 5 its colours
/// of the basic sprite palettes recoloured through that variant's table (a
/// variant above 5 takes variant 1's). Returns the size of one picture in
/// pixels, or 0 when nothing was loaded.
s32 SpritePictureCache_Load(u32 slot, u32 *buffer, s32 number, u32 variant) {
  struct AnimationMetadata *metadata;
  struct ActorSystemState *system;
  struct SpritePictureCache *record;
  struct SpritePictureCacheSource *entry;
  u32 *offset;
  u8 *pixels;
  u8 *end;
  u8 *colorMap;
  u32 size;
  u32 count;
  u32 index;
  u16 entryNumber;
  s32 resource;

  if (slot >= RESOURCE_SLOT_COUNT)
    return 0;
  system = gActorSystemState;
  record = &system->pictureCaches[slot];
  metadata = Resource_GetMetadataRecordFar(number);
  record->tag = (slot << PICTURE_CACHE_TAG_SLOT_SHIFT) | number;
  record->pictures = buffer;

  // Resolve the animation number to its packed resource.
  entry = gSpritePictureCache_Sources;
  for (count = 0; count < PICTURE_CACHE_SOURCES_MAX; count++) {
    s16 *numbers = &entry->number;

    resource = entry->resource;
    entryNumber = *numbers;
    entry++;
    if (entryNumber == 0)
      return 0;
    if (entryNumber == number)
      break;
  }
  size = Decompress_TaggedLz(Resource_GetTableEntry(resource), buffer);

  // Turn the picture-offset directory into pointers before the pixel payload.
  offset = buffer;
  for (count = 0; count < PICTURE_CACHE_PICTURES_MAX; count++) {
    if (*offset == 0)
      break;
    *offset += (u32)buffer;
    offset++;
  }
  if (variant != 0) {
    index = variant - 1;
    pixels = (u8 *)(offset + 1);
    end = (u8 *)buffer + size;
    if (index > 4)
      index = 0;
    // Colours of palettes 14 and 15 are kept; only the basic palettes' change.
    colorMap = (u8 *)gSpriteColorTables[index];
    for (; pixels < end; pixels++) {
      u32 colorIndex = *pixels;

      if (colorIndex < BASIC_SPRITE_COLORS) {
        colorIndex = colorMap[colorIndex];
        *pixels = colorIndex;
      }
    }
  }
  return metadata->width * metadata->height;
}

/// Returns the buffer loaded into the sprite picture cache with this tag, or 0.
s32 SpritePictureCache_FindByTag(s32 tag) {
  u32 i;
  struct SpritePictureCache *slot = gActorSystemState->pictureCaches;

  for (i = 0; i < RESOURCE_SLOT_COUNT; i++, slot++) {
    if (slot->tag == tag)
      return (s32)slot->pictures;
  }
  return 0;
}

/// Resets each sprite animation of a sprite from its animation's metadata: its
/// facing kind, pictures, scripts and codec, with no picture shown and no
/// script running.
s32 Sprite_ResetAnimations(struct Sprite *sprite) {
  s32 i;

  for (i = 0; i < sprite->count; i++) {
    struct SpriteAnimation *layer = sprite->entries[i];
    struct AnimationMetadata *metadata =
        Resource_GetMetadataRecordFar(layer->animationId);
    void **pictures;

    if (metadata->width == 0)
      continue;

    if (i == 0) {
      // The first sprite animation supplies the dimensions and origin of the
      // whole sprite.
      sprite->width = metadata->width;
      sprite->height = metadata->height;
      sprite->scale = metadata->scale << 8;
      sprite->offsetY = metadata->adjustY;
      sprite->offsetX = metadata->adjustX;
    }

    // An animation whose metadata has no pictures takes them from the resource
    // slot loaded under its id (SpritePictureCache_Load).
    pictures = metadata->pictures;
    if (pictures == 0)
      pictures = (void **)SpritePictureCache_FindByTag(layer->animationId);

    layer->kind = metadata->drawKind;
    layer->pictures = pictures;
    layer->scripts = metadata->animation;
    layer->mode = metadata->pictureCodec;
    // SPRITE_PICTURE_NONE and no script: Sprite_ComposePicture skips the sprite
    // animation until an animation is selected on it.
    layer->picture = SPRITE_PICTURE_NONE;
    layer->script = 0;
    layer->scriptOffset = 0;
  }

  return 0;
}

/// Resets one sprite animation from its animation's metadata, as
/// Sprite_ResetAnimations does for each sprite animation.
void SpriteAnimation_Reset(struct SpriteAnimation *layer) {
  void **pictures;
  struct AnimationMetadata *metadata;

  if (layer != NULL) {
    metadata = Resource_GetMetadataRecordFar(layer->animationId);
    if (metadata->width != 0) {
      // Keep the sprite animation inactive until an animation script is
      // selected.
      pictures = metadata->pictures;
      if (pictures == 0)
        pictures = (void **)SpritePictureCache_FindByTag(layer->animationId);
      layer->kind = metadata->drawKind;
      layer->scripts = metadata->animation;
      layer->pictures = pictures;
      layer->mode = metadata->pictureCodec;
      layer->picture = SPRITE_PICTURE_NONE;
      layer->script = NULL;
      layer->scriptOffset = 0;
    }
  }
}

/// Adds a sprite animation of animation `id` to a sprite, in the first of its
/// four slots that is free. Returns the new sprite animation, 0 when none is
/// free, or -1 when the sprite's four slots are taken.
s32 Sprite_AddAnimation(struct Sprite *sprite, s32 id) {
  s32 layerHandle = (s32)sprite->entries[0];
  s32 freeIndex = 0;
  struct SpriteAnimation **entry;
  struct AnimationMetadata *metadata;

  // Find an unused sprite animation slot before reserving a sprite animation.
  if (layerHandle != 0) {
    entry = &sprite->entries[0];
    do {
      freeIndex++;
      if (freeIndex >= ANIM_LAYER_COUNT)
        break;
      entry++;
      layerHandle = (s32)*entry;
    } while (layerHandle != 0);
  }
  if (freeIndex == ANIM_LAYER_COUNT)
    return -1;
  layerHandle = (s32)SpriteAnimation_Alloc(id);
  if (layerHandle == 0)
    return 0;
  sprite->entries[freeIndex] = (struct SpriteAnimation *)layerHandle;
  metadata = Resource_GetMetadataRecordFar(id);
  // Only the first registered sprite animation establishes the sprite's
  // geometry.
  if (sprite->count == 0) {
    sprite->width = metadata->width;
    sprite->height = metadata->height;
    sprite->scale = metadata->scale << 8;
    sprite->offsetY = metadata->adjustY;
    sprite->offsetX = metadata->adjustX;
  }
  if (freeIndex == sprite->count)
    sprite->count = freeIndex + 1;
  return layerHandle;
}

/// Removes the sprite animation handle from a sprite and clears it, trimming
/// the sprite's animation count when no later sprite animation remains.
void Sprite_RemoveAnimation(struct Sprite *sprite, s32 layer) {
  s32 laterLayers;
  u32 index;
  u32 i;

  if (sprite != NULL && layer != 0) {
    SpriteAnimation_Clear((struct SpriteAnimation *)layer);
    // Locate the handle, then trim only empty slots at the end of the list.
    for (index = 0; index < ANIM_LAYER_COUNT; index++) {
      if (layer == (s32)sprite->entries[index])
        break;
    }
    if (index != ANIM_LAYER_COUNT) {
      sprite->entries[index] = 0;
      laterLayers = 0;
      for (i = index + 1; i < ANIM_LAYER_COUNT; i++) {
        if (sprite->entries[i] != 0)
          laterLayers++;
      }
      if (laterLayers == 0)
        sprite->count = index;
    }
  }
}

/// Removes and clears a sprite's sprite animation `index` (0 to 3), trimming
/// the sprite's animation count when no later sprite animation remains.
void Sprite_RemoveAnimationAt(struct Sprite *sprite, u32 index) {
  struct SpriteAnimation *layer;
  s32 laterLayers;
  u32 i;

  if (sprite != NULL && index < ANIM_LAYER_COUNT) {
    layer = sprite->entries[index];
    if (layer != NULL) {
      SpriteAnimation_Clear(layer);
      sprite->entries[index] = NULL;
      laterLayers = 0;
      // Interior holes stay available; only trailing holes reduce the count.
      for (i = index + 1; i < ANIM_LAYER_COUNT; i++) {
        if (sprite->entries[i] != NULL)
          laterLayers++;
      }
      if (laterLayers == 0)
        sprite->count = index;
    }
  }
}

/// Starts animation `animation` on one sprite animation, when its set has
/// that many; SPRITE_ANIMATION_KEEP_PLACE keeps its place and timer.
void SpriteAnimation_Start(struct SpriteAnimation *layer, s32 animation) {
  s32 keepPlace;
  struct AnimationMetadata *metadata;
  s32 scriptAddress;

  // Unlike Sprite_StartAnimation it leaves SPRITE_ANIMATION_KEEP_PLACE in the
  // index, so with that bit set the animation starts only in a set of more
  // than 128 animations.
  keepPlace = SPRITE_ANIMATION_KEEP_PLACE & animation;
  if (layer->scripts != 0) {
    metadata = Resource_GetMetadataRecordFar(layer->animationId);
    if (animation < metadata->animationCount) {
      scriptAddress = (s32)layer->scripts[animation];
      layer->kind = metadata->drawKind;
      layer->script = (u8 *)scriptAddress;
      layer->step = SPRITE_ANIMATION_NORMAL_STEP;
      if (keepPlace == 0) {
        layer->scriptOffset = keepPlace;
        layer->timer = keepPlace;
      }
    }
  }
}

/// Starts `animation` (its SPRITE_ANIMATION_INDEX_MASK bits) on every sprite
/// animation of a sprite, unless the sprite plays it already;
/// SPRITE_ANIMATION_KEEP_PLACE keeps each one's place and timer.
s32 Sprite_StartAnimation(struct Sprite *sprite, s32 animation) {
  s32 keepPlace;
  s32 i;

  keepPlace = animation & SPRITE_ANIMATION_KEEP_PLACE;
  animation &= SPRITE_ANIMATION_INDEX_MASK;

  if (sprite->lastNo != animation) {
    i = 0;
    goto test_entry;
  entry_loop:
    // Only the first sprite animation supplies the sprite's offsets.
    {
      struct SpriteAnimation *layer = sprite->entries[i];
      struct AnimationMetadata *metadata;
      s32 selectedAnimation;

      if (layer != 0 && layer->scripts != 0) {
        metadata = Resource_GetMetadataRecordFar(layer->animationId);
        if (animation < metadata->animationCount) {
          selectedAnimation = (s32)layer->scripts[animation];
          layer->kind = metadata->drawKind;
          layer->script = (u8 *)selectedAnimation;
          layer->step = SPRITE_ANIMATION_NORMAL_STEP;
          if (keepPlace == 0) {
            layer->scriptOffset = 0;
            layer->timer = 0;
          }
          if (i == 0) {
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

/// Sets each playing sprite animation's timer to the given duration in frames.
/// Returns 0.
s32 Sprite_SetAnimationTimers(struct Sprite *sprite, s32 frames) {
  u8 count = sprite->count;
  struct SpriteAnimation **layerList;
  s32 remaining;

  if (count != 0) {
    // The sprite animation timer counts sixteenths of a frame.
    frames <<= 4;
    layerList = sprite->entries;
    remaining = count;
    do {
      struct SpriteAnimation *layer = *layerList++;
      if (layer != 0 && layer->scripts != 0)
        layer->timer = frames;
      remaining--;
    } while (remaining != 0);
  }
  return 0;
}

/// Sets the timer step, the animation speed, of each playing sprite animation
/// of a sprite (16 is normal speed).
void Sprite_SetAnimationSpeed(struct Sprite *sprite, s32 step) {
  u8 count = sprite->count;
  if (count != 0) {
    struct SpriteAnimation **layerList = sprite->entries;
    s32 remaining = count;
    do {
      struct SpriteAnimation *layer = *layerList++;
      if (layer != 0 && layer->scripts != 0)
        layer->step = step;
      remaining--;
    } while (remaining != 0);
  }
}

/// Allocates and zeroes the 64 sprites and 64 sprite animations
/// (HEAP_SLOT_SPRITES and HEAP_SLOT_SPRITE_ANIMATIONS, from EWRAM first in
/// ACTOR_SYSTEM_WORLD_MAP and IWRAM first otherwise), loads the basic sprite
/// palettes, loads the shadow tiles into VRAM_CACHE_ENTRY_ACTOR_SHADOW and puts
/// the sprite tile uploader in RAM.
void ActorSystem_Configure(s32 mode) {
  void *sprites;
  void *layers;
  volatile u32 zero;

  // On the world map (ACTOR_SYSTEM_WORLD_MAP) the two pools are taken from
  // EWRAM first; elsewhere from IWRAM first, though a battle has already taken
  // both from EWRAM.
  if (mode == ACTOR_SYSTEM_WORLD_MAP) {
    sprites = HeapSlot_AllocEwram(HEAP_SLOT_SPRITES,
                                  ANIM_OBJECT_COUNT * sizeof(struct Sprite));
    layers =
        HeapSlot_AllocEwram(HEAP_SLOT_SPRITE_ANIMATIONS,
                            ANIM_OBJECT_COUNT * sizeof(struct SpriteAnimation));
  } else {
    sprites = HeapSlot_Alloc(HEAP_SLOT_SPRITES,
                             ANIM_OBJECT_COUNT * sizeof(struct Sprite));
    layers = HeapSlot_Alloc(HEAP_SLOT_SPRITE_ANIMATIONS,
                            ANIM_OBJECT_COUNT * sizeof(struct SpriteAnimation));
  }
  // Load the sprites' basic colours, clear both pools, then load the shared
  // shadow (128 bytes: the two 8x8 tiles of a 16x8, 256-colour sprite) and
  // the tile uploader.
  Palette_LoadBasicSpriteColors();
  Dma_Fill32Cell(zero, 0, sprites, ANIM_OBJECT_COUNT * sizeof(struct Sprite));
  Dma_Fill32Cell(zero, 0, layers,
                 ANIM_OBJECT_COUNT * sizeof(struct SpriteAnimation));
  VramCache_Load(VRAM_CACHE_ENTRY_ACTOR_SHADOW, ANIM_SHADOW_TILE_BYTES,
                 gActor_ShadowTiles);
  RamCode_LoadSlot(HEAP_SLOT_PICTURE_UPLOADER, Tile_CopyStrided,
                   Tile_CopyStridedCodeSize);
}

/// Takes a free sprite animation and starts it on the first animation of
/// animation `id`; NULL when none is free.
struct SpriteAnimation *SpriteAnimation_Alloc(s32 id) {
  struct AnimationMetadata *metadata;
  struct SpriteAnimation *candidate;
  struct SpriteAnimation *found;
  struct SpriteAnimation *layer;
  s32 i;
  void **pictures;
  u8 **scripts;

  found = NULL;
  metadata = Resource_GetMetadataRecordFar(id);
  candidate = gSpriteAnimations[0];
  layer = NULL;

  if (metadata->width != 0) {
    // A zero facing kind marks a free sprite animation.
    for (i = 0; i < ANIM_OBJECT_COUNT; i++, candidate++) {
      if (candidate->kind == 0) {
        found = candidate;
        break;
      }
    }
    if (found != NULL) {
      // Bind the metadata and start its first animation at command zero. UB:
      // the word view aliases a pointer field.
#ifdef UBFIX
      pictures = metadata->pictures;
#else
      pictures = (void **)*(s32 *)&metadata->pictures;
#endif
      layer = found;
      layer->animationId = id;
      if (pictures == 0)
        pictures = (void **)SpritePictureCache_FindByTag(id);

      scripts = metadata->animation;
      layer->scripts = scripts;
      layer->pictures = pictures;
      layer->mode = metadata->pictureCodec;
      layer->picture = SPRITE_PICTURE_NONE;
      layer->script = scripts[0];
      layer->scriptOffset = 0;
      layer->kind = metadata->drawKind;
      layer->colorMode = 0;
    }
  }
  return layer;
}

/// Zeroes one sprite animation (struct SpriteAnimation); does nothing for NULL.
/// Zeroing kind frees the sprite animation for SpriteAnimation_Alloc.
void SpriteAnimation_Clear(struct SpriteAnimation *record) {
  if (record != 0) {
    Dma_Fill32(0, record, sizeof(*record));
  }
}

// Creating and releasing animated sprites, and measuring animation lengths.

/// Creates a sprite for animation `id`, taking one of the 64 free sprites and a
/// VRAM cache entry for its tiles.
struct Sprite *Sprite_Create(s32 id) {
  struct AnimationMetadata *metadata;
  struct Sprite *sprite;
  struct Sprite *freeSprite;
#ifdef UBFIX
  u32 attributes[2 * sizeof(struct AnimationSpritePart) / sizeof(u32)];
#else
  u32 *attribute;
#endif
  s32 slot;
  s32 tile;
  s32 i;
  u32 shape;

  freeSprite = NULL;
  metadata = Resource_GetMetadataRecordFar(id);
  slot = VramCache_FindFreeEntry();
  sprite = gSprites;
  if (metadata->width == 0)
    return NULL;
  // A width of 0 marks a free sprite.
  for (i = 0; i < ANIM_OBJECT_COUNT; i++, sprite++) {
    if (sprite->width == 0) {
      freeSprite = sprite;
      break;
    }
  }
  if (freeSprite == NULL)
    return NULL;
  if (slot == VRAM_CACHE_ENTRY_COUNT)
    return NULL;
  // Claim the cache entry with no VRAM yet (size 0): the first
  // Sprite_ComposePicture that draws the sprite gives it width * height bytes.
  tile = VramCache_Load(slot, 0, 0);
  if (tile == 0)
    return NULL;
  freeSprite->slot = slot;
  freeSprite->rotation = 0;
  freeSprite->flags = SPRITE_FLAG_SHADOW;

  // The OAM shape and size bits (attributes 0 and 1) for the image's width
  // and height in pixels.
  switch ((u32)SPRITE_SIZE_KEY(metadata->width, metadata->height)) {
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

  // Part 0 is the image; part 1 is its shadow, from the shared shadow tiles.
  // UB: the word cursor aliases the sprite and crosses its members.
  // UBFIX copies the same representation from an actual word array.
#ifdef UBFIX
  attributes[0] = 0;
  attributes[1] = shape | OBJ_256_COLORS;
  attributes[2] = tile | ANIM_OAM_FIELD_PRIORITY;
  attributes[3] = 0;
  attributes[4] = OBJ_SIZE_16X8 | OBJ_256_COLORS;
  attributes[5] =
      (gVramCacheEntries[VRAM_CACHE_ENTRY_ACTOR_SHADOW].offset >> 5) |
      ANIM_OAM_FIELD_PRIORITY;
  memcpy(freeSprite, attributes, sizeof(attributes));
#else
  attribute = (u32 *)freeSprite;
  *attribute++ = 0;
  *attribute++ = shape | OBJ_256_COLORS;
  *attribute++ = tile | ANIM_OAM_FIELD_PRIORITY;
  *attribute++ = 0;
  *attribute++ = OBJ_SIZE_16X8 | OBJ_256_COLORS;
  *attribute = (gVramCacheEntries[VRAM_CACHE_ENTRY_ACTOR_SHADOW].offset >> 5) |
               ANIM_OAM_FIELD_PRIORITY;
#endif
  // DEAD: a test of the call's result for -1 that does nothing either way; the
  // ROM keeps the -1 it builds after the call.
  if (Sprite_AddAnimation(sprite, id) == -1) {
    /* FAKEMATCH(dead): a store of the byte it reloads. */
    freeSprite->dirty = freeSprite->dirty;
  }
  return freeSprite;
}

/// Releases a sprite: its VRAM cache entry (unless retainCacheEntry is set),
/// its four sprite animations, and then zeroes its 56 bytes.
void Sprite_Destroy(struct Sprite *sprite) {
  s32 i;

  if (sprite) {
    // Release its VRAM cache entry and sprite animations before clearing the
    // sprite itself.
    if (!sprite->retainCacheEntry)
      VramCache_FreeEntry(sprite->slot);
    for (i = 0; i < ANIM_LAYER_COUNT; i++)
      SpriteAnimation_Clear(sprite->entries[i]);
    Dma_Fill32(0, sprite, sizeof(*sprite));
  }
}

/// Returns how long, in frames, the first `timedCount` timed commands of
/// animation `animation` of animation `id` last: the sum of the arguments of
/// its picture, wait and blank commands, up to timedCount of them or the first
/// play, hold, jump or end command.
s32 SpriteAnimation_SumCommandLengths(s32 id, u32 animation, s32 timedCount) {
  struct AnimationMetadata *metadata;
  u8 *script;
  u8 command;
  u8 duration;
  s32 durationSum = 0;

  metadata = Resource_GetMetadataRecordFar(id);
  if (animation >= metadata->animationCount) {
    return 0;
  }
  script = ((u8 **)metadata->animation)[animation];
  // Only timed commands count; control-flow commands terminate this scan.
  for (;;) {
    command = script[0];
    duration = script[1];
    script += 2;
    if (command == ANIMCMD_PLAY || command == ANIMCMD_HOLD ||
        command == ANIMCMD_JUMP || command == ANIMCMD_END) {
      break;
    }
    if (command == ANIMCMD_WAIT || command == ANIMCMD_BLANK ||
        command < ANIMCMD_END) {
      durationSum += duration;
      timedCount--;
      if (timedCount == 0) {
        break;
      }
    }
  }
  return durationSum;
}
