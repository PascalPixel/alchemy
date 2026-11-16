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

enum {
  ANIM_SHADOW_TILE_BYTES = 16 * 8, ///< 16x8 pixels, 256 colours
  /// Priority 2, the field's for its sprites: in front of a map layer at 3, or
  /// at 2 since a sprite wins a tie, and behind one at 1.
  ANIM_OAM_FIELD_PRIORITY = OBJ_PRIORITY(2)
};

/// The sprite as Sprite_Create fills it: the image's three attribute
/// words, its layout, then the shadow's three words.
struct ResourceSprite {
  u32 imageAttributes[3];
  s32 scale;
  u8 slot;
  u8 retainCacheEntry : 1;
  u8 useOwnScale : 1;
  u8 sunkRows : 6;
  u16 rotation;
  u8 width;
  u8 height;
  s8 offsetX;
  s8 offsetY;
  u8 lastNo;
  u8 dirty;
  u8 flags;
  u8 count;
  u32 shadowAttributes[3];
  struct SpriteAnimation *entries[4];
};

#undef SpriteAnimation_Alloc

/// Loads the pictures of animation id number into picture cache slot's buffer:
/// the resource listed for it in gSpritePictureCache_Sources, decoded, its
/// picture offsets turned into pointers, and for variant 1-5 its pixel colours
/// (those up to 223, the basic sprite palettes 0 to 13) remapped through that
/// variant's conversion table; a variant above 5 uses variant 1's.
s32 SpritePictureCache_Load(u32 slot, u32 *buffer, s32 number, u32 variant) {
  struct AnimationMetadata *metadata;
  struct ActorSystemWork *work;
  struct SpritePictureCache *record;
  struct SpritePictureCacheSource *entry;
  u32 *offset;
  u8 *text;
  u8 *end;
  u8 *table;
  u32 size;
  u32 count;
  u32 index;
  u16 entryNumber;
  s32 resource;

  if (slot >= SPRITE_PICTURE_CACHE_COUNT)
    return 0;
  work = Ram_HeapSlots->actorSystem;
  record = &work->pictureCaches[slot];
  metadata = Resource_GetMetadataRecordFar(number);
  record->tag = (slot << 12) | number;
  record->pictures = buffer;

  entry = gSpritePictureCache_Sources;
  for (count = 0; count < 256; count++) {
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
  for (count = 0; count < 256; count++) {
    if (*offset == 0)
      break;
    *offset += (u32)buffer;
    offset++;
  }
  if (variant != 0) {
    index = variant - 1;
    text = (u8 *)(offset + 1);
    end = (u8 *)buffer + size;
    if (index > 4)
      index = 0;
    table = gResourceSlot_ConversionTables[index];
    for (; text < end; text++) {
      u32 c = *text;

      if (c <= 0xdf) {
        c = table[c];
        *text = c;
      }
    }
  }
  return metadata->width * metadata->height;
}

/// Returns the buffer loaded into the sprite picture cache with this tag, or 0.
s32 SpritePictureCache_FindByTag(s32 key) {
  struct ActorSystemWork *work = Ram_HeapSlots->actorSystem;
  u32 no;
  struct SpritePictureCache *p;

  no = 0;
  for (p = work->pictureCaches; no < SPRITE_PICTURE_CACHE_COUNT; no++, p++) {
    if (p->tag == key)
      return (s32)p->pictures;
  }
  return 0;
}

/// Resets each sprite animation of a sprite from its animation's metadata: its
/// facing kind, pictures, scripts and codec, with no picture shown and no
/// script running.
s32 Sprite_ResetAnimations(struct Sprite *state) {
  s32 index;
  struct SpriteAnimation **slot;

  index = 0;
  if (index < state->count) {
    slot = state->entries;
    do {
      struct SpriteAnimation *object = *slot++;
      struct AnimationMetadata *metadata =
          Resource_GetMetadataRecordFar(object->animationId);
      void **pictures;
      u8 **animation;

      if (metadata->width == 0)
        continue;

      if (index == 0) {
        state->width = metadata->width;
        state->height = metadata->height;
        state->scale = metadata->scale << 8;
        state->offsetY = metadata->adjustY;
        state->offsetX = metadata->adjustX;
      }

      pictures = metadata->pictures;
      if (pictures == 0)
        pictures = (void **)SpritePictureCache_FindByTag(object->animationId);

      object->kind = metadata->drawKind;
      animation = metadata->animation;
      // UB: the pictures are stored through a u8 ** view of the field, the
      // scripts' type, which keeps the store after the scripts' load; stored
      // as void **, it passes the load and the drawing kind's store.
#ifdef UBFIX
      object->pictures = pictures;
#else
      *(u8 ***)&object->pictures = (u8 **)pictures;
#endif
      object->scripts = animation;
      object->mode = metadata->pictureCodec;
      object->picture = 0xff;
      object->script = 0;
      object->scriptOffset = 0;
    } while (++index < state->count);
  }

  return 0;
}

/// Resets a sprite animation from its metadata: its pictures, scripts, codec
/// and drawing kind, with no script running and no picture shown.
void SpriteAnimation_Reset(struct SpriteAnimation *work) {
  s32 value;
  s32 animation;
  struct AnimationMetadata *info;

  if (work != NULL) {
    info = Resource_GetMetadataRecordFar(work->animationId);
    if (info->width != 0) {
      value = info->pictures;
      if (value == 0)
        value = SpritePictureCache_FindByTag(work->animationId);
      work->kind = info->drawKind;
      Io_Write32(0, (u32 *)&work->script);
      animation = info->animation;
      work->pictures = (void **)value;
      work->scripts = (u8 **)animation;
      work->scriptOffset = 0;
      work->mode = info->pictureCodec;
      work->picture = 0xff;
    }
  }
}

/// Adds a sprite animation of animation id to a sprite, in the first of its
/// four slots that is free; returns it, 0 when none could be taken, or -1 when
/// all four are in use.
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

/// Removes and clears a sprite's animation number no (0-3), trimming the
/// sprite's animation count when no later sprite animation remains.
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

/// Starts animation no on a sprite animation when its metadata has it; with
/// bit 7 of no set it keeps the place and timer of the script already running.
void SpriteAnimation_Start(struct SpriteAnimation *work, s32 no) {
  s32 hi;
  struct AnimationMetadata *info;
  s32 value;

  hi = 0x80 & no;
  if (work->scripts != 0) {
    info = Resource_GetMetadataRecordFar(work->animationId);
    if (no < info->animationCount) {
      value = (s32)work->scripts[no];
      work->kind = info->drawKind;
      work->script = (u8 *)value;
      work->step = 0x10;
      if (hi == 0) {
        work->scriptOffset = hi;
        work->timer = hi;
      }
    }
  }
}

/// Starts animation flags & 0x7f on every sprite animation of a sprite (bit 7
/// keeps each sprite animation's place and timer), unless the sprite plays it
/// already.
s32 Sprite_StartAnimation(struct Sprite *state, s32 flags) {
  s32 highBit;
  s32 index;

  highBit = flags & 0x80;
  flags &= 0x7f;

  if (state->lastNo != flags) {
    index = 0;
    goto test_entry;
  entry_loop:
    {
      struct SpriteAnimation *entry = state->entries[index];
      struct AnimationMetadata *metadata;
      s32 selectedAnimation;

      if (entry == 0)
        goto next_entry;
      if (entry->scripts == 0)
        goto next_entry;

      metadata = Resource_GetMetadataRecordFar(entry->animationId);
      if (flags >= metadata->animationCount)
        goto next_entry;

      selectedAnimation = (s32)entry->scripts[flags];
      entry->kind = metadata->drawKind;
      entry->script = (u8 *)selectedAnimation;
      entry->step = 0x10;
      if (highBit == 0) {
        entry->scriptOffset = 0;
        entry->timer = 0;
      }
      if (index == 0) {
        state->offsetY = metadata->adjustY;
        state->offsetX = metadata->adjustX;
      }
      goto next_entry;
    }
  next_entry:
    index++;
  test_entry:
    if (index < state->count)
      goto entry_loop;
    state->lastNo = flags;
  }
  return 0;
}

/// Sets each playing sprite animation's timer to the given duration in frames.
/// Returns 0.
s32 Sprite_SetAnimationTimers(struct Sprite *sprite, s32 frames) {
  s32 i;

  for (i = 0; i < sprite->count; i++) {
    struct SpriteAnimation *layer = sprite->entries[i];

    // The sprite animation timer counts sixteenths of a frame.
    if (layer != NULL && layer->scripts != NULL) {
      layer->timer = frames << 4;
    }
  }
  return 0;
}

/// Sets the timer step, the animation speed, of each playing sprite animation
/// of a sprite (16 is normal speed).
void Sprite_SetAnimationSpeed(struct Sprite *sprite, s32 step) {
  s32 i;

  for (i = 0; i < sprite->count; i++) {
    struct SpriteAnimation *layer = sprite->entries[i];

    if (layer != NULL && layer->scripts != NULL)
      layer->step = step;
  }
}

/// Takes the sprite and layer pools (from the fixed heap blocks in mode 3,
/// the heap otherwise) and clears them, loads the shadow tiles into VRAM cache
/// entry VRAM_CACHE_ENTRY_ACTOR_SHADOW and copies the strided tile copier into
/// RAM.
void ActorSystem_Configure(s32 mode) {
  void *sprites;
  void *layers;
  volatile u32 zero;

  if (mode == 3) {
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
  Palette_LoadBasicSpriteColors();
  Dma_Fill32Cell(zero, 0, sprites, ANIM_OBJECT_COUNT * sizeof(struct Sprite));
  Dma_Fill32Cell(zero, 0, layers,
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
  struct SpriteAnimation *object;
  s32 i;
  void **pictures;
  u8 **animation;

  found = NULL;
  metadata = Resource_GetMetadataRecordFar(id);
  entry = Ram_HeapSlots->spriteAnimations;
  object = NULL;

  if (metadata->width != 0) {
    for (i = 0; i < ANIM_OBJECT_COUNT; i++, entry++) {
      if (entry->kind == 0) {
        found = entry;
        break;
      }
    }
    if (found != NULL) {
      // UB: the word view aliases a pointer field. It keeps the zero load
      // ahead of pictures as native; reading the pointer swaps those loads.
#ifdef UBFIX
      pictures = metadata->pictures;
#else
      pictures = (void **)*(s32 *)&metadata->pictures;
#endif
      object = found;
      object->animationId = id;
      if (pictures == 0)
        pictures = (void **)SpritePictureCache_FindByTag(id);

      animation = metadata->animation;
      object->scripts = animation;
      object->pictures = pictures;
      object->mode = metadata->pictureCodec;
      object->picture = 0xff;
      object->script = animation[0];
      object->scriptOffset = 0;
      object->kind = metadata->drawKind;
      object->colorMode = 0;
    }
  }
  return object;
}

// Resources: clear an animation record.

/// Zeroes one SpriteAnimation; does nothing for NULL.
void SpriteAnimation_Clear(void *dst) {
  if (dst != 0) {
    volatile u32 zero;

    zero = 0;
    Dma_Set(&zero, dst,
            ((DMA_ENABLE | DMA_32BIT | DMA_SRC_FIXED) << 16) |
                (sizeof(struct SpriteAnimation) / 4),
            REG_DMA3);
  }
}

// Resources: create/release animated sprite objects and calculate frame
// durations.

/// Creates an animated sprite object for animation id, using one of 64 free
/// objects and a VRAM cache entry. Returns the object, or NULL on failure.
struct ResourceObject *Sprite_Create(s32 id) {
  struct AnimationMetadata *metadata;
  struct ResourceSprite *sprite;
  struct ResourceSprite *freeSprite;
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
  for (i = 0; i < 64; i++, sprite++) {
    if (sprite->width == 0) { // Width 0 marks a free object.
      freeSprite = sprite;
      break;
    }
  }
  if (freeSprite == NULL)
    return NULL;
  if (slot == VRAM_CACHE_ENTRY_COUNT)
    return NULL;
  tile = VramCache_Load(slot, 0, 0);
  if (tile == 0)
    return NULL;
  freeSprite->rotation = 0;
  freeSprite->flags = 1;
  freeSprite->slot = slot;

  // Derive OBJ shape and size bits (attributes 0 and 1) from pixel dimensions.
  switch ((u32)((metadata->width << 8) + metadata->height)) {
  case 0x0808:
    shape = OBJ_SIZE_8X8;
    break;
  case 0x0810:
    shape = OBJ_SIZE_8X16;
    break;
  case 0x1008:
    shape = OBJ_SIZE_16X8;
    break;
  case 0x1010:
    shape = OBJ_SIZE_16X16;
    break;
  case 0x1020:
    shape = OBJ_SIZE_16X32;
    break;
  case 0x2010:
    shape = OBJ_SIZE_32X16;
    break;
  case 0x2020:
    shape = OBJ_SIZE_32X32;
    break;
  case 0x2040:
    shape = OBJ_SIZE_32X64;
    break;
  case 0x4020:
    shape = OBJ_SIZE_64X32;
    break;
  case 0x4040:
    shape = OBJ_SIZE_64X64;
    break;
  default:
    shape = 0;
    break;
  }

  // Part 0 is the image; its shadow is part 1, from the shared shadow tiles.
  attribute = (u32 *)freeSprite;
  *attribute++ = 0;
  *attribute++ = shape | OBJ_256_COLORS;
  *attribute = tile | ANIM_OAM_FIELD_PRIORITY;
  attribute = freeSprite->shadowAttributes;
  *attribute++ = 0;
  *attribute++ = OBJ_SIZE_16X8 | OBJ_256_COLORS;
  *attribute = (gVramCacheEntries[VRAM_CACHE_ENTRY_ACTOR_SHADOW].offset >> 5) |
               ANIM_OAM_FIELD_PRIORITY;
  if (Sprite_AddAnimation(sprite, id) == -1) {
    /* FAKEMATCH(dead): a store of the byte it reloads. */
    freeSprite->dirty = freeSprite->dirty;
  }
  return freeSprite;
}

/// Releases a sprite: gives back its VRAM cache entry unless it keeps it
/// (retainCacheEntry), clears its four sprite animations and zeroes it.
void Sprite_Destroy(struct Sprite *sprite) {
  volatile u32 zero;
  s32 n;
  struct SpriteAnimation **layer;

  if (sprite) {
    if (!sprite->retainCacheEntry)
      VramCache_FreeEntry(sprite->slot);
    layer = sprite->entries;
    n = 3;
    do {
      SpriteAnimation_Clear(*layer++);
    } while (--n >= 0);
    zero = 0;
    Dma_Set(&zero, sprite,
            ((DMA_ENABLE | DMA_32BIT | DMA_SRC_FIXED) << 16) |
                (sizeof(*sprite) / 4),
            REG_DMA3);
  }
}

/// Sums the first cnt frame durations of animation id's entry number `no`.
/// Commands are two bytes (command, value); ANIMCMD_PLAY, ANIMCMD_HOLD,
/// ANIMCMD_JUMP and ANIMCMD_END end the list, and the values of ANIMCMD_WAIT,
/// ANIMCMD_BLANK and the frames below ANIMCMD_END count.
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
