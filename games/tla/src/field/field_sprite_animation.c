/// @file
/// Sprites, their animations and the picture caches they draw from.
// Part of field.c.

#include "field_internal.h"
#include "field/field.h"
#include "field/resource_slot.h"
#include "system/system.h"
#include "types.h"
#include "graphics/resource.h"
#include "field/sprite.h"
#include "system/sprite_metadata.h"
#include "system/ram_buffer.h"
#include "debug/debug.h"
#include "system/heap_release.h"
#include "system/io_reg.h"
#include "system/dma.h"
#include "edition.h"
#include "graphics/vram_tab.h"
#include "system/scene.h"

/// A sprite size as one number, width in the high byte and height in the low,
/// to pick its OAM shape and size with one switch.
#define SPRITE_SIZE_KEY(width, height) (((width) << 8) + (height))

/// Sizes and limits of the field's sprite pictures.
enum {
  ANIM_SHADOW_TILE_BYTES = 16 * 8, ///< 16x8 pixels, 256 colours
  /// Priority 2, the field's for its sprites: in front of a map layer at 3, or
  /// at 2 since a sprite wins a tie, and behind one at 1.
  ANIM_OAM_FIELD_PRIORITY = OBJ_PRIORITY(2),
  /// A picture cache's tag holds its slot above the animation id's bits.
  PICTURE_CACHE_TAG_SLOT_SHIFT = 12,
  /// Rows of gSpritePictureCache_Sources searched, at most.
  PICTURE_CACHE_SOURCES_MAX = 256,
  /// Picture offsets at the head of a decoded set, at most.
  PICTURE_CACHE_PICTURES_MAX = 256,
  /// The colours of palettes 0 to 13, which Palette_LoadBasicSpriteColors
  /// loads; a variant remaps only these.
  BASIC_SPRITE_COLORS = 14 * 16
};

#undef SpriteAnimation_Alloc

/// Loads the pictures of animation id number into picture cache slot's buffer:
/// the resource listed for it in gSpritePictureCache_Sources, decoded, its
/// picture offsets turned into pointers, and for variant 1-5 its pixel colours
/// (those up to 223, the basic sprite palettes 0 to 13) remapped through that
/// variant's conversion table; a variant above 5 uses variant 1's.
s32 SpritePictureCache_Load(u32 slot, u32 *buffer, s32 number, u32 variant) {
  struct AnimationMetadata *metadata;
  struct ActorSystemState *system;
  struct SpritePictureCache *cache;
  struct SpritePictureCacheSource *entry;
  u32 *offset;
  u8 *pixel;
  u8 *end;
  u8 *table;
  u32 size;
  u32 count;
  u32 index;
  u16 entryNumber;
  s32 resource;

  if (slot >= SPRITE_PICTURE_CACHE_COUNT)
    return 0;
  system = Ram_HeapSlots->actorSystem;
  cache = &system->pictureCaches[slot];
  metadata = Resource_GetMetadataRecordFar(number);
  cache->tag = (slot << PICTURE_CACHE_TAG_SLOT_SHIFT) | number;
  cache->pictures = buffer;

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

  offset = buffer;
  for (count = 0; count < PICTURE_CACHE_PICTURES_MAX; count++) {
    if (*offset == 0)
      break;
    *offset += (u32)buffer;
    offset++;
  }
  if (variant != 0) {
    index = variant - 1;
    pixel = (u8 *)(offset + 1);
    end = (u8 *)buffer + size;
    if (index > 4)
      index = 0;
    table = gSpriteColorTables[index];
    for (; pixel < end; pixel++) {
      u32 c = *pixel;

      if (c < BASIC_SPRITE_COLORS) {
        c = table[c];
        *pixel = c;
      }
    }
  }
  return metadata->width * metadata->height;
}

/// Returns the pictures loaded into the sprite picture cache with this tag, or
/// 0.
s32 SpritePictureCache_FindByTag(s32 tag) {
  struct ActorSystemState *system = Ram_HeapSlots->actorSystem;
  u32 i;
  struct SpritePictureCache *cache;

  i = 0;
  for (cache = system->pictureCaches; i < SPRITE_PICTURE_CACHE_COUNT;
       i++, cache++) {
    if (cache->tag == tag)
      return (s32)cache->pictures;
  }
  return 0;
}

/// Resets each sprite animation of a sprite from its animation's metadata: its
/// facing kind, pictures, scripts and codec, with no picture shown and no
/// script running.
s32 Sprite_ResetAnimations(struct Sprite *sprite) {
  s32 index;
  struct SpriteAnimation **slot;

  index = 0;
  if (index < sprite->count) {
    slot = sprite->entries;
    for (; index < sprite->count; index++) {
      struct SpriteAnimation *spriteAnimation = *slot++;
      struct AnimationMetadata *metadata =
          Resource_GetMetadataRecordFar(spriteAnimation->animationId);
      void **pictures;
      u8 **animation;

      if (metadata->width == 0)
        continue;

      if (index == 0) {
        sprite->width = metadata->width;
        sprite->height = metadata->height;
        sprite->scale = metadata->scale << 8;
        sprite->offsetY = metadata->adjustY;
        sprite->offsetX = metadata->adjustX;
      }

      pictures = metadata->pictures;
      if (pictures == 0)
        pictures =
            (void **)SpritePictureCache_FindByTag(spriteAnimation->animationId);

      spriteAnimation->kind = metadata->drawKind;
      animation = metadata->animation;
      // UB: the pictures are stored through a u8 ** view of the field, the
      // scripts' type.
#ifdef UBFIX
      spriteAnimation->pictures = pictures;
#else
      *(u8 ***)&spriteAnimation->pictures = (u8 **)pictures;
#endif
      spriteAnimation->scripts = animation;
      spriteAnimation->mode = metadata->pictureCodec;
      spriteAnimation->picture = SPRITE_PICTURE_NONE;
      spriteAnimation->script = 0;
      spriteAnimation->scriptOffset = 0;
    }
  }

  return 0;
}

/// Resets a sprite animation from its metadata: its pictures, scripts, codec
/// and drawing kind, with no script running and no picture shown.
void SpriteAnimation_Reset(struct SpriteAnimation *spriteAnimation) {
  void **pictures;
  u8 **scripts;
  struct AnimationMetadata *metadata;

  if (spriteAnimation != NULL) {
    metadata = Resource_GetMetadataRecordFar(spriteAnimation->animationId);
    if (metadata->width != 0) {
      pictures = metadata->pictures;
      if (pictures == 0)
        pictures =
            (void **)SpritePictureCache_FindByTag(spriteAnimation->animationId);
      spriteAnimation->kind = metadata->drawKind;
      Io_Write32(0, (u32 *)&spriteAnimation->script);
      scripts = metadata->animation;
      spriteAnimation->pictures = pictures;
      spriteAnimation->scripts = scripts;
      spriteAnimation->scriptOffset = 0;
      spriteAnimation->mode = metadata->pictureCodec;
      spriteAnimation->picture = SPRITE_PICTURE_NONE;
    }
  }
}

/// Adds a sprite animation of animation id to a sprite, in the first of its
/// four slots that is free; returns it, 0 when none could be taken, or -1 when
/// all four are in use.
s32 Sprite_AddAnimation(struct Sprite *sprite, s32 id) {
  s32 spriteAnimation = (s32)sprite->entries[0];
  s32 freeIndex = 0;
  struct SpriteAnimation **entry;
  struct AnimationMetadata *metadata;

  if (spriteAnimation != 0) {
    entry = &sprite->entries[0];
    do {
      freeIndex++;
      if (freeIndex >= SPRITE_ANIMATION_COUNT)
        break;
      entry++;
      spriteAnimation = (s32)*entry;
    } while (spriteAnimation != 0);
  }
  if (freeIndex == SPRITE_ANIMATION_COUNT)
    return -1;
  spriteAnimation = (s32)SpriteAnimation_Alloc(id);
  if (spriteAnimation == 0)
    return 0;
  sprite->entries[freeIndex] = (struct SpriteAnimation *)spriteAnimation;
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
  return spriteAnimation;
}

/// Removes the sprite animation handle from a sprite and clears it, trimming
/// the sprite's animation count when no later sprite animation remains.
void Sprite_RemoveAnimation(struct Sprite *sprite, s32 spriteAnimation) {
  s32 laterAnimations;
  u32 index;
  u32 i;

  if (sprite != NULL && spriteAnimation != 0) {
    SpriteAnimation_Clear((struct SpriteAnimation *)spriteAnimation);
    // Locate the handle, then trim only empty slots at the end of the list.
    for (index = 0; index < SPRITE_ANIMATION_COUNT; index++) {
      if (spriteAnimation == (s32)sprite->entries[index])
        break;
    }
    if (index != SPRITE_ANIMATION_COUNT) {
      sprite->entries[index] = 0;
      laterAnimations = 0;
      for (i = index + 1; i < SPRITE_ANIMATION_COUNT; i++) {
        if (sprite->entries[i] != 0)
          laterAnimations++;
      }
      if (laterAnimations == 0)
        sprite->count = index;
    }
  }
}

/// Removes and clears the sprite animation in a sprite's slot index (0 to 3),
/// trimming the sprite's animation count when no later sprite animation
/// remains.
void Sprite_RemoveAnimationAt(struct Sprite *sprite, u32 index) {
  struct SpriteAnimation *spriteAnimation;
  s32 laterAnimations;
  u32 i;

  if (sprite != NULL && index < SPRITE_ANIMATION_COUNT) {
    spriteAnimation = sprite->entries[index];
    if (spriteAnimation != NULL) {
      SpriteAnimation_Clear(spriteAnimation);
      sprite->entries[index] = NULL;
      laterAnimations = 0;
      // Interior holes stay available; only trailing holes reduce the count.
      for (i = index + 1; i < SPRITE_ANIMATION_COUNT; i++) {
        if (sprite->entries[i] != NULL)
          laterAnimations++;
      }
      if (laterAnimations == 0)
        sprite->count = index;
    }
  }
}

/// Starts the indexed animation on a sprite animation when its metadata
/// contains it. Unlike Sprite_StartAnimation it does not mask
/// SPRITE_ANIMATION_KEEP_PLACE off, so with it set the index would pass
/// animationCount only in a set of more than 128 animations.
void SpriteAnimation_Start(struct SpriteAnimation *spriteAnimation,
                           s32 animation) {
  s32 keepPlace;
  struct AnimationMetadata *metadata;
  s32 scriptAddress;

  keepPlace = SPRITE_ANIMATION_KEEP_PLACE & animation;
  if (spriteAnimation->scripts != 0) {
    metadata = Resource_GetMetadataRecordFar(spriteAnimation->animationId);
    if (animation < metadata->animationCount) {
      scriptAddress = (s32)spriteAnimation->scripts[animation];
      spriteAnimation->kind = metadata->drawKind;
      spriteAnimation->script = (u8 *)scriptAddress;
      spriteAnimation->step = SPRITE_ANIMATION_NORMAL_STEP;
      if (keepPlace == 0) {
        spriteAnimation->scriptOffset = keepPlace;
        spriteAnimation->timer = keepPlace;
      }
    }
  }
}

/// Starts an animation on every sprite animation of a sprite
/// (SPRITE_ANIMATION_KEEP_PLACE keeps each one's place and timer), unless the
/// sprite plays it already.
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
      struct SpriteAnimation *spriteAnimation = sprite->entries[i];
      struct AnimationMetadata *metadata;
      s32 selectedAnimation;

      if (spriteAnimation != 0 && spriteAnimation->scripts != 0) {
        metadata = Resource_GetMetadataRecordFar(spriteAnimation->animationId);
        if (animation < metadata->animationCount) {
          selectedAnimation = (s32)spriteAnimation->scripts[animation];
          spriteAnimation->kind = metadata->drawKind;
          spriteAnimation->script = (u8 *)selectedAnimation;
          spriteAnimation->step = SPRITE_ANIMATION_NORMAL_STEP;
          if (keepPlace == 0) {
            spriteAnimation->scriptOffset = 0;
            spriteAnimation->timer = 0;
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
  s32 i;

  for (i = 0; i < sprite->count; i++) {
    struct SpriteAnimation *spriteAnimation = sprite->entries[i];

    // The sprite animation timer counts sixteenths of a frame.
    if (spriteAnimation != NULL && spriteAnimation->scripts != NULL) {
      spriteAnimation->timer = frames << 4;
    }
  }
  return 0;
}

/// Sets the timer step, the animation speed, of each playing sprite animation
/// of a sprite (16 is normal speed).
void Sprite_SetAnimationSpeed(struct Sprite *sprite, s32 step) {
  s32 i;

  for (i = 0; i < sprite->count; i++) {
    struct SpriteAnimation *spriteAnimation = sprite->entries[i];

    if (spriteAnimation != NULL && spriteAnimation->scripts != NULL)
      spriteAnimation->step = step;
  }
}

/// Takes the sprite and sprite animation pools (from EWRAM first on the World
/// Map, ACTOR_SYSTEM_WORLD_MAP, and IWRAM first otherwise) and clears them,
/// loads the shadow tiles into VRAM cache entry VRAM_CACHE_ENTRY_ACTOR_SHADOW
/// and copies the strided tile copier into RAM.
void ActorSystem_Configure(s32 mode) {
  void *sprites;
  void *spriteAnimations;
  volatile u32 zero;

  if (mode == ACTOR_SYSTEM_WORLD_MAP) {
    sprites = HeapSlot_AllocEwram(HEAP_SLOT_SPRITES,
                                  ANIM_OBJECT_COUNT * sizeof(struct Sprite));
    spriteAnimations =
        HeapSlot_AllocEwram(HEAP_SLOT_SPRITE_ANIMATIONS,
                            ANIM_OBJECT_COUNT * sizeof(struct SpriteAnimation));
  } else {
    sprites = HeapSlot_Alloc(HEAP_SLOT_SPRITES,
                             ANIM_OBJECT_COUNT * sizeof(struct Sprite));
    spriteAnimations =
        HeapSlot_Alloc(HEAP_SLOT_SPRITE_ANIMATIONS,
                       ANIM_OBJECT_COUNT * sizeof(struct SpriteAnimation));
  }
  Palette_LoadBasicSpriteColors();
  Dma_Fill32Cell(zero, 0, sprites, ANIM_OBJECT_COUNT * sizeof(struct Sprite));
  Dma_Fill32Cell(zero, 0, spriteAnimations,
                 ANIM_OBJECT_COUNT * sizeof(struct SpriteAnimation));
  VramCache_Load(VRAM_CACHE_ENTRY_ACTOR_SHADOW, ANIM_SHADOW_TILE_BYTES,
                 gActor_ShadowTiles);
  RamCode_LoadSlot(HEAP_SLOT_PICTURE_UPLOADER, Render_CopyStridedCode,
                   Render_CopyStridedCodeSize);
}

/// Frees the actors' shadow tiles (VRAM_CACHE_ENTRY_ACTOR_SHADOW).
void ActorShadow_FreeTiles(void) {
  VramCache_FreeEntry(VRAM_CACHE_ENTRY_ACTOR_SHADOW);
}

/// Loads the actors' shadow tiles into VRAM_CACHE_ENTRY_ACTOR_SHADOW.
void ActorShadow_LoadTiles(void) {
  const u8 *data = gActor_ShadowTiles;
  s32 size = ANIM_SHADOW_TILE_BYTES;

  VramCache_Load(VRAM_CACHE_ENTRY_ACTOR_SHADOW, size, data);
}

/// Takes a free sprite animation and starts it on animation id's first
/// animation.
struct SpriteAnimation *SpriteAnimation_Alloc(s32 id) {
  struct AnimationMetadata *metadata;
  struct SpriteAnimation *entry;
  struct SpriteAnimation *found;
  struct SpriteAnimation *spriteAnimation;
  s32 i;
  void **pictures;
  u8 **animation;

  found = NULL;
  metadata = Resource_GetMetadataRecordFar(id);
  entry = Ram_HeapSlots->spriteAnimations;
  spriteAnimation = NULL;

  if (metadata->width != 0) {
    for (i = 0; i < ANIM_OBJECT_COUNT; i++, entry++) {
      if (entry->kind == 0) {
        found = entry;
        break;
      }
    }
    if (found != NULL) {
      // UB: the word view aliases a pointer field.
#ifdef UBFIX
      pictures = metadata->pictures;
#else
      pictures = (void **)*(s32 *)&metadata->pictures;
#endif
      spriteAnimation = found;
      spriteAnimation->animationId = id;
      if (pictures == 0)
        pictures = (void **)SpritePictureCache_FindByTag(id);

      animation = metadata->animation;
      spriteAnimation->scripts = animation;
      spriteAnimation->pictures = pictures;
      spriteAnimation->mode = metadata->pictureCodec;
      spriteAnimation->picture = SPRITE_PICTURE_NONE;
      spriteAnimation->script = animation[0];
      spriteAnimation->scriptOffset = 0;
      spriteAnimation->kind = metadata->drawKind;
      spriteAnimation->colorMode = 0;
    }
  }
  return spriteAnimation;
}

/// Zeroes one sprite animation (struct SpriteAnimation); does nothing for NULL.
/// Zeroing kind frees the sprite animation for SpriteAnimation_Alloc.
void SpriteAnimation_Clear(void *record) {
  if (record != 0) {
    Dma_Fill32(0, record, sizeof(struct SpriteAnimation));
  }
}

// Creating and releasing animated sprites, and measuring animation lengths.

/// Creates a sprite for animation id, taking one of the 64 free sprites and a
/// VRAM cache entry for its tiles.
struct ResourceObject *Sprite_Create(s32 id) {
  struct AnimationMetadata *metadata;
  struct Sprite *sprite;
  struct Sprite *freeSprite;
  u32 *attribute;
  s32 slot;
  s32 tile;
  s32 i;
  u32 shape;

  freeSprite = NULL;
  metadata = Resource_GetMetadataRecordFar(id);
  slot = VramCache_FindFreeEntry();
  sprite = Ram_HeapSlots->sprites;
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
  freeSprite->rotation = 0;
  freeSprite->flags = SPRITE_FLAG_SHADOW;
  freeSprite->slot = slot;

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
  attribute = (u32 *)&freeSprite->image;
  *attribute++ = 0;
  *attribute++ = shape | OBJ_256_COLORS;
  *attribute = tile | ANIM_OAM_FIELD_PRIORITY;
  attribute = (u32 *)&freeSprite->shadow;
  *attribute++ = 0;
  *attribute++ = OBJ_SIZE_16X8 | OBJ_256_COLORS;
  *attribute = (gVramCacheEntries[VRAM_CACHE_ENTRY_ACTOR_SHADOW].offset >> 5) |
               ANIM_OAM_FIELD_PRIORITY;
  // DEAD: a test of the call's result for -1 that does nothing either way; the
  // ROM keeps the -1 it builds after the call.
  if (Sprite_AddAnimation(sprite, id) == -1) {
    /* FAKEMATCH(dead): a store of the byte it reloads. */
    freeSprite->dirty = freeSprite->dirty;
  }
  return (struct ResourceObject *)freeSprite;
}

/// Releases a sprite: gives back its VRAM cache entry unless it keeps it
/// (retainCacheEntry), clears its four sprite animations and zeroes it.
void Sprite_Destroy(struct Sprite *sprite) {
  volatile u32 zero;
  s32 n;
  struct SpriteAnimation **entry;

  if (sprite) {
    if (!sprite->retainCacheEntry)
      VramCache_FreeEntry(sprite->slot);
    entry = sprite->entries;
    for (n = 0; n < SPRITE_ANIMATION_COUNT; n++) {
      SpriteAnimation_Clear(*entry++);
    }
    Dma_Fill32Cell(zero, 0, sprite, sizeof(*sprite));
  }
}

/// Returns how many frames the first timedCount timed steps of animation
/// number animation of sprite id last. Commands are two bytes (command,
/// value); ANIMCMD_PLAY, ANIMCMD_HOLD, ANIMCMD_JUMP and ANIMCMD_END end the
/// count, and the values of ANIMCMD_WAIT, ANIMCMD_BLANK and the pictures
/// below ANIMCMD_END count.
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
