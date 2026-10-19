/// @file
/// Part of field.c, the field object, compiled only through it.
/// Sprites, their animation layers and the resource slots they draw from.

#include "../field_internal.h"

struct ResourceSlotNumber {
  u16 resource;
  s16 number;
};

extern struct ResourceSlotNumber gResourceSlot_NumberTable[];

/// Loads the frames of animation id number into resource slot slot's buffer:
/// the resource listed for it in gResourceSlot_NumberTable, decoded, its frame
/// offsets turned into pointers, and for variant 1-5 its pixel colours (those
/// up to 0xdf) remapped through that variant's conversion table. Returns the
/// image's size in pixels (width * height), or 0 when the slot or id is not
/// known.
s32 ResourceSlot_Load(u32 slot, u32 *buffer, s32 number, u32 variant) {
  struct AnimationMetadata *metadata;
  struct ObjectSystemWork *work;
  struct ResourceSlot *record;
  struct ResourceSlotNumber *entry;
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
  work = gMenuCtrlWork;
  record = &work->slots[slot];
  metadata = Resource_GetMetadataRecordFar(number);
  record->tag = (slot << 12) | number;
  record->buffer = buffer;

  // Resolve the animation number to its packed resource.
  entry = gResourceSlot_NumberTable;
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
  size = Resource_DecodeType01(Resource_GetTableEntry(resource), buffer);

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
    // Values above 0xdf are kept; only the remappable palette range changes.
    colorMap = gResourceSlot_ConversionTables[index];
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

/// Returns the buffer loaded into the resource slot with this tag, or 0.
s32 Animation_LookupValueByKey(s32 tag) {
  u32 i;
  struct ResourceSlot *slot = gMenuCtrlWork->slots;

  for (i = 0; i < RESOURCE_SLOT_COUNT; i++, slot++) {
    if (slot->tag == tag)
      return (s32)slot->buffer;
  }
  return 0;
}

/// Resets each layer of a sprite from its animation's metadata: its facing
/// kind, frames, scripts and codec, with no frame shown and no script
/// running. The first layer also sets the sprite's size, scale and offsets.
/// Returns 0.
s32 Animation_InitializeObjects(struct AnimationObject *sprite) {
  s32 i;

  for (i = 0; i < sprite->count; i++) {
    struct AnimationEntry *layer = sprite->entries[i];
    struct AnimationMetadata *metadata =
        Resource_GetMetadataRecordFar(layer->animId);
    void **frames;

    if (metadata->width == 0)
      continue;

    if (i == 0) {
      // The first layer supplies the dimensions and origin of the whole sprite.
      sprite->width = metadata->width;
      sprite->height = metadata->height;
      sprite->scale = metadata->scale << 8;
      sprite->offsetY = metadata->adjustY;
      sprite->offsetX = metadata->adjustX;
    }

    // Some frame directories are supplied by the resource-slot cache.
    frames = metadata->frames;
    if (frames == 0)
      frames = (void **)Animation_LookupValueByKey(layer->animId);

    layer->kind = metadata->drawKind;
    layer->frames = frames;
    layer->scripts = metadata->animation;
    layer->mode = metadata->frameCodec;
    layer->frame = 0xff;
    layer->script = 0;
    layer->pos = 0;
  }

  return 0;
}

/// Resets one animation layer from its animation's metadata, as
/// Animation_InitializeObjects does for each layer.
void Animation_InitWorkFromMetadata(struct AnimationEntry *layer) {
  void **frames;
  s32 zero;
  struct AnimationMetadata *metadata;

  if (layer != NULL) {
    metadata = Resource_GetMetadataRecordFar(layer->animId);
    if (metadata->width != 0) {
      // Keep the layer inactive until an animation script is selected.
      frames = metadata->frames;
      if (frames == 0)
        frames = (void **)Animation_LookupValueByKey(layer->animId);
      layer->kind = metadata->drawKind;
      layer->scripts = metadata->animation;
      layer->frames = frames;
      layer->mode = metadata->frameCodec;
      zero = 0;
      layer->frame = 0xff;
      layer->script = (u8 *)zero;
      layer->pos = zero;
    }
  }
}

/// Adds a layer of animation id to a sprite, in its first free layer of
/// four. The first layer also sets the sprite's size, scale and offsets.
/// Returns the layer, 0 when no layer record is free, or -1 when the sprite
/// has four layers already.
s32 ResourceMetadata_Register(struct AnimationObject *sprite, s32 id) {
  s32 layerHandle = (s32)sprite->entries[0];
  s32 freeIndex = 0;
  struct AnimationEntry **entry;
  struct AnimationMetadata *metadata;

  // Find an unused layer slot before reserving a layer record.
  if (layerHandle != 0) {
    entry = &sprite->entries[0];
    do {
      freeIndex++;
      if (freeIndex >= ANIMATION_LAYER_COUNT)
        break;
      entry++;
      layerHandle = (s32)*entry;
    } while (layerHandle != 0);
  }
  if (freeIndex == ANIMATION_LAYER_COUNT)
    return -1;
  layerHandle = (s32)AnimationObject_Allocate(id);
  if (layerHandle == 0)
    return 0;
  sprite->entries[freeIndex] = (struct AnimationEntry *)layerHandle;
  metadata = Resource_GetMetadataRecordFar(id);
  // Only the first registered layer establishes the sprite's geometry.
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

/// Removes the layer handle from a sprite and clears it, trimming the
/// sprite's layer count when no later layer remains.
void ResourceMetadata_Unregister(struct AnimationObject *sprite, s32 layer) {
  s32 laterLayers;
  u32 index;
  u32 i;

  if (sprite != NULL && layer != 0) {
    ResourceMetadata_ClearRecord((struct AnimationEntry *)layer);
    // Locate the handle, then trim only empty slots at the end of the list.
    for (index = 0; index < ANIMATION_LAYER_COUNT; index++) {
      if (layer == (s32)sprite->entries[index])
        break;
    }
    if (index != ANIMATION_LAYER_COUNT) {
      sprite->entries[index] = 0;
      laterLayers = 0;
      for (i = index + 1; i < ANIMATION_LAYER_COUNT; i++) {
        if (sprite->entries[i] != 0)
          laterLayers++;
      }
      if (laterLayers == 0)
        sprite->count = index;
    }
  }
}

/// Removes and clears a sprite's layer number no (0-3), trimming the sprite's
/// layer count when no later layer remains.
void ResourceMetadata_ReleaseSlot(struct AnimationObject *sprite, u32 index) {
  struct AnimationEntry *layer;
  s32 laterLayers;
  u32 i;

  if (sprite != NULL && index < ANIMATION_LAYER_COUNT) {
    layer = sprite->entries[index];
    if (layer != NULL) {
      ResourceMetadata_ClearRecord(layer);
      sprite->entries[index] = NULL;
      laterLayers = 0;
      // Interior holes stay available; only trailing holes reduce the count.
      for (i = index + 1; i < ANIMATION_LAYER_COUNT; i++) {
        if (sprite->entries[i] != NULL)
          laterLayers++;
      }
      if (laterLayers == 0)
        sprite->count = index;
    }
  }
}

/// Starts the indexed animation on a layer when its metadata contains it.
/// Bit 7 preserves the command cursor and timer; it remains part of the index.
void Animation_SetWorkEntry(struct AnimationEntry *layer, s32 animation) {
  s32 keepPlace;
  struct AnimationMetadata *metadata;
  s32 scriptAddress;

  keepPlace = 0x80 & animation;
  if (layer->scripts != 0) {
    metadata = Resource_GetMetadataRecordFar(layer->animId);
    if (animation < metadata->animationCount) {
      scriptAddress = (s32)layer->scripts[animation];
      layer->kind = metadata->drawKind;
      layer->script = (u8 *)scriptAddress;
      layer->step = ANIMATION_NORMAL_STEP;
      if (keepPlace == 0) {
        layer->pos = keepPlace;
        layer->timer = keepPlace;
      }
    }
  }
}

/// Starts animation flags & 0x7f on every layer of a sprite (bit 7 keeps
/// each layer's place and timer), unless the sprite plays it already. The
/// first layer's animation also sets the sprite's offsets. Returns 0.
s32 AnimationObjects_SelectAnimation(struct AnimationObject *sprite,
                                     s32 animation) {
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
    // Only the first layer supplies the sprite's offsets.
    {
      struct AnimationEntry *layer = sprite->entries[i];
      struct AnimationMetadata *metadata;
      s32 selectedAnimation;

      if (layer != 0 && layer->scripts != 0) {
        metadata = Resource_GetMetadataRecordFar(layer->animId);
        if (animation < metadata->animationCount) {
          selectedAnimation = (s32)layer->scripts[animation];
          layer->kind = metadata->drawKind;
          layer->script = (u8 *)selectedAnimation;
          layer->step = ANIMATION_NORMAL_STEP;
          if (keepPlace == 0) {
            layer->pos = 0;
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

/// Sets each animated layer's timer to the given duration in frames.
/// Returns 0.
s32 AnimationObjects_SetActiveFrameTimers(struct AnimationObject *sprite,
                                          s32 frames) {
  u8 count = sprite->count;
  struct AnimationEntry **layerList;
  s32 remaining;

  if (count != 0) {
    // The layer timer counts sixteenths of a frame.
    frames <<= 4;
    layerList = sprite->entries;
    remaining = count;
    do {
      struct AnimationEntry *layer = *layerList++;
      if (layer != 0 && layer->scripts != 0)
        layer->timer = frames;
      remaining--;
    } while (remaining != 0);
  }
  return 0;
}

/// Sets the timer step, the animation speed, of each animated layer of a
/// sprite (16 is normal speed).
void AnimationObjects_SetActiveTimerSteps(struct AnimationObject *sprite,
                                          s32 step) {
  u8 count = sprite->count;
  if (count != 0) {
    struct AnimationEntry **layerList = sprite->entries;
    s32 remaining = count;
    do {
      struct AnimationEntry *layer = *layerList++;
      if (layer != 0 && layer->scripts != 0)
        layer->step = step;
      remaining--;
    } while (remaining != 0);
  }
}

/// Allocates and zeroes the 64 sprites and 64 animation layers (from the
/// fixed blocks in mode 3, the heap otherwise), sets up the palette DMA,
/// loads the shadow tiles into VRAM cache entry 93 and puts the sprite tile
/// uploader in RAM.
void ObjectSystem_Configure(s32 mode) {
  void *sprites;
  void *layers;
  volatile u32 zero;

  // World-map mode keeps the sprite and layer pools in the fixed heap blocks.
  if (mode == 3) {
    sprites = Runtime_AllocateBlock(4, ANIMATION_OBJECT_COUNT *
                                           sizeof(struct AnimationObject));
    layers = Runtime_AllocateBlock(3, ANIMATION_OBJECT_COUNT *
                                          sizeof(struct AnimationEntry));
  } else {
    sprites = Runtime_AllocateHeapBlock(4, ANIMATION_OBJECT_COUNT *
                                               sizeof(struct AnimationObject));
    layers = Runtime_AllocateHeapBlock(3, ANIMATION_OBJECT_COUNT *
                                              sizeof(struct AnimationEntry));
  }
  // Clear both pools, then prepare the shared shadow and tile uploader.
  Palette_LoadBasicObjColors();
  Dma_Fill32Cell(zero, 0, sprites,
                 ANIMATION_OBJECT_COUNT * sizeof(struct AnimationObject));
  Dma_Fill32Cell(zero, 0, layers,
                 ANIMATION_OBJECT_COUNT * sizeof(struct AnimationEntry));
  VramCache_Load(93, 128, gObject_ShadowTiles);
  RamCode_LoadSlot(HEAP_SLOT_FRAME_UPLOADER, Tile_CopyStrided,
                   Tile_CopyStridedCodeSize);
}

/// Takes a free animation layer record and starts it on animation id's first
/// animation. Returns the layer, or NULL when none is free or the id has no
/// image.
struct AnimationEntry *AnimationObject_Allocate(s32 id) {
  struct AnimationMetadata *metadata;
  struct AnimationEntry *candidate;
  struct AnimationEntry *found;
  struct AnimationEntry *layer;
  s32 i;
  void **frames;
  u8 **scripts;

  found = NULL;
  metadata = Resource_GetMetadataRecordFar(id);
  candidate = gAnimationObjects[0];
  layer = NULL;

  if (metadata->width != 0) {
    // A zero facing kind marks a free layer record.
    for (i = 0; i < ANIMATION_OBJECT_COUNT; i++, candidate++) {
      if (candidate->kind == 0) {
        found = candidate;
        break;
      }
    }
    if (found != NULL) {
      // Bind the metadata and start its first animation at command zero.
      // Read as a word, the frames load does not wait on the animId store, so
      // the zero for pos and param loads first as native; as metadata->frames,
      // 1 of 61 instructions differs in tbs-en.
      frames = (void **)*(s32 *)&metadata->frames;
      layer = found;
      layer->animId = id;
      if (frames == 0)
        frames = (void **)Animation_LookupValueByKey(id);

      scripts = metadata->animation;
      layer->scripts = scripts;
      layer->frames = frames;
      layer->mode = metadata->frameCodec;
      layer->frame = 0xff;
      layer->script = scripts[0];
      layer->pos = 0;
      layer->kind = metadata->drawKind;
      layer->param = 0;
    }
  }
  return layer;
}

/// Zeroes one animation layer (struct AnimationEntry); does nothing for NULL.
void ResourceMetadata_ClearRecord(struct AnimationEntry *record) {
  if (record != 0) {
    Dma_Fill32(0, record, sizeof(*record));
  }
}

// Creating and releasing animated sprites, and measuring animation lengths.

/// Creates a sprite for animation id, taking one of the 64 free sprites and a
/// VRAM cache entry for its tiles. Returns the sprite, or NULL when there is
/// no room or the id has no image.
struct AnimationObject *ResourceObject_Create(s32 id) {
  struct AnimationMetadata *metadata;
  struct AnimationObject *sprite;
  struct AnimationObject *freeSprite;
  u32 *attribute;
  s32 slot;
  s32 tile;
  s32 i;
  u32 shape;

  freeSprite = NULL;
  metadata = Resource_GetMetadataRecordFar(id);
  slot = Resource_FindFreeEntry();
  sprite = gSpriteObjects;
  if (metadata->width == 0)
    return NULL;
  // A width of 0 marks a free sprite.
  for (i = 0; i < ANIMATION_OBJECT_COUNT; i++, sprite++) {
    if (sprite->width == 0) {
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
  freeSprite->slot = slot;
  freeSprite->rotation = 0;
  freeSprite->flags = 1;

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
  // 16x8, 256-colour sprite of the shadow tiles (VRAM cache 93), priority 2.
  attribute = (u32 *)freeSprite;
  *attribute++ = 0;
  *attribute++ = shape | 0x2000;
  *attribute++ = tile | 0x800;
  *attribute++ = 0;
  *attribute++ = 0x6000;
  *attribute = (gVramBlockCache[93].offset >> 5) | 0x800;
  if (ResourceMetadata_Register(sprite, id) == -1) {
    /* FAKEMATCH(dead): a store of the byte it reloads keeps the -1 the native
     * builds after the call and never uses; with the call alone, 2 of 152
     * instructions differ in tbs-en. */
    freeSprite->dirty = freeSprite->dirty;
  }
  return freeSprite;
}

/// Releases a sprite: its VRAM cache entry (unless retainCacheEntry is set),
/// its four animation layers, and then zeroes its 56 bytes.
void ResourceObject_Release(struct AnimationObject *sprite) {
  s32 remaining;
  struct AnimationEntry **layer;

  if (sprite) {
    // Release dependent records before clearing the sprite itself.
    if (!sprite->retainCacheEntry)
      Resource_ResetEntry(sprite->slot);
    layer = sprite->entries;
    remaining = ANIMATION_LAYER_COUNT - 1;
    do {
      ResourceMetadata_ClearRecord(*layer++);
    } while (--remaining >= 0);
    Dma_Fill32(0, sprite, sizeof(*sprite));
  }
}

/// Returns how long the first frameCount frames of animation number animation
/// of id last: the sum of the arguments of its frame, wait and blank commands,
/// up to frameCount of them or the first play, hold, jump or end command.
s32 ResourceMetadata_SumCommandLengths(s32 id, u32 animation, s32 frameCount) {
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
    if (command == ANIM_CMD_PLAY || command == ANIM_CMD_HOLD ||
        command == ANIM_CMD_JUMP || command == ANIM_CMD_END) {
      break;
    }
    if (command == ANIM_CMD_WAIT || command == ANIM_CMD_BLANK ||
        command < ANIM_CMD_END) {
      durationSum += duration;
      frameCount--;
      if (frameCount == 0) {
        break;
      }
    }
  }
  return durationSum;
}
