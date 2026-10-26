/// @file
/// Sprites, their anims and the frame caches they draw from.
// Part of field.c.

#include "../field_internal.h"

enum {
  ANIM_SHADOW_CACHE_ENTRY = 93,
  ANIM_SHADOW_TILE_BYTES = 16 * 8, ///< 16x8 pixels, 256 colours
  ANIM_OAM_FULL_COLOR = 1 << 13,
  /// Priority 2, the field's for its sprites: in front of a map layer at 3, or
  /// at 2 since a sprite wins a tie, and behind one at 1.
  ANIM_OAM_FIELD_PRIORITY = 2 << 10,
  ANIM_OAM_WIDE = 1 << 14
};

/// A row of gSpriteFrameCache_Sources: the packed resource that holds an
/// animation number's frames, ending at number 0.
struct SpriteFrameCacheSource {
  u16 resource;
  s16 number;
};

extern struct SpriteFrameCacheSource gSpriteFrameCache_Sources[];

/// Loads the frames of animation id number into frame cache slot's
/// buffer: the resource listed for it in gSpriteFrameCache_Sources, decoded,
/// its frame offsets turned into pointers, and for variant 1-5 its pixel
/// colours (those up to 0xdf, the basic sprite palettes 0 to 13) remapped
/// through that variant's conversion table; a variant above 5 uses variant 1's.
/// The caller owns buffer, which must hold the decoded resource. Returns the
/// image's size in pixels (width * height), or 0 when the slot or id is not
/// known. Sets the slot's tag to (slot << 12) | number and its buffer; with
/// a null buffer and number 0 it does only that, which is how the battle frees
/// a frame cache. Called by the sprite viewer
/// and, as SpriteFrameCache_LoadFar, by the battle for its enemy sprites
/// (variant is the class's palette).
s32 SpriteFrameCache_Load(u32 slot, u32 *buffer, s32 number, u32 variant) {
  struct AnimationMetadata *metadata;
  struct ObjectSystemWork *system;
  struct SpriteFrameCache *record;
  struct SpriteFrameCacheSource *entry;
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
  system = gObjectSystemWork;
  record = &system->frameCaches[slot];
  metadata = Resource_GetMetadataRecordFar(number);
  record->tag = (slot << 12) | number;
  record->frames = buffer;

  // Resolve the animation number to its packed resource.
  entry = gSpriteFrameCache_Sources;
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

  // Turn the frame-offset directory into pointers before the pixel payload.
  offset = buffer;
  for (count = 0; count < 256; count++) {
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
    // Values above 0xdf (palettes 14 and 15, which
    // Palette_LoadBasicSpriteColors does not load) are kept; only the basic
    // palettes' colours change.
    colorMap = (u8 *)gSpriteColorTables[index];
    for (; pixels < end; pixels++) {
      u32 colorIndex = *pixels;

      if (colorIndex <= 0xdf) {
        colorIndex = colorMap[colorIndex];
        *pixels = colorIndex;
      }
    }
  }
  return metadata->width * metadata->height;
}

/// Returns the buffer loaded into the sprite frame cache with this tag, or 0.
/// An animation id that carries its slot in bits 12 and up finds its frames
/// here; called by the anim set-up functions below.
s32 SpriteFrameCache_FindByTag(s32 tag) {
  u32 i;
  struct SpriteFrameCache *slot = gObjectSystemWork->frameCaches;

  for (i = 0; i < RESOURCE_SLOT_COUNT; i++, slot++) {
    if (slot->tag == tag)
      return (s32)slot->frames;
  }
  return 0;
}

/// Resets each anim of a sprite from its animation's metadata: its facing
/// kind, frames, scripts and codec, with no frame shown and no script
/// running. The first anim also sets the sprite's size, scale and offsets.
/// Returns 0. Called by Anim_SetIndexAndInitializeObjects.
s32 Sprite_ResetAnims(struct Sprite *sprite) {
  s32 i;

  for (i = 0; i < sprite->count; i++) {
    struct SpriteAnim *layer = sprite->entries[i];
    struct AnimationMetadata *metadata =
        Resource_GetMetadataRecordFar(layer->animationId);
    void **frames;

    if (metadata->width == 0)
      continue;

    if (i == 0) {
      // The first anim supplies the dimensions and origin of the whole sprite.
      sprite->width = metadata->width;
      sprite->height = metadata->height;
      sprite->scale = metadata->scale << 8;
      sprite->offsetY = metadata->adjustY;
      sprite->offsetX = metadata->adjustX;
    }

    // An animation whose metadata has no frames takes them from the resource
    // slot loaded under its id (SpriteFrameCache_Load).
    frames = metadata->frames;
    if (frames == 0)
      frames = (void **)SpriteFrameCache_FindByTag(layer->animationId);

    layer->kind = metadata->drawKind;
    layer->frames = frames;
    layer->scripts = metadata->animation;
    layer->mode = metadata->frameCodec;
    // Frame 255 and no script: Sprite_ComposeFrame skips the anim
    // until an animation is selected on it.
    layer->frame = 0xff;
    layer->script = 0;
    layer->scriptOffset = 0;
  }

  return 0;
}

/// Resets one sprite anim from its animation's metadata, as
/// Sprite_ResetAnims does for each anim. Called by the sprite
/// viewer.
void SpriteAnim_Reset(struct SpriteAnim *layer) {
  void **frames;
  s32 zero;
  struct AnimationMetadata *metadata;

  if (layer != NULL) {
    metadata = Resource_GetMetadataRecordFar(layer->animationId);
    if (metadata->width != 0) {
      // Keep the anim inactive until an animation script is selected.
      frames = metadata->frames;
      if (frames == 0)
        frames = (void **)SpriteFrameCache_FindByTag(layer->animationId);
      layer->kind = metadata->drawKind;
      layer->scripts = metadata->animation;
      layer->frames = frames;
      layer->mode = metadata->frameCodec;
      zero = 0;
      layer->frame = 0xff;
      layer->script = (u8 *)zero;
      layer->scriptOffset = zero;
    }
  }
}

/// Adds an anim of animation id to a sprite, in its first free anim of
/// four. The first anim also sets the sprite's size, scale and offsets.
/// Returns the anim, 0 when no sprite anim is free, or -1 when the sprite
/// has four anims already. Called by Sprite_Create for a sprite's
/// first anim, by Object_RegisterSpriteMetadata and the sprite
/// viewer, and as Sprite_AddAnimFar by the battle, the Psynergy
/// effects and the field map's actors.
s32 Sprite_AddAnim(struct Sprite *sprite, s32 id) {
  s32 layerHandle = (s32)sprite->entries[0];
  s32 freeIndex = 0;
  struct SpriteAnim **entry;
  struct AnimationMetadata *metadata;

  // Find an unused anim slot before reserving a sprite anim.
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
  layerHandle = (s32)SpriteAnim_Alloc(id);
  if (layerHandle == 0)
    return 0;
  sprite->entries[freeIndex] = (struct SpriteAnim *)layerHandle;
  metadata = Resource_GetMetadataRecordFar(id);
  // Only the first registered anim establishes the sprite's geometry.
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

/// Removes the anim handle from a sprite and clears it, trimming the
/// sprite's anim count when no later anim remains. Called through
/// Sprite_RemoveAnimFar by the battle's objects.
void Sprite_RemoveAnim(struct Sprite *sprite, s32 layer) {
  s32 laterLayers;
  u32 index;
  u32 i;

  if (sprite != NULL && layer != 0) {
    SpriteAnim_Clear((struct SpriteAnim *)layer);
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

/// Removes and clears a sprite's anim number no (0-3), trimming the sprite's
/// anim count when no later anim remains.
void Sprite_RemoveAnimAt(struct Sprite *sprite, u32 index) {
  struct SpriteAnim *layer;
  s32 laterLayers;
  u32 i;

  if (sprite != NULL && index < ANIM_LAYER_COUNT) {
    layer = sprite->entries[index];
    if (layer != NULL) {
      SpriteAnim_Clear(layer);
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

// Unlike Sprite_StartAnim it does not mask bit 7 off, so with bit 7 set the
// index would pass animationCount only in a set of more than 128 animations.
// Nothing sets it: the 309 play commands in the sprites' animation scripts all
// name animations below 128, and the battle passes 0 or 1.
/// Starts the indexed animation on a sprite anim when its metadata contains it.
/// Bit 7 preserves the command cursor and timer; it remains part of the index.
/// Reached through the animation script's play command (Sprite_ComposeFrame)
/// and, as SpriteAnim_StartFar, from the battle's motion and objects.
void SpriteAnim_Start(struct SpriteAnim *layer, s32 animation) {
  s32 keepPlace;
  struct AnimationMetadata *metadata;
  s32 scriptAddress;

  keepPlace = 0x80 & animation;
  if (layer->scripts != 0) {
    metadata = Resource_GetMetadataRecordFar(layer->animationId);
    if (animation < metadata->animationCount) {
      scriptAddress = (s32)layer->scripts[animation];
      layer->kind = metadata->drawKind;
      layer->script = (u8 *)scriptAddress;
      layer->step = ANIM_NORMAL_STEP;
      if (keepPlace == 0) {
        layer->scriptOffset = keepPlace;
        layer->timer = keepPlace;
      }
    }
  }
}

/// Starts animation flags & 0x7f on every anim of a sprite (bit 7 keeps
/// each anim's place and timer), unless the sprite plays it already. The
/// first anim's animation also sets the sprite's offsets. Returns 0.
/// The engine's usual way to change what a sprite plays: called by the
/// object dispatch and the player control, and as
/// Sprite_StartAnimFar by the battle, the shops, the menus,
/// the battle animations, the Psynergy effects and the map scripts.
s32 Sprite_StartAnim(struct Sprite *sprite, s32 animation) {
  s32 keepPlace;
  s32 i;

  keepPlace = animation & 0x80;
  animation &= 0x7f;

  // The loop is written with goto, which keeps an 8-byte frame: as for (i = 0;
  // i < sprite->count; i++), loop.c strength-reduces entries[i] into a spilled
  // cursor and 16 of 76 instructions differ in tbs-en.
  if (sprite->lastNo != animation) {
    i = 0;
    goto test_entry;
  entry_loop:
    // Only the first anim supplies the sprite's offsets.
    {
      struct SpriteAnim *layer = sprite->entries[i];
      struct AnimationMetadata *metadata;
      s32 selectedAnimation;

      if (layer != 0 && layer->scripts != 0) {
        metadata = Resource_GetMetadataRecordFar(layer->animationId);
        if (animation < metadata->animationCount) {
          selectedAnimation = (s32)layer->scripts[animation];
          layer->kind = metadata->drawKind;
          layer->script = (u8 *)selectedAnimation;
          layer->step = ANIM_NORMAL_STEP;
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

/// Sets each playing anim's timer to the given duration in frames.
/// Returns 0.
s32 Sprite_SetAnimTimers(struct Sprite *sprite, s32 frames) {
  u8 count = sprite->count;
  struct SpriteAnim **layerList;
  s32 remaining;

  if (count != 0) {
    // The anim timer counts sixteenths of a frame.
    frames <<= 4;
    layerList = sprite->entries;
    remaining = count;
    do {
      struct SpriteAnim *layer = *layerList++;
      if (layer != 0 && layer->scripts != 0)
        layer->timer = frames;
      remaining--;
    } while (remaining != 0);
  }
  return 0;
}

/// Sets the timer step, the animation speed, of each playing anim of a
/// sprite (16 is normal speed). Called by the object dispatch and, as
/// Sprite_SetAnimSpeedFar, by the Sanctum, which stops a
/// party member's icon with 0 and restarts it with 16.
void Sprite_SetAnimSpeed(struct Sprite *sprite, s32 step) {
  u8 count = sprite->count;
  if (count != 0) {
    struct SpriteAnim **layerList = sprite->entries;
    s32 remaining = count;
    do {
      struct SpriteAnim *layer = *layerList++;
      if (layer != 0 && layer->scripts != 0)
        layer->step = step;
      remaining--;
    } while (remaining != 0);
  }
}

/// Allocates and zeroes the 64 sprites and 64 sprite anims
/// (HEAP_SLOT_SPRITES and HEAP_SLOT_SPRITE_ANIMS, from EWRAM
/// first in OBJECT_SYSTEM_WORLD_MAP and IWRAM first otherwise), loads the basic
/// sprite palettes, loads the shadow tiles into VRAM cache entry 93 and puts
/// the sprite tile uploader in RAM. Called by ObjectSystem_Initialize.
/// Battle_Run (OBJECT_SYSTEM_BATTLE) takes both slots from EWRAM, at
/// the same sizes, before it starts the object system, so in battle this gets
/// those blocks back.
void ObjectSystem_Configure(s32 mode) {
  void *sprites;
  void *layers;
  volatile u32 zero;

  // On the world map (OBJECT_SYSTEM_WORLD_MAP) the two pools are taken from
  // EWRAM first; elsewhere from IWRAM first, though a battle has already taken
  // both from EWRAM.
  if (mode == 3) {
    sprites = AllocHeapSlotEwram(HEAP_SLOT_SPRITES,
                                 ANIM_OBJECT_COUNT * sizeof(struct Sprite));
    layers = AllocHeapSlotEwram(HEAP_SLOT_SPRITE_ANIMS,
                                ANIM_OBJECT_COUNT * sizeof(struct SpriteAnim));
  } else {
    sprites = AllocHeapSlot(HEAP_SLOT_SPRITES,
                            ANIM_OBJECT_COUNT * sizeof(struct Sprite));
    layers = AllocHeapSlot(HEAP_SLOT_SPRITE_ANIMS,
                           ANIM_OBJECT_COUNT * sizeof(struct SpriteAnim));
  }
  // Load the sprites' basic colours, clear both pools, then load the shared
  // shadow (128 bytes: the two 8x8 tiles of a 16x8, 256-colour sprite) and
  // the tile uploader.
  Palette_LoadBasicSpriteColors();
  Dma_Fill32Cell(zero, 0, sprites, ANIM_OBJECT_COUNT * sizeof(struct Sprite));
  Dma_Fill32Cell(zero, 0, layers,
                 ANIM_OBJECT_COUNT * sizeof(struct SpriteAnim));
  VramCache_Load(ANIM_SHADOW_CACHE_ENTRY, ANIM_SHADOW_TILE_BYTES,
                 gObject_ShadowTiles);
  RamCode_LoadSlot(HEAP_SLOT_FRAME_UPLOADER, Tile_CopyStrided,
                   Tile_CopyStridedCodeSize);
}

/// Takes a free sprite anim and starts it on animation id's first
/// animation. Returns the anim, or NULL when none is free or the id has no
/// image. Called by Sprite_AddAnim.
struct SpriteAnim *SpriteAnim_Alloc(s32 id) {
  struct AnimationMetadata *metadata;
  struct SpriteAnim *candidate;
  struct SpriteAnim *found;
  struct SpriteAnim *layer;
  s32 i;
  void **frames;
  u8 **scripts;

  found = NULL;
  metadata = Resource_GetMetadataRecordFar(id);
  candidate = gSpriteAnims[0];
  layer = NULL;

  if (metadata->width != 0) {
    // A zero facing kind marks a free sprite anim.
    for (i = 0; i < ANIM_OBJECT_COUNT; i++, candidate++) {
      if (candidate->kind == 0) {
        found = candidate;
        break;
      }
    }
    if (found != NULL) {
      // Bind the metadata and start its first animation at command zero.
      // UB: the word view aliases a pointer field. It keeps the zero load
      // ahead of frames as native; reading the pointer swaps those loads.
#ifdef UBFIX
      frames = metadata->frames;
#else
      frames = (void **)*(s32 *)&metadata->frames;
#endif
      layer = found;
      layer->animationId = id;
      if (frames == 0)
        frames = (void **)SpriteFrameCache_FindByTag(id);

      scripts = metadata->animation;
      layer->scripts = scripts;
      layer->frames = frames;
      layer->mode = metadata->frameCodec;
      layer->frame = 0xff;
      layer->script = scripts[0];
      layer->scriptOffset = 0;
      layer->kind = metadata->drawKind;
      layer->colorMode = 0;
    }
  }
  return layer;
}

/// Zeroes one sprite anim (struct SpriteAnim); does nothing for NULL.
/// Zeroing kind frees the anim for SpriteAnim_Alloc. Called by the
/// anim release functions here and, as SpriteAnim_ClearFar, by
/// the Psynergy effects and map scripts.
void SpriteAnim_Clear(struct SpriteAnim *record) {
  if (record != 0) {
    Dma_Fill32(0, record, sizeof(*record));
  }
}

// Creating and releasing animated sprites, and measuring animation lengths.

/// Creates a sprite for animation id, taking one of the 64 free sprites and a
/// VRAM cache entry for its tiles. Returns the sprite, or NULL when there is
/// no room or the id has no image. The caller owns it until
/// Sprite_Destroy. When no sprite anim is free the sprite is still
/// returned, with no anim and a width of 0, so it also counts as free. Called
/// by Object_CreateField and the sprite viewer, and as Sprite_CreateFar
/// by the battle, the battle animations, the Psynergy effects and the menus.
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
  // Sprite_ComposeFrame that draws the sprite gives it width * height
  // bytes. A 0 tile means no VRAM block is free, or that block 0 is the
  // first free one (VramCache_Load).
  tile = VramCache_Load(slot, 0, 0);
  if (tile == 0)
    return NULL;
  freeSprite->slot = slot;
  freeSprite->rotation = 0;
  freeSprite->flags = SPRITE_FLAG_SHADOW;

  // The OAM shape and size bits (attributes 0 and 1) for the image's width
  // and height in pixels.
  switch ((u32)((metadata->width << 8) + metadata->height)) {
  case 0x0808: // 8x8
    shape = 0;
    break;
  case 0x0810: // 8x16
    shape = 0x8000;
    break;
  case 0x1008: // 16x8
    shape = 0x4000;
    break;
  case 0x1010: // 16x16
    shape = 0x40000000;
    break;
  case 0x1020: // 16x32
    shape = 0x80008000;
    break;
  case 0x2010: // 32x16
    shape = 0x80004000;
    break;
  case 0x2020: // 32x32
    shape = 0x80000000;
    break;
  case 0x2040: // 32x64
    shape = 0xc0008000;
    break;
  case 0x4020: // 64x32
    shape = 0xc0004000;
    break;
  case 0x4040: // 64x64
    shape = 0xc0000000;
    break;
  default:
    shape = 0;
    break;
  }

  // Part 0 is the image: 256 colours, priority 2. Part 1 is its shadow: a
  // 16x8, 256-colour sprite of the shared shadow tiles, priority 2.
  // UB: the native word cursor aliases the sprite and crosses its members.
  // UBFIX copies the same representation from an actual word array.
#ifdef UBFIX
  attributes[0] = 0;
  attributes[1] = shape | ANIM_OAM_FULL_COLOR;
  attributes[2] = tile | ANIM_OAM_FIELD_PRIORITY;
  attributes[3] = 0;
  attributes[4] = ANIM_OAM_FULL_COLOR | ANIM_OAM_WIDE;
  attributes[5] = (gVramCacheEntries[ANIM_SHADOW_CACHE_ENTRY].offset >> 5) |
                  ANIM_OAM_FIELD_PRIORITY;
  memcpy(freeSprite, attributes, sizeof(attributes));
#else
  attribute = (u32 *)freeSprite;
  *attribute++ = 0;
  *attribute++ = shape | ANIM_OAM_FULL_COLOR;
  *attribute++ = tile | ANIM_OAM_FIELD_PRIORITY;
  *attribute++ = 0;
  *attribute++ = ANIM_OAM_FULL_COLOR | ANIM_OAM_WIDE;
  *attribute = (gVramCacheEntries[ANIM_SHADOW_CACHE_ENTRY].offset >> 5) |
               ANIM_OAM_FIELD_PRIORITY;
#endif
  if (Sprite_AddAnim(sprite, id) == -1) {
    /* FAKEMATCH(dead): a store of the byte it reloads keeps the -1 the native
     * builds after the call and never uses; with the call alone, 2 of 152
     * instructions differ in tbs-en. */
    freeSprite->dirty = freeSprite->dirty;
  }
  return freeSprite;
}

/// Releases a sprite: its VRAM cache entry (unless retainCacheEntry is set),
/// its four sprite anims, and then zeroes its 56 bytes. Called by
/// Object_Release and, as Sprite_DestroyFar, by the battle
/// and its animations, the Psynergy effects and the menus.
void Sprite_Destroy(struct Sprite *sprite) {
  s32 remaining;
  struct SpriteAnim **layer;

  if (sprite) {
    // Release its VRAM cache entry and anims before clearing the sprite itself.
    if (!sprite->retainCacheEntry)
      VramCache_FreeEntry(sprite->slot);
    layer = sprite->entries;
    remaining = ANIM_LAYER_COUNT - 1;
    do {
      SpriteAnim_Clear(*layer++);
    } while (--remaining >= 0);
    Dma_Fill32(0, sprite, sizeof(*sprite));
  }
}

/// Returns how long the first frameCount frames of animation number animation
/// of id last: the sum of the arguments of its frame, wait and blank commands,
/// up to frameCount of them or the first play, hold, jump or end command.
/// Called through SpriteAnim_SumCommandLengthsFar by the battle's
/// staging.
s32 SpriteAnim_SumCommandLengths(s32 id, u32 animation, s32 frameCount) {
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
      frameCount--;
      if (frameCount == 0) {
        break;
      }
    }
  }
  return durationSum;
}
