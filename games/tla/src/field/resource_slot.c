// ---- games/tla/src/field/resource_slot.c ----
#include "battle.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
/* VIEW(resource_slot): routine AnimationObject_Allocate as s32 AnimationObject_Allocate(s32 id); the definition has struct AnimationEntry *AnimationObject_Allocate(s32 id); tried as the definition's struct AnimationEntry *AnimationObject_Allocate(s32 id), which the object already declares, it does not compile in tla-ja: games/tla/src/field/resource_slot.c:482: conflicting types for `AnimationObject_Allocate'. */
extern s32 AnimationObject_Allocate__resource_slot(s32 id) __asm__("AnimationObject_Allocate");
#define AnimationObject_Allocate AnimationObject_Allocate__resource_slot
#include "system.h"
#undef AnimationObject_Allocate
#include "types.h"
#include "resource.h"
#include "sprite.h"
#include "sprite_metadata.h"
#include "ram_buffer.h"

enum { RESOURCE_SLOT_COUNT = 8 };

struct ResourceSlotNumber {
  u16 resource;
  s16 number;
};

extern struct ResourceSlotNumber gResourceSlot_NumberTable[];

s32 ResourceSlot_Load(u32 slot, u32 *buffer, s32 number, u32 variant) {
  struct AnimationMetadata *metadata;
  struct ObjectSystemWork *work;
  struct ResourceSlot *record;
  struct ResourceSlotNumber *entry;
  u32 *offset;
  u8 *text;
  u8 *end;
  u8 *table;
  u32 size;
  u32 count;
  u32 index;
  u16 entryNumber;
  s32 resource;

  if (slot >= RESOURCE_SLOT_COUNT)
    return 0;
  work = Ram_HeapSlots->objectSystem;
  record = &work->slots[slot];
  metadata = Resource_GetMetadataRecordFar(number);
  record->tag = (slot << 12) | number;
  record->buffer = buffer;

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

s32 Animation_LookupValueByKey(s32 key) {
  struct ObjectSystemWork *work = Ram_HeapSlots->objectSystem;
  u32 no;
  struct ResourceSlot *p;

  no = 0;
  for (p = work->slots; no < RESOURCE_SLOT_COUNT; no++, p++) {
    if (p->tag == key)
      return (s32)p->buffer;
  }
  return 0;
}

// actor_view.h's ActorSpriteView, with the bytes this function writes named
// from the projection draft's layout (field_sprite.h); its other members as
// the tree has them.
/* VIEW(resource_slot): tag struct ActorSpriteView as struct ActorSpriteView__resource_slot; actor_view.h has another definition; tried with the definition the rest of the object has, it does not compile in tla-ja: games/tla/src/field/resource_slot.c:113: dereferencing pointer to incomplete type. */
struct ActorSpriteView__resource_slot {
  u8 unk00[5];
  u8 unk05Low : 2;
  u8 stateFlags : 2;
  u8 unk05High : 4;
  u8 unk06[6];
  s32 scale;
  u8 unk10;
  u8 unk11Low : 1;
  u8 displayFlag : 1;
  u8 unk11High : 6;
  s16 value12;
  u8 width;
  u8 height;
  s8 offsetX;
  s8 offsetY;
  u8 unk18;
  u8 dirty;
  u8 value1a;
  u8 count;
  u8 unk1c[0x0c];
  struct AnimationEntry *entries[4];
};
s32 InitializeAnimationObjects(struct ActorSpriteView__resource_slot *state) {
  s32 index;
  struct AnimationEntry **slot;

  index = 0;
  if (index < state->count) {
    slot = state->entries;
    do {
      struct AnimationEntry *object = *slot++;
      struct AnimationMetadata *metadata =
          Resource_GetMetadataRecordFar(object->animId);
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
        pictures = (void **)Animation_LookupValueByKey(object->animId);

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
      object->pos = 0;
    } while (++index < state->count);
  }

  return 0;
}

#include "debug.h"

void Animation_InitializeWorkFromMetadata(struct AnimationEntry *work) {
  s32 value;
  s32 z;
  s32 animation;
  struct AnimationMetadata *info;

  if (work != NULL) {
    info = Resource_GetMetadataRecordFar(work->animId);
    if (info->width != 0) {
      value = info->pictures;
      if (value == 0)
        value = Animation_LookupValueByKey(work->animId);
      z = 0;
      work->kind = info->drawKind;
      work->script = (u8 *)z;
      /* FAKEMATCH(dead): an empty once-only block keeps the kind's store and
       * the script's clear ahead of the lists' stores; without it, 6 of 30
       * instructions differ in tla-en. */
      do {
      } while (0);
      animation = info->animation;
      work->pictures = (void **)value;
      work->scripts = (u8 **)animation;
      work->pos = z;
      work->mode = info->pictureCodec;
      work->picture = 0xff;
    }
  }
}

struct MetadataSlotState {
  u8 unk00[0x0c];
  s32 scale;
  u8 unk10[4];
  u8 width;
  u8 height;
  s8 offsetX;
  s8 offsetY;
  u8 unk18[3];
  u8 count;
  u8 unk1c[0x0c];
  s32 slots[4];
};

s32 ResourceMetadata_Register(struct MetadataSlotState *state, s32 id) {
  s32 value = state->slots[0];
  s32 index = 0;
  s32 *slot;
  struct AnimationMetadata *metadata;

  if (value != 0) {
    slot = &state->slots[0];
    do {
      index++;
      if (index > 3)
        break;
      slot++;
      value = *slot;
    } while (value != 0);
  }
  if (index == 4)
    return -1;
  value = AnimationObject_Allocate__resource_slot(id);
  if (value == 0)
    return 0;
  state->slots[index] = value;
  metadata = Resource_GetMetadataRecordFar(id);
  if (state->count == 0) {
    state->width = metadata->width;
    state->height = metadata->height;
    state->scale = metadata->scale << 8;
    state->offsetY = metadata->adjustY;
    state->offsetX = metadata->adjustX;
  }
  if (index == state->count)
    state->count = index + 1;
  return value;
}

void ResourceMetadata_Unregister(struct MetadataSlotState *state, s32 handle) {
  s32 count;
  u32 slot;
  u32 i;

  if (state != NULL && handle != 0) {
    ResourceMetadata_ClearRecord((void *)handle);
    for (slot = 0; slot <= 3; slot++) {
      if (handle == state->slots[slot])
        break;
    }
    if (slot != 4) {
      state->slots[slot] = 0;
      count = 0;
      for (i = slot + 1; i <= 3; i++) {
        if (state->slots[i] != 0)
          count++;
      }
      if (count == 0)
        state->count = slot;
    }
  }
}

void ResourceMetadata_ReleaseSlot(struct MetadataSlotState *state, u32 no) {
  s32 slot;
  s32 count;
  u32 i;

  if (state != NULL && no <= 3) {
    slot = state->slots[no];
    if (slot != 0) {
      ResourceMetadata_ClearRecord((void *)slot);
      state->slots[no] = 0;
      count = 0;
      for (i = no + 1; i <= 3; i++) {
        if (state->slots[i] != 0)
          count++;
      }
      if (count == 0)
        state->count = no;
    }
  }
}

void Animation_SetWorkEntry(struct AnimationEntry *work, s32 no) {
  s32 hi;
  struct AnimationMetadata *info;
  s32 value;

  hi = 0x80 & no;
  if (work->scripts != 0) {
    info = Resource_GetMetadataRecordFar(work->animId);
    if (no < info->animationCount) {
      value = (s32)work->scripts[no];
      work->kind = info->drawKind;
      work->script = (u8 *)value;
      work->step = 0x10;
      if (hi == 0) {
        work->pos = hi;
        work->timer = hi;
      }
    }
  }
}

struct AnimationGroup {
  u8 unk00[0x16];
  s8 offsetX;
  s8 offsetY;
  u8 lastNo;
  u8 unk19[2];
  u8 count;
  u8 unk1c[0x0c];
  struct AnimationEntry *entries[4];
};

s32 Animation_ApplyChildArgument(struct AnimationGroup *state, s32 flags) {
  s32 highBit;
  s32 index;

  highBit = flags & 0x80;
  flags &= 0x7f;

  if (state->lastNo != flags) {
    index = 0;
    goto test_entry;
  entry_loop:
    {
      struct AnimationEntry *entry = state->entries[index];
      struct AnimationMetadata *metadata;
      s32 selectedAnimation;

      if (entry == 0)
        goto next_entry;
      if (entry->scripts == 0)
        goto next_entry;

      metadata = Resource_GetMetadataRecordFar(entry->animId);
      if (flags >= metadata->animationCount)
        goto next_entry;

      selectedAnimation = (s32)entry->scripts[flags];
      entry->kind = metadata->drawKind;
      entry->script = (u8 *)selectedAnimation;
      entry->step = 0x10;
      if (highBit == 0) {
        entry->pos = 0;
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

struct ChildObject {
  u8 unk00[2];
  u16 value;
  u8 unk04[8];
  s32 active;
  u8 unk10[5];
  u8 mode;
};

struct ChildGroup {
  u8 unk00[27];
  u8 count;
  u8 unk1c[12];
  struct ChildObject *children[1];
};

s32 Func_08022b80(struct ChildGroup *group, s32 value) {
  s32 i;

  for (i = 0; i < group->count; i++) {
    struct ChildObject *child = group->children[i];

    if (child != NULL && child->active != 0) {
      child->value = value << 4;
    }
  }
  return 0;
}

void Animation_ApplyChildValue(struct ChildGroup *group, s32 value) {
  s32 i;

  for (i = 0; i < group->count; i++) {
    struct ChildObject *child = group->children[i];

    if (child != NULL && child->active != 0)
      *((u8 *)child + 21) = value;
  }
}

// ---- games/tla/src/field/vram_block_93.c ----
#include "heap_release.h"
#include "io_reg.h"
#include "dma.h"

#define ANIMATION_OBJECT_COUNT 64

s32 Animation_LookupValueByKey(s32 key);
void Palette_LoadBasicObjColors(void);

extern const u8 gData_0802e89c[];
extern u8 Render_CopyStridedCode[];
extern u8 Render_CopyStridedCodeSize[];

/// Takes the sprite and layer pools (from the fixed heap blocks in mode 3,
/// the heap otherwise) and clears them, loads the shadow tiles into VRAM cache
/// entry 93 and copies the strided tile copier into RAM.
// As ☀️'s function of the same name.
void ObjectSystem_Configure(s32 mode) {
  void *sprites;
  void *layers;
  volatile u32 zero;

  if (mode == 3) {
    sprites = HeapSlot_AllocEwram(16, ANIMATION_OBJECT_COUNT *
                                          sizeof(struct AnimationObject));
    layers = HeapSlot_AllocEwram(12, ANIMATION_OBJECT_COUNT *
                                         sizeof(struct AnimationEntry));
  } else {
    sprites = HeapSlot_Alloc(16, ANIMATION_OBJECT_COUNT *
                                     sizeof(struct AnimationObject));
    layers = HeapSlot_Alloc(12, ANIMATION_OBJECT_COUNT *
                                    sizeof(struct AnimationEntry));
  }
  Palette_LoadBasicObjColors();
  Dma_Fill32Cell(zero, 0, sprites,
                 ANIMATION_OBJECT_COUNT * sizeof(struct AnimationObject));
  Dma_Fill32Cell(zero, 0, layers,
                 ANIMATION_OBJECT_COUNT * sizeof(struct AnimationEntry));
  VramCache_Load(93, 128, gData_0802e89c);
  {
    u32 size = (u32)Render_CopyStridedCodeSize;

    Dma_Set(Render_CopyStridedCode, HeapSlot_Alloc(84, size),
            DMA_COPY_WORDS(size >> 2), REG_DMA3);
  }
}

void VramBlock_Reset93(void) { Resource_ResetEntry(93); }

void VramBlock_Load93(void) {
  const u8 *data = gData_0802e89c;
  s32 size = 128;

  VramCache_Load(93, size, data);
}

struct AnimationEntry *AnimationObject_Allocate(s32 id) {
  struct AnimationMetadata *metadata;
  struct AnimationEntry *entry;
  struct AnimationEntry *found;
  struct AnimationEntry *object;
  s32 i;
  void **pictures;
  u8 **animation;

  found = NULL;
  metadata = Resource_GetMetadataRecordFar(id);
  entry = Ram_HeapSlots->unk0c[0];
  object = NULL;

  if (metadata->width != 0) {
    for (i = 0; i < ANIMATION_OBJECT_COUNT; i++, entry++) {
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
      object->animId = id;
      if (pictures == 0)
        pictures = (void **)Animation_LookupValueByKey(id);

      animation = metadata->animation;
      object->scripts = animation;
      object->pictures = pictures;
      object->mode = metadata->pictureCodec;
      object->picture = 0xff;
      object->script = animation[0];
      object->pos = 0;
      object->kind = metadata->drawKind;
      object->param = 0;
    }
  }
  return object;
}

// Resources: clear an animation record.

/// Zeroes one AnimationEntry; does nothing for NULL.
void ResourceMetadata_ClearRecord(void *dst) {
  if (dst != 0) {
    volatile u32 zero;

    zero = 0;
    Dma_Set(&zero, dst,
            ((DMA_ENABLE | DMA_32BIT | DMA_SRC_FIXED) << 16) |
                (sizeof(struct AnimationEntry) / 4),
            REG_DMA3);
  }
}

// Resources: create/release animated sprite objects and calculate frame
// durations.

#include "edition.h"

struct ResourceObjectWork {
  u8 unk00[16];
  u8 resource; ///< VRAM cache index.
  u8 flags;    ///< Bit 0: do not return the VRAM cache entry.
  u8 unk12[22];
  void *children[4];
};

typedef char
    ResourceObjectWork_size[sizeof(struct ResourceObjectWork) == 56 ? 1 : -1];

#include "vram_tab.h"

/// The sprite as ResourceObject_Create fills it: the image's three attribute
/// words, its layout, then the shadow's three words.
struct ResourceSprite {
  u32 imageAttributes[3];
  s32 scale;
  u8 slot;
  u8 retainCacheEntry : 1;
  u8 useOwnScale : 1;
  u8 unk11 : 6;
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
  struct AnimationEntry *entries[4];
};


/// Creates an animated sprite object for animation id, using one of 64 free
/// objects and a VRAM cache entry. Returns the object, or NULL on failure.
// As ☀️'s function of the same name.
struct ResourceObject *ResourceObject_Create(s32 id) {
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
  slot = Resource_FindFreeEntry();
  sprite = Ram_HeapSlots->unk0c[1];
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

  // Part 0: 256 colors, priority 2.
  // Part 1: 16x8, 256 colors, tiles from VRAM cache 93, priority 2.
  attribute = (u32 *)freeSprite;
  *attribute++ = 0;
  *attribute++ = shape | 0x2000;
  *attribute = tile | 0x800;
  attribute = freeSprite->shadowAttributes;
  *attribute++ = 0;
  *attribute++ = 0x6000;
  *attribute = (gResourceTableEntries[93].offset >> 5) | 0x800;
  if (ResourceMetadata_Register(sprite, id) == -1) {
    /* FAKEMATCH(dead): a store of the byte it reloads keeps the -1 built after
     * the call and never used; with the call alone, 20 of 154 instructions
     * differ in tla-en. */
    freeSprite->dirty = freeSprite->dirty;
  }
  return (struct ResourceObject *)freeSprite;
}

/// Releases an object: gives back its VRAM cache entry unless flags bit 0 is
/// set, releases its four children and zeroes the 56-byte workspace.
void ResourceObject_Release(struct ResourceObjectWork *work) {
  volatile u32 zero;
  s32 n;
  void **child;

  if (work) {
    if (!(work->flags & 1))
      Resource_ResetEntry(work->resource);
    child = work->children;
    n = 3;
    do {
      ResourceMetadata_ClearRecord(*child++);
    } while (--n >= 0);
    zero = 0;
    Dma_Set(&zero, work,
            ((DMA_ENABLE | DMA_32BIT | DMA_SRC_FIXED) << 16) |
                (sizeof(*work) / 4),
            REG_DMA3);
  }
}

#include "scene.h"

/// Sums the first cnt frame durations of animation id's entry number `no`.
/// Commands are two bytes (command, value); 254, 241, 253 and 239 end the
/// list, and the values of commands 245, 255 and those up to 238 count.
s32 ResourceMetadata_SumCommandLengths(s32 id, u32 no, s32 cnt) {
  struct AnimationMetadata *info;
  u8 *p;
  u8 op;
  u8 val;
  s32 sum = 0;

  info = Resource_GetMetadataRecordFar(id);
  if (no >= info->animationCount) {
    return 0;
  }
  p = ((u8 **)info->animation)[no];
  for (;;) {
    op = p[0];
    val = p[1];
    p += 2;
    if (op == 254 || op == 241 || op == 253 || op == 239) {
      break;
    }
    if (op == 245 || op == 255 || op <= 238) {
      sum += val;
      cnt--;
      if (cnt == 0) {
        break;
      }
    }
  }
  return sum;
}
#undef ANIMATION_OBJECT_COUNT

// ---- games/tla/src/field/map/render_animated_tile_picture.c ----
struct AnimTrack {
  u8 unk00[0x10];
  u8 *script;
  u8 index;
};

struct AnimObject {
  u8 unk00[0x18];
  u8 frame;
  u8 unk19[0xf];
  struct AnimTrack *track;
};

void Animation_WaitForTrackEnd(struct AnimObject *object) {
  u8 frame = object->frame;
  struct AnimTrack *track = object->track;
  s32 i;

  if (object != 0) {
    i = 0;
    if (track->script[track->index] != 241) {
      do {
        wait(1);
        i++;
        if (i > 89)
          break;
        if (track->script[track->index] == 241)
          break;
      } while (object->frame == frame);
    }
  }
}

void Map_RenderAnimatedTilePicture(u8 *object, u32 position) {
  u16 *destination;
  s32 count;
  u32 row;
  u8 *table;
  u32 highMask;
  u32 indexMask;
  u8 offsetMask;

  destination =
      (u16 *)(0x06010000 + gResourceTableEntries[object[0x10]].offset);
  count = (object[0x14] * object[0x15]) / 64;
  row = 0;

  if (row < (u32)count) {
    table = gMap_TileDissolveOrder;
    highMask = 0xFF00;
    indexMask = 0x3F;
    offsetMask = 0x3E;

    for (; row < (u32)count; row++, destination += 32, position++) {
      if ((u32)(position - 0x40) <= 0x3F) {
        u8 entry;
        u16 *cell;
        u32 parity;

        entry = table[(position + row * 16) & indexMask];
        cell = (u16 *)((u8 *)destination + (entry & offsetMask));
        parity = 1;
        parity &= entry;
        if (parity != 0)
          *cell = *(u8 *)cell;
        else
          *cell &= highMask;
      }
    }
  }
}

void Map_RenderAnimatedTilePicturesForObject(u8 *object) {
  u32 pos;

  pos = 0;
  do {
    Map_RenderAnimatedTilePicture(object, pos);
    Map_RenderAnimatedTilePicture(object, pos + 1);
    Map_RenderAnimatedTilePicture(object, pos + 2);
    Map_RenderAnimatedTilePicture(object, pos + 3);
    pos += 4;
    wait(1);
  } while (pos <= 0x7f);
}

void Map_RenderAllAnimatedTilePictures(u8 **table, s32 cnt) {
  u8 **top;
  u8 **p;
  s32 n;
  u32 pos;
  u32 pos1;
  u32 pos2;
  u32 pos3;

  top = table;
  pos = 0;
  do {
    if (cnt > 0) {
      pos1 = pos + 1;
      p = top;
      pos2 = pos + 2;
      pos3 = pos + 3;
      n = cnt;
      do {
        Map_RenderAnimatedTilePicture(*p, pos);
        Map_RenderAnimatedTilePicture(*p, pos1);
        Map_RenderAnimatedTilePicture(*p, pos2);
        n -= 1;
        Map_RenderAnimatedTilePicture(*p++, pos3);
      } while (n != 0);
    }
    wait(1U);
    pos += 4;
  } while (pos <= 0x7FU);
}

struct Thing {
  u8 filler0[8];
  s32 field8;
  u8 filler12[4];
  s32 field16;
};

s32 Map_GetScreenRelativePosition(struct Thing *obj, s32 *out) {
  u8 *state = (u8 *)Ram_HeapSlots->mapWork;
  s32 *org = (s32 *)(state + 228);
  s32 a;
  s32 b;
  s32 x;
  s32 y;

  a = org[0] & 0xffff0000;
  b = org[1] & 0xffff0000;
  x = obj->field8 - a;
  y = obj->field16 - b;
  if ((u32)(x + 0x001fffff) <= 0x012ffffe && y > 0 && y < 0xe00000) {
    *out++ = x >> 16;
    *out = y >> 16;
    return 0;
  }
  *out++ = 0;
  *out = 0;
  return -1;
}

#include "task.h"

struct MapAnimationWork {
  u8 unk00[6];
  u8 unk06;
  u8 unk07;
  u8 unk08[84];
};

void Func_0802493c(void);
void Func_08023f9c(void);
void Func_080246b8(void);
void Func_0802386c(void);

/// Sets up the map animation work for map mode: its 92-byte work and an 8 KB
/// buffer, both cleared by DMA from one zero word, and the animation callbacks
/// (mode 4 runs its own; modes 3 and 4 take ObjectSystem_UpdateCameraFixed at
/// 0x480, the others Func_0802386c).
// No ☀️ twin found; ☀️'s field.c starts its map animations the same way.
void Func_080230e0(s32 mode) {
  struct MapAnimationWork *work;
  void *buffer;
  // The fill word Dma_Fill32Cell reads its zero from, one for both fills.
  volatile u32 zero;

  work = HeapSlot_AllocEwram(24, sizeof(*work));
  buffer = HeapSlot_AllocEwram(20, 0x2000);
  ObjectSystem_Configure(mode);
  Dma_Fill32Cell(zero, 0, buffer, 0x2000);
  Dma_Fill32Cell(zero, 0, work, sizeof(*work));
  if (mode == 4) {
    Task_Add(Func_0802493c, TASK_PASS_VBLANK + 10);
  } else {
    Task_Add(Func_08023f9c, TASK_PASS_VBLANK + 11);
    Task_Add(Func_080246b8, TASK_PASS_VBLANK + 10);
  }
  if (mode == 3 || mode == 4)
    Task_Add(ObjectSystem_UpdateCameraFixed, TASK_PASS_VBLANK);
  else
    Task_Add(Func_0802386c, TASK_PASS_VBLANK);
  work->unk06 = 15;
  work->unk07 = 0;
}

/// Two empty functions follow; nothing in the ROM refers to either.
void Func_0802319c(void) {}

void Func_080231a0(void) {}

// Objects: create and release field objects, and render them each frame.

#include "objdisp.h"
#include "actor.h"
/* VIEW(render_animated_tile_picture): routine Animation_ApplyChildArgument as s32 Animation_ApplyChildArgument(struct ActorSpriteView *, s32); the definition has s32 Animation_ApplyChildArgument(struct AnimationGroup *state, s32 flags); tried as the definition's s32 Animation_ApplyChildArgument(struct AnimationGroup *state, s32 flags), which the object already declares, it does not compile in tla-ja: games/tla/include/actor_view.h:65: conflicting types for `Animation_ApplyChildArgument'. */
struct ActorSpriteView;
extern s32 Animation_ApplyChildArgument__render_animated_tile_picture(struct ActorSpriteView *, s32) __asm__("Animation_ApplyChildArgument");
#define Animation_ApplyChildArgument Animation_ApplyChildArgument__render_animated_tile_picture
/* VIEW(render_animated_tile_picture): routine Animation_ApplyChildValue as void Animation_ApplyChildValue(struct ActorSpriteView *, s32); the definition has void Animation_ApplyChildValue(struct ChildGroup *group, s32 value); tried as the definition's void Animation_ApplyChildValue(struct ChildGroup *group, s32 value), which the object already declares, it does not compile in tla-ja: games/tla/include/actor_view.h:66: conflicting types for `Animation_ApplyChildValue'. */
struct ActorSpriteView;
extern void Animation_ApplyChildValue__render_animated_tile_picture(struct ActorSpriteView *, s32) __asm__("Animation_ApplyChildValue");
#define Animation_ApplyChildValue Animation_ApplyChildValue__render_animated_tile_picture
/* VIEW(render_animated_tile_picture): routine InitializeAnimationObjects as s32 InitializeAnimationObjects(struct ActorSpriteView *); the definition has s32 InitializeAnimationObjects(struct ActorSpriteView__resource_slot *state); tried as the definition's s32 InitializeAnimationObjects(struct ActorSpriteView__resource_slot *state), which the object already declares, it does not compile in tla-ja: games/tla/include/actor_view.h:67: conflicting types for `InitializeAnimationObjects'. */
struct ActorSpriteView;
extern s32 InitializeAnimationObjects__render_animated_tile_picture(struct ActorSpriteView *) __asm__("InitializeAnimationObjects");
#define InitializeAnimationObjects InitializeAnimationObjects__render_animated_tile_picture
/* VIEW(render_animated_tile_picture): routine ResourceMetadata_Register as s32 ResourceMetadata_Register(struct ActorSpriteView *, s32); the definition has s32 ResourceMetadata_Register(struct MetadataSlotState *state, s32 id); tried as the definition's s32 ResourceMetadata_Register(struct MetadataSlotState *state, s32 id), which the object already declares, it does not compile in tla-ja: games/tla/include/actor_view.h:68: conflicting types for `ResourceMetadata_Register'. */
struct ActorSpriteView;
extern s32 ResourceMetadata_Register__render_animated_tile_picture(struct ActorSpriteView *, s32) __asm__("ResourceMetadata_Register");
#define ResourceMetadata_Register ResourceMetadata_Register__render_animated_tile_picture
#include "actor_view.h"
#undef ResourceMetadata_Register
#undef InitializeAnimationObjects
#undef Animation_ApplyChildValue
#undef Animation_ApplyChildArgument
#include "ram_code.h"
#include "battle_system.h"
#include "transform.h"
#include "macros.h"

/// Returns an unused object (one without a script) from the 64 slots, or 0
/// if none is free.
void *Actor_FindFreeObject(void) {
  struct Actor *obj = Ram_HeapSlots->scriptObjects;
  void *ret = 0;
  s32 n = 0;

  while (n <= 63) {
    if (obj->script == 0) {
      ret = obj;
      break;
    }
    n++;
    obj++;
  }
  return ret;
}

/// Releases the object's graphics and zeroes the object to free it.
/// obj: object.
void Actor_Release(struct Actor *obj) {
  volatile u32 zero;
  volatile u32 *src;
  s32 n;
  struct ResourceObjectWork **child;

  if (obj) {
    switch (obj->animationKind & 15) {
    case 1: // One graphic.
      ResourceObject_Release(
          (struct ResourceObjectWork *)((struct ActorSpriteView *)obj->sprite));
      break;
    case 2: // Up to four graphics.
      child = (struct ResourceObjectWork **)((struct ActorSpriteView **)
                                                 obj->sprite);
      n = 3;
      do {
        struct ResourceObjectWork *res = *child++;
        if (res)
          ResourceObject_Release(res);
      } while (--n >= 0);
      break;
    }
    src = &zero;
    *src = 0;
    Dma_Set(src, obj,
            (DMA_ENABLE | DMA_32BIT | DMA_SRC_FIXED) << 16 |
                (sizeof(struct Actor) / 4),
            REG_DMA3);
  }
}


struct Actor *Func_08023220(s32 id, s32 x, s32 y, s32 z) {
  struct Actor *object;
  void *sprite;
  s32 kind;
  void **entry;
  volatile u32 zero;

  kind = id / 4096;
  id &= 0xfff;
  object = Actor_FindFreeObject();
  if (object != NULL) {
    object->radius = 16;
    switch (kind) {
    case 0:
      sprite = ResourceObject_Create(id);
      if (sprite != NULL) {
        object->animationKind = 1;
        object->sprite = sprite;
        object->radius = Resource_GetMetadataRecordFar(id)->boxY >> 1;
      } else {
        object->animationKind = 0;
      }
      break;
    case 2:
      entry = (void **)((u32 *)Ram_HeapSlots->objectSystem +
                        Ram_HeapSlots->objectSystem->count++ + 2);
      object->animationKind = kind;
      object->sprite = entry;
      zero = 0;
      Dma_Set(&zero, entry, 0x85000004, REG_DMA3);
      sprite = ResourceObject_Create(id);
      if (sprite != NULL) {
        object->radius = Resource_GetMetadataRecordFar(id)->boxY >> 1;
        *entry = sprite;
        entry++;
      }
      sprite = ResourceObject_Create(id + 1);
      if (sprite != NULL)
        *entry = sprite;
      break;
    }
  }
  if (object != NULL) {
    Actor_SetPositionAndResetMotion(object, x, y, z);
    object->script = (s32 *)gActorScript_EndScript;
    object->speedLimit = 0x20000;
    object->step = 0;
    object->scaleX = 0x10000;
    object->scaleY = 0x10000;
    object->acceleration = 0x10000;
    object->motionFlags = 3;
    object->gravity = 0x10000;
    object->bounce = 0x4000;
    object->collisionFlags = 0;
    object->actionFlags = 1;
    object->facing = 0x4000;
    object->unk4c = 0;
    object->timer = x / 65536;
    object->unk66 = z / 65536;
    *(s32 *)&object->unk70[12] = 0;
    *(s32 *)&object->unk70[0] = 0;
    *(s32 *)&object->unk70[4] = 0;
    *(s32 *)&object->unk70[8] = 0;
  }
  return object;
}

/// Points an object at its script and clears its script state.
void Actor_StartScript(struct Actor *object, const s32 *script) {
  if (object != 0) {
    object->step = 0;
    object->script = script;
    object->movementState = 0;
    object->unk5c[1] = 0;
    object->unk57 = 0;
  }
}

void Actor_SelectAnimation(struct Actor *object, s32 argument) {
  struct ActorSpriteView **items;
  struct ActorSpriteView *item;
  s32 count;

  if (object != 0) {
    switch (object->animationKind & 0xf) {
    case 1:
      Animation_ApplyChildArgument__render_animated_tile_picture(((struct ActorSpriteView *)object->sprite),
                                   argument);
      break;
    case 2:
      items = ((struct ActorSpriteView **)object->sprite);
      count = 3;
      do {
        item = *items++;
        if (item != 0)
          Animation_ApplyChildArgument__render_animated_tile_picture(item, argument);
        count--;
      } while (count >= 0);
      break;
    }
  }
}

void Actor_SetAnimationSpeed(struct Actor *object, s32 value) {
  s32 count;
  struct ActorSpriteView *child;
  struct ActorSpriteView **children;

  if (object != 0) {
    switch (object->animationKind & 0xf) {
    case 1:
      Animation_ApplyChildValue__render_animated_tile_picture(((struct ActorSpriteView *)object->sprite),
                                value);
      return;
    case 2:
      children = ((struct ActorSpriteView **)object->sprite);
      count = 3;
      do {
        child = *children++;
        if (child != 0)
          Animation_ApplyChildValue__render_animated_tile_picture(child, value);
        count--;
      } while (count >= 0);
      break;
    }
  }
}

void Actor_SetAnimationAndSpeed(struct Actor *object, s32 argument, s32 value) {
  struct ActorSpriteView **items;
  struct ActorSpriteView *item;
  s32 count;

  if (object != 0) {
    switch (object->animationKind & 0xf) {
    case 1:
      Animation_ApplyChildArgument__render_animated_tile_picture(((struct ActorSpriteView *)object->sprite),
                                   argument);
      Animation_ApplyChildValue__render_animated_tile_picture(((struct ActorSpriteView *)object->sprite),
                                value);
      break;
    case 2:
      items = ((struct ActorSpriteView **)object->sprite);
      for (count = 3; count >= 0; count--) {
        item = *items++;
        if (item != 0) {
          Animation_ApplyChildArgument__render_animated_tile_picture(item, argument);
          Animation_ApplyChildValue__render_animated_tile_picture(item, value);
        }
      }
      break;
    }
  }
}

void Actor_SetSpriteRotation(struct Actor *object, u32 value) {
  if (object != 0 && (object->animationKind & 0xf) == 1)
    ((struct ActorSpriteView *)object->sprite)->value12 = value;
}

void Animation_SetIndexAndInitializeObjects(struct Actor *object, s32 index) {
  struct ActorSpriteView *child;

  if (object != NULL && (object->animationKind & 0xf) == 1) {
    child = ((struct ActorSpriteView *)object->sprite);
    if (index >= 0) {
      child->entries[0]->animId = index;
      InitializeAnimationObjects__render_animated_tile_picture(child);
    }
  }
}

void Actor_RegisterSpriteMetadata(struct Actor *object, s32 value) {
  if (object != 0 && (object->animationKind & 0xf) == 1) {
    struct ActorSpriteView *child = ((struct ActorSpriteView *)object->sprite);
    if (value >= 0)
      ResourceMetadata_Register__render_animated_tile_picture(child, value);
  }
}

#include "main_routine_work.h"

extern const s32 gData_0802f098[];
extern const s32 gData_0802f0b0[];
extern const s32 gData_0802f0c8[];
extern const s32 gData_0802f0e0[];
extern const s32 gData_0802f0f8[];
extern const s32 gData_0802f110[];
extern const s32 gData_0802f128[];
extern const s32 gData_0802f140[];
extern const s32 gData_0802f158[];
extern const s32 gData_0802f170[];
extern const s32 gData_0802f188[];
extern const s32 gData_0802f1a0[];
extern const s32 gData_0802f1b8[];
extern const s32 gData_0802f1e8[];

/// Starts an actor on the script that approaches its watched actor
/// (SC_APPROACH_WATCHED), watching watched.
void Actor_StartApproachWatched(struct Actor *actor, struct Actor *watched) {
  Actor_StartScript(actor, gData_0802f1e8);
  actor->watchedObject = watched;
}

/// Starts the dispatch script for gMainRoutineWork's render mode, waiting a
/// frame first in mode 9.
void Func_08023524(struct Actor *object) {
  const s32 *script;
  struct MainRoutineWork *party = &gMainRoutineWork;

  switch (party->renderMode) {
  case 0:
    script = gData_0802f140;
    break;
  case 1:
    script = gData_0802f158;
    break;
  case 2:
    script = gData_0802f0b0;
    break;
  case 3:
    script = gData_0802f0c8;
    break;
  case 4:
    script = gData_0802f0e0;
    break;
  case 5:
    script = gData_0802f0f8;
    break;
  case 6:
    script = gData_0802f170;
    break;
  case 7:
    script = gData_0802f188;
    break;
  case 8:
    script = gData_0802f1a0;
    break;
  case 9:
    wait(1);
    script = gData_0802f1b8;
    break;
  case 10:
    script = gData_0802f110;
    break;
  case 11:
    script = gData_0802f128;
    break;
  case 12:
    script = gActorScript_EndScript;
    break;
  default:
    script = gData_0802f098;
    break;
  }
  Actor_StartScript(object, script);
}

void Actor_StartIdle(struct Actor *object) {
  Actor_StartScript(object, gActorScript_EndScript);
}

void Actor_StartCamera(struct Actor *object, struct Actor *watched) {
  Actor_StartScript(object, gActor_CameraScript);
  if (watched != NULL) {
    object->acceleration = 0x8000;
    object->speedLimit = 0x40000;
    object->watchedObject = watched;
    object->timer = 0;
  }
}

void Actor_WaitForCommandEnd(struct Actor *object) {
  s32 cnt = 0;

  if (object->script != 0 &&
      ((const s32 *)object->script)[object->step] != 17) {
    do {
      wait(1);
      cnt++;
      if (cnt > 599)
        break;
    } while (object->script != 0 &&
             ((const s32 *)object->script)[object->step] != 17);
  }
}

void Actor_SetChildByteValue(struct Actor *object, u32 value) {
  if (object != NULL && (object->animationKind & 0xf) == 1)
    ((struct ActorSpriteView *)object->sprite)->value1a = value;
}

// ---- games/tla/src/field/object/f023680.c ----
void Animation_SetStateFlags(struct Actor *object, u32 value) {
  if (object != NULL && object->animationKind == 1)
    ((struct ActorSpriteView *)object->sprite)->stateFlags = value;
}

void Animation_SetDisplayFlag(struct Actor *object, u32 value) {
  if (object != NULL && object->animationKind == 1)
    ((struct ActorSpriteView *)object->sprite)->displayFlag = value;
}

#include "field_sprite.h"

void Animation_ApplyChildValues(struct Actor *object, u32 value) {
  if (object != NULL && object->animationKind == 1)
    Animation_ApplyChildValuesToRecord(
        ((struct ActorSpriteView *)object->sprite), value);
}

void Actor_SetSpritePriorityDirect(struct Actor *object, s32 priority) {
  struct FieldSprite *sprite;

  if (object == NULL || object->animationKind != 1) {
    return;
  }
  sprite = ((struct FieldSprite *)object->sprite);
  sprite->priority = priority;
  sprite->partPriority = priority;
  object->priorityFlags &= ~1;
}

#include "render_input.h"
#include "fixed_math.h"

struct IconSprite {
  u8 unk00[4];
  struct RenderOam oam;
  s32 scale;
  u8 vramSlot;
  u8 unk11[3];
  u8 width;
  u8 height;
  u8 unk16;
  s8 unk17;
  u8 unk18;
  u8 unk19;
  u8 unk1a;
  u8 unk1b;
};

struct IconOwner {
  u8 unk00[80];
  struct IconSprite *sprite;
  u8 unk54[8];
  u8 unk5c;
  u8 unk5d[5];
  u8 iconId;
};

void Func_08038248(s32 id);

/// Shows icon id on its owner's 16 by 16 sprite: Func_08038248 prepares the
/// icon in heap slot 68, whose tiles at +0x400 go to the sprite's VRAM cache
/// slot, and the sprite is reset to scale 1.0, 16-colour palette 0.
// No ☀️ twin found.
void Func_0802372c(struct IconOwner *owner, s32 id) {
  struct IconSprite *sprite;
  u8 *buffer;
  s32 tile;

  if (owner == 0)
    return;
  owner->unk5c = 1;
  owner->iconId = id;
  sprite = owner->sprite;
  if (sprite == 0)
    return;
  buffer = HeapSlot_Alloc(68, 0x608);
  Func_08038248(id);
  tile = VramCache_Load(sprite->vramSlot, 128, buffer + 0x400);
  HeapSlot_Release(68);
  sprite->unk1b = 0;
  sprite->unk19 = 0;
  sprite->width = 16;
  sprite->height = 16;
  sprite->unk16 = 0;
  sprite->unk17 = 4;
  sprite->scale = Q16_ONE;
  sprite->oam.index = tile;
  sprite->oam.palette256 = 0;
  sprite->oam.palette = 0;
}

s32 Func_080237c8(u8 *object) {
  u8 *track = *(u8 **)(object + 0x50);

  if (object != NULL && track != NULL && track != (u8 *)-0x28)
    return **(s16 **)(track + 0x28);
  return 0;
}


void Func_080237e8(u8 *object) {
  if (object != NULL)
    Animation_WaitForTrackEnd(*(void **)(object + 0x50));
}

void Func_0802386c(void);
void ObjectSystem_UpdateCameraFixed(void);

void Graphics_EnableObjLayerAndCallbacks(void) {
  u16 dispcnt;

  Task_SuspendAll(Func_0802386c);
  Task_SuspendAll(ObjectSystem_UpdateCameraFixed);
  BattleEffect_ApplyColorToTargetBufferFar(0x10000, 1);
  BattleEffect_StartBufferInterpolationFar(1);
  wait(1);
  dispcnt = REG_DISPCNT;
  REG_DISPCNT = (dispcnt & 0xf1ff) | DISPCNT_OBJ_ON;
}

void ActorSystem_ResumeProjectionAndHideLayers(void) {
  u16 dispcnt;

  Task_ResumeAll(Func_0802386c);
  Task_ResumeAll(ObjectSystem_UpdateCameraFixed);
  dispcnt = REG_DISPCNT;
  REG_DISPCNT = dispcnt & 0xe1ff;
}

s32 Actor_ReturnTrue(void) { return 1; }

#include "map.h"
#include "iwram_call.h"
#include "field/sprite.h"
#include "field/field.h"

struct PlacedSprite {
  u8 unk00[9];
  u8 low09 : 2;
  u8 priority : 2;
  u8 high09 : 4;
  u8 unk0a[2];
  s32 scale; ///< 16.16 fixed point
  u8 slot;
  u8 retainCacheEntry; ///< bit 0
  u8 unk12[7];
  u8 dirty;
  u8 unk1a[0x0b];
  u8 low25 : 2;
  u8 shadowPriority : 2;
  u8 high25 : 4;
};

struct CameraPoint {
  s32 x;
  s32 y;
  s32 z;
};

struct MapPriorityWork {
  u8 unk000[0x101];
  s8 basePriority;
};

// The decoder's block: its code, tables and work area (ld_script.ld). Only
// the code and tables are copied.
extern u8 Render_DecodePictureCode[];
extern u8 Render_DecodePictureCopyEnd[];
extern u8 Render_DecodePictureBlockSize[];

s32 Func_0802d45c(s32 terrainId, s32 x, s32 z);
void Func_08022318(struct PlacedSprite *sprite, struct SpritePlacement *point,
                   s32 *scale, s32 facing, s32 depth);
void Func_08021a84(struct PlacedSprite *sprite, s32 facing);

#define BASE_PRIORITY(map) (((struct MapPriorityWork *)(map))->basePriority)
// The priority field (bits 15..14) of a cell's word.
#define CELL_PRIORITY(cells, x, z)                                             \
  (((cells)[(((z) >> 20) << 7) + ((x) >> 20)] << 16) >> 30)
// The layer field (bits 13..12) of a cell's word: the layer plus one.
#define CELL_LAYER(cells, x, z)                                                \
  (((cells)[(((z) >> 20) << 7) + ((x) >> 20)] << 18) >> 30)

void Func_0802386c(void) {
  // Unused: gives the stack frame its 36 bytes, as The Broken Seal's.
  struct CameraPoint unusedPoints[3];
  struct SpritePlacement point;
  struct SpriteScale scale;
  s32 objectIndex;
  struct MapWork *map;
  s32 *view;
  s32 viewX;
  s32 viewZ;
  struct ObjectSystemWork *system;
  struct Actor *object;
  u32 kind;
  s32 dx;
  s32 dz;
  s32 originY;
  struct PlacedSprite *sprite;
  s32 screenTop;
  u8 *priorityFlags;
  s32 layer;
  u8 *terrainId;
  u32 *cells;
  s32 feetZ;
  s32 objectHeight;
  s32 probeX;
  s32 priority;
  s32 leftPriority;
  s32 middlePriority;
  s32 rightPriority;
  s32 height;
  s32 rise;
  u32 layerPlusOne;
  s8 *basePriority;

  map = Ram_HeapSlots->mapWork;
  view = &map->viewX;
  viewX = view[0] & 0xffff0000;
  viewZ = view[1] & 0xffff0000;
  system = Ram_HeapSlots->objectSystem;
  originY = map->origin[1];
  // The decoder's copy in a once-only block, as in
  // ObjectSystem_UpdateCameraFixed, keeps the first call's arguments after the
  // followed point's height; as a plain call, 6 of 606 instructions differ in
  // tla-en.
  do {
    Dma_Set(
        Render_DecodePictureCode,
        HeapSlot_Alloc(80, (s32)Render_DecodePictureBlockSize),
        DMA_COPY_WORDS(
            (u32)(Render_DecodePictureCopyEnd - Render_DecodePictureCode) >> 2),
        REG_DMA3);
  } while (0);
  {
    // The zero is set before the objects are read.
    u32 zero = 0;

    object = Ram_HeapSlots->scriptObjects;
    system->used = zero;
  }
  for (objectIndex = 0; objectIndex < 64; objectIndex++, object++) {
    if (object->script == NULL)
      continue;
    if (object->x != 0 || object->z != 0) {
      kind = object->animationKind & 15;
      if (kind == 0)
        continue;
      if (kind != 1)
        continue;
      if ((u16)system->suspended != 0 && !(object->unk5c[0] & 1)) {
        struct PlacedSprite *frozen = object->sprite;

        VramCache_ReleaseVram(frozen->slot);
        frozen->dirty = kind;
        continue;
      }
      sprite = object->sprite;
      dx = object->x - viewX;
      dz = object->z - viewZ;
      screenTop = dz - object->y;
      if (dx > -0x200000 && dx < 0x1100000 && screenTop > -0x200000 &&
          screenTop < 0xe00000 &&
          (priorityFlags = &object->priorityFlags, !(*priorityFlags & 16))) {
        terrainId = &object->terrainId;
        layer = *terrainId & 3;
        cells = (u32 *)map->layers[layer].cells;
        if (*priorityFlags & 1) {
          probeX = object->x - 0x70000;
          feetZ = object->z + 0x20000;
          objectHeight = object->terrainHeight >> 16;
          priority = 0;
          leftPriority = 0;
          middlePriority = 0;
          if (!(*priorityFlags & 32)) {
            /* FAKEMATCH(temporary): the base priority read through a
             * function-scope pointer, which gives the two spills at sp+12 and
             * sp+16 their order; as priority = BASE_PRIORITY(map), 8 of 606
             * instructions differ in tla-en. */
            basePriority = &BASE_PRIORITY(map);
            priority = *basePriority;
            if (Func_0802d45c(layer, probeX, feetZ) == object->terrainHeight) {
              leftPriority = CELL_PRIORITY(cells, probeX, feetZ);
              height = Func_0802d45c(layer, probeX, feetZ + 0x100000) >> 16;
              rise =
                  (Func_0802d45c(layer, probeX, feetZ + 0x200000) >> 16) - 16;
              if (rise > height)
                height = rise;
              if (height > objectHeight)
                priority = BASE_PRIORITY(map) + 1;
            }
            probeX += 0xd0000;
            if (Func_0802d45c(layer, probeX, feetZ) == object->terrainHeight) {
              middlePriority = CELL_PRIORITY(cells, probeX, feetZ);
              height = Func_0802d45c(layer, probeX, feetZ + 0x100000) >> 16;
              rise =
                  (Func_0802d45c(layer, probeX, feetZ + 0x200000) >> 16) - 16;
              if (rise > height)
                height = rise;
              if (height > objectHeight)
                priority = BASE_PRIORITY(map) + 1;
            }
            probeX -= 0x60000;
            rightPriority = CELL_PRIORITY(cells, probeX, feetZ);
            height = Func_0802d45c(layer, probeX, feetZ + 0x100000) >> 16;
            rise = (Func_0802d45c(layer, probeX, feetZ + 0x200000) >> 16) - 16;
            if (rise > height)
              height = rise;
            if (height > objectHeight)
              priority = BASE_PRIORITY(map) + 1;
          } else {
            if (object->terrainHeight == Func_0802d45c(layer, probeX, feetZ))
              leftPriority = CELL_PRIORITY(cells, probeX, feetZ);
            probeX += 0xd0000;
            if (object->terrainHeight == Func_0802d45c(layer, probeX, feetZ))
              middlePriority = CELL_PRIORITY(cells, probeX, feetZ);
            probeX -= 0x60000;
            rightPriority = CELL_PRIORITY(cells, probeX, feetZ);
          }
          // An odd priority is preferred, left first.
          if (leftPriority != 0 || middlePriority != 0 || rightPriority != 0) {
            priority = leftPriority;
            if (!(priority & 1)) {
              if (middlePriority != 0)
                priority = middlePriority;
              if (!(priority & 1) && rightPriority != 0)
                priority = rightPriority;
            }
          }
          if (priority != 0 && object->animationKind == 1) {
            struct PlacedSprite *placed = object->sprite;

            if ((u32)priority > 3)
              priority = 3;
            placed->priority = priority;
            placed->shadowPriority = priority;
          }
        }
        layerPlusOne = CELL_LAYER(cells, object->x, object->z);
        if (layerPlusOne != 0)
          *terrainId = layerPlusOne - 1;
        scale.x = Iwram_MulQ16(object->scaleX, sprite->scale);
        scale.y = Iwram_MulQ16(object->scaleY, sprite->scale);
        point.x = dx;
        point.height = object->y - originY;
        point.y = dz - originY;
        point.groundHeight = object->terrainHeight - originY;
        if (*priorityFlags & 2) {
          point.height += 0xfec00000;
          point.y += 0xfec00000;
          point.groundHeight += 0xfec00000;
        }
        if (*priorityFlags & 4) {
          point.height += 0x1400000;
          point.y += 0x1400000;
          point.groundHeight += 0x1400000;
        }
        if (*priorityFlags & 8)
          Func_08022318(sprite, &point, scale.values, object->facing,
                        ((u8 *)object)[0x63]);
        else
          Render_ApplyProjectedPlacement(sprite, SpritePlacement_Values(&point),
                                         scale.values, object->facing);
        continue;
      }
      if (object->unk5c[0] & 2) {
        Func_08021a84(sprite, object->facing);
      } else if (!(object->unk5c[0] & 1) && !(sprite->retainCacheEntry & 1)) {
        VramCache_ReleaseVram(sprite->slot);
        sprite->dirty = 1;
      }
    } else {
      // At (0, 0): not placed; only the sprite's VRAM is given up.
      kind = object->animationKind & 15;
      if (kind == 1) {
        struct PlacedSprite *frozen = object->sprite;

        if (object->unk5c[0] == 0 && !(frozen->retainCacheEntry & 1)) {
          VramCache_ReleaseVram(frozen->slot);
          frozen->dirty = kind;
        }
      }
    }
  }
  HeapSlot_Release(80);
}


// The fixed camera: each frame it places every object's sprites, facing them
// by the angle from the eye to the target. The Lost Age places each object in
// a function of its own, the body of ☀️'s ObjectSystem_UpdateCameraFixed
// loop.

struct FixedCamera {
  struct CameraPoint eye;
  struct CameraPoint target;
  struct CameraPoint *eyeOverride;    ///< when set, used as the eye
  struct CameraPoint *targetOverride; ///< when set, used as the target
};

struct PlacedObject {
  u8 unk00[2];
  u16 active;
  u8 unk04[2];
  u16 angle;
  s32 position[4];
  s32 scale[2];
  u8 unk20[2];
  u8 layer;
  u8 flags;
  u8 unk24[0x2c];
  void *target;
  u8 kind;
  u8 unk55[0x2b];
};

extern const s32 gData_0802eb90[2];
extern const s32 gData_0802eb98[12];
s32 ArcTan2(s32 x, s32 y);
s32 GameFlag_Test(s32 flag);
void Func_0802254c(void *sprite, s32 *point, const s32 *scale, s32 facing,
                   s32 depth);

s32 ObjectCamera_ReturnTrue(void) { return 1; }

/// Places an object's sprite (kind 1, unless flag 0x10 hides it) or each of its
/// group's four sprites (kind 2), turned by angle. While battle commands are
/// chosen (flag 363) the sprites are drawn at 1.0 scale.
void Func_08023d80(struct PlacedObject *object, s32 angle) {
  const s32 *scale = object->scale;
  s32 *point;
  void *sprite;
  void **sprites;
  s32 remaining;

  if (GameFlag_Test(363))
    scale = gData_0802eb90;
  switch (object->kind & 15) {
  case 1:
    point = object->position;
    sprite = object->target;
    if (!(object->flags & 0x10))
      Func_0802254c(sprite, point, scale, object->angle + angle, object->layer);
    break;
  case 2:
    sprites = object->target;
    for (remaining = 3; remaining >= 0; remaining--) {
      sprite = *sprites++;
      if (sprite != NULL)
        Func_0802254c(sprite, object->position, scale, object->angle + angle,
                      object->layer);
    }
    break;
  case 0:
    break;
  }
}

/// Each frame, with the fixed camera: places every active object, from the
/// 64th down. While battle commands are chosen (flag 363) it uses the fixed
/// view matrix and turns the objects by an eighth of a turn less.
void ObjectSystem_UpdateCameraFixed(void) {
  struct FixedCamera *camera = Ram_HeapSlots->cameraWork;
  struct ObjectSystemWork *system = Ram_HeapSlots->objectSystem;
  struct CameraPoint *eye;
  struct CameraPoint *target;
  struct PlacedObject *object;
  s32 angle;
  s32 i;
  // Unused: gives the stack frame its 12 bytes.
  struct CameraPoint unused;

  // The decoder's copy in a once-only block makes the stack frame before the
  // first call's arguments; as a plain call, 6 of 93 instructions differ in
  // tla-en.
  do {
    Dma_Set(
        Render_DecodePictureCode,
        HeapSlot_Alloc(80, (s32)Render_DecodePictureBlockSize),
        DMA_COPY_WORDS(
            (u32)(Render_DecodePictureCopyEnd - Render_DecodePictureCode) >> 2),
        REG_DMA3);
  } while (0);
  eye = &camera->eye;
  target = &camera->target;
  if (camera->eyeOverride != NULL)
    eye = camera->eyeOverride;
  if (camera->targetOverride != NULL)
    target = camera->targetOverride;
  angle = (s16)ArcTan2((eye->x - target->x) >> 16, (eye->z - target->z) >> 16);
  system->used = 0;
  if (GameFlag_Test(363)) {
    Render_ResetTransformState();
    angle += 0xffffe000;
    Iwram_TransformMatrixEntry(gData_0802eb98);
    Transform_ApplyLookAt((s32 *)eye, (s32 *)target);
  } else {
    SceneTransform_RewindStack();
    Transform_SetLookAt((s32 *)eye, (s32 *)target);
  }
  object = Ram_HeapSlots->scriptObjects;
  object += 63;
  for (i = 63; i >= 0; i--, object--) {
    if (object->active != 0)
      Func_08023d80(object, angle);
  }
  HeapSlot_Release(80);
}

// Object scripts: move the instruction cursor and control byte54.

#include "script.h"

/// Jumps to the script the next word points to and returns 1 to continue
/// with its first command.
s32 Script_JumpToNextOperandPointer(struct ScriptInterpreter *sc) {
  s32 zero;

  // Jump to the script pointed to by the following word.
  sc->script = (const s32 *)sc->script[sc->cursor + 1];
  zero = 0;
  sc->cursor = zero;
  return 1;
}

/// Clears byte54 and advances to the next command.
s32 Script_ClearByte54(struct ScriptInterpreter *sc) {
  sc->byte54 = 0;
  sc->cursor++;
  return 1;
}

/// Sets byte54 to 1 and advances to the next command.
s32 Script_SetByte54(struct ScriptInterpreter *sc) {
  sc->byte54 = 1;
  sc->cursor++;
  return 1;
}


/// Waits up to 600 frames for a field object's movement target to be unset.
void Script_WaitForEventTimeout(void *fieldObject) {
  s32 waitedFrames;

  waitedFrames = 0;
  while (waitedFrames < 600 && Actor_IsTargetUnset(fieldObject) == 0) {
    wait(1);
    waitedFrames++;
  }
}

s32 Actor_IsTargetUnset(struct Actor *object) {
  s32 first;
  s32 second;

  if (object->motionFlags == 0) {
    second = object->targetX;
    if (second != (s32)0x80000000)
      return 0;
    first = object->targetY;
  } else {
    first = object->targetX;
    second = (s32)0x80000000;
  }
  if (first != second || object->targetZ != first)
    return 0;
  return 1;
}

s32 Func_08023f94(struct Actor *object) { return Actor_IsTargetUnset(object); }

/// The words of an object's script. As in The Broken Seal's ActorScriptWords,
/// a flat word pointer changes one load's base and index order.
struct MotionScript {
  s32 word[1];
};

/// An object as the motion update reads it (struct Actor's layout).
struct MotionObject {
  struct MotionScript *script;
  s16 step;
  u16 angle;
  s32 x;
  s32 y;
  s32 z;
  s32 groundHeight;
  s32 scaleX;
  s32 scaleY;
  u16 radius;
  u8 terrainId;
  u8 priorityFlags;
  s32 velocityX;
  s32 velocityY;
  s32 velocityZ;
  s32 speedLimit;
  s32 acceleration;
  s32 targetX;
  s32 targetY;
  s32 targetZ;
  s32 bounce;
  s32 gravity;
  s32 unk4c;
  void *animation;
  u8 animationKind;
  u8 motionFlags;
  u8 arrivalAxis;
  u8 unk57;
  u8 snapToTarget;
  u8 collisionFlags;
  u8 actionFlags;
  u8 paused;
  u8 unk5c[2];
  s16 wait;
  u8 refused;
  u8 frozen;
  u8 unk62[0x0a];
  void (*update)(struct MotionObject *object);
  s32 moved[3];
  struct MotionObject *standingOn;
};

typedef s32 (*MotionCommand)(struct MotionObject *object);

extern const MotionCommand gData_0802f204[];
extern const s32 gData_0802ebc8[];

s32 IwramMulQ16(s32 a, s32 b) __attribute__((long_call));

#define TARGET_UNSET ((s32)0x80000000)

/// A whole-pixel distance squared.
static __inline__ s32 Object_Square(s32 value) { return value * value; }

void Func_08023f9c(void) {
  struct MotionObject *object;
  s32 objectsRemaining;
  s32 x;
  s32 y;
  s32 z;
  s32 ground;
  s32 arrivedAtTarget;
  s32 pos[3];
  s32 dx;
  s32 dy;
  s32 dz;
  s32 velocityX;
  s32 velocityY;
  s32 velocityZ;
  s32 distance;
  s32 speed;
  s32 reducedSpeed;
  s32 ratio;
  s32 maximumChange;
  s32 standing;

  object = Ram_HeapSlots->scriptObjects;
  for (objectsRemaining = 63; objectsRemaining >= 0;
       objectsRemaining--, object++) {
    struct MotionScript *script;

    arrivedAtTarget = 0;
    script = object->script;
    if (script == NULL)
      continue;
    if (object->update != NULL) {
      object->update(object);
      script = object->script;
    }
    if (script == NULL)
      continue;
    if (object->paused != 0) {
      object->moved[0] = arrivedAtTarget;
      object->moved[1] = arrivedAtTarget;
      object->moved[2] = arrivedAtTarget;
      continue;
    }

    if (object->wait != 0) {
      object->wait--;
    } else {
      for (;;) {
        u32 command = script->word[object->step];

        if (command <= 63) {
          if (gData_0802f204[command](object) == 0)
            break;
        } else {
          object->step++;
        }
        script = object->script;
      }
      if (object->script == NULL)
        continue;
    }

    x = object->x;
    y = object->y;
    z = object->z;
    if (object->frozen == 0) {
      if (object->motionFlags == 0) {
        if (object->targetX != TARGET_UNSET) {
          dx = (object->targetX - x) / 65536;
          dy = (object->targetY - y) / 65536;
          dz = (object->targetZ - z) / 65536;
          distance = Iwram_Sqrt(Object_Square(dx) + Object_Square(dy) +
                                Object_Square(dz));
          if (distance == 0) {
            x = object->targetX;
            y = object->targetY;
            z = object->targetZ;
          } else {
            ratio = Iwram_RatioMulQ14(distance << 16, object->acceleration);
            object->velocityX += dx * ratio;
            object->velocityY += dy * ratio;
            object->velocityZ += dz * ratio;
            velocityX = object->velocityX;
            velocityY = object->velocityY;
            velocityZ = object->velocityZ;
            speed = FixedSqrt(Iwram_MulQ16(velocityX, velocityX) +
                              Iwram_MulQ16(velocityY, velocityY) +
                              Iwram_MulQ16(velocityZ, velocityZ));
            if (speed > object->speedLimit) {
              ratio = Iwram_RatioMulQ14(speed, object->speedLimit);
              object->velocityX = Iwram_MulQ16(velocityX, ratio);
              object->velocityY = Iwram_MulQ16(velocityY, ratio);
              object->velocityZ = Iwram_MulQ16(velocityZ, ratio);
            }
          }
        } else {
          dx = object->velocityX;
          dy = object->velocityY;
          dz = object->velocityZ;
          speed = FixedSqrt(Iwram_MulQ16(dx, dx) + Iwram_MulQ16(dy, dy) +
                            Iwram_MulQ16(dz, dz));
          if (speed != 0) {
            reducedSpeed = speed - object->acceleration;
            if (reducedSpeed < 0)
              reducedSpeed = 0;
            ratio = Iwram_RatioMulQ14(speed, reducedSpeed);
            object->velocityX = Iwram_MulQ16(dx, ratio);
            object->velocityY = Iwram_MulQ16(dy, ratio);
            object->velocityZ = Iwram_MulQ16(dz, ratio);
          } else {
            object->velocityX = speed;
            object->velocityY = speed;
            object->velocityZ = speed;
          }
        }
      } else {
        // On the ground: steer in the XZ plane the same way.
        if (object->targetX != TARGET_UNSET) {
          dx = (object->targetX - x) / 65536;
          dz = (object->targetZ - z) / 65536;
          distance = Iwram_Sqrt(Object_Square(dx) + Object_Square(dz)) << 16;
          if (distance <= 0x00ffffff) {
            dx = object->targetX - x;
            dz = object->targetZ - z;
            distance = FixedSqrt(Iwram_MulQ16(dx, dx) + Iwram_MulQ16(dz, dz));
          }
          if (distance == 0) {
            x = object->targetX;
            z = object->targetZ;
          } else {
            ratio = Iwram_RatioMulQ14(distance, object->acceleration);
            object->velocityX += Iwram_MulQ16(dx, ratio);
            object->velocityZ += Iwram_MulQ16(dz, ratio);
            velocityX = object->velocityX;
            velocityZ = object->velocityZ;
            speed = FixedSqrt(Iwram_MulQ16(velocityX, velocityX) +
                              Iwram_MulQ16(velocityZ, velocityZ));
            if (speed > object->speedLimit) {
              ratio = Iwram_RatioMulQ14(speed, object->speedLimit);
              object->velocityX = Iwram_MulQ16(velocityX, ratio);
              object->velocityZ = Iwram_MulQ16(velocityZ, ratio);
            }
          }
        } else {
          dx = object->velocityX;
          dz = object->velocityZ;
          speed = FixedSqrt(Iwram_MulQ16(dx, dx) + Iwram_MulQ16(dz, dz));
          if (speed != 0) {
            reducedSpeed = speed - object->acceleration;
            if (reducedSpeed < 0)
              reducedSpeed = 0;
            ratio = Iwram_RatioMulQ14(speed, reducedSpeed);
            object->velocityX = Iwram_MulQ16(dx, ratio);
            object->velocityZ = Iwram_MulQ16(dz, ratio);
          } else {
            object->velocityX = speed;
            object->velocityZ = speed;
          }
        }

        // Follow the ground, or the top of an object 16 pixels below.
        if (object->motionFlags & 1) {
          velocityX = x + object->velocityX;
          velocityZ = z + object->velocityZ;
          standing = 0;
          object->standingOn = NULL;
          if (object->priorityFlags & 0x40) {
            struct MotionObject *other = Ram_HeapSlots->scriptObjects;
            s32 i;

            for (i = 0; i < 64; i++, other++) {
              s32 dy;

              if (other->script == NULL)
                continue;
              if (!(other->priorityFlags & 0x80))
                continue;
              if (other == object)
                continue;
              if (((velocityX - other->x + 0x80000) >> 20) != 0)
                continue;
              if (((velocityZ - other->z + 0x80000) >> 20) != 0)
                continue;
              dy = y - other->y;
              if ((dy - 0x100000 < 0 ? -(dy - 0x100000) : dy - 0x100000) >
                  0x7ffff)
                continue;
              standing = 1;
              ground = other->y + 0x100000;
              object->standingOn = other;
              break;
            }
          }
          if (!standing)
            ground = Func_0802d45c(object->terrainId, velocityX, velocityZ);
          velocityY = ground - object->groundHeight;
          if (ground - y > -0x40000)
            y += velocityY;
          if (velocityY < 0)
            velocityY = -velocityY;
          maximumChange = object->acceleration / 2;
          if (velocityY > maximumChange)
            velocityY = maximumChange;
          velocityY *= 3;
          // This slowdown and the landing bounce below call IwramMulQ16 by its
          // linked name, so each call reads the routine's address from the
          // literal pool after the call before it.
          if (velocityY != 0 && !(object->motionFlags & 16) && !standing) {
            dx = object->velocityX;
            dy = object->velocityY;
            dz = object->velocityZ;
            speed = FixedSqrt(IwramMulQ16(dx, dx) + IwramMulQ16(dy, dy) +
                              IwramMulQ16(dz, dz));
            if (speed != 0) {
              reducedSpeed = speed - velocityY;
              if (reducedSpeed < 0)
                reducedSpeed = 0;
              ratio = Iwram_RatioMulQ14(speed, reducedSpeed);
              object->velocityX = IwramMulQ16(dx, ratio);
              object->velocityY = IwramMulQ16(dy, ratio);
              object->velocityZ = IwramMulQ16(dz, ratio);
            }
          }
          object->groundHeight = ground;
        }

        // Gravity, and a bounce that loses energy on landing.
        if (object->motionFlags & 2) {
          ground = object->groundHeight;
          if (y > ground)
            object->velocityY -= object->gravity;
          if (y + object->velocityY <= ground) {
            if (object->velocityY < 0) {
              s32 rebound = -IwramMulQ16(object->velocityY, object->bounce);

              object->velocityY = rebound;
              y = ground - rebound;
              if ((rebound < 0 ? -rebound : rebound) <= object->gravity) {
                y = ground;
                object->velocityY = 0;
              }
            } else {
              y = ground;
            }
          }
        }

        // Float: bob up and down on a triangle wave.
        if (object->motionFlags & 4) {
          u32 phase = object->bounce & 63;

          if (object->motionFlags & 8)
            object->velocityY =
                gData_0802ebc8[phase >> 1] * object->gravity / 16;
          else
            object->velocityY =
                gData_0802ebc8[phase >> 1] * object->gravity / 64;
          object->bounce++;
        }
      }
    }

    x += object->velocityX;
    y += object->velocityY;
    z += object->velocityZ;
    if (object->collisionFlags & 0x80) {
      pos[0] = x;
      pos[1] = y;
      pos[2] = z;
      if (ScriptObject_CheckOverlap(object, pos) != 0) {
        object->refused++;
        continue;
      }
      object->refused = 0;
    }

    switch (object->arrivalAxis) {
    case 16:
      if (x == object->targetX ||
          ((object->x - object->targetX) ^ (x - object->targetX)) < 0)
        arrivedAtTarget = 1;
      break;
    case 17:
      if (y == object->targetY ||
          ((object->y - object->targetY) ^ (y - object->targetY)) < 0)
        arrivedAtTarget = 1;
      break;
    case 18:
      if (z == object->targetZ ||
          ((object->z - object->targetZ) ^ (z - object->targetZ)) < 0)
        arrivedAtTarget = 1;
      break;
    }
    if (arrivedAtTarget != 0) {
      if (object->snapToTarget != 0) {
        x = object->targetX;
        object->velocityX = 0;
        z = object->targetZ;
        object->velocityZ = 0;
        if (object->motionFlags == 0) {
          y = object->targetY;
          object->velocityY = 0;
        }
      }
      object->targetX = TARGET_UNSET;
      object->targetY = TARGET_UNSET;
      object->targetZ = TARGET_UNSET;
      object->arrivalAxis = 0;
    }

    object->moved[0] = x - object->x;
    object->moved[1] = y - object->y;
    object->moved[2] = z - object->z;
    object->x = x;
    object->y = y;
    object->z = z;

    // Turn to face the direction of travel, at most a sixteenth of a turn a
    // frame.
    if (object->actionFlags & 1) {
      x = object->velocityX;
      z = object->velocityZ;
      if (x != 0 || z != 0) {
        s32 facingStep;
        s32 maximumFacingStep;

        facingStep = (s16)(ArcTan2(z, x) - object->angle);

        maximumFacingStep = 0x1000;
        if (facingStep > maximumFacingStep)
          facingStep = maximumFacingStep;
        maximumFacingStep = -0x1000;
        if (facingStep < maximumFacingStep)
          facingStep = maximumFacingStep;
        object->angle += facingStep;
      }
    }
  }
}


#define WORD_AT(base, offset) (*(s32 *)((u8 *)(base) + (offset)))

void Func_080246b8(void) {
  struct Actor *object = Ram_HeapSlots->scriptObjects;
  s32 i;

  for (i = 63; i >= 0; i--, object++) {
    if (object->script != NULL) {
      s32 x = object->x;
      s32 y = object->y;
      s32 z = object->z;

      if (object->priorityFlags & 64) {
        struct Actor *carrier = (struct Actor *)WORD_AT(object, 0x7c);

        if (carrier != NULL) {
          if (WORD_AT(carrier, 0x74) != 0) {
            if (y == object->terrainHeight) {
              object->terrainHeight = carrier->y + 0x100000;
              y = object->terrainHeight;
            } else {
              object->terrainHeight += WORD_AT(carrier, 0x74);
              y += WORD_AT(carrier, 0x74);
            }
          }
          x += WORD_AT(carrier, 0x70);
          z += WORD_AT(carrier, 0x78);
        }
      }
      SET_POSITION(object, x, y, z);
    }
  }
}

// Objects: set position and cancel motion.

/// Sets the position, clears the destination and zeroes the velocity.
/// obj: object.
/// x, y, z: 16.16 fixed-point position.
void Actor_SetPositionAndResetMotion(struct Actor *obj, s32 x, s32 y, s32 z) {
  obj->z = z;
  obj->targetX = 0x80000000; // No destination.
  obj->targetY = 0x80000000;
  obj->targetZ = 0x80000000;
  obj->x = x;
  obj->y = y;
  SET_VELOCITY(obj, 0, 0, 0);
}

void Actor_SetMoveTarget(struct Actor *object, s32 x, s32 y, s32 z) {
  s32 dx;
  s32 distance;
  s32 dz;
  s32 squaredZ;
  s32 dy;
  u8 *arrivalAxis;

  dx = (x - object->x) / Q16_ONE;
  dy = (y - object->y) / Q16_ONE;
  dz = (z - object->z) / Q16_ONE;
  squaredZ = dz * dz;
  distance = Iwram_Sqrt(dx * dx + dy * dy + squaredZ) << 16;
  if (distance < 0x100000) {
    dx = x - object->x;
    dy = y - object->y;
    dz = z - object->z;
    distance = FixedSqrt(Iwram_MulQ16(dx, dx) + Iwram_MulQ16(dy, dy) +
                         Iwram_MulQ16(dz, dz));
  }
  if (distance < 0x10000) {
    object->x = x;
    object->y = y;
    object->z = z;
    object->targetX = TARGET_UNSET;
    object->targetY = TARGET_UNSET;
    object->targetZ = TARGET_UNSET;
    return;
  }
  if (object->snapToTarget == 0) {
    s32 brakingDistance;
    s32 ratio;

    brakingDistance =
        Iwram_RatioMulQ14(object->acceleration,
                          Iwram_MulQ16(object->speedLimit, object->speedLimit));
    if (distance > brakingDistance)
      brakingDistance = distance - brakingDistance / 2;
    else
      brakingDistance = distance / 2;
    ratio = Iwram_RatioMulQ14(distance, brakingDistance);
    x = object->x + Iwram_MulQ16(x - object->x, ratio);
    y = object->y + Iwram_MulQ16(y - object->y, ratio);
    z = object->z + Iwram_MulQ16(z - object->z, ratio);
  }
  object->targetX = x;
  object->targetY = y;
  object->targetZ = z;
  dx = x - object->x;
  dy = y - object->y;
  dz = z - object->z;
  arrivalAxis = &object->unk56;
  *arrivalAxis = 16;
  if ((dx < 0 ? -dx : dx) < (dz < 0 ? -dz : dz)) {
    *arrivalAxis = 18;
    dx = dz;
  }
  if (object->motionFlags == 0) {
    if (dx < 0)
      dx = -dx;
    if (dx < (dy < 0 ? -dy : dy))
      *arrivalAxis = 17;
  }
}

void ObjectList_CollectActive(u8 *object, u8 **out) {
  u8 *end = object + 0x700;

  do {
    if (*(u16 *)(object + 2) != 0)
      *out++ = object;
    object += 128;
  } while (object != end);
  *out = NULL;
}
#undef BASE_PRIORITY
#undef CELL_PRIORITY
#undef CELL_LAYER
#undef TARGET_UNSET
#undef WORD_AT

// ---- games/tla/src/field/object/f02493c.c ----
#define TARGET_UNSET ((s32)0x80000000)

/* VIEW(f02493c): tag struct MotionObject as struct MotionObject__f02493c; resource_slot.c has another definition; tried with the definition the rest of the object has, it does not compile in tla-ja: games/tla/src/field/resource_slot.c:2499: structure has no member named `terrainHeight'. */
struct MotionObject__f02493c {
  void *script;
  u16 scriptCursor;
  u16 facing;
  s32 x;
  s32 y;
  s32 z;
  s32 terrainHeight;
  u8 unk18[0x0c];
  s32 velocityX;
  s32 velocityY;
  s32 velocityZ;
  s32 speedLimit;
  s32 acceleration;
  s32 targetX;
  s32 targetY;
  s32 targetZ;
  s32 bounce;
  s32 gravity;
  u8 unk4c[9];
  u8 motionFlags;
  u8 arrivalAxis;
  u8 unk57;
  u8 snapToTarget;
  u8 collisionFlags;
  u8 steeringFlags;
  u8 paused;
  u8 unk5c[5];
  u8 frozen;
  u8 unk62[0x1e];
};

s32 ArcTan2(s32 y, s32 x);

void Func_0802493c(void) {
  struct MotionObject__f02493c *list[15];
  struct MotionObject__f02493c **next;
  struct MotionObject__f02493c *object;
  s32 y;
  s32 arrivedAtTarget;
  s32 x;
  s32 z;
  s32 dx;
  s32 dz;
  s32 distance;
  s32 ratio;
  s32 facingStep;

  object = Ram_HeapSlots->scriptObjects;
  ObjectList_CollectActive(object, list);
  object = list[0];
  next = &list[1];
  while (object != NULL) {
    x = object->x;
    y = object->y;
    z = object->z;
    if (object->frozen == 0) {
      arrivedAtTarget = 0;
      if (object->targetX != TARGET_UNSET) {
        dx = (object->targetX - x) / 65536;
        dz = (object->targetZ - z) / 65536;
        distance = Iwram_Sqrt(dx * dx + dz * dz) << 16;
        if (distance <= 0xffffff) {
          dx = object->targetX - x;
          dz = object->targetZ - z;
          distance = Iwram_Sqrt(Iwram_MulQ16(dx, dx) + Iwram_MulQ16(dz, dz))
                     << 8;
        }
        if (distance == 0) {
          x = object->targetX;
          z = object->targetZ;
        } else {
          ratio = Iwram_RatioMulQ14(distance, object->acceleration);
          object->velocityX += Iwram_MulQ16(dx, ratio);
          object->velocityZ += Iwram_MulQ16(dz, ratio);
          dx = object->velocityX;
          dz = object->velocityZ;
          distance = Iwram_Sqrt(Iwram_MulQ16(dx, dx) + Iwram_MulQ16(dz, dz))
                     << 8;
          if (distance > object->speedLimit) {
            ratio = Iwram_RatioMulQ14(distance, object->speedLimit);
            object->velocityX = Iwram_MulQ16(dx, ratio);
            object->velocityZ = Iwram_MulQ16(dz, ratio);
          }
        }
      } else {
        dx = object->velocityX;
        dz = object->velocityZ;
        if ((dx | dz) != 0) {
          distance = Iwram_Sqrt(Iwram_MulQ16(dx, dx) + Iwram_MulQ16(dz, dz))
                     << 8;
          if (distance != 0) {
            if (distance - object->acceleration < 0) {
              object->velocityX = 0;
              object->velocityZ = 0;
            } else {
              ratio =
                  Iwram_RatioMulQ14(distance, distance - object->acceleration);
              object->velocityX = Iwram_MulQ16(dx, ratio);
              object->velocityZ = Iwram_MulQ16(dz, ratio);
            }
          } else {
            object->velocityX = 0;
            object->velocityZ = 0;
          }
        }
      }
      if (object->motionFlags & 2) {
        if (y > object->terrainHeight) {
          object->velocityY -= object->gravity;
        } else if (object->velocityY < 0) {
          y = object->terrainHeight;
          object->velocityY = -Iwram_MulQ16(object->velocityY, object->bounce);
          if ((object->velocityY < 0 ? -object->velocityY
                                     : object->velocityY) <= object->gravity)
            object->velocityY = 0;
        }
      }
    }
    x += object->velocityX;
    y += object->velocityY;
    z += object->velocityZ;
    if (object->arrivalAxis != 0)
      switch (object->arrivalAxis) {
      case 16:
        if (x == object->targetX ||
            ((object->x - object->targetX) ^ (x - object->targetX)) < 0)
          arrivedAtTarget = 1;
        break;
      case 17:
        if (y == object->targetY ||
            ((object->y - object->targetY) ^ (y - object->targetY)) < 0)
          arrivedAtTarget = 1;
        break;
      case 18:
        if (z == object->targetZ ||
            ((object->z - object->targetZ) ^ (z - object->targetZ)) < 0)
          arrivedAtTarget = 1;
        break;
      }
    if (arrivedAtTarget) {
      if (object->snapToTarget) {
        object->velocityX = 0;
        object->velocityZ = 0;
        x = object->targetX;
        z = object->targetZ;
        if (object->motionFlags == 0) {
          y = object->targetY;
          object->velocityY = 0;
        }
      }
      object->targetX = TARGET_UNSET;
      object->targetY = TARGET_UNSET;
      object->targetZ = TARGET_UNSET;
      object->arrivalAxis = 0;
    }
    object->x = x;
    object->y = y;
    object->z = z;
    if (object->steeringFlags & 1) {
      x = object->velocityX;
      z = object->velocityZ;
      if (x != 0 || z != 0) {
        facingStep = (s16)(ArcTan2(z, x) - object->facing);
        if (facingStep > 0x1000)
          facingStep = 0x1000;
        if (facingStep < -0x1000)
          facingStep = -0x1000;
        object->facing += facingStep;
      }
    }
    object = *next++;
  }
}


s32 Actor_IsTargetUnset(struct Actor *object);

/// The actor as SC_SYNC reads it: the frames a solid object has blocked it
/// are a byte of the actor itself, which struct Actor leaves unnamed.
struct WaitingActor {
  u8 unk00[0x38];
  s32 targetX;
  s32 targetY;
  s32 targetZ;
  u8 unk44[0x12];
  u8 unk56;
  u8 unk57[9];
  u8 delay;
};

s32 Script_Wait(struct ScriptInterpreter *interpreter) {
  s16 cursor = interpreter->cursor;

  interpreter->lookupResult = interpreter->script[cursor + 1] - 1;
  interpreter->cursor += 2;
  return 0;
}

/// Waits until the object reaches its target or 60 frames pass; on the
/// timeout the target is cleared. Returns 1 when the script moves on.
s32 Script_WaitForArrival(struct ScriptInterpreter *interpreter) {
  struct WaitingActor *object = (struct WaitingActor *)interpreter;

  if (object->delay > 59) {
    object->delay = 0;
    object->targetX = 0x80000000;
    object->targetY = 0x80000000;
    object->targetZ = 0x80000000;
    object->unk56 = 0;
    interpreter->cursor++;
    return 1;
  }
  if (Actor_IsTargetUnset((struct Actor *)interpreter) != 0) {
    interpreter->cursor++;
    return 1;
  }
  return 0;
}

s32 Script_InvokeCallback(struct ScriptInterpreter *interpreter) {
  s16 initial = interpreter->cursor;
  ScriptCommand callback = (ScriptCommand)interpreter->script[initial + 1];

  if (callback(interpreter) != 0)
    return 0;
  if (interpreter->cursor == initial)
    interpreter->cursor += 2;
  return 1;
}

s32 Script_FindLabel(struct ScriptInterpreter *interpreter, u32 key) {
  u32 *entries;
  s32 index;
  s16 *field = &interpreter->lookupResult;
  s32 zero = 0;

  *field = zero;
  if (key == 0) {
    return 0;
  }

  key &= 0xBFFFFFFF;
  entries = (u32 *)interpreter->script;
  index = 0;
  do {
    if (*entries++ == key) {
      return index + 1;
    }
    index++;
  } while (index <= 0x3FF);
  return 0;
}

s32 Script_RepeatOrJump(struct ScriptInterpreter *interpreter) {
  const s32 *arguments;
  s32 repeatLimit;
  s32 jumpKey;
  u8 *repeatCount;
  s32 nextCount;

  arguments = &interpreter->script[interpreter->cursor + 1];
  repeatLimit = *arguments++;
  jumpKey = *arguments;
  if (repeatLimit == 0xFFFF) {
    interpreter->cursor = Script_FindLabel(interpreter, jumpKey);
  } else {
    repeatCount = &interpreter->repeatCount;
    nextCount = *repeatCount + 1;
    *repeatCount = nextCount;
    if ((s32)(u8)nextCount < (s32)(s16)repeatLimit) {
      interpreter->cursor = Script_FindLabel(interpreter, jumpKey);
    } else {
      *repeatCount = 0;
      interpreter->cursor = interpreter->cursor + 3;
    }
  }
  return 1;
}

s32 Script_JumpToLabel(struct ScriptInterpreter *interpreter) {
  interpreter->cursor = Script_FindLabel(
      interpreter, interpreter->script[interpreter->cursor + 1]);
  return 1;
}

u32 Script_JumpIfTrue(struct ScriptInterpreter *interpreter) {
  s32 index = interpreter->cursor;
  const s32 *table = interpreter->script;
  u32 value = table[index + 1];

  if (interpreter->conditionResult != 0) {
    interpreter->cursor = Script_FindLabel(interpreter, value);
  } else {
    interpreter->cursor += 2;
  }
  return 1;
}

u32 Script_JumpIfFalse(struct ScriptInterpreter *interpreter) {
  s32 index = interpreter->cursor;
  const s32 *table = interpreter->script;
  u32 value = table[index + 1];

  if (interpreter->conditionResult == 0) {
    interpreter->cursor = Script_FindLabel(interpreter, value);
  } else {
    interpreter->cursor += 2;
  }
  return 1;
}


s32 Script_End(struct ScriptInterpreter *interpreter) {
  s32 result;

  interpreter->script = gScript_MainScript;
  interpreter->cursor = (result = 0);
  return result;
}

s32 GameFlag_Test(s32);

s32 Script_TestFlag(struct ScriptInterpreter *interpreter) {
  s32 done;

  interpreter->conditionResult =
      GameFlag_Test(interpreter->script[interpreter->cursor + 1]);
  done = 1;
  interpreter->cursor = (u16)interpreter->cursor + 2;
  return done;
}

void GameFlag_ClearBit(s32);

s32 Script_SetFlagAndTest(struct ScriptInterpreter *interpreter) {
  s32 value;

  value = interpreter->script[interpreter->cursor + 1];
  interpreter->conditionResult = GameFlag_Test(value);
  GameFlag_SetBit(value);
  interpreter->cursor += 2;
  return 1;
}

s32 Script_ClearFlagAndTest(struct ScriptInterpreter *interpreter) {
  s32 value;

  value = interpreter->script[interpreter->cursor + 1];
  interpreter->conditionResult = GameFlag_Test(value);
  GameFlag_ClearBit(value);
  interpreter->cursor += 2;
  return 1;
}

s32 Script_ToggleFlag(struct ScriptInterpreter *interpreter) {
  s32 value;

  value = interpreter->script[interpreter->cursor + 1];
  interpreter->conditionResult = GameFlag_Test(value);
  if (interpreter->conditionResult == 1)
    GameFlag_ClearBit(value);
  else
    GameFlag_SetBit(value);
  interpreter->cursor += 2;
  return 1;
}

struct Actor;
void Actor_SelectAnimation(struct Actor *object, s32 argument);

s32 Script_SetAnimation(struct ScriptInterpreter *interpreter) {
  Actor_SelectAnimation((struct Actor *)interpreter,
                        interpreter->script[interpreter->cursor + 1]);
  interpreter->cursor += 2;
  return 1;
}

void Actor_Release();

s32 Actor_ReleaseAndClear(void *object) {
  Actor_Release(object);
  return 0;
}

s32 Script_Nop(struct ScriptInterpreter *interpreter) {
  interpreter->cursor += 2;
  return 1;
}

#include "field_event.h"

s32 Script_PlaySound(struct ScriptInterpreter *interpreter) {
  Sound_PlayFar(interpreter->script[interpreter->cursor + 1]);
  interpreter->cursor += 2;
  return 1;
}

s32 Script_CallWithOperand(struct ScriptInterpreter *interpreter) {
  Func_080c88c8(interpreter, interpreter->script[interpreter->cursor + 1]);
  interpreter->cursor += 2;
  return 1;
}

s32 ScriptObject_CheckOverlap(struct Actor *object, s32 *values) {
  s32 index;
  u8 *flags;
  struct Actor *entry = Ram_HeapSlots->scriptObjects;

  flags = &entry->collisionFlags;
  for (index = 0; index < 64; index++, entry++, flags += sizeof(*entry)) {
    if (entry->script != NULL && (1 & *flags) && entry != object) {
      if (Field_CheckSpheresOverlap(&entry->x, entry->radius - 2, values,
                                    object->radius - 2) >= 0)
        return -1;
    }
  }
  return 0;
}

struct Actor *ScriptObject_FindOverlappingEntry(struct Actor *object,
                                                s32 *values) {
  s32 index;
  u8 *flags;
  struct Actor *entry = Ram_HeapSlots->scriptObjects;

  flags = &entry->collisionFlags;
  for (index = 0; index < 64; index++, entry++, flags += sizeof(*entry)) {
    if (entry->script != NULL && (1 & *flags) && entry != object) {
      if (Field_CheckSpheresOverlap(&entry->x, entry->radius - 2, values,
                                    object->radius - 2) >= 0)
        return entry;
    }
  }
  return NULL;
}
#undef TARGET_UNSET

// ---- games/tla/src/field/script/apply_relative_position.c ----
#include "actor_script_view.h"

s32 Func_0802d45c(s32 terrainId, s32 x, s32 z);

s32 Func_08024fe0(struct ActorScriptView *object) {
  const s32 *argument = object->script + (s16)object->scriptCursor + 1;
  s32 x = *argument++;
  s32 y = *argument++;
  s32 z = *argument;
  s32 height;

  Actor_SetPositionAndResetMotion(object, x, y, z);
  if (y == -1 && (object->flags & 1)) {
    height = Func_0802d45c(object->terrainId, object->x, object->z);
    object->y = object->y - object->terrainHeight + height;
    object->terrainHeight = height;
  }
  object->scriptCursor += 4;
  return 1;
}

s32 Func_08025038(struct ActorScriptView *object) {
  s32 height;

  Actor_SetPositionAndResetMotion(object, 0, object->y, 0);
  if (object->flags & 1) {
    height = Func_0802d45c(object->terrainId, object->x, object->z);
    object->y = object->y - object->terrainHeight + height;
    object->terrainHeight = height;
  }
  object->scriptCursor += 1;
  return 1;
}

/// SC_MOVE(x, y, z): sends the object towards (x, y, z).
s32 ActorScript_ApplyAbsolutePosition(struct ActorScriptView *object) {
  const s32 *argument = object->script + (s16)object->scriptCursor + 1;
  s32 x = *argument++;
  s32 y = *argument++;
  s32 z = *argument;

  Actor_SetMoveTarget(object, x, y, z);
  object->scriptCursor += 4;
  return 1;
}

s32 Func_0802509c(struct ActorScriptView *object) {
  const s32 *argument = object->script + (s16)object->scriptCursor + 1;
  s32 x = *argument++;
  s32 z = *argument;

  Actor_SetMoveTarget(object, x, object->y, z);
  object->scriptCursor += 3;
  return 1;
}

s32 Script_ApplyRelativePosition(struct ActorScriptView *object) {
  const s32 *cursor = object->script + (s16)object->scriptCursor + 1;
  s32 first = *cursor++;
  s32 second = *cursor++;
  s32 third = *cursor;

  Actor_SetMoveTarget(object, object->x + first, object->y + second,
                       object->z + third);
  object->scriptCursor += 4;
  return 1;
}


s32 Script_StoreAngleToWatchedObject(struct ActorScriptView *object) {
  struct ActorScriptView *target;

  target = object->watchedObject;
  object->scriptValue = ArcTan2((s32)((u32)target->z - (u32)object->z),
                                (s32)((u32)target->x - (u32)object->x));
  object->scriptCursor++;
  return 1;
}

s32 Script_ApplyWatchedObjectPosition(struct ActorScriptView *object) {
  struct ActorScriptView *target;

  target = object->watchedObject;
  Actor_SetMoveTarget(object, target->x, target->y, target->z);
  object->scriptCursor++;
  return 1;
}

// ---- games/tla/src/field/script/offset.c ----

s32 Script_ApplyLocalOffsetPosition(struct Actor *object) {
  s32 offset[3];

  Actor_SetMoveTarget(object, object->x + offset[0], object->y + offset[1],
                      object->z + offset[2]);
  object->step += 3;
  return 1;
}


#define TARGET_UNSET ((s32)0x80000000)
#define DISTANCE(a, b) ((a) - (b) < 0 ? (b) - (a) : (a) - (b))

s32 IwramMulQ16(s32 a, s32 b) __attribute__((long_call));

s32 Func_08025160(struct Actor *interpreter) {
  struct Actor *cameraObject = interpreter;
  struct Actor *leader;
  struct MapWork *bounds;
  s32 minX;
  s32 minZ;
  s32 maxX;
  s32 maxZ;
  s32 x;
  s32 y;
  s32 z;
  s32 dz;
  s32 dy;
  s32 dx;
  s32 distance;
  s32 moveStep;
  s16 *region;

  bounds = Ram_HeapSlots->mapWork;
  leader = cameraObject->watchedObject;
  minX = bounds->minX + 0x780000;
  minZ = bounds->minZ + leader->y + 0x600000;
  maxX = bounds->maxX - 0x780000;
  maxZ = bounds->maxZ + leader->y - 0x400000;
  cameraObject->motionFlags = 0;
  if (leader != NULL && leader->script != NULL) {
    x = leader->x;
    y = leader->y;
    z = leader->z;
    cameraObject->targetX = TARGET_UNSET;
    cameraObject->targetY = TARGET_UNSET;
    cameraObject->targetZ = TARGET_UNSET;
    if (x < minX)
      x = minX;
    if (z < minZ)
      z = minZ;
    if (x > maxX)
      x = maxX;
    if (z > maxZ)
      z = maxZ;
    if (*(s16 **)bounds->unk0fc != NULL) {
      region = *(s16 **)bounds->unk0fc;
      while (region[0] != -1) {
        minX = (region[0] << 16) - 0x780000;
        minZ = (region[1] << 16) + leader->y - 0x400000;
        maxX = (region[2] << 16) + 0x780000;
        maxZ = (region[3] << 16) + leader->y + 0x600000;
        if (x >= minX && z >= minZ && x <= maxX && z <= maxZ) {
          dx = minX;
          if (DISTANCE(minX, x) > DISTANCE(maxX, x))
            dx = maxX;
          dz = minZ;
          if (DISTANCE(minZ, z) > DISTANCE(maxZ, z))
            dz = maxZ;
          if (DISTANCE(dx, x) <= DISTANCE(dz, z))
            x = dx;
          else
            z = dz;
          break;
        }
        region += 4;
      }
    }
    if (cameraObject->timer != 0) {
      cameraObject->x = x;
      cameraObject->y = y;
      cameraObject->z = z;
    } else {
      dx = (x - cameraObject->x) / 0x10000;
      /* FAKEMATCH(dead): this dy, overwritten below, as in The Broken Seal's
       * FieldCamera_FollowLeaderInBounds, which steers register allocation;
       * without it, 129 of 345 instructions differ in tla-en. */
      dy = (y - cameraObject->y) / 0x10000;
      dz = (z - cameraObject->z) / 0x10000;
      distance = Iwram_Sqrt(dx * dx + dz * dz) << 16;
      dx = x - cameraObject->x;
      dy = y - cameraObject->y;
      dz = z - cameraObject->z;
      if (distance < 0x400000)
        distance = FixedSqrt(IwramMulQ16(dx, dx) + IwramMulQ16(dz, dz));
      if ((moveStep = distance / 8) > cameraObject->speedLimit)
        moveStep = cameraObject->speedLimit;
      if (distance < 0x4000) {
        cameraObject->x = x;
        cameraObject->z = z;
      } else {
        if (distance > moveStep) {
          s32 ratio = Iwram_RatioMulQ14(distance, dx);
          s32 (*mulQ16)(s32, s32) = IwramMulQ16;
          dx = mulQ16(ratio, moveStep);
          dz = mulQ16(Iwram_RatioMulQ14(distance, dz), moveStep);
        }
        cameraObject->x += dx;
        cameraObject->z += dz;
      }
      if ((dy < 0 ? -dy : dy) > 0x8000)
        dy /= 4;
      cameraObject->y += dy;
    }
  }
  cameraObject->step++;
  return 1;
}


s32 Func_08025428(struct Actor *cameraObject) {
  struct Actor *leader;
  struct MapWork *bounds;
  s32 minX;
  s32 minZ;
  s32 maxX;
  s32 maxZ;
  s32 x;
  s32 y;
  s32 z;
  s32 dz;
  s32 dy;
  s32 dx;
  s32 distance;
  s32 moveStep;

  bounds = Ram_HeapSlots->mapWork;
  leader = cameraObject->watchedObject;
  minX = bounds->minX + 0x780000;
  minZ = bounds->minZ + 0x600000;
  maxX = bounds->maxX - 0x780000;
  maxZ = bounds->maxZ - 0x400000;
  cameraObject->motionFlags = 0;
  if (leader != NULL && leader->script != NULL) {
    x = leader->x;
    y = leader->y;
    z = leader->z;
    cameraObject->targetX = TARGET_UNSET;
    cameraObject->targetY = TARGET_UNSET;
    cameraObject->targetZ = TARGET_UNSET;
    if (x < minX)
      x = minX;
    if (z < minZ)
      z = minZ;
    if (x > maxX)
      x = maxX;
    if (z > maxZ)
      z = maxZ;
    if (cameraObject->timer != 0) {
      cameraObject->x = x;
      cameraObject->y = y;
      cameraObject->z = z;
    } else {
      dx = (x - cameraObject->x) / Q16_ONE;
      /* FAKEMATCH(dead): this dy, overwritten below, ranks y above the bounds
       * in global allocation and keeps it in r5; without it, 54 of 228
       * instructions differ in tla-en. */
      dy = (y - cameraObject->y) / Q16_ONE;
      dz = (z - cameraObject->z) / Q16_ONE;
      distance = Iwram_Sqrt(dx * dx + dz * dz) << 16;
      dx = x - cameraObject->x;
      dy = y - cameraObject->y;
      dz = z - cameraObject->z;
      if (distance < 0x400000)
        distance = FixedSqrt(Iwram_MulQ16(dx, dx) + Iwram_MulQ16(dz, dz));
      if ((moveStep = distance / 8) > cameraObject->speedLimit)
        moveStep = cameraObject->speedLimit;
      if (distance < 0x4000) {
        cameraObject->x = x;
        cameraObject->z = z;
      } else {
        if (distance > moveStep) {
          dx = Iwram_MulQ16(Iwram_RatioMulQ14(distance, dx), moveStep);
          dz = Iwram_MulQ16(Iwram_RatioMulQ14(distance, dz), moveStep);
        }
        cameraObject->x += dx;
        cameraObject->z += dz;
      }
      if ((dy < 0 ? -dy : dy) > 0x8000)
        dy /= 4;
      cameraObject->y += dy;
    }
  }
  cameraObject->step++;
  return 1;
}

void Actor_SelectAnimation(struct Actor *object, s32 argument);

/// SC_APPROACH_WATCHED: follows the watched object at its speed. More than 16
/// pixels away it walks to 16 pixels short of it (animation 2) and runs on;
/// within 16 it stands (animation 1) and waits for the next frame.
// As ☀️'s function of the same name.
s32 Actor_ApproachWatchedActor(struct Actor *object) {
  s32 distancePixels;
  s32 moveZ;
  s32 dx;
  s32 dz;
  s32 deltaZPixels;
  s32 deltaXPixels;
  s32 remainingDistance;
  s32 moveX;
  struct Actor *watchedObject;

  watchedObject = object->watchedObject;
  object->speedLimit = watchedObject->speedLimit;
  object->acceleration = watchedObject->acceleration;
  dx = watchedObject->x - object->x;
  dz = watchedObject->z - object->z;
  deltaXPixels = dx >> 16;
  deltaZPixels = dz >> 16;
  distancePixels =
      Iwram_Sqrt((deltaXPixels * deltaXPixels) + (deltaZPixels * deltaZPixels));
  if (distancePixels > 0x10) {
    remainingDistance = distancePixels - 0x10;
    moveX = dx * remainingDistance / distancePixels;
    moveZ = dz * remainingDistance / distancePixels;
    Actor_SetMoveTarget(object, object->x + moveX, object->y,
                        object->z + moveZ);
    Actor_SelectAnimation(object, 2);
    object->step = (object->step + 1);
    return 1;
  }
  Actor_SelectAnimation(object, 1);
  return 0;
}


struct ScriptObjectEntry;

struct FieldPosition {
  s32 x;
  s32 y;
  s32 z;
};

void Vector_AddPolarOffset(s32 radius, s32 angle,
                           struct FieldPosition *position);

s32 Func_08025698(struct ActorScriptView *object) {
  struct FieldPosition pos;
  struct FieldPosition probe;
  s32 base;
  const s32 *args;
  s32 range;
  s32 limit;
  s32 radius;
  s32 heading;
  s32 tries;
  s32 dx;
  s32 dz;

  args = &object->script[(s16)object->scriptCursor + 1];
  base = *args++;
  range = *args++;
  limit = *args / 0x10000;
  // The tries are a loop written with goto: as while (++tries <= 7) with
  // continue, 116 of 188 instructions differ in tla-en.
  tries = 0;
  limit = limit * limit;
retry:
  tries++;
  if (tries <= 7) {
    pos.x = object->x;
    pos.y = object->y;
    pos.z = object->z;
    radius = base + Iwram_MulQ16(Random16(), range);
    heading = object->scriptValue + (Random16() >> 2) - (Random16() >> 2);
    Vector_AddPolarOffset(radius, heading, &pos);
    if (ScriptObject_CheckOverlap((struct ScriptObjectEntry *)object,
                                  (s32 *)&pos) != 0)
      goto retry;
    if (Func_0802d87c((struct Actor *)object, &pos) != 0)
      goto retry;
    radius += 0x80000;
    probe.x = object->x;
    probe.y = object->y;
    probe.z = object->z;
    Vector_AddPolarOffset(radius, heading, &probe);
    probe.x = object->x;
    probe.y = object->y;
    probe.z = object->z;
    Vector_AddPolarOffset(radius, heading + 0x2000, &probe);
    if (Func_0802d87c((struct Actor *)object, &probe) != 0)
      goto retry;
    probe.x = object->x;
    probe.y = object->y;
    probe.z = object->z;
    Vector_AddPolarOffset(radius, heading - 0x2000, &probe);
    if (Func_0802d87c((struct Actor *)object, &probe) != 0)
      goto retry;
    // The offsets in a once-only block keep the division on the loaded x; as
    // plain statements, 16 of 188 instructions differ in tla-en.
    do {
      dx = pos.x / 0x10000 - object->homeX;
      dz = pos.z / 0x10000 - object->homeZ;
    } while (0);
    if (dx * dx + dz * dz > limit)
      goto retry;
    goto found;
  }
  object->scriptValue += 0x8000;
  object->turnedBack = 1;
  return 0;
found:
  {
    /* FAKEMATCH(pin): the object moved to r0 before the target's words load;
     * as a plain local, 2 of 188 instructions differ in tla-en. */
    register struct Actor *moved asm("r0") = (struct Actor *)object;

    /* FAKEMATCH(barrier): the empty asm holding the pin above, which keeps the
     * move ahead of the loads; without it, 2 of 188 instructions differ in
     * tla-en. */
    asm("");
    Actor_SetMoveTarget(moved, pos.x, pos.y, pos.z);
  }
  object->scriptCursor += 4;
  return 1;
}

// The script command that wanders near home.

struct ScriptObjectEntry;

s32 Func_0802d87c(struct Actor *object, struct FieldPosition *position);

/// Moves the object to a random spot within its home radius, as The Broken
/// Seal's ActorScript_WanderNearHome does: up to seven tries at a point a
/// random distance away in a random turn of its heading, each refused if
/// anything overlaps it or blocks the path or the cells beside it. An object
/// outside its home radius (or out of tries) heads back toward home instead.
/// Operands: the minimum distance, the distance range and the
/// home radius.
s32 Func_08025830(struct ActorScriptView *object) {
  struct FieldPosition candidatePosition;
  struct FieldPosition probePosition;
  s32 minimumDistance;
  const s32 *operands;
  s32 distanceRange;
  s32 homeRadiusSquared;
  s32 travelDistance;
  u16 heading;
  u16 angle;
  s32 attempts;
  s32 dx;
  s32 dz;

  operands = &object->script[(s16)object->scriptCursor + 1];
  minimumDistance = *operands++;
  distanceRange = *operands++;
  homeRadiusSquared = *operands / 0x10000;
  angle = object->scriptValue;
  homeRadiusSquared *= homeRadiusSquared;
  attempts = 0;
  dx = object->x / 0x10000 - object->homeX;
  dz = object->z / 0x10000 - object->homeZ;
  if (dx * dx + dz * dz <= homeRadiusSquared) {
  roam:
    attempts++;
    if (attempts <= 7) {
      travelDistance =
          minimumDistance + Iwram_MulQ16(Random16(), distanceRange);
      heading = angle + (Random16() >> 2) - (Random16() >> 2);
      candidatePosition.x = object->x;
      candidatePosition.y = object->y;
      candidatePosition.z = object->z;
      Vector_AddPolarOffset(0x80000, heading, &candidatePosition);
      if (ScriptObject_CheckOverlap((struct ScriptObjectEntry *)object,
                                    (s32 *)&candidatePosition) != 0)
        goto roam;
      candidatePosition.x = object->x;
      candidatePosition.y = object->y;
      candidatePosition.z = object->z;
      Vector_AddPolarOffset(travelDistance, heading, &candidatePosition);
      if (Func_0802d87c((struct Actor *)object, &candidatePosition) != 0)
        goto roam;
      probePosition.x = object->x;
      probePosition.y = object->y;
      travelDistance += 0x80000;
      probePosition.z = object->z;
      Vector_AddPolarOffset(travelDistance, heading, &probePosition);
      if (Func_0802d87c((struct Actor *)object, &probePosition) != 0)
        goto roam;
      probePosition.x = object->x;
      probePosition.y = object->y;
      probePosition.z = object->z;
      Vector_AddPolarOffset(travelDistance, heading + 0x2000, &probePosition);
      if (Func_0802d87c((struct Actor *)object, &probePosition) != 0)
        goto roam;
      probePosition.x = object->x;
      probePosition.y = object->y;
      probePosition.z = object->z;
      Vector_AddPolarOffset(travelDistance, heading - 0x2000, &probePosition);
      if (Func_0802d87c((struct Actor *)object, &probePosition) != 0)
        goto roam;
      probePosition.x = object->x;
      probePosition.y = object->y;
      probePosition.z = object->z;
      Vector_AddPolarOffset(travelDistance, heading + 0x4000, &probePosition);
      if (Func_0802d87c((struct Actor *)object, &probePosition) != 0)
        goto roam;
      probePosition.x = object->x;
      probePosition.y = object->y;
      probePosition.z = object->z;
      Vector_AddPolarOffset(travelDistance, heading - 0x4000, &probePosition);
      if (Func_0802d87c((struct Actor *)object, &probePosition) != 0)
        goto roam;
      dx = candidatePosition.x / 0x10000 - object->homeX;
      dz = candidatePosition.z / 0x10000 - object->homeZ;
      if (dx * dx + dz * dz > homeRadiusSquared)
        goto roam;
      object->flags59 |= 2;
      Actor_SetMoveTarget((struct Actor *)object, candidatePosition.x,
                          candidatePosition.y, candidatePosition.z);
      goto done;
    }
  }
  attempts = 0;
  angle = (u16)ArcTan2(dz, dx) + 0x8000;
back:
  attempts++;
  if (attempts <= 7) {
    travelDistance = minimumDistance + Iwram_MulQ16(Random16(), distanceRange);
    heading = angle + (Random16() >> 2) - (Random16() >> 2);
    candidatePosition.x = object->x;
    candidatePosition.y = object->y;
    candidatePosition.z = object->z;
    Vector_AddPolarOffset(0x80000, heading, &candidatePosition);
    if (ScriptObject_CheckOverlap((struct ScriptObjectEntry *)object,
                                  (s32 *)&candidatePosition) != 0)
      goto back;
    candidatePosition.x = object->x;
    candidatePosition.y = object->y;
    candidatePosition.z = object->z;
    Vector_AddPolarOffset(travelDistance, heading, &candidatePosition);
    if (Func_0802d87c((struct Actor *)object, &candidatePosition) != 0)
      goto back;
    object->flags59 &= ~2;
    Actor_SetMoveTarget((struct Actor *)object, candidatePosition.x,
                        candidatePosition.y, candidatePosition.z);
  }
done:
  object->scriptCursor += 4;
  return 1;
}

void Script_SetOrCompareAddress(struct ScriptOperands *state, s32 operation,
                                s32 value) {
  s8 result;

  if (operation == 0) {
    state->scriptAddress = value;
    return;
  }
  if (operation == 1) {
    state->scriptAddress += value * 4;
    return;
  }
  result = 0;
  if (state->scriptAddress == value) {
    result = 1;
  }
  state->comparisonResult = result;
}

void Script_SetOrCompareCursor(struct ScriptOperands *state, s32 operation,
                               s32 value) {
  if (operation == 0) {
    state->cursor = value;
  } else if (operation == 1) {
    state->cursor = (s16)state->cursor + value;
  } else {
    state->comparisonResult = (s16)state->cursor == (s16)value;
  }
}

void Script_SetOrCompareUnsignedHalfword(struct ScriptOperands *state,
                                         s32 operation, s32 value) {
  s32 result;

  if (operation == 0) {
    state->unsignedHalfword = value;
  } else if (operation == 1) {
    state->unsignedHalfword = (u16)((u32)state->unsignedHalfword + (u32)value);
  } else {
    u32 current = state->unsignedHalfword;

    result = 0;
    if (current == (u16)value)
      result = 1;
    state->comparisonResult = result;
  }
}

void Script_SetOrCompareX(struct ScriptOperands *state, s32 operation,
                          s32 value) {
  s8 result;

  if (operation == 0) {
    state->word08 = value;
    return;
  }
  if (operation == 1) {
    state->word08 += value;
    return;
  }
  result = 0;
  if (state->word08 == value) {
    result = 1;
  }
  state->comparisonResult = result;
}

void Script_SetOrCompareY(struct ScriptOperands *state, s32 operation,
                          s32 value) {
  s8 result;

  if (operation == 0) {
    state->word0c = value;
    return;
  }
  if (operation == 1) {
    state->word0c += value;
    return;
  }
  result = 0;
  if (state->word0c == value) {
    result = 1;
  }
  state->comparisonResult = result;
}

void Script_SetOrCompareZ(struct ScriptOperands *state, s32 operation,
                          s32 value) {
  s8 result;

  if (operation == 0) {
    state->word10 = value;
    return;
  }
  if (operation == 1) {
    state->word10 += value;
    return;
  }
  result = 0;
  if (state->word10 == value) {
    result = 1;
  }
  state->comparisonResult = result;
}

void Script_SetOrCompareRadius(struct ScriptOperands *state, s32 operation,
                               s32 value) {
  s8 result;
  u32 current;

  if (operation == 0) {
    state->halfword20 = value;
    return;
  }
  if (operation == 1) {
    state->halfword20 += value;
    return;
  }
  current = state->halfword20;
  result = 0;
  if (current == (u32)(s16)value)
    result = 1;
  state->comparisonResult = result;
}

void Script_SetOrCompareScaleX(struct ScriptOperands *state, s32 operation,
                               s32 value) {
  s8 result;

  if (operation == 0) {
    state->word18 = value;
    return;
  }
  if (operation == 1) {
    state->word18 += value;
    return;
  }
  result = 0;
  if (state->word18 == value) {
    result = 1;
  }
  state->comparisonResult = result;
}

void Script_SetOrCompareScaleY(struct ScriptOperands *state, s32 operation,
                               s32 value) {
  s8 result;

  if (operation == 0) {
    state->word1c = value;
    return;
  }
  if (operation == 1) {
    state->word1c += value;
    return;
  }
  result = 0;
  if (state->word1c == value) {
    result = 1;
  }
  state->comparisonResult = result;
}

void Script_SetOrCompareVelocityX(struct ScriptOperands *state, s32 operation,
                                  s32 value) {
  s8 result;

  if (operation == 0) {
    state->word24 = value;
    return;
  }
  if (operation == 1) {
    state->word24 += value;
    return;
  }
  result = 0;
  if (state->word24 == value) {
    result = 1;
  }
  state->comparisonResult = result;
}

void Script_SetOrCompareVelocityY(struct ScriptOperands *state, s32 operation,
                                  s32 value) {
  s8 result;

  if (operation == 0) {
    state->word28 = value;
    return;
  }
  if (operation == 1) {
    state->word28 += value;
    return;
  }
  result = 0;
  if (state->word28 == value) {
    result = 1;
  }
  state->comparisonResult = result;
}

void Script_SetOrCompareVelocityZ(struct ScriptOperands *state, s32 operation,
                                  s32 value) {
  s8 result;

  if (operation == 0) {
    state->word2c = value;
    return;
  }
  if (operation == 1) {
    state->word2c += value;
    return;
  }
  result = 0;
  if (state->word2c == value) {
    result = 1;
  }
  state->comparisonResult = result;
}

void Script_SetOrCompareSpeedLimit(struct ScriptOperands *state, s32 operation,
                                   s32 value) {
  s8 result;

  if (operation == 0) {
    state->word30 = value;
    return;
  }
  if (operation == 1) {
    state->word30 += value;
    return;
  }
  result = 0;
  if (state->word30 == value) {
    result = 1;
  }
  state->comparisonResult = result;
}

void Script_SetOrCompareAcceleration(struct ScriptOperands *state,
                                     s32 operation, s32 value) {
  s8 result;

  if (operation == 0) {
    state->word34 = value;
    return;
  }
  if (operation == 1) {
    state->word34 += value;
    return;
  }
  result = 0;
  if (state->word34 == value) {
    result = 1;
  }
  state->comparisonResult = result;
}

void Script_SetOrCompareTargetX(struct ScriptOperands *state, s32 operation,
                                s32 value) {
  s8 result;

  if (operation == 0) {
    state->word38 = value;
    return;
  }
  if (operation == 1) {
    state->word38 += value;
    return;
  }
  result = 0;
  if (state->word38 == value) {
    result = 1;
  }
  state->comparisonResult = result;
}

void Script_SetOrCompareTargetY(struct ScriptOperands *state, s32 operation,
                                s32 value) {
  s8 result;

  if (operation == 0) {
    state->word3c = value;
    return;
  }
  if (operation == 1) {
    state->word3c += value;
    return;
  }
  result = 0;
  if (state->word3c == value) {
    result = 1;
  }
  state->comparisonResult = result;
}

void Script_SetOrCompareTargetZ(struct ScriptOperands *state, s32 operation,
                                s32 value) {
  s8 result;

  if (operation == 0) {
    state->word40 = value;
    return;
  }
  if (operation == 1) {
    state->word40 += value;
    return;
  }
  result = 0;
  if (state->word40 == value) {
    result = 1;
  }
  state->comparisonResult = result;
}

void Script_SetOrCompareVerticalMotion(struct ScriptOperands *state,
                                       s32 operation, s32 value) {
  s8 result;

  if (operation == 0) {
    state->word44 = value;
    return;
  }
  if (operation == 1) {
    state->word44 += value;
    return;
  }
  result = 0;
  if (state->word44 == value) {
    result = 1;
  }
  state->comparisonResult = result;
}

void Script_SetOrCompareGravity(struct ScriptOperands *state, s32 operation,
                                s32 value) {
  s8 result;

  if (operation == 0) {
    state->word48 = value;
    return;
  }
  if (operation == 1) {
    state->word48 += value;
    return;
  }
  result = 0;
  if (state->word48 == value) {
    result = 1;
  }
  state->comparisonResult = result;
}

void Script_SetOrCompareTerrainHeight(struct ScriptOperands *state,
                                      s32 operation, s32 value) {
  s8 result;

  if (operation == 0) {
    state->word14 = value;
    return;
  }
  if (operation == 1) {
    state->word14 += value;
    return;
  }
  result = 0;
  if (state->word14 == value) {
    result = 1;
  }
  state->comparisonResult = result;
}

void Script_SetOrCompareWord4c(struct ScriptOperands *state, s32 operation,
                               s32 value) {
  s8 result;

  if (operation == 0) {
    state->word4c = value;
    return;
  }
  if (operation == 1) {
    state->word4c += value;
    return;
  }
  result = 0;
  if (state->word4c == value) {
    result = 1;
  }
  state->comparisonResult = result;
}

void Script_SetOrCompareAddress50(struct ScriptOperands *state, s32 operation,
                                  s32 value) {
  s8 result;

  if (operation == 0) {
    state->address50 = value;
    return;
  }
  if (operation == 1) {
    state->address50 += value * 4;
    return;
  }
  result = 0;
  if (state->address50 == value) {
    result = 1;
  }
  state->comparisonResult = result;
}

void Script_SetOrCompareByte54(struct ScriptOperands *state, s32 operation,
                               s32 value) {
  if (operation == 0) {
    state->byte54 = value;
  } else if (operation == 1) {
    state->byte54 += value;
  } else {
    state->comparisonResult = state->byte54 == (u8)value;
  }
}

void Script_SetOrCompareMotionFlags(struct ScriptOperands *state, s32 operation,
                                    s32 value) {
  if (operation == 0) {
    state->byte55 = value;
  } else if (operation == 1) {
    state->byte55 += value;
  } else {
    state->comparisonResult = state->byte55 == (u8)value;
  }
}

void Script_SetOrCompareArrivalAxis(struct ScriptOperands *state, s32 operation,
                                    s32 value) {
  if (operation == 0) {
    state->byte56 = value;
  } else if (operation == 1) {
    state->byte56 += value;
  } else {
    state->comparisonResult = state->byte56 == (u8)value;
  }
}

void Script_SetOrCompareComparisonResult(struct ScriptOperands *state,
                                         s32 operation, s32 value) {
  s32 result;

  if (operation == 0) {
    state->comparisonResult = value;
  } else if (operation == 1) {
    state->comparisonResult = (u8)((u32)state->comparisonResult + (u32)value);
  } else {
    u8 current = state->comparisonResult;

    result = 0;
    if (current == (u8)value)
      result = 1;
    state->comparisonResult = result;
  }
}

typedef void (*OperandFunc)(struct ScriptOperands *, s32, s32);
extern OperandFunc gScript_OperandHandlerTable[];

void Script_SetOrCompareSnapToTarget(struct ScriptOperands *state,
                                     s32 operation, s32 value) {
  if (operation == 0) {
    state->byte58 = value;
  } else if (operation == 1) {
    state->byte58 += value;
  } else {
    state->comparisonResult = state->byte58 == (u8)value;
  }
}

void Script_SetOrCompareCollisionFlags(struct ScriptOperands *state,
                                       s32 operation, s32 value) {
  if (operation == 0) {
    state->byte59 = value;
  } else if (operation == 1) {
    state->byte59 += value;
  } else {
    state->comparisonResult = state->byte59 == (u8)value;
  }
}

void Script_SetOrCompareSteeringFlags(struct ScriptOperands *state,
                                      s32 operation, s32 value) {
  if (operation == 0) {
    state->byte5a = value;
  } else if (operation == 1) {
    state->byte5a += value;
  } else {
    state->comparisonResult = state->byte5a == (u8)value;
  }
}

void Script_SetOrComparePaused(struct ScriptOperands *state, s32 operation,
                               s32 value) {
  if (operation == 0) {
    state->byte5b = value;
  } else if (operation == 1) {
    state->byte5b += value;
  } else {
    state->comparisonResult = state->byte5b == (u8)value;
  }
}

void Script_SetOrCompareRepeatCount(struct ScriptOperands *work, s32 operation,
                                    s32 value) {
  s32 result;

  if (operation == 0) {
    work->byte5d = value;
  } else if (operation == 1) {
    work->byte5d += value;
  } else {
    result = 0;
    if (work->byte5d == (u8)value)
      result = 1;
    work->comparisonResult = result;
  }
}

void Script_SetOrCompareHalfword5e(struct ScriptOperands *work, s32 operation,
                                   s32 value) {
  s8 result;

  if (operation == 0) {
    work->halfword5e = value;
    return;
  }
  if (operation == 1) {
    work->halfword5e += value;
    return;
  }
  result = 0;
  if ((s16)work->halfword5e == (s16)value) {
    result = 1;
  }
  work->comparisonResult = result;
}

void Script_SetOrCompareSignedHalfword64(struct ScriptOperands *work,
                                         s32 operation, s32 value) {
  if (operation == 0) {
    work->signedHalfword64 = value;
  } else if (operation == 1) {
    work->signedHalfword64 += value;
  } else {
    work->comparisonResult = work->signedHalfword64 == (s16)value;
  }
}

void Script_SetOrCompareSignedHalfword66(struct ScriptOperands *work,
                                         s32 operation, s32 value) {
  if (operation == 0) {
    work->signedHalfword66 = value;
  } else if (operation == 1) {
    work->signedHalfword66 += value;
  } else {
    work->comparisonResult = work->signedHalfword66 == (s16)value;
  }
}

void Script_SetOrCompareWord68(struct ScriptOperands *work, s32 operation,
                               s32 value) {
  s8 result;

  if (operation == 0) {
    work->word68 = value;
    return;
  }
  if (operation == 1) {
    work->word68 += value;
    return;
  }
  result = 0;
  if (work->word68 == value) {
    result = 1;
  }
  work->comparisonResult = result;
}

void Script_SetOrCompareUpdateAddress(struct ScriptOperands *work,
                                      s32 operation, s32 value) {
  s8 result;

  if (operation == 0) {
    work->word6c = value;
    return;
  }
  if (operation == 1) {
    work->word6c += value;
    return;
  }
  result = 0;
  if (work->word6c == value) {
    result = 1;
  }
  work->comparisonResult = result;
}

void Script_SetOrCompareByte62(struct ScriptOperands *work, s32 operation,
                               s32 value) {
  s32 result;
  if (operation == 0) {
    work->byte62 = value;
  } else if (operation == 1) {
    work->byte62 += value;
  } else {
    result = 0;
    if (work->byte62 == (u8)value)
      result = 1;
    work->comparisonResult = result;
  }
}

void Script_SetOrCompareByte63(struct ScriptOperands *work, s32 operation,
                               s32 value) {
  s32 result;
  if (operation == 0) {
    work->byte63 = value;
  } else if (operation == 1) {
    work->byte63 += value;
  } else {
    result = 0;
    if (work->byte63 == (u8)value)
      result = 1;
    work->comparisonResult = result;
  }
}

s32 Script_ApplyOperandSet(struct ScriptOperands *work) {
  s16 index = work->cursor;
  s32 *entry = (s32 *)work->scriptAddress + index + 1;
  OperandFunc callback = gScript_OperandHandlerTable[entry[0]];

  if (callback != 0)
    callback(work, 0, entry[1]);
  work->cursor += 3;
  return 1;
}

s32 Script_ApplyOperandAdd(struct ScriptOperands *work) {
  s16 index = work->cursor;
  s32 *entry = (s32 *)work->scriptAddress + index + 1;
  OperandFunc callback = gScript_OperandHandlerTable[entry[0]];

  if (callback != 0)
    callback(work, 1, entry[1]);
  work->cursor += 3;
  return 1;
}

s32 Script_ApplyOperandCompare(struct ScriptOperands *work) {
  s16 index = work->cursor;
  s32 *entry = (s32 *)work->scriptAddress + index + 1;
  OperandFunc callback = gScript_OperandHandlerTable[entry[0]];

  if (callback != 0)
    callback(work, 2, entry[1]);
  work->cursor += 3;
  return 1;
}

struct ScriptObject {
  u8 unk00[0x6c];
  s32 value;
};

void ScriptObject_SetValue(struct ScriptObject *object, s32 value) {
  if (object != NULL)
    object->value = value;
}

#include "input.h"
#include "frame.h"
#include "event_work.h"

struct PartyKeys {
  u8 unk000[0x23c];
  u16 dashKeys;
};

struct UnitHp {
  u8 unk00[0x38];
  s16 hp;
};

// The heading of each direction-pad combination, -1 for none.
extern const s16 gData_0802ec5c[16];

s32 Func_0802d958(struct Actor *object, struct FieldPosition *position);
void *Unit_Get(s32 owner);
s32 Func_0802d45c(s32 terrainId, s32 x, s32 z);

s32 Func_0802632c(struct Actor *object) {
  struct FieldPosition position;
  struct FieldPosition testPosition;
  struct FieldPosition contact;
  s16 angleOffsets[22];
  s32 collision;
  s16 heading;
  s32 angle;
  s32 movementAnimation;
  u16 direction;
  s32 angleIndex;
  s32 headingCount;
  struct Actor *other;
  s32 pressed;

  movementAnimation = 2;
  collision = 0;
  object->motionFlags = 4;
  if (gInput.held & ((struct PartyKeys *)&gMainRoutineWork)->dashKeys) {
    object->speedLimit = 0x8000;
    object->acceleration = 0x2000;
    movementAnimation = 5;
  } else {
    object->speedLimit = 0x8000;
    object->acceleration = 0x2000;
  }

  angle = gData_0802ec5c[(gInput.held >> 4) & 15];
  if ((u16)angle == 0xffff) {
    collision |= 4;
  } else {
    collision = 0;
    position.x = object->x;
    position.y = object->y;
    position.z = object->z;
    Vector_AddPolarOffset(0x80000, (u16)angle, &position);
    if (gDebugMode != 0 && (gInput.held & 0x200)) {
      heading = angle;
    } else {
      if (Func_0802d958(object, &position) == 0) {
        testPosition.x = object->x;
        testPosition.y = object->y;
        testPosition.z = object->z;
        Vector_AddPolarOffset(0x80000, (u16)angle + 0x1000, &testPosition);
        if (Func_0802d958(object, &testPosition) == 0) {
          testPosition.x = object->x;
          testPosition.y = object->y;
          testPosition.z = object->z;
          Vector_AddPolarOffset(0x80000, (u16)angle - 0x1000, &testPosition);
          if (Func_0802d958(object, &testPosition) == 0) {
            testPosition.x = object->x;
            testPosition.y = object->y;
            testPosition.z = object->z;
            Vector_AddPolarOffset(0x80000, (u16)angle + 0x2000, &testPosition);
            if (Func_0802d958(object, &testPosition) == 0) {
              testPosition.x = object->x;
              testPosition.y = object->y;
              testPosition.z = object->z;
              Vector_AddPolarOffset(0x80000, (u16)angle - 0x2000,
                                    &testPosition);
              if (Func_0802d958(object, &testPosition) == 0) {
                heading = angle;
                goto check_objects;
              }
            }
          }
        }
      }
      direction = angle;
      headingCount = 0;
      angleOffsets[headingCount++] = direction + 0x1000;
      angleOffsets[headingCount++] = direction - 0x1000;
      angleOffsets[headingCount++] = direction + 0x2000;
      angleOffsets[headingCount++] = direction - 0x2000;
      angleOffsets[headingCount++] = direction + 0x3000;
      angleOffsets[headingCount++] = direction - 0x3000;
      for (angleIndex = 0; angleIndex < headingCount; angleIndex++) {
        heading = angleOffsets[angleIndex];
        position.x = object->x;
        position.y = object->y;
        position.z = object->z;
        Vector_AddPolarOffset(0x80000, (u16)heading, &position);
        if (Func_0802d958(object, &position) != 0)
          continue;
        testPosition.x = object->x;
        testPosition.y = object->y;
        testPosition.z = object->z;
        Vector_AddPolarOffset(0x80000, (u16)heading + 0x1000, &testPosition);
        if (Func_0802d958(object, &testPosition) != 0)
          continue;
        testPosition.x = object->x;
        testPosition.y = object->y;
        testPosition.z = object->z;
        Vector_AddPolarOffset(0x80000, (u16)heading - 0x1000, &testPosition);
        if (Func_0802d958(object, &testPosition) != 0)
          continue;
        testPosition.x = object->x;
        testPosition.y = object->y;
        testPosition.z = object->z;
        Vector_AddPolarOffset(0x80000, (u16)heading + 0x2000, &testPosition);
        if (Func_0802d958(object, &testPosition) != 0)
          continue;
        testPosition.x = object->x;
        testPosition.y = object->y;
        testPosition.z = object->z;
        Vector_AddPolarOffset(0x80000, (u16)heading - 0x2000, &testPosition);
        if (Func_0802d958(object, &testPosition) == 0)
          goto check_objects;
      }
      position.x = object->x;
      position.y = object->y;
      position.z = object->z;
      collision |= 1;

    check_objects:
      contact.x = object->x;
      contact.y = object->y;
      contact.z = object->z;
      Vector_AddPolarOffset(0x40000, (u16)heading, &contact);
      other = Ram_HeapSlots->scriptObjects;
      for (angleIndex = 63; angleIndex >= 0; angleIndex -= 1, other++) {
        s32 radius;

        radius = object->radius - 2;
        if (other->script == 0)
          continue;
        if ((other->collisionFlags & 1) == 0)
          continue;
        if (other == object)
          continue;
        if (Field_CheckSpheresOverlap(&other->x, radius, &contact.x,
                                      other->radius - 2) < 0)
          continue;
        collision |= 2;
      }
    }
  }

  if (Ram_HeapSlots->eventWork != NULL) {
    struct EventWork *work = Ram_HeapSlots->eventWork;

    if (collision & 3)
      work->blockedSteps++;
    else
      work->blockedSteps = 0;
  }
  if (collision != 0) {
    s32 action;

    action = 9;
    if (((struct UnitHp *)Unit_Get(gMainRoutineWork.currentOwner))->hp == 0)
      action = 22;
    Actor_SelectAnimation(object, action);
  } else {
    Actor_SelectAnimation(object, movementAnimation);
  }

  if (collision != 0) {
    object->targetX = 0x80000000;
    object->targetY = 0x80000000;
    object->targetZ = 0x80000000;
    if (collision & 3) {
      s32 difference;

      difference = (u16)angle - object->facing;
      difference = (s16)difference;
      if (difference > 0x1000)
        difference = 0x1000;
      if (difference < -0x1000)
        difference = -0x1000;
      object->facing += difference;
    }
    object->timer = 0;
    object->unk66 = 2;
  } else {
    Actor_SetMoveTarget(object, position.x, position.y, position.z);
    if (object->timer != 0)
      object->timer--;
  }

  pressed = 0;
  if (Ram_HeapSlots->eventWork == NULL)
    return 0;
  if ((gInput.pressed & gMainRoutineWork.keys234) ||
      (gInput.pressed & gMainRoutineWork.keys230) ||
      (gInput.pressed & gMainRoutineWork.keys236) ||
      (gInput.pressed & gMainRoutineWork.keys238) ||
      (gInput.pressed & gMainRoutineWork.keys23a))
    pressed = 1;
  if (pressed) {
    Sound_PlayFar(113);
  } else {
    object->terrainHeight =
        Func_0802d45c(object->terrainId, object->x, object->z);
    object->step++;
  }
  return 1;
}
#undef TARGET_UNSET
#undef DISTANCE

// ---- games/tla/src/field/script/f0267bc.c ----

/* VIEW(f0267bc): tag struct PartyKeys as struct PartyKeys__f0267bc; resource_slot.c has another definition; tried with the definition the rest of the object has, it does not compile in tla-ja: games/tla/src/field/resource_slot.c:4350: structure has no member named `keys23c'. */
struct PartyKeys__f0267bc {
  u8 unk000[0x23c];
  u16 keys23c;
};

struct LeaderUnit {
  u8 unk00[0x38];
  s16 hp;
};

extern u8 gDebugMode;
// The heading of each direction-pad combination, -1 for none.
extern const s16 gData_0802ec5c[16];

void Vector_AddPolarOffset(s32 radius, s32 angle,
                           struct FieldPosition *position);
s32 Func_0802d87c(struct Actor *object, struct FieldPosition *position);
s32 Field_CheckSpheresOverlap(s32 *a, s32 radiusA, s32 *b, s32 radiusB);
s32 ScriptObject_CheckOverlap(struct Actor *object, s32 *values);
/* VIEW(f0267bc): routine Unit_Get as struct LeaderUnit *Unit_Get(s32 owner); offset has void *Unit_Get(s32 owner); tried as the definition's void *Unit_Get(s32 owner), which the object already declares, it does not compile in tla-ja: games/tla/src/field/resource_slot.c:4584: request for member `hp' in something not a structure or union. */
extern struct LeaderUnit *Unit_Get__f0267bc(s32 owner) __asm__("Unit_Get");
void Actor_SelectAnimation(struct Actor *object, s32 argument);
void Actor_SetMoveTarget(struct Actor *actor, s32 x, s32 y, s32 z);
s32 Field_CheckConfiguredKeys(void);

// Two 16.16 coordinates fall in the same 16-pixel cell, rounding to nearest.
#define SAME_CELL(a, b) ((((a) - (b) + 0x80000) >> 20) == 0)

s32 Func_080267bc(struct Actor *object) {
  struct FieldPosition position;
  struct FieldPosition testPosition;
  struct FieldPosition contact;
  s16 angleOffsets[22];
  s32 collision;
  s32 pushed;
  s32 movementAnimation;
  u16 angle;
  s16 heading;
  s32 angleIndex;
  s32 onObject;
  struct Actor *other;
  struct EventWork *work;

  movementAnimation = 2;
  collision = 0;
  pushed = 0;
  if (gInput.held & ((struct PartyKeys__f0267bc *)&gMainRoutineWork)->keys23c) {
    object->speedLimit = 0x10000;
    object->acceleration = 0xccc;
    movementAnimation = 5;
  } else {
    object->speedLimit = 0x10000;
    object->acceleration = 0xccc;
  }

  angle = gData_0802ec5c[(gInput.held >> 4) & 15];
  if (angle == 0xffff) {
    collision |= 4;
    goto counted;
  }
  // DEAD: collision is already 0 here.
  collision = 0;
  position.x = object->x;
  position.y = object->y;
  position.z = object->z;
  Vector_AddPolarOffset(0x80000, angle, &position);
  if (object->priorityFlags & 0x40) {
    s32 withinHalfCell;

    onObject = 0;
    other = (struct Actor *)Ram_HeapSlots->scriptObjects;
    withinHalfCell = 0x7ffff;
    for (angleIndex = 0; angleIndex < 64; angleIndex++, other++) {
      s32 delta;
      s32 dy;

      if (other->script == NULL)
        continue;
      if (!(other->priorityFlags & 0x80))
        continue;
      if (other == object)
        continue;
      delta = object->y - other->y;
      dy = delta - 0x100000;
      if (dy < 0)
        dy = 0x100000 - delta;
      if (dy > withinHalfCell)
        continue;
      if (SAME_CELL(object->x, other->x) && SAME_CELL(object->z, other->z))
        onObject = 1;
      if (SAME_CELL(position.x, other->x) && SAME_CELL(position.z, other->z))
        goto clear;
    }
    if (onObject && !Func_0802d87c(object, &position)) {
      // DEAD: collision is already 0 here.
      collision = 0;
      goto counted;
    }
  }
  /* FAKEMATCH(dead): a test that sets onObject to 1 when it is nonzero, which
   * nothing reads after; as no test, 12 of 713 instructions differ in
   * tla-en. */
  if (onObject)
    onObject = 1;
  if (gDebugMode != 0 && (gInput.held & 0x200)) {
    /* FAKEMATCH(dead): a store of the 0 collision already holds, which
     * cross-jumping folds into clear's; as goto clear, 36 of 713 instructions
     * differ in tla-en. */
    collision = 0;
    goto counted;
  }
  if (!Func_0802d87c(object, &position)) {
    testPosition.x = object->x;
    testPosition.y = object->y;
    testPosition.z = object->z;
    Vector_AddPolarOffset(0x80000, angle + 0x1000, &testPosition);
    if (!Func_0802d87c(object, &testPosition)) {
      testPosition.x = object->x;
      testPosition.y = object->y;
      testPosition.z = object->z;
      Vector_AddPolarOffset(0x80000, angle - 0x1000, &testPosition);
      if (!Func_0802d87c(object, &testPosition)) {
        testPosition.x = object->x;
        testPosition.y = object->y;
        testPosition.z = object->z;
        Vector_AddPolarOffset(0x80000, angle + 0x2000, &testPosition);
        if (!Func_0802d87c(object, &testPosition)) {
          testPosition.x = object->x;
          testPosition.y = object->y;
          testPosition.z = object->z;
          Vector_AddPolarOffset(0x80000, angle - 0x2000, &testPosition);
          if (!Func_0802d87c(object, &testPosition)) {
            heading = angle;
            goto push_objects;
          }
        }
      }
    }
  }
  {
    s32 headingCount = 0;

    angleOffsets[headingCount++] = angle + 0x1000;
    angleOffsets[headingCount++] = angle - 0x1000;
    angleOffsets[headingCount++] = angle + 0x2000;
    angleOffsets[headingCount++] = angle - 0x2000;
    angleOffsets[headingCount++] = angle + 0x3000;
    angleOffsets[headingCount++] = angle - 0x3000;
    /* FAKEMATCH(dead): a test of headingCount that is never true; as no test,
     * 183 of 713 instructions differ in tla-en. */
    if (headingCount == 0)
      goto clear;
    for (angleIndex = 0; angleIndex < headingCount; angleIndex++) {
      heading = angleOffsets[angleIndex];
      position.x = object->x;
      position.y = object->y;
      position.z = object->z;
      Vector_AddPolarOffset(0x80000, (u16)heading, &position);
      if (Func_0802d87c(object, &position))
        continue;
      testPosition.x = object->x;
      testPosition.y = object->y;
      testPosition.z = object->z;
      Vector_AddPolarOffset(0x80000, (u16)heading + 0x1000, &testPosition);
      if (Func_0802d87c(object, &testPosition))
        continue;
      testPosition.x = object->x;
      testPosition.y = object->y;
      testPosition.z = object->z;
      Vector_AddPolarOffset(0x80000, (u16)heading - 0x1000, &testPosition);
      if (Func_0802d87c(object, &testPosition))
        continue;
      testPosition.x = object->x;
      testPosition.y = object->y;
      testPosition.z = object->z;
      Vector_AddPolarOffset(0x80000, (u16)heading + 0x2000, &testPosition);
      if (Func_0802d87c(object, &testPosition))
        continue;
      testPosition.x = object->x;
      testPosition.y = object->y;
      testPosition.z = object->z;
      Vector_AddPolarOffset(0x80000, (u16)heading - 0x2000, &testPosition);
      if (!Func_0802d87c(object, &testPosition))
        goto push_objects;
    }
  }
  goto blocked;

clear:
  // DEAD: collision is already 0 here.
  collision = 0;
  goto counted;

blocked:
  position.x = object->x;
  position.y = object->y;
  position.z = object->z;
  collision |= 1;

push_objects:
  contact.x = object->x;
  contact.y = object->y;
  contact.z = object->z;
  Vector_AddPolarOffset(0x40000, (u16)heading, &contact);
  other = (struct Actor *)Ram_HeapSlots->scriptObjects;
  for (angleIndex = 63; angleIndex >= 0; angleIndex--, other++) {
    s32 radius;

    radius = object->radius - 2;
    if (other->script == NULL)
      continue;
    if (!(other->collisionFlags & 1))
      continue;
    if (other == object)
      continue;
    if (Field_CheckSpheresOverlap(&other->x, radius, &contact.x,
                                  other->radius - 2) < 0)
      continue;
    if ((other->collisionFlags & 2) && other->movementState == 0) {
      u16 direction;

      direction = ArcTan2(other->z - object->z, other->x - object->x);
      testPosition.x = other->x;
      testPosition.y = other->y;
      testPosition.z = other->z;
      Vector_AddPolarOffset(0x4000, direction, &testPosition);
      if (!ScriptObject_CheckOverlap(other, &testPosition.x)) {
        testPosition.x = other->x;
        testPosition.y = other->y;
        testPosition.z = other->z;
        Vector_AddPolarOffset(0xa0000, direction, &testPosition);
        if (!Func_0802d87c(other, &testPosition)) {
          testPosition.x = other->x;
          testPosition.y = other->y;
          testPosition.z = other->z;
          Vector_AddPolarOffset(0xa0000, direction + 0x1000, &testPosition);
          // The same probe is tested twice.
          if (!Func_0802d87c(other, &testPosition) &&
              !Func_0802d87c(other, &testPosition)) {
            testPosition.x = other->x;
            testPosition.y = other->y;
            testPosition.z = other->z;
            Vector_AddPolarOffset(0xa0000, direction - 0x1000, &testPosition);
            if (!Func_0802d87c(other, &testPosition)) {
              Vector_AddPolarOffset(0x4000, direction,
                                    (struct FieldPosition *)&other->x);
              other->targetX = 0x80000000;
              other->targetY = 0x80000000;
              other->targetZ = 0x80000000;
              pushed |= 1;
              continue;
            }
          }
        }
      }
    }
    collision |= 2;
  }
  if (collision == 0 && pushed != 0) {
    object->speedLimit = 0x4000;
    object->acceleration = 0x2000;
  }

counted:
  work = Ram_HeapSlots->eventWork;
  if (work != NULL) {
    if (collision & 3)
      work->blockedSteps++;
    else
      work->blockedSteps = 0;
  }
  if (pushed != 0) {
    Actor_SelectAnimation(object, 8);
  } else if (collision != 0) {
    s32 action;

    action = 9;
    if (Unit_Get__f0267bc(gMainRoutineWork.currentOwner)->hp == 0)
      action = 22;
    Actor_SelectAnimation(object, action);
  } else {
    Actor_SelectAnimation(object, movementAnimation);
  }

  if (collision != 0) {
    object->targetX = 0x80000000;
    object->targetY = 0x80000000;
    object->targetZ = 0x80000000;
    if (collision & 3) {
      s32 difference;

      difference = (s16)(angle - object->facing);
      if (difference > 0x1000)
        difference = 0x1000;
      if (difference < -0x1000)
        difference = -0x1000;
      object->facing += difference;
    }
    object->timer = 0;
    object->unk66 = 2;
  } else {
    Actor_SetMoveTarget(object, position.x, position.y, position.z);
    if (object->timer != 0)
      object->timer--;
  }
  Field_CheckConfiguredKeys();
  object->step++;
  return 1;
}

/// The requests the configured keys raise.
// Named as in ☀️'s FieldWork.
struct EventPrompt {
  u8 unk000[0x15a];
  u16 menu;
  u16 examine;
  u16 start;
  u8 unk160[6];
  u16 choice[2];
  u16 result;
  u8 unk16c[30];
  s8 mode;
  u8 unk18b[0xb21];
  s8 pendingSecond;
};

s32 GameFlag_Test(s32 flag);

s32 Func_08026dd0(u32 value) {
  struct EventPrompt *event = Ram_HeapSlots->eventWork;
  u32 kind = value >> 14;
  s32 code = value & 0x3fff;

  if (GameFlag_Test(0x107) != 0) {
    event->result = 250;
  } else if (event->mode == 3) {
    // The held keys, read again for the second test through a volatile
    // pointer; through a plain pointer, 14 of 69 instructions differ in
    // tla-en.
    volatile u32 *held = &gInput.held;

    if (*held & 0x100)
      event->result = 0xfc88;
    else if (*held & 0x200)
      event->result = 0xfc87;
  } else {
    switch (kind) {
    case 0:
      event->choice[0] = code;
      break;
    case 1:
      event->choice[1] = code;
      break;
    }
  }
  return code;
}

/// Checks the keys the player has configured: the menu, examine and start
/// keys set their request and return 1, the two shortcut keys pass on their
/// assignment (Func_08026dd0). While a second event is pending, any of the
/// five requests command 0x209b instead. Returns 0 when none is pressed.
// As ☀️'s function of the same name.
s32 Field_CheckConfiguredKeys(void) {
  struct EventPrompt *work = Ram_HeapSlots->eventWork;
  s32 result = 0;

  if (work == NULL)
    return 0;
  if (work->pendingSecond != 0) {
    if ((gInput.pressed & gMainRoutineWork.keys234) ||
        (gInput.pressed & gMainRoutineWork.keys230) ||
        (gInput.pressed & gMainRoutineWork.keys236) ||
        (gInput.pressed & gMainRoutineWork.keys238) ||
        (gInput.pressed & gMainRoutineWork.keys23a)) {
      work->choice[0] = 0x209b;
      return 1;
    }
    return 0;
  } else {
    if (gInput.pressed & gMainRoutineWork.keys234) {
      work->menu = 1;
      result = 1;
    } else if (gInput.pressed & gMainRoutineWork.keys230) {
      work->examine = 1;
      result = 1;
    } else if (gInput.pressed & gMainRoutineWork.keys236) {
      work->start = 1;
      result = 1;
    } else if (gInput.pressed & gMainRoutineWork.keys238) {
      result = Func_08026dd0(gMainRoutineWork.packedEvents[0]);
    } else if (gInput.pressed & gMainRoutineWork.keys23a) {
      result = Func_08026dd0(gMainRoutineWork.packedEvents[1]);
    }
  }
  return result;
}

s32 Field_CheckSpheresOverlap(s32 *a, s32 radius_a, s32 *b, s32 radius_b) {
  s32 dx = (*a++ - *b++) >> 16;
  s32 dy = (*a++ - *b++) >> 16;
  s32 dz = (*a - *b) >> 16;
  s32 radius = radius_a + radius_b;

  if (!(dx > 0x400000) && !(dz > 0x400000) &&
      dx * dx + dy * dy + dz * dz < radius * radius)
    return 0;
  return -1;
}


struct Actor *Func_08023220(s32 kind, s32 x, s32 y, s32 z);
extern const s32 gData_0802ec88[];

struct Child {
  u8 unk00[9];
  u8 low9 : 2;
  u8 layer : 2;
  u8 high9 : 4;
  u8 unk0a[0x10];
  u8 value1a;
};

void Func_08026fc8(struct Actor *object, s32 mode) {
  struct Actor *created;
  struct Child *sprite;

  created = Func_08023220(14, object->x, object->y + 0x40000, object->z);
  sprite = object->sprite;
  if (!(object->unk66 & 1))
    Sound_PlayFar(623);
  if (created != NULL) {
    struct Child *child = created->sprite;

    created->terrainHeight = object->terrainHeight;
    Actor_StartScript(created, gData_0802ec88);
    created->motionFlags = 0;
    if (child != NULL) {
      Animation_ApplyChildArgument((struct ActorSpriteView *)child, 1);
      child->value1a = 0;
      child->layer = sprite->layer;
      if (mode == 2) {
        Animation_ApplyChildValuesToRecord((struct ActorSpriteView *)child, 13);
        Animation_ApplyChildValue((struct ActorSpriteView *)child, 8);
      }
    }
  }
}
#undef SAME_CELL

// ---- games/tla/src/field/script/f027064.c ----
#include "version.h"


struct PlayerSprite {
  u8 unk00[9];
  u8 low09 : 2;
  u8 priority : 2;
  u8 high09 : 4;
  u8 unk0a[7];
  u8 shape11;
  u16 rotation;
  u8 unk14[5];
  u8 dirty;
  u8 flags;
};

/* VIEW(f027064): tag struct PartyKeys as struct PartyKeys__f027064; resource_slot.c has another definition; tried with the definition the rest of the object has, it does not compile in tla-ja: games/tla/src/field/resource_slot.c:4836: structure has no member named `keys23c'. */
struct PartyKeys__f027064 {
  u8 unk000[0x23c];
  u16 keys23c;
};

struct MapFlagsWork {
  u8 unk000[0x17];
  u8 groundEffects; ///< bits: 3..2 the wading kind | 1..0 footprints
};


extern u8 gDebugMode;
extern struct MapCell gMapCellBuffer[];
// The heading of each direction-pad combination, -1 for none.
extern const s16 gData_0802ec5c[16];
extern const s32 gData_0802ec7c[];

void Vector_AddPolarOffset(s32 radius, s32 angle,
                           struct FieldPosition *position);
s32 Func_0802d87c(struct Actor *object, struct FieldPosition *position);
s32 Field_CheckSpheresOverlap(s32 *a, s32 radiusA, s32 *b, s32 radiusB);
s32 ScriptObject_CheckOverlap(struct Actor *object, s32 *values);
/* VIEW(f027064): routine Unit_Get as struct LeaderUnit *Unit_Get(s32 owner); offset has void *Unit_Get(s32 owner); tried as the definition's void *Unit_Get(s32 owner), which the object already declares, it does not compile in tla-ja: games/tla/src/field/resource_slot.c:5066: request for member `hp' in something not a structure or union. */
extern struct LeaderUnit *Unit_Get__f027064(s32 owner) __asm__("Unit_Get");
void Actor_SelectAnimation(struct Actor *object, s32 argument);
void Actor_SetMoveTarget(struct Actor *actor, s32 x, s32 y, s32 z);
s32 FixedSqrt(s32 value);
struct Actor *Func_08023220(s32 kind, s32 x, s32 y, s32 z);
void Actor_StartScript(struct Actor *, const s32 *);
void Func_08026fc8(struct Actor *object, s32 kind);
s32 Field_CheckConfiguredKeys(void);

// Two 16.16 coordinates fall in the same 16-pixel cell, rounding to nearest.
#define SAME_CELL(a, b) ((((a) - (b) + 0x80000) >> 20) == 0)

s32 Func_08027064(struct Actor *object) {
  struct FieldPosition position;
  struct FieldPosition testPosition;
  struct FieldPosition contact;
  s16 angleOffsets[22];
  s32 collision;
  s32 pushed;
  struct PlayerSprite *sprite;
  s16 heading;
  s32 angleIndex;
  s32 movementAnimation;
  s32 headingCount;
  s32 footprintAngle;
  s32 onObject;
  struct EventWork *work;
  struct MapFlagsWork *map;
  u16 angle;
  u16 direction;
  struct Actor *other;
  s32 oldFlags;
  u32 attribute;

  sprite = object->sprite;
  movementAnimation = 2;
  onObject = 0;
  work = Ram_HeapSlots->eventWork;
  map = (struct MapFlagsWork *)Ram_HeapSlots->mapWork;
  collision = 0;
  pushed = 0;
  if (gInput.held & ((struct PartyKeys__f027064 *)&gMainRoutineWork)->keys23c) {
    object->speedLimit = 0x18000;
    object->acceleration = 0x4000;
    movementAnimation = 5;
  } else {
    object->speedLimit = 0x10000;
    object->acceleration = 0x4000;
  }

  angle = gData_0802ec5c[(gInput.held >> 4) & 15];
  if ((u16)angle == 0xffff) {
    collision |= 4;
    goto counted;
  }
  collision = 0;
  position.x = object->x;
  position.y = object->y;
  position.z = object->z;
  Vector_AddPolarOffset(0x80000, (u16)angle, &position);
  if (object->priorityFlags & 0x40) {
    // Standing on another object a cell below: the step is clear when the
    // object under the step (and, after Japanese, both its sides) is a still
    // one.
#if TLA_PATCH_1
    testPosition.x = position.x;
    testPosition.y = position.y;
    testPosition.z = position.z;
    Vector_AddPolarOffset(0x20000, (u16)angle + 0x4000, &testPosition);
    contact.x = position.x;
    contact.y = position.y;
    contact.z = position.z;
    Vector_AddPolarOffset(0x20000, (u16)angle - 0x4000, &contact);
#endif
    angleIndex = 0;
    other = (struct Actor *)Ram_HeapSlots->scriptObjects;
    for (; angleIndex < 64; angleIndex++, other++) {
      s32 dy;

      if (other->script == NULL)
        continue;
      if (!(other->priorityFlags & 0x80))
        continue;
      if (other == object)
        continue;
      dy = object->y - other->y;
      if ((dy - 0x100000 < 0 ? -(dy - 0x100000) : dy - 0x100000) > 0x7ffff)
        continue;
      if (SAME_CELL(object->x, other->x) && SAME_CELL(object->z, other->z))
        onObject = 1;
      if (SAME_CELL(position.x, other->x) && SAME_CELL(position.z, other->z) &&
#if TLA_PATCH_1
          SAME_CELL(testPosition.x, other->x) &&
          SAME_CELL(testPosition.z, other->z) &&
          SAME_CELL(contact.x, other->x) && SAME_CELL(contact.z, other->z) &&
#endif
          other->targetX == 0x80000000 && other->targetZ == other->targetX)
        goto clear;
    }
    if (onObject && !Func_0802d87c(object, &position)) {
      collision = 0;
      heading = angle;
      goto push_objects;
    }
  }
  if (gDebugMode != 0 && (gInput.held & 0x200)) {
    collision = 0;
    heading = angle;
    goto counted;
  }
  if (!Func_0802d87c(object, &position)) {
    testPosition.x = object->x;
    testPosition.y = object->y;
    testPosition.z = object->z;
    Vector_AddPolarOffset(0x80000, (u16)angle + 0x1000, &testPosition);
    if (!Func_0802d87c(object, &testPosition)) {
      testPosition.x = object->x;
      testPosition.y = object->y;
      testPosition.z = object->z;
      Vector_AddPolarOffset(0x80000, (u16)angle - 0x1000, &testPosition);
      if (!Func_0802d87c(object, &testPosition)) {
        testPosition.x = object->x;
        testPosition.y = object->y;
        testPosition.z = object->z;
        Vector_AddPolarOffset(0x80000, (u16)angle + 0x2000, &testPosition);
        if (!Func_0802d87c(object, &testPosition)) {
          testPosition.x = object->x;
          testPosition.y = object->y;
          testPosition.z = object->z;
          Vector_AddPolarOffset(0x80000, (u16)angle - 0x2000, &testPosition);
          if (!Func_0802d87c(object, &testPosition)) {
            heading = angle;
            goto push_objects;
          }
        }
      }
    }
  }
  direction = angle;
  headingCount = 0;
  angleOffsets[headingCount++] = direction + 0x1000;
  angleOffsets[headingCount++] = direction - 0x1000;
  angleOffsets[headingCount++] = direction + 0x2000;
  angleOffsets[headingCount++] = direction - 0x2000;
  angleOffsets[headingCount++] = direction + 0x3000;
  angleOffsets[headingCount++] = direction - 0x3000;
  for (angleIndex = 0; angleIndex < headingCount; angleIndex++) {
    heading = angleOffsets[angleIndex];
    position.x = object->x;
    position.y = object->y;
    position.z = object->z;
    Vector_AddPolarOffset(0x80000, (u16)heading, &position);
    if (Func_0802d87c(object, &position))
      continue;
    testPosition.x = object->x;
    testPosition.y = object->y;
    testPosition.z = object->z;
    Vector_AddPolarOffset(0x80000, (u16)heading + 0x1000, &testPosition);
    if (Func_0802d87c(object, &testPosition))
      continue;
    testPosition.x = object->x;
    testPosition.y = object->y;
    testPosition.z = object->z;
    Vector_AddPolarOffset(0x80000, (u16)heading - 0x1000, &testPosition);
    if (Func_0802d87c(object, &testPosition))
      continue;
    testPosition.x = object->x;
    testPosition.y = object->y;
    testPosition.z = object->z;
    Vector_AddPolarOffset(0x80000, (u16)heading + 0x2000, &testPosition);
    if (Func_0802d87c(object, &testPosition))
      continue;
    testPosition.x = object->x;
    testPosition.y = object->y;
    testPosition.z = object->z;
    Vector_AddPolarOffset(0x80000, (u16)heading - 0x2000, &testPosition);
    if (!Func_0802d87c(object, &testPosition))
      goto push_objects;
  }
  position.x = object->x;
  position.y = object->y;
  position.z = object->z;
  collision |= 1;
  goto push_objects;

clear:
  collision = 0;
  heading = angle;

push_objects:
  // Push the movable objects just ahead a quarter pixel further, when they
  // have room; any other solid object there blocks the step.
  contact.x = object->x;
  contact.y = object->y;
  contact.z = object->z;
  Vector_AddPolarOffset(0x40000, (u16)heading, &contact);
  other = (struct Actor *)Ram_HeapSlots->scriptObjects;
  for (angleIndex = 63; angleIndex >= 0; angleIndex--, other++) {
    s32 radius;

    radius = object->radius - 2;
    if (other->script == NULL)
      continue;
    if (!(other->collisionFlags & 1))
      continue;
    if (other == object)
      continue;
    if (Field_CheckSpheresOverlap(&other->x, radius, &contact.x,
                                   other->radius - 2) < 0)
      continue;
    if ((other->collisionFlags & 2) && other->movementState == 0) {
      direction = ArcTan2(other->z - object->z, other->x - object->x);
      testPosition.x = other->x;
      testPosition.y = other->y;
      testPosition.z = other->z;
      heading = direction;
      Vector_AddPolarOffset(0x4000, direction, &testPosition);
      if (!ScriptObject_CheckOverlap(other, &testPosition.x)) {
        testPosition.x = other->x;
        testPosition.y = other->y;
        testPosition.z = other->z;
        Vector_AddPolarOffset(0xa0000, direction, &testPosition);
        if (!Func_0802d87c(other, &testPosition)) {
          testPosition.x = other->x;
          testPosition.y = other->y;
          testPosition.z = other->z;
          Vector_AddPolarOffset(0xa0000, direction + 0x1000, &testPosition);
          // The same probe is tested twice.
          if (!Func_0802d87c(other, &testPosition) &&
              !Func_0802d87c(other, &testPosition)) {
            testPosition.x = other->x;
            testPosition.y = other->y;
            testPosition.z = other->z;
            Vector_AddPolarOffset(0xa0000, direction - 0x1000, &testPosition);
            if (!Func_0802d87c(other, &testPosition)) {
              Vector_AddPolarOffset(0x4000, direction,
                                    (struct FieldPosition *)&other->x);
              other->targetX = 0x80000000;
              other->targetY = 0x80000000;
              other->targetZ = 0x80000000;
              pushed |= 1;
              continue;
            }
          }
        }
      }
    }
    collision |= 2;
  }
  if (collision == 0 && pushed != 0) {
    object->speedLimit = 0x4000;
    object->acceleration = 0x2000;
  }

counted:
  if (work != NULL) {
    if (collision & 3)
      work->blockedSteps++;
    else
      work->blockedSteps = 0;
    work->heldKeys = gInput.held;
  }
  if (pushed != 0) {
    Actor_SelectAnimation(object, 8);
  } else if (collision != 0) {
    s32 action;

    action = 9;
    if (Unit_Get__f027064(gMainRoutineWork.currentOwner)->hp == 0)
      action = 22;
    Actor_SelectAnimation(object, action);
  } else {
    Actor_SelectAnimation(object, movementAnimation);
  }

  if (collision != 0) {
    object->targetX = 0x80000000;
    object->targetY = 0x80000000;
    object->targetZ = 0x80000000;
    object->velocityX = 0;
    object->velocityZ = 0;
    if (collision & 3) {
      s32 difference;

      difference = (s16)((u16)angle - object->facing);
      if (difference > 0x1000)
        difference = 0x1000;
      if (difference < -0x1000)
        difference = -0x1000;
      object->facing += difference;
    }
    object->timer = 0;
    object->unk66 = 2;
  } else {
    s32 speed;

    Actor_SetMoveTarget(object, position.x, position.y, position.z);
    speed = FixedSqrt(Iwram_MulQ16(object->velocityX, object->velocityX) +
                      Iwram_MulQ16(object->velocityZ, object->velocityZ));
    object->velocityX = collision;
    object->velocityZ = collision;
    Vector_AddPolarOffset(speed, (u16)heading,
                          (struct FieldPosition *)&object->velocityX);
    if (object->timer != 0)
      object->timer--;
  }

  // Ground that wades: hide the lower half of the sprite while on it, and
  // leave the wading effect as the leader moves.
  if (map->groundEffects & 12) {
    struct MapCell *cells;

    if (object->terrainId <= 2)
      cells = ((struct MapWork *)map)->layers[object->terrainId].cells;
    else
      cells = gMapCellBuffer;
    cells += object->x / 0x100000 + (object->z / 0x100000) * 128;
    /* FAKEMATCH(temporary): the cell attribute read into a local before the
     * flags; as a read at the test, 4 of 1094 instructions differ in tla-en. */
    attribute = cells->attributeB;
    oldFlags = sprite->flags;
    if (attribute & 16) {
      if (!(oldFlags & 8)) {
        sprite->shape11 = (sprite->shape11 & 3) | 16;
        sprite->flags = ((u8)oldFlags | 4) & ~1;
      }
      if (object->timer == 0 && collision == 0 && !onObject) {
        Func_08026fc8(object, (u32)(map->groundEffects << 28) >> 30);
        if (movementAnimation == 5)
          object->timer = 8;
        else
          object->timer = 12;
        object->unk66 ^= 1;
      }
    } else if (!(oldFlags & 8)) {
      sprite->flags = ((u8)oldFlags | 1) & ~4;
    }
    if (oldFlags != sprite->flags)
      sprite->dirty = 1;
  }

  // Footprints on maps that show them, alternating left and right.
  footprintAngle = (u16)angle;
  if ((map->groundEffects & 3) && object->timer == 0 && collision == 0) {
    struct Actor *effect;

    effect = Func_08023220(0x10e, object->x, object->y, object->z);
    if (effect != NULL) {
      struct PlayerSprite *effectSprite;

      effect->terrainHeight = object->terrainHeight;
      effectSprite = effect->sprite;
      Actor_StartScript(effect, gData_0802ec7c);
      effect->priorityFlags = 2;
      effect->motionFlags = collision;
      if (effectSprite != NULL) {
        Animation_ApplyChildArgument(effectSprite, 1);
        effectSprite->flags = collision;
        effectSprite->rotation = footprintAngle + 0x4000;
        effectSprite->priority = 3;
      }
      if (*(s16 *)&object->unk66 == 2) {
        Animation_ApplyChildArgument(effectSprite, 2);
        object->unk66 = collision;
      }
      if (*(s16 *)&object->unk66 != 0)
        effect->facing = 0x8000;
      if (movementAnimation == 5)
        object->timer = 12;
      else
        object->timer = 18;
      object->unk66 ^= 1;
    }
  }
  Field_CheckConfiguredKeys();
  object->step++;
  return 1;
}
#undef SAME_CELL

// ---- games/tla/src/field/script/f0279b0.c ----

struct WorldCamera {
  u8 unk000[0x11e];
  u16 yaw;
};

/* VIEW(f0279b0): tag struct PlayerSprite as struct PlayerSprite__f0279b0; resource_slot.c has another definition; tried with the definition the rest of the object has, it does not compile in tla-ja: games/tla/src/field/resource_slot.c:5352: structure has no member named `entry'. */
struct PlayerSprite__f0279b0 {
  u8 unk00[5];
  u8 low05 : 2;
  u8 objectMode : 2;
  u8 high05 : 4;
  u8 unk06[3];
  u8 low09 : 2;
  u8 priority : 2;
  u8 high09 : 4;
  u8 unk0a[0x10];
  u8 flags;
  u8 unk1b[0x11];
  u8 *entry;
};


// The heading of each direction-pad combination, -1 for none.
extern const s16 gData_0802ec5c[16];
// The camera yaw each direction-pad combination turns toward.
extern const s32 gData_0802eca0[16];
extern const s32 gData_0802ec94[];

void Vector_AddPolarOffset(s32 radius, s32 angle,
                           struct FieldPosition *position);
s32 Func_0802db64(struct Actor *object, struct FieldPosition *position);
s32 GameFlag_Test(s32 flag);
void Actor_SelectAnimation(struct Actor *object, s32 argument);
void Actor_SetMoveTarget(struct Actor *actor, s32 x, s32 y, s32 z);
s32 FixedSqrt(s32 value);
struct Actor *Func_08023220(s32 kind, s32 x, s32 y, s32 z);
void Actor_StartScript(struct Actor *, const s32 *);
s32 Field_CheckConfiguredKeys(void);

s32 Func_080279b0(struct Actor *object) {
  struct FieldPosition position;
  struct FieldPosition leftPosition;
  struct FieldPosition rightPosition;
  s16 angleOffsets[22];
  s32 collision;
  s32 angle;
  u16 direction;
  s32 angleIndex;
  s32 movementAnimation;
  s32 count;
  s32 difference;
  s32 terrainCollision;
  s32 speed;
  s32 targetCameraYaw;
  struct WorldCamera *camera;
  s32 yawStep;
  struct PlayerSprite__f0279b0 *playerSprite;
  struct Actor *ripple;
  struct PlayerSprite__f0279b0 *rippleSprite;
  struct EventWork *work;

  movementAnimation = 2;
  collision = 0;
  count = 0;
  if (gInput.held & ((struct PartyKeys *)&gMainRoutineWork)->dashKeys) {
    object->speedLimit = 0x10000;
    object->acceleration = 0x8000;
    movementAnimation = 5;
  } else {
    object->speedLimit = 0x8000;
    object->acceleration = 0x4000;
  }

  angle = gData_0802ec5c[(gInput.held >> 4) & 15];
  direction = angle;
  if (direction == 0xffff) {
    collision |= 4;
  } else {
    collision = 0;
    position.x = object->x;
    position.y = object->y;
    position.z = object->z;
    Vector_AddPolarOffset(0x80000, direction, &position);
    if (gDebugMode == 0 ||
        ((gInput.held & 0x200) == 0 && GameFlag_Test(0x163) == 0)) {
      leftPosition.x = object->x;
      leftPosition.y = object->y;
      leftPosition.z = object->z;
      Vector_AddPolarOffset(0x80000, direction + 0x1555, &leftPosition);
      rightPosition.x = object->x;
      rightPosition.y = object->y;
      rightPosition.z = object->z;
      Vector_AddPolarOffset(0x80000, direction - 0x1555, &rightPosition);
      if ((Func_0802db64(object, &position) |
           Func_0802db64(object, &leftPosition) |
           Func_0802db64(object, &rightPosition)) != 0) {
        angleOffsets[count++] = direction + 0x1000;
        angleOffsets[count++] = direction - 0x1000;
        angleOffsets[count++] = direction + 0x2000;
        angleOffsets[count++] = direction - 0x2000;
        angleOffsets[count++] = direction + 0x3000;
        angleOffsets[count++] = direction - 0x3000;
        for (angleIndex = 0; angleIndex < count; angleIndex++) {
          direction = angleOffsets[angleIndex];
          position.x = object->x;
          position.y = object->y;
          position.z = object->z;
          Vector_AddPolarOffset(0x80000, direction, &position);
          leftPosition.x = object->x;
          leftPosition.y = object->y;
          leftPosition.z = object->z;
          Vector_AddPolarOffset(0x80000, direction + 0x1555, &leftPosition);
          rightPosition.x = object->x;
          rightPosition.y = object->y;
          rightPosition.z = object->z;
          Vector_AddPolarOffset(0x80000, direction - 0x1555, &rightPosition);
          if ((Func_0802db64(object, &position) |
               Func_0802db64(object, &leftPosition) |
               Func_0802db64(object, &rightPosition)) != 0)
            continue;
          angle = (s16)direction;
          goto update_object;
        }
        collision |= 1;
      }
    }
  }

update_object:
  work = Ram_HeapSlots->eventWork;
  if (work != NULL) {
    if ((collision & 3) != 0)
      work->blockedSteps++;
    else
      work->blockedSteps = 0;
  }
  if (collision != 0)
    Actor_SelectAnimation(object, 9);
  else
    Actor_SelectAnimation(object, movementAnimation);

  if (collision != 0) {
    object->targetX = 0x80000000;
    object->targetY = 0x80000000;
    object->targetZ = 0x80000000;
    object->velocityX = 0;
    object->velocityZ = 0;
    if ((collision & 3) != 0) {
      direction = angle;
      difference = (s16)(direction - object->facing);
      if (difference > 0x1000)
        difference = 0x1000;
      if (difference < -0x1000)
        difference = -0x1000;
      object->facing += difference;
    }
    object->timer = 0;
    object->unk66 = 2;
  } else {
    Actor_SetMoveTarget(object, position.x, position.y, position.z);
    speed = FixedSqrt(Iwram_MulQ16(object->velocityX, object->velocityX) +
                      Iwram_MulQ16(object->velocityZ, object->velocityZ));
    object->velocityX = collision;
    object->velocityZ = collision;
    Vector_AddPolarOffset(speed, (u16)angle,
                          (struct FieldPosition *)&object->velocityX);
    if (object->timer != 0)
      object->timer--;
  }

  camera = (struct WorldCamera *)Ram_HeapSlots->mapWork;
  targetCameraYaw = gData_0802eca0[(gInput.held >> 4) & 15];
  yawStep = (s16)(targetCameraYaw - camera->yaw) / 8;
  if (yawStep > 0x200)
    yawStep = 0x200;
  if (yawStep < -0x200)
    yawStep = -0x200;
  if (yawStep > -16 && yawStep < 16)
    yawStep = targetCameraYaw - camera->yaw;
  camera->yaw += yawStep;

  if (object->animationKind == 1) {
    playerSprite = object->sprite;
    terrainCollision = GetWorldMapCollision((struct WorldPosition *)&object->x);
    if (terrainCollision == 9) {
      playerSprite->entry[6] = 1;
      playerSprite->flags = 0;
    } else {
      playerSprite->entry[6] = 9;
      playerSprite->flags = 1;
    }
    if (terrainCollision == 6 && object->timer == 0 && collision == 0) {
      ripple = Func_08023220(0x10d, object->x, object->y, object->z);
      if (ripple != 0) {
        rippleSprite = ripple->sprite;
        Actor_StartScript(ripple, gData_0802ec94);
        ripple->motionFlags = collision;
        ripple->terrainId = 1;
        if (rippleSprite != 0) {
          Animation_ApplyChildArgument(rippleSprite, 1);
          rippleSprite->flags = collision;
          rippleSprite->objectMode = 1;
          rippleSprite->priority = 2;
        }
        object->timer = 10;
      }
    }
  }
  Field_CheckConfiguredKeys();
  if (gInput.pressed & gMainRoutineWork.keys230)
    ((struct EventWork *)Ram_HeapSlots->eventWork)->unk16a = 251;
  object->step++;
  return 1;
}


/// SC_CMD_25: checks the configured keys (Field_CheckConfiguredKeys) and steps
/// the script on. Returns 1.
s32 Field_CheckConfiguredKeysAndCount(struct ActorScriptView *interpreter) {
  Field_CheckConfiguredKeys();
  interpreter->scriptCursor++;
  return 1;
}

s32 Func_0802d87c(struct Actor *object, struct FieldPosition *position);
s32 Field_CheckSpheresOverlap(s32 *a, s32 radiusA, s32 *b, s32 radiusB);
s32 ScriptObject_CheckOverlap(struct Actor *object, s32 *values);

// Two 16.16 coordinates fall in the same 16-pixel cell, rounding to nearest.
#define SAME_CELL(a, b) ((((a) - (b) + 0x80000) >> 20) == 0)

s32 Func_08027e20(struct Actor *interpreter) {
  struct Actor *object = interpreter;

  struct FieldPosition position;
  struct FieldPosition testPosition;
  struct FieldPosition contact;
  s16 angleOffsets[22];
  s32 collision;
  s32 pushed;
  s16 heading;
  u16 angle;
  u16 direction;
  s32 angleIndex;
  s32 headingCount;
  s32 onObject;
  s32 difference;
  struct Actor *other;
  struct EventWork *work;

  collision = 0;
  pushed = 0;
  // Debug: holding L spins through four empty loops, a short stall each
  // frame.
  if (gDebugMode != 0) {
    s32 delayKeyMask;

    delayKeyMask = 0x200;
    if (gInput.held & delayKeyMask) {
      s32 delayIterations;

      delayIterations = delayKeyMask;
    wait_a:
      --delayIterations;
      if (delayIterations != 0)
        goto wait_a;
      delayIterations = 95;
    wait_b:
      delayIterations--;
      if (delayIterations >= 0)
        goto wait_b;
      delayIterations = 63;
    wait_c:
      delayIterations--;
      if (delayIterations >= 0)
        goto wait_c;
      delayIterations = 63;
    wait_d:
      delayIterations--;
      if (delayIterations >= 0)
        goto wait_d;
    }
  }
  object->speedLimit = 0x10000;
  object->acceleration = 0x4000;
  if (GameFlag_Test(0x17f) != 0 && (gInput.held & 2)) {
    object->speedLimit = 0x40000;
    object->acceleration = 0x10000;
  }

  angle = gData_0802ec5c[(gInput.held >> 4) & 15];
  if ((u16)angle == 0xffff) {
    collision |= 4;
    goto counted;
  }
  collision = 0;
  position.x = object->x;
  position.y = object->y;
  position.z = object->z;
  Vector_AddPolarOffset(0x130000, (u16)angle, &position);
  if (object->priorityFlags & 0x40) {
    onObject = 0;
    other = (struct Actor *)Ram_HeapSlots->scriptObjects;
    for (angleIndex = 0; angleIndex < 64; angleIndex++, other++) {
      s32 dy;

      if (other->script == NULL)
        continue;
      if (!(other->priorityFlags & 0x80))
        continue;
      if (other == object)
        continue;
      dy = object->y - other->y;
      if ((dy - 0x100000 < 0 ? -(dy - 0x100000) : dy - 0x100000) > 0x7ffff)
        continue;
      if (SAME_CELL(object->x, other->x) && SAME_CELL(object->z, other->z))
        onObject = 1;
      if (SAME_CELL(position.x, other->x) && SAME_CELL(position.z, other->z)) {
        collision = 0;
        heading = angle;
        goto counted;
      }
    }
    if (onObject && !Func_0802d87c(object, &position)) {
      collision = 0;
      heading = angle;
      goto counted;
    }
  }
  if (gDebugMode != 0 && (gInput.held & 0x200)) {
    collision = 0;
    heading = angle;
    goto counted;
  }
  if (!Func_0802d87c(object, &position)) {
    testPosition.x = object->x;
    testPosition.y = object->y;
    testPosition.z = object->z;
    Vector_AddPolarOffset(0x130000, (u16)angle + 0x1000, &testPosition);
    if (!Func_0802d87c(object, &testPosition)) {
      testPosition.x = object->x;
      testPosition.y = object->y;
      testPosition.z = object->z;
      Vector_AddPolarOffset(0x130000, (u16)angle - 0x1000, &testPosition);
      if (!Func_0802d87c(object, &testPosition)) {
        testPosition.x = object->x;
        testPosition.y = object->y;
        testPosition.z = object->z;
        Vector_AddPolarOffset(0x130000, (u16)angle + 0x2000, &testPosition);
        if (!Func_0802d87c(object, &testPosition)) {
          testPosition.x = object->x;
          testPosition.y = object->y;
          testPosition.z = object->z;
          Vector_AddPolarOffset(0x130000, (u16)angle - 0x2000, &testPosition);
          if (!Func_0802d87c(object, &testPosition)) {
            heading = angle;
            goto push_objects;
          }
        }
      }
    }
  }
  direction = angle;
  headingCount = 0;
  angleOffsets[headingCount++] = direction + 0x1000;
  angleOffsets[headingCount++] = direction - 0x1000;
  angleOffsets[headingCount++] = direction + 0x2000;
  angleOffsets[headingCount++] = direction - 0x2000;
  angleOffsets[headingCount++] = direction + 0x3000;
  angleOffsets[headingCount++] = direction - 0x3000;
  // An empty once-only loop keeps the last store before the counter and
  // places the first loop's exit block after this loop; without it, 243 of
  // 827 instructions differ in tla-en.
  do {
  } while (0);
  for (angleIndex = 0; angleIndex < headingCount; angleIndex++) {
    heading = angleOffsets[angleIndex];
    position.x = object->x;
    position.y = object->y;
    position.z = object->z;
    Vector_AddPolarOffset(0x130000, (u16)heading, &position);
    if (Func_0802d87c(object, &position))
      continue;
    testPosition.x = object->x;
    testPosition.y = object->y;
    testPosition.z = object->z;
    Vector_AddPolarOffset(0x130000, (u16)heading + 0x1000, &testPosition);
    if (Func_0802d87c(object, &testPosition))
      continue;
    testPosition.x = object->x;
    testPosition.y = object->y;
    testPosition.z = object->z;
    Vector_AddPolarOffset(0x130000, (u16)heading - 0x1000, &testPosition);
    if (Func_0802d87c(object, &testPosition))
      continue;
    testPosition.x = object->x;
    testPosition.y = object->y;
    testPosition.z = object->z;
    Vector_AddPolarOffset(0x130000, (u16)heading + 0x2000, &testPosition);
    if (Func_0802d87c(object, &testPosition))
      continue;
    testPosition.x = object->x;
    testPosition.y = object->y;
    testPosition.z = object->z;
    Vector_AddPolarOffset(0x130000, (u16)heading - 0x2000, &testPosition);
    if (!Func_0802d87c(object, &testPosition))
      goto push_objects;
  }
  position.x = object->x;
  position.y = object->y;
  position.z = object->z;
  collision |= 1;

push_objects:
  contact.x = object->x;
  contact.y = object->y;
  contact.z = object->z;
  Vector_AddPolarOffset(0xf0000, (u16)heading, &contact);
  other = (struct Actor *)Ram_HeapSlots->scriptObjects;
  for (angleIndex = 63; angleIndex >= 0; angleIndex--, other++) {
    s32 radius;

    radius = object->radius - 2;
    if (other->script == NULL)
      continue;
    if (!(other->collisionFlags & 1))
      continue;
    if (other == object)
      continue;
    if (Field_CheckSpheresOverlap(&other->x, radius, &contact.x,
                                  other->radius - 2) < 0)
      continue;
    if ((other->collisionFlags & 2) && other->movementState == 0) {
      direction = ArcTan2(other->z - object->z, other->x - object->x);
      testPosition.x = other->x;
      testPosition.y = other->y;
      testPosition.z = other->z;
      heading = direction;
      Vector_AddPolarOffset(0x4000, direction, &testPosition);
      if (!ScriptObject_CheckOverlap(other, &testPosition.x)) {
        testPosition.x = other->x;
        testPosition.y = other->y;
        testPosition.z = other->z;
        Vector_AddPolarOffset(0x150000, direction, &testPosition);
        if (!Func_0802d87c(other, &testPosition)) {
          testPosition.x = other->x;
          testPosition.y = other->y;
          testPosition.z = other->z;
          Vector_AddPolarOffset(0x150000, direction + 0x1000, &testPosition);
          // The same probe is tested twice.
          if (!Func_0802d87c(other, &testPosition) &&
              !Func_0802d87c(other, &testPosition)) {
            testPosition.x = other->x;
            testPosition.y = other->y;
            testPosition.z = other->z;
            Vector_AddPolarOffset(0x150000, direction - 0x1000, &testPosition);
            if (!Func_0802d87c(other, &testPosition)) {
              Vector_AddPolarOffset(0x4000, direction,
                                    (struct FieldPosition *)&other->x);
              other->targetX = 0x80000000;
              other->targetY = 0x80000000;
              other->targetZ = 0x80000000;
              pushed |= 1;
              continue;
            }
          }
        }
      }
    }
    collision |= 2;
  }
  if (collision == 0 && pushed != 0) {
    object->speedLimit = 0x4000;
    object->acceleration = 0x2000;
  }

counted:
  work = Ram_HeapSlots->eventWork;
  if (work != NULL) {
    if (collision & 3)
      work->blockedSteps++;
    else
      work->blockedSteps = 0;
    work->heldKeys = gInput.held;
  }
  if (pushed != 0 || collision != 0)
    Actor_SelectAnimation(object, 1);
  else
    Actor_SelectAnimation(object, 2);

  if (gInput.pressed & gMainRoutineWork.keys230) {
    ((struct EventWork *)Ram_HeapSlots->eventWork)->unk16a = 252;
    collision |= 4;
  }

  if (collision != 0) {
    object->targetX = 0x80000000;
    object->targetY = 0x80000000;
    object->targetZ = 0x80000000;
    object->velocityX = 0;
    object->velocityZ = 0;
    if (collision & 3) {
      difference = (s16)((u16)angle - object->facing);
      if (difference > 0x1000)
        difference = 0x400;
      if (difference < -0x1000)
        difference = -0x400;
      object->facing += difference;
    }
    object->timer = 0;
    object->unk66 = 2;
  } else {
    s32 speed;

    Actor_SetMoveTarget(object, position.x, position.y, position.z);
    speed = FixedSqrt(Iwram_MulQ16(object->velocityX, object->velocityX) +
                      Iwram_MulQ16(object->velocityZ, object->velocityZ));
    object->velocityX = collision;
    object->velocityZ = collision;
    Vector_AddPolarOffset(speed, (u16)heading,
                          (struct FieldPosition *)&object->velocityX);
    if (object->timer != 0)
      object->timer--;
    object->actionFlags &= ~1;
    difference = (s16)((u16)angle - object->facing);
    if (difference > 0x1000)
      difference = 0x400;
    if (difference < -0x1000)
      difference = -0x400;
    object->facing += difference;
  }
  if (object->animationKind == 1)
    GetWorldMapCollision((struct WorldPosition *)&object->x);
  Field_CheckConfiguredKeys();
  object->step++;
  return 1;
}
#undef SAME_CELL

// ---- games/tla/src/field/script/s28534.c ----


/* VIEW(s28534): tag struct PlayerSprite as struct PlayerSprite__s28534; resource_slot.c has another definition; tried with the definition the rest of the object has, it does not compile in tla-ja: games/tla/src/field/resource_slot.c:5966: structure has no member named `objectMode'. */
struct PlayerSprite__s28534 {
  u8 unk00[5];
  u8 low05 : 2;
  u8 objectMode : 2;
  u8 high05 : 4;
  u8 unk06[3];
  u8 low09 : 2;
  u8 priority : 2;
  u8 high09 : 4;
  u8 unk0a[0x10];
  u8 flags;
};

/* VIEW(s28534): tag struct PartyKeys as struct PartyKeys__s28534; resource_slot.c has another definition; tried with the definition the rest of the object has, it does not compile in tla-ja: games/tla/src/field/resource_slot.c:5905: structure has no member named `keys23c'. */
struct PartyKeys__s28534 {
  u8 unk000[0x23c];
  u16 keys23c;
};

struct WorldEventWork {
  u8 unk000[0xd39];
  s8 unkd39;
  u8 terrainKind;
};

// The heading of each direction-pad combination, -1 for none.
extern const s16 gData_0802ec5c[16];
// The camera yaw each direction-pad combination turns toward.
extern const s32 gData_0802eca0[16];
extern const s32 gData_0802ec94[];

void Vector_AddPolarOffset(s32 radius, s32 angle,
                           struct FieldPosition *position);
s32 Func_0802db88(struct Actor *object, struct FieldPosition *position);
void GameFlag_ClearBit(s32 flag);
void Actor_SelectAnimation(struct Actor *object,
                                            s32 argument);
void Actor_SetMoveTarget(struct Actor *actor, s32 x, s32 y, s32 z);
s32 FixedSqrt(s32 value);
struct Actor *Func_08023220(s32 kind, s32 x, s32 y, s32 z);
void Actor_StartScript(struct Actor *, const s32 *);
s32 Field_CheckConfiguredKeys(void);

/// Script command 0x20, a sibling of Func_080279b0 in The Broken Seal's later
/// Actor_MoveOnWorldMap layout (one probe ahead, then four probes 22.5 and 45
/// degrees off it; six retry headings). Records the kind of ground under the
/// object in the event work (byte 0xd3a), moves at a run, and lets the shortcut
/// keys post requests 252 and, while byte 0xd39 is set, clear flag 0x147 and
/// post 253; turns at most 0x400 a step while moving.
s32 Func_08028534(struct Actor *object) {
  struct FieldPosition position;
  struct FieldPosition testPosition;
  s16 angleOffsets[28];
  s32 collision;
  s32 angle;
  u16 direction;
  s32 angleIndex;
  s32 count;
  s32 difference;
  s32 terrain;
  s32 speed;
  s32 targetCameraYaw;
  struct WorldCamera *camera;
  s32 yawStep;
  struct Actor *ripple;
  struct PlayerSprite__s28534 *rippleSprite;
  struct EventWork *work;
  struct WorldEventWork *worldWork;
  u32 kind;

  count = 0;
  terrain = GetWorldMapCollision((struct WorldPosition *)&object->x);
  worldWork = Ram_HeapSlots->eventWork;
  if (terrain >= 1 && terrain <= 3)
    worldWork->terrainKind = 0;
  if (terrain >= 5 && terrain <= 6)
    worldWork->terrainKind = 1;
  if (terrain == 4)
    worldWork->terrainKind = 2;
  if (terrain >= 7 && terrain <= 12)
    worldWork->terrainKind = 3;

  collision = 0;
  object->speedLimit = 0x14000;
  object->acceleration = 0x8000;
  angle = gData_0802ec5c[(gInput.held >> 4) & 15];
  direction = angle;
  if (direction == 0xffff) {
    collision |= 4;
  } else {
    position.x = object->x;
    position.y = object->y;
    position.z = object->z;
    Vector_AddPolarOffset(0x80000, direction, &position);
    if (gDebugMode == 0 || (gInput.held & 0x200) == 0) {
      if (Func_0802db88(object, &position) == 0) {
        testPosition.x = object->x;
        testPosition.y = object->y;
        testPosition.z = object->z;
        Vector_AddPolarOffset(0x80000, direction + 0x1000, &testPosition);
        if (Func_0802db88(object, &testPosition) == 0) {
          testPosition.x = object->x;
          testPosition.y = object->y;
          testPosition.z = object->z;
          Vector_AddPolarOffset(0x80000, direction - 0x1000, &testPosition);
          if (Func_0802db88(object, &testPosition) == 0) {
            testPosition.x = object->x;
            testPosition.y = object->y;
            testPosition.z = object->z;
            Vector_AddPolarOffset(0x80000, direction + 0x2000, &testPosition);
            if (Func_0802db88(object, &testPosition) == 0) {
              testPosition.x = object->x;
              testPosition.y = object->y;
              testPosition.z = object->z;
              Vector_AddPolarOffset(0x80000, direction - 0x2000,
                                    &testPosition);
              if (Func_0802db88(object, &testPosition) == 0)
                goto update_object;
            }
          }
        }
      }
      angleOffsets[count++] = direction + 0x1000;
      angleOffsets[count++] = direction - 0x1000;
      angleOffsets[count++] = direction + 0x2000;
      angleOffsets[count++] = direction - 0x2000;
      angleOffsets[count++] = direction + 0x3000;
      angleOffsets[count++] = direction - 0x3000;
      for (angleIndex = 0; angleIndex < count; angleIndex++) {
        direction = angleOffsets[angleIndex];
        position.x = object->x;
        position.y = object->y;
        position.z = object->z;
        Vector_AddPolarOffset(0x80000, direction, &position);
        if (Func_0802db88(object, &position) != 0)
          continue;
        testPosition.x = object->x;
        testPosition.y = object->y;
        testPosition.z = object->z;
        Vector_AddPolarOffset(0x80000, direction + 0x1000, &testPosition);
        if (Func_0802db88(object, &testPosition) != 0)
          continue;
        testPosition.x = object->x;
        testPosition.y = object->y;
        testPosition.z = object->z;
        Vector_AddPolarOffset(0x80000, direction - 0x1000, &testPosition);
        if (Func_0802db88(object, &testPosition) != 0)
          continue;
        testPosition.x = object->x;
        testPosition.y = object->y;
        testPosition.z = object->z;
        Vector_AddPolarOffset(0x80000, direction + 0x2000, &testPosition);
        if (Func_0802db88(object, &testPosition) != 0)
          continue;
        testPosition.x = object->x;
        testPosition.y = object->y;
        testPosition.z = object->z;
        Vector_AddPolarOffset(0x80000, direction - 0x2000, &testPosition);
        if (Func_0802db88(object, &testPosition) != 0)
          continue;
        angle = (s16)direction;
        goto update_object;
      }
      collision |= 1;
    }
  }

update_object:
  work = Ram_HeapSlots->eventWork;
  if (work != NULL) {
    if ((collision & 3) != 0)
      work->blockedSteps++;
    else
      work->blockedSteps = 0;
    work->heldKeys = gInput.held;
  }
  if (collision != 0)
    Actor_SelectAnimation(object, 1);
  else
    Actor_SelectAnimation(object, 2);

  if (gInput.pressed & gMainRoutineWork.keys230) {
    ((struct EventWork *)Ram_HeapSlots->eventWork)->unk16a = 252;
    collision |= 4;
  }
  if (gInput.pressed & ((struct PartyKeys__s28534 *)&gMainRoutineWork)->keys23c) {
    struct WorldEventWork *keyWork = Ram_HeapSlots->eventWork;
    if (keyWork->unkd39 != 0) {
      GameFlag_ClearBit(0x147);
      ((struct EventWork *)keyWork)->unk16a = 253;
    }
  }

  if (collision != 0) {
    object->targetX = 0x80000000;
    object->targetY = 0x80000000;
    object->targetZ = 0x80000000;
    object->velocityX = 0;
    object->velocityZ = 0;
    if ((collision & 3) != 0) {
      direction = angle;
      difference = (s16)(direction - object->facing);
      if (difference > 0x1000)
        difference = 0x1000;
      if (difference < -0x1000)
        difference = -0x1000;
      object->facing += difference;
    }
    object->timer = 0;
    object->unk66 = 2;
  } else {
    Actor_SetMoveTarget(object, position.x, position.y, position.z);
    speed = FixedSqrt(Iwram_MulQ16(object->velocityX, object->velocityX) +
                      Iwram_MulQ16(object->velocityZ, object->velocityZ));
    object->velocityX = collision;
    object->velocityZ = collision;
    direction = angle;
    Vector_AddPolarOffset(speed, direction,
                          (struct FieldPosition *)&object->velocityX);
    if (object->timer != 0)
      object->timer--;
    object->actionFlags &= ~1;
    difference = (s16)(direction - object->facing);
    if (difference > 0x1000)
      difference = 0x400;
    if (difference < -0x1000)
      difference = -0x400;
    object->facing += difference;
  }

  camera = (struct WorldCamera *)Ram_HeapSlots->mapWork;
  targetCameraYaw = gData_0802eca0[(gInput.held >> 4) & 15];
  yawStep = (s16)(targetCameraYaw - camera->yaw) / 8;
  if (yawStep > 0x200)
    yawStep = 0x200;
  if (yawStep < -0x200)
    yawStep = -0x200;
  if (yawStep > -16 && yawStep < 16)
    yawStep = targetCameraYaw - camera->yaw;
  camera->yaw += yawStep;

  kind = object->animationKind;
  if (kind == 1) {
    GetWorldMapCollision((struct WorldPosition *)&object->x);
    if (object->timer == 0 && collision == 0) {
      ripple = Func_08023220(0x10d, object->x, object->y, object->z);
      if (ripple != 0) {
        rippleSprite = ripple->sprite;
        Actor_StartScript(ripple, gData_0802ec94);
        ripple->motionFlags = 0;
        ripple->terrainId = kind;
        if (rippleSprite != 0) {
          Animation_ApplyChildArgument(rippleSprite, 2);
          rippleSprite->flags = 0;
          rippleSprite->objectMode = 0;
          rippleSprite->priority = 2;
        }
        object->timer = 10;
      }
    }
  }
  Field_CheckConfiguredKeys();
  object->step++;
  return 1;
}

// ---- games/tla/src/field/script/f028aac.c ----


/* VIEW(f028aac): tag struct PlayerSprite as struct PlayerSprite__f028aac; resource_slot.c has another definition; tried with the definition the rest of the object has, it does not compile in tla-ja: games/tla/src/field/resource_slot.c:6227: structure has no member named `objectMode'. */
struct PlayerSprite__f028aac {
  u8 unk00[5];
  u8 low05 : 2;
  u8 objectMode : 2;
  u8 high05 : 4;
  u8 unk06[3];
  u8 low09 : 2;
  u8 priority : 2;
  u8 high09 : 4;
  u8 unk0a[0x10];
  u8 flags;
};

/* VIEW(f028aac): tag struct WorldEventWork as struct WorldEventWork__f028aac; resource_slot.c has another definition; tried with the definition the rest of the object has, it does not compile in tla-ja: games/tla/src/field/resource_slot.c:6247: structure has no member named `climbing'. */
struct WorldEventWork__f028aac {
  u8 unk000[0xd3a];
  u8 terrainKind;
  s8 climbing;
};

// The heading of each direction-pad combination, -1 for none.
extern const s16 gData_0802ec5c[16];
// The camera yaw each direction-pad combination turns toward.
extern const s32 gData_0802eca0[16];
extern const s32 gData_0802ec94[];

void Vector_AddPolarOffset(s32 radius, s32 angle,
                           struct FieldPosition *position);
s32 Func_0802dbd0(struct Actor *object, struct FieldPosition *position);
s32 Field_CheckSpheresOverlap(s32 *a, s32 radius_a, s32 *b, s32 radius_b);
void Actor_SelectAnimation(struct Actor *object, s32 argument);
void Actor_SetMoveTarget(struct Actor *actor, s32 x, s32 y, s32 z);
s32 FixedSqrt(s32 value);
struct Actor *Func_08023220(s32 kind, s32 x, s32 y, s32 z);
void Actor_StartScript(struct Actor *, const s32 *);
s32 Func_080ad318(void);

s32 Func_08028aac(struct Actor *object) {
  struct FieldPosition position;
  struct FieldPosition testPosition;
  s16 angleOffsets[28];
  s32 collision;
  s32 angle;
  u16 direction;
  s32 angleIndex;
  s32 count;
  s32 difference;
  s32 terrain;
  s32 speed;
  s32 targetCameraYaw;
  struct WorldCamera *camera;
  s32 yawStep;
  struct Actor *ripple;
  struct PlayerSprite__f028aac *rippleSprite;
  struct EventWork *work;
  struct WorldEventWork__f028aac *worldWork;
  struct Actor *other;
  u32 kind;
  struct WorldEventWork__f028aac *flightWork;

  count = 0;
  terrain = GetWorldMapCollision((struct WorldPosition *)&object->x);
  worldWork = Ram_HeapSlots->eventWork;
  if (terrain >= 1 && terrain <= 3)
    worldWork->terrainKind = 0;
  if (terrain >= 5 && terrain <= 6)
    worldWork->terrainKind = 1;
  if (terrain == 4)
    worldWork->terrainKind = 2;
  if (terrain >= 7 && terrain <= 12)
    worldWork->terrainKind = 3;

  object->motionFlags = 0;
  collision = 0;
  object->speedLimit = object->y / 16 + 0x14000;
  object->acceleration = 0x8000;
  angle = gData_0802ec5c[(gInput.held >> 4) & 15];
  direction = angle;
  if (direction == 0xffff) {
    collision |= 4;
    goto update_object;
  }
  position.x = object->x;
  position.y = object->y;
  position.z = object->z;
  Vector_AddPolarOffset(0x80000, direction, &position);
  if (gDebugMode != 0 && (gInput.held & 0x200) != 0)
    goto update_object;
  if (Func_0802dbd0(object, &position) == 0) {
    testPosition.x = object->x;
    testPosition.y = object->y;
    testPosition.z = object->z;
    Vector_AddPolarOffset(0x80000, direction + 0x1000, &testPosition);
    if (Func_0802dbd0(object, &testPosition) == 0) {
      testPosition.x = object->x;
      testPosition.y = object->y;
      testPosition.z = object->z;
      Vector_AddPolarOffset(0x80000, direction - 0x1000, &testPosition);
      if (Func_0802dbd0(object, &testPosition) == 0) {
        testPosition.x = object->x;
        testPosition.y = object->y;
        testPosition.z = object->z;
        Vector_AddPolarOffset(0x80000, direction + 0x2000, &testPosition);
        if (Func_0802dbd0(object, &testPosition) == 0) {
          testPosition.x = object->x;
          testPosition.y = object->y;
          testPosition.z = object->z;
          Vector_AddPolarOffset(0x80000, direction - 0x2000, &testPosition);
          if (Func_0802dbd0(object, &testPosition) == 0)
            goto check_objects;
        }
      }
    }
  }
  angleOffsets[count++] = direction + 0x1000;
  angleOffsets[count++] = direction - 0x1000;
  angleOffsets[count++] = direction + 0x2000;
  angleOffsets[count++] = direction - 0x2000;
  angleOffsets[count++] = direction + 0x3000;
  angleOffsets[count++] = direction - 0x3000;
  for (angleIndex = 0; angleIndex < count; angleIndex++) {
    direction = angleOffsets[angleIndex];
    position.x = object->x;
    position.y = object->y;
    position.z = object->z;
    Vector_AddPolarOffset(0x80000, direction, &position);
    if (Func_0802dbd0(object, &position) != 0)
      continue;
    testPosition.x = object->x;
    testPosition.y = object->y;
    testPosition.z = object->z;
    Vector_AddPolarOffset(0x80000, direction + 0x1000, &testPosition);
    if (Func_0802dbd0(object, &testPosition) != 0)
      continue;
    testPosition.x = object->x;
    testPosition.y = object->y;
    testPosition.z = object->z;
    Vector_AddPolarOffset(0x80000, direction - 0x1000, &testPosition);
    if (Func_0802dbd0(object, &testPosition) != 0)
      continue;
    testPosition.x = object->x;
    testPosition.y = object->y;
    testPosition.z = object->z;
    Vector_AddPolarOffset(0x80000, direction + 0x2000, &testPosition);
    if (Func_0802dbd0(object, &testPosition) != 0)
      continue;
    testPosition.x = object->x;
    testPosition.y = object->y;
    testPosition.z = object->z;
    Vector_AddPolarOffset(0x80000, direction - 0x2000, &testPosition);
    if (Func_0802dbd0(object, &testPosition) != 0)
      continue;
    angle = (s16)direction;
    goto check_objects;
  }
  collision |= 1;

check_objects:
  other = Ram_HeapSlots->scriptObjects;
  for (angleIndex = 63; angleIndex >= 0; angleIndex -= 1, other++) {
    if (other->script != NULL && (other->collisionFlags & 1) &&
        other != object &&
        Field_CheckSpheresOverlap(&other->x, other->radius, &position.x,
                                  object->radius) >= 0)
      collision |= 2;
  }

update_object:
  work = Ram_HeapSlots->eventWork;
  if (work != NULL) {
    if ((collision & 3) != 0)
      work->blockedSteps++;
    else
      work->blockedSteps = 0;
    work->heldKeys = gInput.held;
  }
  if (object->y >= 0x40000)
    Actor_SelectAnimation(object, 6);
  else if (collision != 0)
    Actor_SelectAnimation(object, 1);
  else
    Actor_SelectAnimation(object, 2);

  if (collision != 0) {
    object->targetX = 0x80000000;
    object->targetY = 0x80000000;
    object->targetZ = 0x80000000;
    object->velocityX = 0;
    object->velocityZ = 0;
    if ((collision & 3) != 0) {
      direction = angle;
      difference = (s16)(direction - object->facing);
      if (difference > 0x1000)
        difference = 0x1000;
      if (difference < -0x1000)
        difference = -0x1000;
      object->facing += difference;
    }
    object->timer = 0;
    object->unk66 = 2;
  } else {
    Actor_SetMoveTarget(object, position.x, position.y, position.z);
    speed = FixedSqrt(Iwram_MulQ16(object->velocityX, object->velocityX) +
                      Iwram_MulQ16(object->velocityZ, object->velocityZ));
    object->velocityX = collision;
    object->velocityZ = collision;
    direction = angle;
    Vector_AddPolarOffset(speed, direction,
                          (struct FieldPosition *)&object->velocityX);
    if (object->timer != 0)
      object->timer--;
    object->actionFlags &= ~1;
    difference = (s16)(direction - object->facing);
    if (difference > 0x1000)
      difference = 0x400;
    if (difference < -0x1000)
      difference = -0x400;
    object->facing += difference;
  }

  camera = (struct WorldCamera *)Ram_HeapSlots->mapWork;
  targetCameraYaw = Iwram_MulQ16(gData_0802eca0[(gInput.held >> 4) & 15],
                                 object->y / 4 + 0x10000);
  yawStep = (s16)(targetCameraYaw - camera->yaw) / 8;
  if (yawStep > 32)
    yawStep = 32;
  if (yawStep < -32)
    yawStep = -32;
  if (yawStep >= -1 && yawStep <= 1)
    yawStep = targetCameraYaw - camera->yaw;
  camera->yaw += yawStep;

  kind = object->animationKind;
  if (kind == 1) {
    if (object->y < 0x40000) {
      if (object->timer == 0 && collision == 0) {
        ripple = Func_08023220(0x10d, object->x, 0, object->z);
        if (ripple != 0) {
          rippleSprite = ripple->sprite;
          Actor_StartScript(ripple, gData_0802ec94);
          ripple->motionFlags = 0;
          ripple->terrainId = kind;
          if (rippleSprite != 0) {
            Animation_ApplyChildArgument(rippleSprite, 1);
            rippleSprite->flags = 0;
            rippleSprite->objectMode = 1;
            rippleSprite->priority = 2;
          }
          object->timer = 10;
        }
      }
    } else {
      Actor_SetChildByteValue(object, gFrameCount & kind);
    }
  }

  flightWork = Ram_HeapSlots->eventWork;
  if (gInput.held & (gMainRoutineWork.keys230 | gMainRoutineWork.unk232)) {
    flightWork->climbing = 1;
    object->y += 0x2000;
  } else {
    object->y -= 0x4000;
  }
  if (Func_080ad318() == 0)
    object->y -= 0x4000;
  if (object->y <= 0 && flightWork->climbing != 0)
    ((struct EventWork *)flightWork)->unk16a = 254;
  if (object->y >= 0xc0000)
    object->y = 0xc0000;
  if (object->y <= 0)
    object->y = 0;
  object->step++;
  return 1;
}

// ---- games/tla/src/field/script/move_by_keys.c ----


void Vector_AddPolarOffset(s32 radius, s32 angle,
                           struct FieldPosition *position);
s32 Func_080c8710(struct Actor *object);
s32 Func_080c8700(struct Actor *object);
s32 Func_0802da24(s32 *position);
s32 Func_0802d45c(s32 terrainId, s32 x, s32 z);
void Actor_SelectAnimation(struct Actor *object, s32 argument);
void Actor_SetMoveTarget(struct Actor *actor, s32 x, s32 y, s32 z);

/// The held keys, read again at each use through a volatile pointer, as The
/// Broken Seal's gKeysHeld is declared; as gInput.held, 45 of 241 instructions
/// of Actor_MoveByKeys differ in tla-en.
#define INPUT_HELD (((volatile struct InputState *)&gInput)->held)

/// The script command 0x1b: moves an object 8 pixels at a time by the
/// direction pad, with up and down changing its height (y) rather than its
/// depth. Refuses steps into an occupied cell, from or onto a cell without
/// attribute bit 6, or onto ground too high (up) or too low (down), and
/// records the held keys in the event work. Returns 1.
// As ☀️'s Actor_MoveByKeys.
s32 Actor_MoveByKeys(struct Actor *object) {
  struct FieldPosition position;
  struct MapCell *sourceCell;
  struct MapCell *destinationCell;
  struct EventWork *work;
  u16 angle;
  s32 animation;
  s32 collisionFlags;
  s32 deltaZ;
  s32 terrainHeight;
  struct MapWork *map;
  u32 keys;
  s32 half;

  object->acceleration = 0x4000;
  animation = 12;
  half = 0x8000;
  object->speedLimit = half;
  if (gDebugMode)
    keys = INPUT_HELD;
  keys = INPUT_HELD;
  {
    const u16 *table = gData_0802ec5c;

    angle = *(const u16 *)((const u8 *)table + (((keys >> 4) & 15) << 1));
  }
  if (angle == 0xffff) {
    collisionFlags = 4;
  } else {
    animation = 14;
    if ((angle & 0xf000) != 0) {
      animation = 15;
      if ((angle & 0xf000) != half)
        animation = 10;
    }
    collisionFlags = 0;
    position.x = 0;
    position.y = 0;
    position.z = 0;
    Vector_AddPolarOffset(0x80000, angle, &position);
    position.x += object->x;
    deltaZ = position.z;
    if (deltaZ < 0)
      object->facing = 0xc000;
    if (deltaZ > 0)
      object->facing = 0x4000;
    position.y = object->y - position.z;
    position.z = object->z;
    map = Ram_HeapSlots->mapWork;
    sourceCell = &map->layers[object->terrainId].cells[Func_080c8710(object)];
    destinationCell =
        &map->layers[object->terrainId]
             .cells[position.x / 0x100000 + (position.z / 0x100000) * 128];
    if (Func_0802da24((s32 *)&position) != 0 ||
        !(sourceCell->attributeB & 64) || !(destinationCell->attributeB & 64)) {
      collisionFlags = 4;
      animation = 12;
    } else if (INPUT_HELD & 64) {
      terrainHeight =
          Func_0802d45c(object->terrainId, position.x, position.z - 0x100000);
      if (terrainHeight - object->y < 0x100000)
        goto refused;
    } else if (INPUT_HELD & 128) {
      terrainHeight = Func_0802d45c(object->terrainId, position.x, position.z);
      if (terrainHeight - object->y > -0x80000) {
      refused:
        collisionFlags = 1;
        animation = 12;
      }
    }
  }
  work = Ram_HeapSlots->eventWork;
  if (work != NULL) {
    if (collisionFlags & 3)
      work->blockedSteps++;
    else
      work->blockedSteps = 0;
    work->heldKeys = INPUT_HELD;
  }
  Actor_SelectAnimation(object, animation);
  if (collisionFlags != 0) {
    object->targetX = 0x80000000;
    object->targetY = 0x80000000;
    object->targetZ = 0x80000000;
    object->velocityX = 0;
    object->velocityY = 0;
    object->velocityZ = 0;
  } else {
    Actor_SetMoveTarget(object, position.x, position.y, position.z);
  }
  object->step++;
  return 1;
}


struct ChainSlot {
  s16 unk00;
  s16 unk02;
  u8 unk04[8];
  s32 unk0c;
  u8 unk10[8];
  s32 unk18;
  u8 unk1c[4];
};

struct ChainNode {
  u8 unk00[4];
  s32 x;
  s32 y;
  s32 z;
  u16 unk10;
  s8 visible;
  s8 unk13;
  u8 unk14[12];
};

struct ChainWork {
  u8 unk000[12];
  struct ChainSlot slots[8];
  struct ChainNode nodes[128];
};

struct SpriteAngle {
  u8 unk00[18];
  u16 angle;
};

#define CHAIN_WORK (*(struct ChainWork **)&Ram_HeapSlots->unk84[6])

struct Actor *Actor_GetByIdFar(s32 id);
void Actor_SetAnimationSpeed(struct Actor *object, s32 value);

s32 Func_080292fc(struct Actor *object) {
  struct FieldPosition position;
  s32 collisionFlags;
  struct SpriteAngle *sprite;
  struct ChainWork *chain;
  s32 bestDistance;
  s32 value;
  s32 speed;
  struct ChainNode *best;
  struct ChainNode *node;
  struct ChainSlot *slot;
  s32 count;
  u16 angle;
  s32 turn;
  struct Actor *first;
  struct Actor *second;
  struct EventWork *work;

  sprite = object->sprite;
  chain = CHAIN_WORK;
  speed = 0x10000;
  collisionFlags = 0;
  object->speedLimit = 0x8000;
  object->acceleration = 0x8000;
  object->motionFlags = 0;
  angle = gData_0802ec5c[(INPUT_HELD >> 4) & 15];
  if (angle == 0xffff) {
    best = (struct ChainNode *)object->watchedObject;
    value = 0;
  } else {
    value = 16;
    position.x = 0;
    position.y = 0;
    position.z = 0;
    Vector_AddPolarOffset(0x80000, angle, &position);
    position.x += object->x;
    position.y = object->y - position.z;
    position.z = object->z;
    chain = CHAIN_WORK;
    node = chain->nodes;
    best = NULL;
    bestDistance = 0x10000000;
    if (chain == NULL) {
      collisionFlags = 1;
      /* FAKEMATCH(dead): the event work slot copied onto itself, a second
       * store in the null branch that keeps the loop count from being lifted
       * above the null test; without it, 21 of 300 instructions differ in
       * tla-en. */
      Ram_HeapSlots->eventWork = Ram_HeapSlots->eventWork;
      goto done;
    }
    for (count = 0; count < 128; count++, node++) {
      if (node->visible == 1) {
        s32 dx = (position.x - node->x) >> 8;
        s32 dy = (position.y - node->y) >> 8;
        s32 dz = (position.z - node->z) >> 8;
        s32 distance =
            Iwram_MulQ16(dx, dx) + Iwram_MulQ16(dy, dy) + Iwram_MulQ16(dz, dz);

        if (distance <= 0x400 && bestDistance > distance) {
          bestDistance = distance;
          best = node;
        }
      }
    }
  }
  if (best == NULL) {
    // DEAD: collisionFlags is still 0 here, so the OR reads nothing; the source
    // keeps it.
    collisionFlags |= 1;
  } else {
    slot = &chain->slots[best->unk13 - 1];
    first = Actor_GetByIdFar(slot->unk00);
    second = Actor_GetByIdFar(slot->unk02);
    if (first->x > second->x)
      speed = -speed;
    first = Actor_GetByIdFar(slot->unk00);
    second = Actor_GetByIdFar(slot->unk02);
    if (first->y > second->y) {
      speed = -speed;
      turn = 0xc000;
    } else {
      turn = 0x4000;
    }
    object->scaleX = speed;
    if (best->x == object->x && best->y == object->y && best->z == object->z) {
      collisionFlags = 1;
    } else {
      slot->unk18 = 1;
      slot->unk0c += 0x8000;
      if (slot->unk0c > 0x20000)
        slot->unk0c = 0x20000;
      object->watchedObject = (struct Actor *)best;
      position.x = best->x;
      position.y = best->y;
      position.z = best->z;
      sprite->angle = best->unk10 + turn;
    }
  }
done:
  work = Ram_HeapSlots->eventWork;
  if (work != NULL) {
    if (collisionFlags & 3)
      work->blockedSteps++;
    else
      work->blockedSteps = 0;
    work->heldKeys = INPUT_HELD;
  }
  Actor_SelectAnimation(object, 29);
  Actor_SetAnimationSpeed(object, value);
  if (collisionFlags != 0) {
    object->targetX = 0x80000000;
    object->targetY = 0x80000000;
    object->targetZ = 0x80000000;
    object->velocityX = 0;
    object->velocityY = 0;
    object->velocityZ = 0;
  } else {
    Actor_SetMoveTarget(object, position.x, position.y, position.z);
  }
  object->step++;
  return 1;
}

#include "worka0.h"

static inline void TurnToward(struct Actor *object, u16 angle) {
  s16 delta = angle - object->facing;

  if (delta > 0x1000)
    delta = 0x1000;
  if (delta < -0x1000)
    delta = -0x1000;
  object->facing += delta;
}

s32 Func_08029570(struct Actor *object) {
  struct FieldPosition position;
  s32 collisionFlags;
  struct ObjectAnimationState *sprite;
  s32 argument;
  struct WorkA0 *workA0;
  struct WorkA0Node *best;
  struct WorkA0Node *point;
  struct WorkA0Slot *slot;
  s32 bestDistance;
  u16 angle;
  s32 count;
  struct EventWork *work;

  sprite = object->sprite;
  argument = 1;
  collisionFlags = 0;
  object->speedLimit = 0x8000;
  object->acceleration = 0x8000;
  object->motionFlags = 0;
  angle = gData_0802ec5c[(gInput.held >> 4) & 15];
  if (angle == 0xffff) {
    collisionFlags = 4;
    best = (struct WorkA0Node *)object->watchedObject;
    position.x = object->x;
    position.y = best->y;
    position.z = object->z;
  } else {
    position.x = object->x;
    position.y = object->y;
    position.z = object->z;
    Vector_AddPolarOffset(0x80000, angle, &position);
    // Read through the slot's address: as Ram_HeapSlots->unk84[7], 44 of 324
    // instructions differ in tla-en.
    workA0 = *(struct WorkA0 **)&Ram_HeapSlots->unk84[7];
    point = workA0->items;
    best = NULL;
    bestDistance = 0x10000000;
    if (workA0 == NULL) {
      collisionFlags = 1;
      /* FAKEMATCH(dead): the event work slot copied onto itself, a second
       * store in the null branch that keeps the loop count from being lifted
       * above the null test; without it, 29 of 324 instructions differ in
       * tla-en. */
      Ram_HeapSlots->eventWork = Ram_HeapSlots->eventWork;
    } else {
      for (count = 0; count < 128; count++, point++) {
        if (point->used == 1) {
          s32 dx = (position.x - point->x) >> 8;
          s32 dy = (position.y - point->y) >> 8;
          s32 dz = (position.z - point->z) >> 8;
          s32 distance = Iwram_MulQ16(dx, dx) + Iwram_MulQ16(dy, dy) +
                         Iwram_MulQ16(dz, dz);

          if (distance <= 0x400 && bestDistance > distance) {
            bestDistance = distance;
            best = point;
          }
        }
      }
      if (best == NULL) {
        collisionFlags |= 1;
      } else if (best->x == object->x && best->y == object->y &&
                 best->z == object->z) {
        collisionFlags = 1;
      } else {
        // The slot index, a signed byte (ldrsb) that worka0.h's unk11 does not
        // yet type.
        if ((s8)best->unk11[0] != 0) {
          slot = &workA0->slots[(s8)best->unk11[0] - 1];
          slot->tail = (struct WorkA0Node *)1;
          slot->amplitude += 0x8000;
          if (slot->amplitude > 0x20000)
            slot->amplitude = 0x20000;
          object->watchedObject = (struct Actor *)best;
        }
        /* FAKEMATCH(dead): position.x copied onto itself before the copies, a
         * store that stops GCSE carrying the compare's x load past the pair
         * block; without it, 45 of 324 instructions differ in tla-en. */
        position.x = position.x;
        position.x = best->x;
        position.y = best->y;
        position.z = best->z;
      }
    }
  }
  work = Ram_HeapSlots->eventWork;
  if (work != NULL) {
    if (collisionFlags & 3)
      work->blockedSteps++;
    else
      work->blockedSteps = 0;
    work->heldKeys = gInput.held;
  }
  if (collisionFlags == 0)
    argument = 2;
  Actor_SelectAnimation(object, argument);
  Actor_SetAnimationSpeed(object, 8);
  sprite->unk00[23] = 4;
  if (collisionFlags != 0) {
    object->targetX = 0x80000000;
    object->targetY = 0x80000000;
    object->targetZ = 0x80000000;
    object->velocityX = 0;
    object->velocityY = 0;
    object->velocityZ = 0;
    if (collisionFlags & 3)
      TurnToward(object, angle);
    object->y = position.y;
  } else {
    object->y = position.y;
    Actor_SetMoveTarget(object, position.x, position.y, position.z);
    if (object->x == position.x && object->z == position.z)
      TurnToward(object, angle);
  }
  object->step++;
  return 1;
}

#include "flag_call.h"

#define HALF_AT(base, offset) (*(u16 *)((u8 *)(base) + (offset)))

u32 Random16(void);

void Func_08029810(struct Actor *object) {
  u32 first;
  u32 second;
  s32 x;

  HALF_AT(object->sprite, 18) += 0x400;
  first = Random16();
  second = Random16();
  x = object->x;

  second >>= 1;
  first >>= 1;
  object->x = x + (first - second);
}

s32 Func_080c8828(s32 arg);

/// When one of three party key sets is pressed: plays cue 113 if the far call
/// accepts the argument, else posts event request 0x208b and returns 1 while
/// flag 0x146 is clear.
s32 Func_08029838(s32 arg) {
  struct EventWork *work = Ram_HeapSlots->eventWork;
  /* FAKEMATCH(pin): pressed held in r0, the register the early zero is
   * returned in, so the argument moves to r6; as a plain local, which takes r6
   * itself, 14 of 61 instructions differ in tla-en. */
  register s32 pressed asm("r0") = 0;

  if (work == NULL)
    return 0;
  if ((gInput.pressed & gMainRoutineWork.keys234) ||
      (gInput.pressed & gMainRoutineWork.keys230) ||
      (gInput.pressed & gMainRoutineWork.keys236))
    pressed = 1;
  if (pressed) {
    if (Func_080c8828(arg)) {
      Sound_PlayFar(113);
    } else if (!GameFlag_Test(0x146)) {
      work->unk166 = 0x208b;
      return 1;
    }
  }
  return 0;
}
#undef INPUT_HELD
#undef CHAIN_WORK
#undef HALF_AT

// ---- games/tla/src/field/script/f0298c0.c ----

/* VIEW(f0298c0): tag struct PlayerSprite as struct PlayerSprite__f0298c0; resource_slot.c has another definition; tried with the definition the rest of the object has, it does not compile in tla-ja: games/tla/src/field/resource_slot.c:7305: structure has no member named `entry'. */
struct PlayerSprite__f0298c0 {
  u8 unk00[9];
  u8 low09 : 2;
  u8 priority : 2;
  u8 high09 : 4;
  u8 unk0a[0x10];
  u8 flags;
  u8 unk1b[0x11];
  u8 *entry;
};

struct Unk_WakeActor {
  u8 unk00[0x63];
  u8 unk63;
};

struct Unk_ClimbEventWork {
  u8 unk000[0xd4a];
  /// Read as the low byte of a halfword: as a u8 or s16 field the read waits
  /// for the sprite store (8 of 965 instructions differ in tla-en).
  u16 unkd4a : 8;
  u16 unkd4b : 8;
};

// The heading of each direction-pad combination, -1 for none.
extern const s16 gData_0802ec5c[16];
extern const s32 gData_0802ed28[];
extern const s32 gData_0802ece0[];

void Vector_AddPolarOffset(s32 radius, s32 angle,
                           struct FieldPosition *position);
s32 Func_0802d45c(s32 terrainId, s32 x, s32 z);
s32 Func_0802d7b0(s32 x, s32 z, s32 layer, s32 height);
s32 Func_0802d87c(struct Actor *actor, struct FieldPosition *position);
s32 Func_080c8770(s32 x, s32 z, s32 kind);
void Actor_SelectAnimation(struct Actor *actor, s32 argument);
void Actor_SetMoveTarget(struct Actor *actor, s32 x, s32 y, s32 z);
struct Actor *Func_08023220(s32 kind, s32 x, s32 y, s32 z);
void Actor_StartScript(struct Actor *, const s32 *);

s32 Func_080298c0(struct Actor *actor) {
  struct FieldPosition position;
  struct FieldPosition testPosition;
  s16 angleOffsets[10];
  struct FieldPosition spawn;
  s32 collision;
  s32 heading;
  s32 groundHere;
  s32 y;
  s32 groundAhead;
  s32 angle;
  s32 cellX;
  s32 cellZ;
  s32 angleIndex;
  s32 count;
  u8 *terrainId;
  u8 *wakeTimer;
  struct Actor *effect;
  struct PlayerSprite__f0298c0 *sprite;

  heading = 0;
  count = 0;
  cellX = actor->x & 0xfff00000;
  cellZ = actor->z & 0xfff00000;
  y = actor->y;
  terrainId = &actor->terrainId;
  groundHere = Func_0802d45c(*terrainId, cellX, cellZ);
  groundAhead = Func_0802d45c(*terrainId, cellX, cellZ + 0xfff00000);
  if (actor->timer != 0) {
    if (actor->targetX != 0x80000000)
      goto done;
    actor->timer = 0;
    actor->terrainHeight = groundHere;
  }
  if ((s16)actor->unk66 != 0) {
    if (groundAhead < y) {
      s32 topZ = cellZ + 0xfff80000;

      if (Func_0802d7b0(actor->x, topZ, *terrainId, actor->terrainHeight) ==
          2) {
        actor->y -= 0x10000;
      } else {
        Actor_SetMoveTarget(actor, actor->x, groundAhead, topZ);
        actor->timer = 1;
        actor->unk66 = 0;
        goto done;
      }
    }
    if (groundHere > y) {
      actor->unk66 = 0;
      actor->terrainHeight = groundHere;
      actor->motionFlags = 3;
      goto done;
    }

    collision = 0;
    // DEAD: the debug read of the held keys is never used.
    if (gDebugMode != 0)
      (void)*(volatile u32 *)&gInput.held;
    actor->speedLimit = 0x8000;
    actor->acceleration = 0x4000;
    angle = gData_0802ec5c[(gInput.held >> 4) & 15];
    if ((u16)angle == 0xffff) {
      collision = 4;
    } else {
      position.x = actor->x;
      position.y = 0;
      position.z = 0;
      Vector_AddPolarOffset(0x80000, (u16)angle, &position);
      position.y = actor->y - position.z;
      position.z = actor->z;
      Actor_SelectAnimation(actor, 3);
      testPosition.x = actor->x;
      testPosition.y = 0;
      testPosition.z = 0;
      Vector_AddPolarOffset(0x80000, (u16)angle, &testPosition);
      testPosition.y = actor->y - testPosition.z;
      testPosition.z = actor->z;
      if (Func_0802d87c(actor, &testPosition))
        goto climb_blocked;
      testPosition.x = actor->x;
      testPosition.y = 0;
      testPosition.z = 0;
      Vector_AddPolarOffset(0x80000, (u16)angle + 0x1000, &testPosition);
      testPosition.y = actor->y - testPosition.z;
      testPosition.z = actor->z;
      if (Func_0802d87c(actor, &testPosition))
        goto climb_blocked;
      testPosition.x = actor->x;
      testPosition.y = 0;
      testPosition.z = 0;
      Vector_AddPolarOffset(0x80000, (u16)angle - 0x1000, &testPosition);
      testPosition.y = actor->y - testPosition.z;
      testPosition.z = actor->z;
      if (Func_0802d87c(actor, &testPosition))
        goto climb_blocked;
      testPosition.x = actor->x;
      testPosition.y = 0;
      testPosition.z = 0;
      Vector_AddPolarOffset(0x80000, (u16)angle + 0x2000, &testPosition);
      testPosition.y = actor->y - testPosition.z;
      testPosition.z = actor->z;
      if (Func_0802d87c(actor, &testPosition))
        goto climb_blocked;
      testPosition.x = actor->x;
      testPosition.y = 0;
      testPosition.z = 0;
      Vector_AddPolarOffset(0x80000, (u16)angle - 0x2000, &testPosition);
      testPosition.y = actor->y - testPosition.z;
      testPosition.z = actor->z;
      if (!Func_0802d87c(actor, &testPosition))
        goto climb_moved;
    climb_blocked:
      position.x = actor->x;
      position.y = actor->y;
      position.z = actor->z;
      collision |= 1;
    }
  climb_moved:
    if (collision != 0) {
      actor->targetX = 0x80000000;
      actor->targetY = 0x80000000;
      actor->targetZ = 0x80000000;
      actor->velocityX = 0;
      actor->velocityZ = 0;
    } else {
      s32 speed;

      heading = (s16)angle;
      Actor_SetMoveTarget(actor, position.x, position.y, position.z);
      speed = FixedSqrt(Iwram_MulQ16(actor->velocityX, actor->velocityX) +
                        Iwram_MulQ16(actor->velocityZ, actor->velocityZ));
      actor->velocityX = 0;
      actor->velocityZ = 0;
      Vector_AddPolarOffset(speed, (u16)heading,
                            (struct FieldPosition *)&actor->velocityX);
      actor->velocityY = -actor->velocityZ;
      actor->velocityZ = 0;
    }
    // Every 16th frame, a bit of rubble drops beside the climber.
    if ((gFrameCount & 15) == 0) {
      spawn.x = actor->x - Random16() * 6 + 0x30000;
      spawn.y = actor->y;
      spawn.z = actor->z;
      effect = Func_08023220(0x11d, spawn.x, spawn.y, spawn.z);
      if (effect != NULL) {
        effect->terrainHeight = actor->terrainHeight;
        sprite = effect->sprite;
        Actor_StartScript(effect, gData_0802ed28);
        effect->update = Func_08029810;
        effect->motionFlags = 2;
        if (collision == 0)
          Sound_PlayFar(596);
        effect->gravity = 0xa3d;
        effect->velocityY = 0;
        effect->scaleX = 0x4ccc;
        effect->scaleY = 0x4ccc;
        if (sprite != NULL) {
          Animation_ApplyChildArgument(sprite, 2);
          sprite->flags = 0;
          sprite->priority = 0;
        }
      }
    }
    Func_08029838(actor);
    goto done;
  }

  // On foot.
  collision = 0;
  actor->speedLimit = 0x10000;
  actor->acceleration = 0x4000;
  angle = gData_0802ec5c[(gInput.held >> 4) & 15];
  if ((u16)angle == 0xffff) {
    collision = 4;
    goto counted;
  }
  position.x = actor->x;
  position.y = actor->y;
  position.z = actor->z;
  Vector_AddPolarOffset(0x80000, (u16)angle, &position);
  if (gDebugMode != 0 && (gInput.held & 0x200)) {
    heading = (s16)angle;
    goto counted;
  }
  if (!Func_0802d87c(actor, &position)) {
    testPosition.x = actor->x;
    testPosition.y = actor->y;
    testPosition.z = actor->z;
    Vector_AddPolarOffset(0x80000, (u16)angle + 0x1000, &testPosition);
    if (!Func_0802d87c(actor, &testPosition)) {
      testPosition.x = actor->x;
      testPosition.y = actor->y;
      testPosition.z = actor->z;
      Vector_AddPolarOffset(0x80000, (u16)angle - 0x1000, &testPosition);
      if (!Func_0802d87c(actor, &testPosition)) {
        testPosition.x = actor->x;
        testPosition.y = actor->y;
        testPosition.z = actor->z;
        Vector_AddPolarOffset(0x80000, (u16)angle + 0x2000, &testPosition);
        if (!Func_0802d87c(actor, &testPosition)) {
          testPosition.x = actor->x;
          testPosition.y = actor->y;
          testPosition.z = actor->z;
          Vector_AddPolarOffset(0x80000, (u16)angle - 0x2000, &testPosition);
          if (!Func_0802d87c(actor, &testPosition)) {
            heading = (s16)angle;
            goto counted;
          }
        }
      }
    }
  }
  {
    u16 direction = angle;

    angleOffsets[count++] = direction + 0x1000;
    angleOffsets[count++] = direction - 0x1000;
    angleOffsets[count++] = direction + 0x2000;
    angleOffsets[count++] = direction - 0x2000;
    angleOffsets[count++] = direction + 0x3000;
    angleOffsets[count++] = direction - 0x3000;
  }
  for (angleIndex = 0; angleIndex < count; angleIndex++) {
    heading = angleOffsets[angleIndex];
    position.x = actor->x;
    position.y = actor->y;
    position.z = actor->z;
    Vector_AddPolarOffset(0x80000, (u16)heading, &position);
    if (Func_0802d87c(actor, &position))
      continue;
    testPosition.x = actor->x;
    testPosition.y = actor->y;
    testPosition.z = actor->z;
    Vector_AddPolarOffset(0x80000, (u16)heading + 0x1000, &testPosition);
    if (Func_0802d87c(actor, &testPosition))
      continue;
    testPosition.x = actor->x;
    testPosition.y = actor->y;
    testPosition.z = actor->z;
    Vector_AddPolarOffset(0x80000, (u16)heading - 0x1000, &testPosition);
    if (Func_0802d87c(actor, &testPosition))
      continue;
    testPosition.x = actor->x;
    testPosition.y = actor->y;
    testPosition.z = actor->z;
    Vector_AddPolarOffset(0x80000, (u16)heading + 0x2000, &testPosition);
    if (Func_0802d87c(actor, &testPosition))
      continue;
    testPosition.x = actor->x;
    testPosition.y = actor->y;
    testPosition.z = actor->z;
    Vector_AddPolarOffset(0x80000, (u16)heading - 0x2000, &testPosition);
    if (!Func_0802d87c(actor, &testPosition))
      goto counted;
  }
  position.x = actor->x;
  position.y = actor->y;
  position.z = actor->z;
  collision |= 1;
  // Up against a face more than 8 pixels high: start climbing it.
  if ((gInput.held & 0x40) && groundAhead > y + 0x80000) {
    cellX = (position.x & 0xfff00000) + 0x80000;
    cellZ = (position.z & 0xfff00000) + 0xfff80000;

    if (Func_080c8770(cellX, cellZ, 2)) {
      actor->motionFlags = 0;
      actor->y += 0x10000;
      actor->unk66 = 1;
      actor->velocityX = 0;
      actor->velocityY = 0;
      actor->velocityZ = 0;
      Actor_SelectAnimation(actor, 3);
      goto done;
    }
  }
  // Down at a drop: climb down the face south of the cell.
  if (gInput.held & 0x80) {
    cellX = (actor->x & 0xfff00000) + 0x80000;
    cellZ = (actor->z & 0xfff00000) + 0x180000;

    if (Func_0802d45c(*terrainId, cellX, cellZ) < y &&
        Func_080c8770(cellX, cellZ, 2)) {
      actor->motionFlags = 0;
      Actor_SetMoveTarget(actor, actor->x, groundHere + 0xfffe0000, cellZ);
      actor->timer = 1;
      actor->unk66 = 1;
      actor->velocityX = 0;
      actor->velocityY = 0;
      actor->velocityZ = 0;
      Actor_SelectAnimation(actor, 3);
      goto done;
    }
  }

counted:
  {
    struct EventWork *work = Ram_HeapSlots->eventWork;

    if (work != NULL) {
      if (collision & 3)
        work->blockedSteps++;
      else
        work->blockedSteps = 0;
      work->heldKeys = gInput.held;
    }
  }
  Actor_SelectAnimation(actor, 1);
  if (collision != 0) {
    actor->targetX = 0x80000000;
    actor->targetY = 0x80000000;
    actor->targetZ = 0x80000000;
    actor->velocityX = 0;
    actor->velocityZ = 0;
    if (collision & 3) {
      s32 difference;
      u16 turn = angle;

      difference = (s16)(turn - actor->facing);
      if (difference > 0x1000)
        difference = 0x1000;
      if (difference < -0x1000)
        difference = -0x1000;
      actor->facing += difference;
    }
    wakeTimer = (u8 *)actor + 98;
    *wakeTimer = 0;
  } else {
    s32 speed;

    Actor_SetMoveTarget(actor, position.x, position.y, position.z);
    speed = FixedSqrt(Iwram_MulQ16(actor->velocityX, actor->velocityX) +
                      Iwram_MulQ16(actor->velocityZ, actor->velocityZ));
    actor->velocityX = 0;
    actor->velocityZ = 0;
    Vector_AddPolarOffset(speed, (u16)heading,
                          (struct FieldPosition *)&actor->velocityX);
    wakeTimer = (u8 *)actor + 98;
    if (*wakeTimer != 0)
      (*wakeTimer)--;
  }
  // The retest reads through the actor, not wakeTimer: that keeps its
  // zero path from skipping it.
  if (((u8 *)actor)[98] == 0 && collision == 0) {
    effect = Func_08023220(0x164, actor->x, actor->y, actor->z - 0x20000);
    if (effect != NULL) {
      effect->terrainHeight = actor->terrainHeight;
      sprite = effect->sprite;
      Actor_StartScript(effect, gData_0802ece0);
      effect->priorityFlags = 8;
      // Through a struct member: as a byte offset the store holds back the
      // stack read of collision after it.
      ((struct Unk_WakeActor *)effect)->unk63 = 100;
      effect->motionFlags = 0;
      Sound_PlayFar(596);
      if (sprite != NULL) {
        struct Unk_ClimbEventWork *work = Ram_HeapSlots->eventWork;

        Animation_ApplyChildArgument(sprite, 1);
        sprite->flags = 0;
        // DEAD: overwritten by the next line.
        sprite->priority = 2;
        sprite->priority = work->unkd4a & 3;
      }
      *wakeTimer = 5;
    }
  }
  Func_08029838(actor);

done:
  actor->step++;
  return 1;
}



struct Unk0802a124Actor {
  u8 unk00[0x62];
  u8 unk62;
  u8 unk63;
};

// The camera yaw each direction-pad combination turns toward.
extern const s32 gData_0802eca0[16];

s32 Func_0802db64(struct Actor *object, struct FieldPosition *position);

/// Script command 0x22: moves the object on the world map by the direction pad
/// at a fixed pace (faster while L repeats), trying six turned headings when
/// the way is blocked, as Func_080279b0 does; records the held keys in the
/// event work and leaves an object of kind 0x164 behind it every fifth step.
s32 Func_0802a124(struct Actor *object) {
  struct FieldPosition position;
  struct FieldPosition leftPosition;
  struct FieldPosition rightPosition;
  s16 angleOffsets[22];
  s32 collision;
  s32 angle;
  u16 direction;
  s32 angleIndex;
  s32 count;
  s32 difference;
  s32 y;
  s32 terrainCollision;
  s32 speed;
  s32 targetCameraYaw;
  struct WorldCamera *camera;
  s32 yawStep;
  struct PlayerSprite__f0298c0 *playerSprite;
  struct Actor *ripple;
  struct PlayerSprite__f0298c0 *rippleSprite;
  struct EventWork *work;
  u8 *timer;

  collision = 0;
  count = 0;
  object->unk66 = collision;
  object->timer = 1;
  object->speedLimit = 0x8000;
  object->acceleration = 0x4000;
  if (gInput.repeat & 0x200)
    object->speedLimit = 0x40000;

  angle = gData_0802ec5c[(gInput.held >> 4) & 15];
  direction = angle;
  if (direction == 0xffff) {
    collision |= 4;
  } else {
    position.x = object->x;
    position.y = object->y;
    position.z = object->z;
    Vector_AddPolarOffset(0x80000, direction, &position);
    if (gDebugMode == 0 || (gInput.held & 0x200) == 0) {
      leftPosition.x = object->x;
      leftPosition.y = object->y;
      leftPosition.z = object->z;
      Vector_AddPolarOffset(0x80000, direction + 0x1555, &leftPosition);
      rightPosition.x = object->x;
      /* FAKEMATCH(temporary): y read into its own local before the block; as
       * object->y read in the block, 2 of 470 instructions differ in tla-en. */
      y = object->y;
      // The right probe's y stored in a once-only block; as the plain store,
      // 50 of 470 instructions differ in tla-en.
      do {
        rightPosition.y = y;
      } while (0);
      rightPosition.z = object->z;
      Vector_AddPolarOffset(0x80000, direction - 0x1555, &rightPosition);
      if ((Func_0802db64(object, &position) |
           Func_0802db64(object, &leftPosition) |
           Func_0802db64(object, &rightPosition)) != 0) {
        angleOffsets[count++] = direction + 0x1000;
        angleOffsets[count++] = direction - 0x1000;
        angleOffsets[count++] = direction + 0x2000;
        angleOffsets[count++] = direction - 0x2000;
        angleOffsets[count++] = direction + 0x3000;
        angleOffsets[count++] = direction - 0x3000;
        for (angleIndex = 0; angleIndex < count; angleIndex++) {
          direction = angleOffsets[angleIndex];
          position.x = object->x;
          position.y = object->y;
          position.z = object->z;
          Vector_AddPolarOffset(0x80000, direction, &position);
          leftPosition.x = object->x;
          leftPosition.y = object->y;
          leftPosition.z = object->z;
          Vector_AddPolarOffset(0x80000, direction + 0x1555, &leftPosition);
          rightPosition.x = object->x;
          rightPosition.y = object->y;
          rightPosition.z = object->z;
          Vector_AddPolarOffset(0x80000, direction - 0x1555, &rightPosition);
          if ((Func_0802db64(object, &position) |
               Func_0802db64(object, &leftPosition) |
               Func_0802db64(object, &rightPosition)) != 0)
            continue;
          angle = (s16)direction;
          goto update_object;
        }
        collision |= 1;
      }
    }
  }

update_object:
  work = Ram_HeapSlots->eventWork;
  if (work != NULL) {
    if ((collision & 3) != 0)
      work->blockedSteps++;
    else
      work->blockedSteps = 0;
    work->heldKeys = gInput.held;
  }
  Actor_SelectAnimation(object, 1);

  if (collision != 0) {
    object->targetX = 0x80000000;
    object->targetY = 0x80000000;
    object->targetZ = 0x80000000;
    object->velocityX = 0;
    object->velocityZ = 0;
    if ((collision & 3) != 0) {
      direction = angle;
      difference = (s16)(direction - object->facing);
      if (difference > 0x1000)
        difference = 0x1000;
      if (difference < -0x1000)
        difference = -0x1000;
      object->facing += difference;
    }
    timer = (u8 *)object + 98;
    *timer = 0;
    object->unk66 = 2;
  } else {
    Actor_SetMoveTarget(object, position.x, position.y, position.z);
    speed = FixedSqrt(Iwram_MulQ16(object->velocityX, object->velocityX) +
                      Iwram_MulQ16(object->velocityZ, object->velocityZ));
    object->velocityX = collision;
    object->velocityZ = collision;
    Vector_AddPolarOffset(speed, (u16)angle,
                          (struct FieldPosition *)&object->velocityX);
    timer = (u8 *)object + 98;
    if (*timer != 0)
      (*timer)--;
  }

  camera = (struct WorldCamera *)Ram_HeapSlots->mapWork;
  targetCameraYaw = gData_0802eca0[(gInput.held >> 4) & 15];
  yawStep = (s16)(targetCameraYaw - camera->yaw) / 8;
  if (yawStep > 0x200)
    yawStep = 0x200;
  if (yawStep < -0x200)
    yawStep = -0x200;
  if (yawStep > -16 && yawStep < 16)
    yawStep = targetCameraYaw - camera->yaw;
  camera->yaw += yawStep;

  if (object->animationKind == 1) {
    playerSprite = object->sprite;
    terrainCollision = GetWorldMapCollision((struct WorldPosition *)&object->x);
    if (terrainCollision == 9)
      playerSprite->entry[6] = 1;
    else
      playerSprite->entry[6] = 9;
  }
  if (*timer == 0 && collision == 0) {
    ripple = Func_08023220(0x164, object->x, object->y, object->z - 0x20000);
    if (ripple != 0) {
      ripple->terrainHeight = object->terrainHeight;
      rippleSprite = ripple->sprite;
      Actor_StartScript(ripple, gData_0802ece0);
      ripple->priorityFlags = 8;
      ((struct Unk0802a124Actor *)ripple)->unk63 = 100;
      ripple->motionFlags = collision;
      if (rippleSprite != 0) {
        Animation_ApplyChildArgument(rippleSprite, 1);
        rippleSprite->flags = collision;
        rippleSprite->priority = 2;
        rippleSprite->priority = 1;
      }
      *timer = 5;
    }
  }
  Func_08029838(object);
  object->step++;
  return 1;
}


void SpriteColor_RemapPixels(u8 *buf, s32 cnt) {
  s32 n;
  u8 *p;
  u8 *table;

  p = buf;
  table = gSpriteColorRemapTables;
  n = cnt - 1;
  if (n != -1) {
    do {
      u32 index = *p;

      n -= 1;
      *p = table[index];
      p += 1;
    } while (n != -1);
  }
}

// Map: unpack decoded block data into gMapBlocks.

extern u32 gMapBlocks[];

/// Decoded buffer [1] selects the method; halfword data starts at [2].
/// size: length including the method byte (even means no data).
/// Method 0: copy unchanged.
/// Method 1: high bytes followed by low bytes; reconstruct by XOR with prior
/// values. Method 2: reconstruct by XOR with prior values.
void Tilemap_DecodeStagedBuffer(s32 size) {
  u16 *dst;
  u16 *src;
  s32 n;
  s32 i;

  src = (u16 *)(Ram_MapCellBuffer + 2);
  dst = (u16 *)gMapBlocks;
  n = (size - 1) / 2;
  if (size & 1) {
    switch (((u8 *)gMapCellBuffer)[1]) {
    case 0:
      for (i = 0; i < n; i++)
        *dst++ = *src++;
      break;
    case 1: {
      u32 prev = 0;
      u8 *hi = Ram_MapCellBuffer + 2;
      u8 *lo = hi + n;

      for (i = 0; i < n; i++) {
        u32 v = ((*hi++ << 8) | *lo++) ^ prev;
        *dst++ = v;
        prev = v;
      }
      break;
    }
    case 2: {
      s32 prev = 0;

      for (i = 0; i < n; i++) {
        s32 v = *src++ ^ prev;
        *dst++ = v;
        prev = v;
      }
      break;
    }
    }
  }
}

typedef void (*ConvertFn)(void *destination, void *source, void *saved);

extern u8 Render_ConvertMapCode[];
extern u8 Render_ConvertMapCodeSize[];

static inline s32 CopyWords(void *destination, const void *source, s32 size) {
  /* FAKEMATCH(wrapper): the copy called through this inline, so the map cell
   * buffer's address is loaded again for the conversion; as Iwram_CopyWords
   * called directly, which keeps the address in a saved register, 20 of 40
   * instructions of Tilemap_ConvertBuffer differ in tla-en. */
  return Iwram_CopyWordsEntry(destination, source, size);
}

/// Converts the map cell buffer's 32 KB in place with the ARM routine copied
/// to RAM, keeping a copy of the buffer for it to read.
void Tilemap_ConvertBuffer(void) {
  u8 *saved;
  ConvertFn routine;
  u32 size;

  size = 0x8000;
  saved = Heap_AllocEwram(size);
  CopyWords(saved, Ram_MapCellBuffer, size);
  RamCode_Load(routine, Render_ConvertMapCode, Render_ConvertMapCodeSize);
  routine(((u8 *)gMapCellBuffer + 0x8000), Ram_MapCellBuffer, saved);
  Heap_Release(routine);
  Heap_Release(saved);
}


extern const u16 gData_0802f380[][6];
extern char ResourceId_Map000;

/// Decodes the palette resource of a map's header row (its second halfword)
/// and copies its 224 colors to destination, keeping the current backdrop
/// color in the first entry.
void Func_0802a650(s32 map, void *destination) {
  const u16 *entry;
  u16 *buffer;
  s16 backdrop;

  entry = gData_0802f380[map];
  buffer = Heap_Alloc(0x200);
  backdrop = *(s16 *)0x05000000;
  Resource_DecodeType01(
      Resource_GetTableEntry(entry[1] + (u32)&ResourceId_Map000), buffer);
  buffer[0] = backdrop;
  Dma_Set(buffer, destination, DMA_COPY_WORDS(112), REG_DMA3);
  Heap_Release(buffer);
}

// ---- games/tla/src/field/map/f02a6b8.c ----
// Loads a layered map, the twin of The Broken Seal's MapGrid_LoadMap: from
// layout index's entry in gData_0802f380 it decodes the blocks, the collision
// (filled with -1 first, which The Broken Seal does not do), the cells, the
// shape grid, the tile animations, the blend script and the script at
// 0x0202e000; sets up the three layers from the header, with their cell
// positions and shape pointers; loads the palette and tiles unless flag 0x170
// says they are already loaded; and starts Func_0802ad84.

#include "io_write.h"

struct LayeredSceneEntry {
  u16 resources[6];
};

struct LayeredSceneLayer {
  u8 x;
  u8 y;
  s8 scaleX;
  s8 scaleY;
  s8 speedX;
  s8 speedY;
  u8 periodX;
  u8 periodY;
};

struct LayeredSceneHeader {
  u8 origin[4];
  u8 priority[3];
  u8 charBase[3];
  u16 unk0a;
  struct LayeredSceneLayer layers[3];
  s32 tiles;
  s32 collision;
  s32 tilemap;
  s32 shapes;
  s32 animation;
  s32 blend;
  s32 script;
};


struct MapLayerScroll {
  s32 x, y;
  s32 offsetX, offsetY;
  s32 scaleX, scaleY;
  s32 speedX, speedY;
  s32 phaseX, phaseY;
  u16 maskX, maskY;
  u16 cellX, cellY;
  struct MapCell *cells;
  u8 *shapes;
};

struct MapScrollWork {
  u8 unk000[0x14];
  u16 flags;
  u8 unk016[0xe4 - 0x16];
  s32 viewX, viewY;
  s32 minX, minY, maxX, maxY;
  u8 unk0fc[5];
  u8 unk101;
  u8 unk102[2];
  u8 enabled[3];
  u8 unk107;
  struct MapLayerScroll layers[3];
};

extern char ResourceId_Map000;
extern u8 gMapShapeGrid[];
extern u8 gData_02028000[];

void Fade_FromBlack(s32);
void Tilemap_DecodeStagedBuffer(s32 size);
void Tilemap_ConvertBuffer(void);
void Func_0802cc9c(const u16 *);
void DisplayBlend_StartScript(u16 *);
s32 Resource_DecodeType2(const void *source, void *destination);
s32 GameFlag_Test(s32 flag);
void GameFlag_ClearBit(s32 flag);
void Func_0802ad84(void);

s32 Func_0802a6b8(s32 index) {
  struct LayeredSceneEntry *sceneResources;
  struct MapScrollWork *work;
  struct LayeredSceneHeader *sceneHeader;
  struct MapLayerScroll *mapLayer;
  struct LayeredSceneLayer *layerSource;
  s32 *viewX;
  s32 *viewY;
  s32 layerIndex;
  u8 *graphicsBuffer;
  s32 backdrop;
  u8 *encodedData;
  s32 displayFlags;

  REG_DISPCNT &= ~(DISPCNT_BG1_ON | DISPCNT_BG2_ON | DISPCNT_BG3_ON |
                   DISPCNT_OBJ_ON | DISPCNT_WIN0_ON);
  Fade_FromBlack(0);
  sceneResources = &gData_0802f380[index];
  work = HeapSlot_AllocEwram(32, sizeof(struct MapScrollWork));
  Iwram_ClearWords(work, sizeof(struct MapScrollWork));
  sceneHeader = (struct LayeredSceneHeader *)Resource_GetTableEntry(
      sceneResources->resources[0] + (u32)&ResourceId_Map000);
  encodedData = (u8 *)sceneHeader + sceneHeader->tiles;
  Tilemap_DecodeStagedBuffer(
      Resource_DecodeType01(encodedData, Ram_MapCellBuffer + 1));
  Dma_Fill32(-1, Ram_MapCollision, 0x2000);
  encodedData = (u8 *)sceneHeader + sceneHeader->collision;
  Resource_DecodeType01(encodedData, Ram_MapCollision);
  encodedData = (u8 *)sceneHeader + sceneHeader->tilemap;
  Resource_DecodeType01(encodedData, Ram_MapCellBuffer);
  Tilemap_ConvertBuffer();
  encodedData = (u8 *)sceneHeader + sceneHeader->shapes;
  Resource_DecodeType01(encodedData, gMapShapeGrid);
  if (sceneHeader->animation != 0) {
    encodedData = (u8 *)sceneHeader + sceneHeader->animation;
    Resource_DecodeType01(encodedData, Ram_MapCollision + 0x1000);
    Func_0802cc9c((const u16 *)(Ram_MapCollision + 0x1000));
  }
  if (sceneHeader->blend != 0) {
    encodedData = (u8 *)sceneHeader + sceneHeader->blend;
    Resource_DecodeType01(encodedData, Ram_MapCollision + 0x1e00);
    DisplayBlend_StartScript((u16 *)(Ram_MapCollision + 0x1e00));
  }
  encodedData = (u8 *)sceneHeader + sceneHeader->script;
  Resource_DecodeType01(encodedData, Ram_MapCollision + 0x2000);
  work->minX = sceneHeader->origin[0] << 19;
  work->minY = sceneHeader->origin[1] << 19;
  work->maxX = sceneHeader->origin[2] << 19;
  work->maxY = sceneHeader->origin[3] << 19;
  viewX = &work->viewX;
  *viewX = 0;
  viewY = &work->viewY;
  *viewY = 0;
  work->enabled[0] = sceneHeader->priority[0];
  work->enabled[1] = sceneHeader->priority[1];
  work->enabled[2] = sceneHeader->priority[2];
  work->unk101 = 2;
  if (work->maxX == 0)
    work->maxX = 0x8000000;
  if (work->maxY == 0)
    work->maxY = 0x8000000;
  mapLayer = work->layers;
  layerSource = sceneHeader->layers;
  for (layerIndex = 0; layerIndex < 3; layerIndex++) {
    u32 x = layerSource->x;
    u32 y = layerSource->y;
    s32 scaleX;
    s32 cell;
    // Set inside the loop, where the original's register allocation needs it.
    u8 *shapeGrid;
    u32 maskY;

    mapLayer->offsetX = x << 19;
    mapLayer->offsetY = y << 19;
    mapLayer->scaleY = layerSource->scaleY * 4096;
    mapLayer->speedX = layerSource->speedX * 4096;
    mapLayer->speedY = layerSource->speedY * 4096;
    mapLayer->maskX = layerSource->periodX & 127;
    // Read before the phase stores, which the original makes after it.
    maskY = layerSource->periodY & 127;
    mapLayer->phaseX = 0;
    mapLayer->phaseY = 0;
    mapLayer->maskY = maskY << 7;
    x >>= 1;
    y = (y >> 1) * 128;
    mapLayer->cellY = y;
    scaleX = layerSource->scaleX * 4096;
    cell = y + x;
    shapeGrid = gMapShapeGrid;
    mapLayer->cells = (struct MapCell *)Ram_MapCellBuffer + cell;
    mapLayer->scaleX = scaleX;
    mapLayer->cellX = x;
    mapLayer->shapes = shapeGrid + cell;
    mapLayer->x = Iwram_MulQ16(*viewX, scaleX) + mapLayer->offsetX;
    mapLayer->y = Iwram_MulQ16(*viewY, mapLayer->scaleY) + mapLayer->offsetY;
    layerSource++;
    mapLayer++;
  }
  work->flags = DISPCNT_OBJ_ON;
  if (work->enabled[0] != 0)
    work->flags = DISPCNT_OBJ_ON | DISPCNT_BG3_ON;
  if (work->enabled[1] != 0)
    work->flags |= DISPCNT_BG2_ON;
  if (work->enabled[2] != 0)
    work->flags |= DISPCNT_BG1_ON;
  Io_Write16(work->enabled[0] | BGCNT_CHAR_BASE(sceneHeader->charBase[0]) |
                 BGCNT_SCREEN_BASE(5),
             &REG_BG3CNT);
  Io_Write16(work->enabled[1] | BGCNT_CHAR_BASE(sceneHeader->charBase[1]) |
                 BGCNT_SCREEN_BASE(6),
             &REG_BG2CNT);
  Io_Write16(work->enabled[2] | BGCNT_CHAR_BASE(sceneHeader->charBase[2]) |
                 BGCNT_SCREEN_BASE(7),
             &REG_BG1CNT);
  if (GameFlag_Test(0x170) != 0) {
    GameFlag_ClearBit(0x170);
  } else {
    graphicsBuffer = Heap_Alloc(0x4000);
    if (graphicsBuffer != NULL) {
      backdrop = *(s16 *)BG_PLTT;
      Resource_DecodeType01(
          Resource_GetTableEntry(sceneResources->resources[1] +
                                 (u32)&ResourceId_Map000),
          graphicsBuffer);
      *(s16 *)graphicsBuffer = backdrop;
      Iwram_CopyWords((void *)BG_PLTT, graphicsBuffer, 0x1c0);
      Resource_DecodeType2(Resource_GetTableEntry(sceneResources->resources[2] +
                                                  (u32)&ResourceId_Map000),
                           graphicsBuffer);
      Iwram_CopyWords(BG_CHAR_BLOCK(1), graphicsBuffer, 0x4000);
      Resource_DecodeType2(Resource_GetTableEntry(sceneResources->resources[3] +
                                                  (u32)&ResourceId_Map000),
                           graphicsBuffer);
      Iwram_CopyWords(BG_CHAR_BLOCK(2), graphicsBuffer, 0x4000);
      Resource_DecodeType2(Resource_GetTableEntry(sceneResources->resources[4] +
                                                  (u32)&ResourceId_Map000),
                           graphicsBuffer);
      Iwram_CopyWords(BG_CHAR_BLOCK(3), graphicsBuffer, 0x4000);
      Resource_DecodeType2(Resource_GetTableEntry(sceneResources->resources[5] +
                                                  (u32)&ResourceId_Map000),
                           gData_02028000);
      Heap_Release(graphicsBuffer);
    }
  }
  displayFlags = 0;
  REG_MOSAIC = displayFlags;
  REG_BLDCNT = displayFlags;
  displayFlags = DISPCNT_BG0_ON | DISPCNT_OBJ_1D_MAP;
  // A once-only block: the original builds the task order after this store.
  do {
    REG_DISPCNT = displayFlags;
  } while (0);
  Task_Add(Func_0802ad84, 0x485);
  return 2;
}

// ---- games/tla/src/field/map/f02aa74.c ----
#define ABS(v) ((v) < 0 ? -(v) : (v))

void Func_0802af9c(s32 x, s32 y);
void Func_0802ad84(void);

void Func_0802aa74(void) {
  struct MapWork *work = Ram_HeapSlots->mapWork;
  s32 x = 0;
  s32 height = 0;
  s32 z = 0;
  s32 *followedPosition = work->origin;

  if (followedPosition != NULL) {
    s16 *rectList;
    s16 *rect;

    x = *followedPosition++;
    height = *followedPosition++;
    z = *followedPosition++;
    rectList = *(s16 **)((u8 *)work + 0xfc);
    if (rectList != NULL) {
      for (rect = rectList; rect[0] != -1; rect += 4) {
        s32 left = (rect[0] << 16) - 0x780000;
        s32 top = (rect[1] << 16) + height - 0x400000;
        s32 right = (rect[2] << 16) + 0x780000;
        s32 bottom = (rect[3] << 16) + height + 0x600000;

        if (x >= left && z >= top && x <= right && z <= bottom) {
          s32 nearX = left;
          s32 nearZ;

          if (ABS(nearX - x) > ABS(right - x))
            nearX = right;
          nearZ = top;
          if (ABS(nearZ - z) > ABS(bottom - z))
            nearZ = bottom;
          if (ABS(nearX - x) <= ABS(nearZ - z))
            x = nearX;
          else
            z = nearZ;
          break;
        }
      }
      followedPosition[-3] = x;
      followedPosition[-1] = z;
    }
  }
  Func_0802af9c(x, z - height);
  Func_0802ad84();
}

/// A map layer as its renderers read it: its scroll, then the window over
/// the map's cells (masks and origins that wrap the source cells).
struct MapLayerWindow {
  s32 x, y;
  s32 offsetX, offsetY;
  s32 scaleX, scaleY;
  s32 speedX, speedY;
  s32 phaseX, phaseY;
  u16 maskX, maskY;
  u16 originX, originY;
  struct MapCell *cells;
  u8 *terrainIds;
};

/// Draws a row of 16 cell pictures (32 tiles) of a layer's cells into its BG
/// map (screen block 5 + layer), starting at tile (tileX, tileY); the BG map
/// wraps every 32 tiles and the cells wrap at the layer's window. The Broken
/// Seal's MapGrid_RenderCellPictureRow, with the window. Called by
/// Func_0802ad84 when a layer scrolls onto a new row of cell pictures.
void Func_0802abbc(u32 layer, s32 tileX, s32 tileY) {
  u32 *cells = (u32 *)Ram_MapCellBuffer;
  struct MapWork *map = Ram_HeapSlots->mapWork;
  struct MapLayerWindow *window = (struct MapLayerWindow *)&map->layers[layer];
  u32 originY = window->originY;
  u8 *screenMap = (u8 *)0x06002800 + (layer << 11);
  u32 maskY = window->maskY;
  u32 sourceRow = ((tileY / 2) & 0x7f) << 7;
  u32 screenRowOffset = (tileY & 30) << 5;
  u32 sourceColumn;
  u32 screenColumnOffset;
  u32 maskX;
  u32 originX;
  u32 cellPictureIndex;

  // A window narrower than the 128 by 128 cells starts at its origin.
  if (maskY != 0x3f80) {
    sourceRow = (sourceRow - originY) & maskY;
    cells += originY;
  }
  maskX = window->maskX;
  sourceColumn = (tileX / 2) & 0x7f;
  originX = window->originX;
  screenColumnOffset = tileX & 30;
  sourceColumn &= maskX;
  if (maskX != 0x7f) {
    sourceColumn = (sourceColumn - originX) & maskX;
    cells += originX;
  }
  for (cellPictureIndex = 0; cellPictureIndex <= 15; cellPictureIndex++) {
    u32 *sourceCell;
    u32 *cellPictureRows;
    u8 *screenTile;
    u32 cellPictureWordOffset;

    sourceCell = cells + sourceRow + sourceColumn;
    cellPictureWordOffset = (*sourceCell << 21) >> 20;
    cellPictureRows = (u32 *)Ram_MapBlocks;
    cellPictureRows += cellPictureWordOffset;
    screenTile = screenMap + (screenRowOffset + screenColumnOffset) * 2;
    *(u32 *)screenTile = *cellPictureRows;
    cellPictureRows = (u32 *)(Ram_MapBlocks + 4);
    cellPictureRows += cellPictureWordOffset;
    sourceColumn++;
    *(u32 *)(screenTile + 64) = *cellPictureRows;
    sourceColumn &= maskX;
    screenColumnOffset = (screenColumnOffset + 2) & 30;
  }
}

static __inline__ void CopyCellPictureHalfColumn(u32 *cell, u8 *screenMap,
                                                 u32 screenRowOffset,
                                                 u32 screenColumnOffset,
                                                 u32 tileHalf) {
  u16 *cellPictureEntry;
  u8 *screenTile;
  u32 index = ((*cell << 21) >> 19) + tileHalf;

  cellPictureEntry = (u16 *)Ram_MapBlocks;
  cellPictureEntry += index;
  screenTile =
      screenMap + (screenRowOffset + screenColumnOffset + tileHalf) * 2;
  *(u16 *)screenTile = *cellPictureEntry;
  cellPictureEntry = (u16 *)(Ram_MapBlocks + 4);
  cellPictureEntry += index;
  *(u16 *)(screenTile + 64) = *cellPictureEntry;
}

void Func_0802ac90(u32 layer, s32 tileX, s32 tileY) {
  u32 *cells = (u32 *)Ram_MapCellBuffer;
  struct MapWork *map = Ram_HeapSlots->mapWork;
  struct MapLayerWindow *window = (struct MapLayerWindow *)&map->layers[layer];
  u8 *screenMap = (u8 *)0x06002800 + (layer << 11);
  u32 maskY = window->maskY;
  u32 originY = window->originY;
  u32 sourceRow = ((tileY / 2) & 0x7f) << 7;
  u32 screenRowOffset = (tileY & 30) << 5;
  u32 sourceColumn;
  u32 screenColumnOffset;
  u32 maskX;
  u32 originX;
  u32 tileHalf;
  u32 cellPictureIndex;

  if (maskY != 0x3f80) {
    sourceRow = (sourceRow - originY) & maskY;
    cells = (u32 *)Ram_MapCellBuffer + originY;
  }
  maskX = window->maskX;
  originX = window->originX;
  sourceColumn = (tileX / 2) & 0x7f;
  screenColumnOffset = tileX & 30;
  sourceColumn &= maskX;
  if (maskX != 0x7f) {
    sourceColumn = (sourceColumn - originX) & maskX;
    cells += originX;
  }
  tileHalf = tileX & 1;
  for (cellPictureIndex = 0; cellPictureIndex <= 10; cellPictureIndex++) {
    CopyCellPictureHalfColumn(cells + sourceRow + sourceColumn, screenMap,
                              screenRowOffset, screenColumnOffset, tileHalf);
    sourceRow = (sourceRow + 128) & maskY;
    screenRowOffset = (screenRowOffset + 64) & 0x3c0;
  }
}

#include "bg_scroll.h"

u32 Random16(void);


void Func_0802ad84(void) {
  struct MapWork *work;
  struct MapLayerScroll *layer;
  s32 *origin;
  s32 x, y;
  s32 minX, minY, maxX, maxY;
  s32 baseX, baseY;
  u32 layerIndex;

  work = Ram_HeapSlots->mapWork;
  origin = work->origin;
  layer = (struct MapLayerScroll *)work->layers;
  if (origin == NULL)
    return;
  baseX = *origin++ - 0x780000;
  {
    s32 height = *origin++;

    baseY = *origin - height - 0x600000;
  }
  minX = work->minX + work->shakeX;
  maxX = work->maxX - work->shakeX - 0xf00000;
  minY = work->minZ + work->shakeY;
  maxY = work->maxZ - work->shakeY - 0xa00000;
  if (minX > maxX)
    maxX = minX;
  if (minY > maxY)
    maxY = minY;
  if (baseX < minX)
    baseX = minX;
  if (baseX > maxX)
    baseX = maxX;
  if (baseY < minY)
    baseY = minY;
  if (baseY > maxY)
    baseY = maxY;
  if (work->shakeX != 0) {
    s32 shakeRoll = Random16();

    shakeRoll -= Random16();
    baseX += Iwram_MulQ16(work->shakeX, shakeRoll);
    work->shakeX = Iwram_MulQ16(work->shakeX, work->shakeDecay);
  }
  if (work->shakeY != 0) {
    s32 shakeRoll = Random16();

    shakeRoll -= Random16();
    baseY += Iwram_MulQ16(work->shakeY, shakeRoll);
    work->shakeY = Iwram_MulQ16(work->shakeY, work->shakeDecay);
  }
  {
    s32 step = work->viewZ - baseY;

    if (step > 0x100000) {
      baseY = work->viewZ - 0x100000;
      step = work->viewZ - baseY;
    }
    if (step < -0x100000)
      baseY = work->viewZ + 0x100000;
  }
  work->viewX = baseX;
  work->viewZ = baseY;
  for (layerIndex = 0; layerIndex <= 2; layerIndex++, layer++) {
    baseX = Iwram_MulQ16(work->viewX, layer->scaleX);
    baseY = Iwram_MulQ16(work->viewZ, layer->scaleY);
    if (layer->speedX != 0) {
      layer->phaseX += layer->speedX;
      baseX += layer->phaseX;
    }
    if (layer->speedY != 0) {
      layer->phaseY += layer->speedY;
      baseY += layer->phaseY;
    }
    baseX += layer->offsetX;
    baseY += layer->offsetY;
    x = (u32)baseX >> 19;
    y = (u32)baseY >> 19;
    if ((layer->x ^ baseX) & 0x80000) {
      if (layer->x < baseX)
        Func_0802ac90(layerIndex, x + 30, y);
      else
        Func_0802ac90(layerIndex, x, y);
    }
    if ((layer->y ^ baseY) & 0x100000) {
      if (layer->y < baseY)
        Func_0802abbc(layerIndex, x, y + 20);
      else
        Func_0802abbc(layerIndex, x, y);
    }
    layer->x = baseX;
    layer->y = baseY;
    gBgScroll[3 - layerIndex].x = baseX >> 16;
    gBgScroll[3 - layerIndex].y = baseY >> 16;
  }
}

static __inline__ void CopyCellPictureToScreen(u32 *cell, u8 *screenMap,
                                               u32 screenRowOffset,
                                               u32 screenColumnOffset) {
  u32 *cellPictureRows;
  u8 *screenTile;
  u32 index = (*cell << 21) >> 20;

  cellPictureRows = (u32 *)Ram_MapBlocks;
  cellPictureRows += index;
  screenTile = screenMap + screenRowOffset * 2 + screenColumnOffset * 2;
  *(u32 *)screenTile = *cellPictureRows;
  cellPictureRows = (u32 *)(Ram_MapBlocks + 4);
  cellPictureRows += index;
  *(u32 *)(screenTile + 64) = *cellPictureRows;
}

void Func_0802af9c(s32 x, s32 y) {
  u32 layerIndex;
  u8 *screenMap;
  u32 *cells;
  struct MapWork *work = Ram_HeapSlots->mapWork;
  struct MapLayerWindow *layer = (struct MapLayerWindow *)work->layers;
  u32 originX;

  x -= 0x780000;
  y -= 0x600000;
  if (x < work->minX)
    x = work->minX;
  if (x > work->maxX - 0xf00000)
    x = work->maxX - 0xf00000;
  if (y < work->minZ)
    y = work->minZ;
  if (y > work->maxZ - 0xa00000)
    y = work->maxZ - 0xa00000;
  work->viewX = x;
  work->viewZ = y;
  for (layerIndex = 0; layerIndex <= 2; layerIndex++) {
    if (work->unk0fc[layerIndex + 8] != 0) {
      u32 maskX, maskY, originY;
      u32 sourceRow, sourceColumn, screenRowOffset, screenColumnOffset,
          cellPictureRow, cellPictureColumn;

      x = Iwram_MulQ16(work->viewX, layer->scaleX);
      y = Iwram_MulQ16(work->viewZ, layer->scaleY);
      if (layer->speedX != 0) {
        layer->phaseX += layer->speedX;
        x += layer->phaseX;
      }
      if (layer->speedY != 0) {
        layer->phaseY += layer->speedY;
        y += layer->phaseY;
      }
      x += layer->offsetX;
      y += layer->offsetY;
      x /= 0x80000;
      y /= 0x80000;
      screenMap = (u8 *)0x06002800 + (layerIndex << 11);
      maskY = layer->maskY;
      maskX = layer->maskX;
      originY = layer->originY;
      originX = layer->originX;
      cells = (u32 *)Ram_MapCellBuffer;
      if (maskX != 0x7f)
        cells = (u32 *)Ram_MapCellBuffer + originX;
      if (maskY != 0x3f80)
        cells += originY;
      sourceRow = ((y / 2) & 0x7f) << 7;
      screenRowOffset = (y & 30) << 5;
      if (maskY != 0x3f80)
        sourceRow = (sourceRow - originY) & maskY;
      for (cellPictureRow = 0; cellPictureRow <= 10; cellPictureRow++) {
        sourceColumn = (x / 2) & 0x7f;
        screenColumnOffset = x & 30;
        if (maskX != 0x7f)
          sourceColumn = (sourceColumn - originX) & maskX;
        for (cellPictureColumn = 0; cellPictureColumn <= 15;
             cellPictureColumn++) {
          CopyCellPictureToScreen(cells + sourceRow + sourceColumn, screenMap,
                                  screenRowOffset, screenColumnOffset);
          sourceColumn = (sourceColumn + 1) & maskX;
          screenColumnOffset = (screenColumnOffset + 2) & 30;
        }
        sourceRow = (sourceRow + 128) & maskY;
        screenRowOffset = (screenRowOffset + 64) & 0x3c0;
      }
      layer++;
    }
  }
}


struct TilePos {
  s32 x, y;
};

void Map_CopyCellPictureIndicesRect(s32 srcX, s32 srcY, u32 dstX, u32 dstY,
                                    u32 width, u32 height) {
  u32 *src = (u32 *)Ram_MapCellBuffer + (srcY * 128 + srcX);
  u32 *dst = (u32 *)Ram_MapCellBuffer + (dstY * 128 + dstX);
  struct MapLayer *layer = Ram_HeapSlots->mapWork->layers;
  struct TilePos tile[3];
  s32 i;
  struct TilePos *pos = tile;
  u32 y;
  u32 x;

  // The three layers' visible areas: their scroll positions in cells.
  for (i = 2; i >= 0; i--) {
    pos->x = layer->x >> 20;
    pos->y = layer->y >> 20;
    pos++;
    layer++;
  }
  for (y = dstY; y < dstY + height; y++) {
    for (x = dstX; x < dstX + width; x++) {
      u32 val = *src++;
      u32 cell = val & 0xfff;
      u32 column, row, mapCell, offset, bytes;

      *dst = (*dst & 0xfffff000) | cell;
      column = x & 15;
      dst++;
      i = 0;
      row = y & 15;
      pos = tile;
      // offset is the cell's place in a 32x32 BG map: 128 bytes a cell row,
      // 4 a cell.
      mapCell = row << 5;
      mapCell += column;
      offset = mapCell * 4;
      // The first layer whose visible area (16x12 cells) holds the cell has
      // the cell picture written to its BG map too, and only that one (a map
      // lays its layers out in different parts of the grid); the layers' BG
      // maps follow each other, 2,048 bytes apart, from screen block 5.
      for (; i < 3; i++) {
        if (pos->x <= (s32)x && pos->x + 16 > (s32)x && pos->y <= (s32)y &&
            pos->y + 12 > (s32)y) {
          u32 screenAddress, cellPictureAddress;
          u8 *screenTile, *cellPictureRows;

          // The cell picture's (8 bytes) top two tiles, and its bottom two
          // 64 bytes further on.
          screenAddress = (u32)BG_SCREEN_BLOCK(5);
          screenTile = (u8 *)screenAddress;
          screenTile += offset;
          bytes = cell * 8;
          cellPictureAddress = (u32)Ram_MapBlocks;
          cellPictureRows = (u8 *)cellPictureAddress;
          cellPictureRows += bytes;
          *(u32 *)screenTile = *(u32 *)cellPictureRows;

          cellPictureAddress = (u32)Ram_MapBlocks + 4;
          cellPictureRows = (u8 *)cellPictureAddress;
          cellPictureRows += bytes;
          screenAddress = (u32)BG_SCREEN_BLOCK(5) + 64;
          screenTile = (u8 *)screenAddress;
          screenTile += offset;
          *(u32 *)screenTile = *(u32 *)cellPictureRows;
          break;
        }
        // bytes, reused for the step to the next layer's BG map.
        bytes = 2048;
        offset += bytes;
        pos++;
      }
    }
    src += 128 - width;
    dst += 128 - width;
  }
}

void wait(s32 frames);

struct MapCopyCommand {
  u16 source;
  s16 y, width, height, delay;
} __attribute__((packed, aligned(2)));

void Map_PlayMapEdit(u16 *steps, s32 destinationX, u32 destinationY) {
  struct MapCopyCommand *copyCommand = (struct MapCopyCommand *)steps;
  u32 sourceX = copyCommand->source;
  u32 endMarker;
  u32 firstSourceCopy = sourceX;

  // endMarker set after the copy, which puts the ip copy among its
  // instructions; as an initializer, 2 of 53 instructions differ in tla-en.
  endMarker = 0xffff;
  /* FAKEMATCH(dead): the updates that cancel out emit nothing but end CSE
   * before the comparison, keeping its ip copy while source stays in r0;
   * without them, 5 of 53 instructions differ in tla-en. */
  if (copyCommand) {
    sourceX ^= firstSourceCopy;
    sourceX ^= firstSourceCopy;
  }
  if (firstSourceCopy != endMarker) {
    u32 endMarkerCopy = endMarker;
    s16 *rectangleArgs = (s16 *)(steps + 1);

    while (1) {
      s32 sourceY = rectangleArgs[0];
      s32 width = rectangleArgs[1];
      s32 height = rectangleArgs[2];
      s32 delay = rectangleArgs[3];
      rectangleArgs += 5;

      Map_CopyCellPictureIndicesRect(sourceX, (u16)sourceY, destinationX,
                                     destinationY, (u16)width, (u16)height);
      copyCommand++;
      wait((u16)delay);
      sourceX = copyCommand->source;
      /* FAKEMATCH(dead): updates that cancel out between the load and its
       * copy, so CSE does not swap the load into the copy and source stays in
       * r0; without them, 8 of 53 instructions differ in tla-en. */
      if (copyCommand) {
        sourceX++;
        sourceX--;
      }
      {
        u32 nextSourceCopy = sourceX;

        /* FAKEMATCH(dead): updates that cancel out after the copy end CSE's
         * knowledge of source, so the comparison keeps its ip copy; without
         * them, 7 of 53 instructions differ in tla-en. */
        if (copyCommand) {
          sourceX++;
          sourceX--;
        }
        if (nextSourceCopy == endMarkerCopy)
          break;
      }
    }
  }
}

// Map: copy a cell rectangle.

#include "mapcopy.h"

/// Copies a rectangle's cells, including block indices and attributes.
/// srcX, srcY: source top left, in cells.
/// width, height: size in cells.
/// dstX, dstY: destination top left, in cells.
void Map_CopyCellsRect(s32 srcX, s32 srcY, s32 width, s32 height, s32 dstX,
                       s32 dstY) {
  Map_CopyCellPictureIndicesRect(srcX, srcY, dstX, dstY, width, height);
  Map_CopyCellAttributeRect(srcX, srcY, width, height, dstX, dstY);
}

extern u8 gMapShapeGrid[];

#define MAP_CELL_TILE_MASK 0x00000fff
#define MAP_CELL_ATTRIBUTE_MASK 0xfffff000

void Map_CopyCellAttributeRect(s32 srcX, s32 srcY, s32 width, s32 height,
                               s32 dstX, s32 dstY) {
  u32 *cells = (u32 *)Ram_MapCellBuffer;
  s32 dst = (dstY << 7) + dstX;
  u8 *dstShapes = gMapShapeGrid + dst;
  s32 src = (srcY << 7) + srcX;
  u8 *srcShapes = gMapShapeGrid + src;
  u32 *dstCells = cells + dst;
  u16 row;
  u16 col;

  cells += src;
  for (row = 0; row < height; row++) {
    u32 *dstCell = dstCells + (row << 7);
    u32 *srcCell = cells + (row << 7);
    u8 *dstShape = dstShapes + (row << 7);
    u8 *srcShape = srcShapes + (row << 7);

    for (col = 0; col < width; col++) {
      *dstCell = (*dstCell & MAP_CELL_TILE_MASK) |
                 (*srcCell++ & MAP_CELL_ATTRIBUTE_MASK);
      dstCell++;
      *dstShape = *srcShape;
      dstShape++;
      srcShape++;
    }
  }
}

/* VIEW(f02aa74): variable gMapCellBuffer as extern u8 gMapCellBuffer[]; f027064 has extern struct MapCell gMapCellBuffer[]; tried as the rest of the object declares it, it does not compile in tla-ja: games/tla/src/field/resource_slot.c:8266: `gMapCellBuffer__f02aa74' undeclared (first use in this function). */
extern u8 gMapCellBuffer__f02aa74[] __asm__("gMapCellBuffer");

void Func_0802b450(s32 srcX, s32 srcY, s32 width, s32 height, s32 dstX,
                   s32 dstY) {
  s32 y;
  u32 *src = (u32 *)gMapCellBuffer__f02aa74 + (srcY * 128 + srcX);
  u32 *dst = &((u32 *)gMapCellBuffer__f02aa74)[dstY * 128 + dstX];
  struct MapLayer *layer = Ram_HeapSlots->mapWork->layers;
  struct TilePos views[3];
  s32 i;
  struct TilePos *view = views;
  s32 x;

  for (i = 2; i >= 0; i--) {
    view->x = layer->x >> 20;
    view->y = layer->y >> 20;
    view++;
    layer++;
  }
  for (y = dstY; y < dstY + height; y++) {
    {
      for (x = dstX; x < dstX + width; x++) {
        u32 cell = *src++;
        u32 cellPictureId;
        u32 offset;
        u32 row;
        u32 n;
        u32 column;
        u32 scaled;
        u32 mask;

        // cellPictureId holds its mask before the masked cell; as cell & 0xfff,
        // 110 of 148 instructions differ in tla-en.
        cellPictureId = 0xfff;
        mask = 0x7ff;
        /* FAKEMATCH(barrier): an empty asm on mask once it holds 0x7ff, which
         * ranks mask above the destination word in local allocation, so mask
         * keeps r3, the word r2 and 0xfffff800 r1; without it, 23 of 145
         * instructions differ in tla-en. */
        __asm__("" : "+r"(mask));
        cellPictureId = cell & cellPictureId;
        mask &= cell;
        mask |= *dst & 0xfffff800;
        *dst = mask;
        dst++;
        view = views;
        column = x & 15;
        i = 0;
        row = y & 15;
        n = row << 5;
        n += column;
        offset = n * 4;
        for (; i < 3; i += 1) {
          if (view->x <= x && view->x + 16 > x && view->y <= y &&
              view->y + 12 > y) {
            u32 screenAddress, pictureAddress;
            u8 *screenTile, *pictureRows;

            screenAddress = 0x06002800;
            screenTile = (u8 *)screenAddress;
            screenTile += offset;
            scaled = cellPictureId * 8;
            pictureAddress = (u32)Ram_MapBlocks;
            pictureRows = (u8 *)pictureAddress;
            pictureRows += scaled;
            *(u32 *)screenTile = *(u32 *)pictureRows;

            pictureAddress = (u32)Ram_MapBlocks + 4;
            pictureRows = (u8 *)pictureAddress;
            pictureRows += scaled;
            screenAddress = 0x06002840;
            screenTile = (u8 *)screenAddress;
            screenTile += offset;
            *(u32 *)screenTile = *(u32 *)pictureRows;
            break;
          }
          // scaled, reused for the step to the next layer's BG map.
          scaled = 2048;
          offset += scaled;
          view++;
        }
      }
      src += 128 - width;
      dst += 128 - width;
    }
  }
}

#include "game_flags.h"

/// Copies a rectangle of map cells as they are, with their shape grid bytes,
/// from (srcX, srcY) to (dstX, dstY), width by height cells.
void Func_0802b590(s32 srcX, s32 srcY, s32 width, s32 height, s32 dstX,
                   s32 dstY) {
  u32 *cells = (u32 *)Ram_MapCellBuffer;
  s32 dst = (dstY << 7) + dstX;
  u8 *dstShapes = gMapShapeGrid + dst;
  s32 src = (srcY << 7) + srcX;
  u8 *srcShapes = gMapShapeGrid + src;
  u32 *dstCells = cells + dst;
  u16 row;
  u16 col;

  cells += src;
  for (row = 0; row < height; row++) {
    u32 *dstCell = dstCells + (row << 7);
    u32 *srcCell = cells + (row << 7);
    u8 *dstShape = dstShapes + (row << 7);
    u8 *srcShape = srcShapes + (row << 7);

    for (col = 0; col < width; col++) {
      *dstCell++ = *srcCell++;
      *dstShape++ = *srcShape++;
    }
  }
}

void Map_SetShownLayers(u32 v) {
  struct MapWork *state = Ram_HeapSlots->mapWork;
  u32 mask = v & 0xe00;
  u32 flags = state->flags;

  flags = (flags & 0xf1ff) | mask;
  state->flags = flags;
}

extern u8 gData_0202e000[];
extern u8 gData_0202e002[];
u32 Map_ReadEditU16(u8 **cursor);

void Map_ApplySetFlagEdits(void) {
  u8 *cursor = gData_0202e002;
  u32 entry;
  s32 srcX, srcY, dstX, dstY, width, height;

  cursor = gData_0202e000 + (u16)Map_ReadEditU16(&cursor);
next:
  entry = (u16)Map_ReadEditU16(&cursor);
  if (((entry + 1) & 0xffff) == 0)
    return;
  if (GameFlag_Test(entry & 0xfff) != 0) {
    srcX = *cursor++;
    srcY = *cursor++;
    dstX = *cursor++;
    dstY = *cursor++;
    width = *cursor++;
    height = *cursor++;
    if (entry & 0x1000)
      Map_CopyCellAttributeRect(srcX, srcY, width, height, dstX, dstY);
    else
      Map_CopyCellPictureIndicesRect(srcX, srcY, dstX, dstY, width, height);
  } else {
    cursor += 6;
  }
  goto next;
}

u32 Map_ReadEditU16(u8 **cursor) {
  u32 low = *(*cursor)++;
  u32 high = *(*cursor)++;

  return low | (high << 8);
}

void Map_ApplyFlagEdits(s32 flag, s32 attributes);

void Map_SetFlagAndApplyEdits(s32 flag) {
  flag &= 0xfff;
  GameFlag_SetBit(flag);
  Map_ApplyFlagEdits(flag, 0);
}

void Map_SetFlagAndApplyAttributeEdits(s32 flag) {
  flag &= 0xfff;
  GameFlag_SetBit(flag);
  Map_ApplyFlagEdits(flag, 1);
}

void Map_ApplyFlagEdits(s32 flag, s32 attributes) {
  u8 *cursor = gData_0202e002;
  u32 entry;
  s32 srcX, srcY, dstX, dstY, width, height;

  cursor = gData_0202e000 + (u16)Map_ReadEditU16(&cursor);
next:
  entry = (u16)Map_ReadEditU16(&cursor);
  if (((entry + 1) & 0xffff) == 0)
    return;
  if ((entry & 0xfff) == flag) {
    srcX = *cursor++;
    srcY = *cursor++;
    dstX = *cursor++;
    dstY = *cursor++;
    width = *cursor++;
    height = *cursor++;
    if (attributes != 0)
      Map_CopyCellAttributeRect(srcX, srcY, width, height, dstX, dstY);
    else
      Map_CopyCellPictureIndicesRect(srcX, srcY, dstX, dstY, width, height);
  } else {
    cursor += 6;
  }
  goto next;
}

extern char ResourceId_Map000;

void Map_GetHeaderSize(s32 map, s32 *size) {
  u8 *header;

  // Indexed as gData_0802f380[map][0], the load reads [table, offset].
  header = Resource_GetTableEntry(
      *(const u16 *)(map * 12 + (u32)gData_0802f380) + (u32)&ResourceId_Map000);
  size[0] = header[0];
  if (header[2] == 0)
    size[1] = 256;
  else
    size[1] = header[2];
  size[2] = header[1];
  if (header[3] == 0)
    size[3] = 256;
  else
    size[3] = header[3];
}

struct MapHeader {
  u8 unk00[44];
  s32 cells;
  s32 shapes;
};
void Tilemap_ConvertBuffer(void);

void Map_LoadCellsAndShapes(void) {
  u8 *source;
  struct MapHeader *header = Resource_GetTableEntry(
      *(const u16 *)(gMainRoutineWork.map * 12 + (u32)gData_0802f380) +
      (u32)&ResourceId_Map000);

  source = (u8 *)header + header->cells;
  Resource_DecodeType01(source, Ram_MapCellBuffer);
  Tilemap_ConvertBuffer();
  source = (u8 *)header + header->shapes;
  Resource_DecodeType01(source, gMapShapeGrid);
}


struct PerspectiveTiles {
  u8 unk000[0x114];
  u32 *tiles;
  u8 unk118[0x830];
  u16 quarterTiles[8];
};

s32 Map_WriteLayerCellTile(s32 layer, s32 x, s32 y, s32 tile, s32 update) {
  struct PerspectiveTiles *perspective =
      (struct PerspectiveTiles *)Ram_HeapSlots->mapWork;
  u32 *tileOffsets = perspective->tiles;
  u16 *shownQuarter;
  u16 *decodedTile;
  u16 *sourceTiles;
  u8 *quarterCells;
  u16 *screenTiles;
  u32 rowIndex;
  u32 columnIndex;
  u32 cellPictureId;

  x &= 1;
  y &= 1;
  shownQuarter = &perspective->quarterTiles[(layer * 2 + y) * 2 + x];
  if (update == 0 && tile == *shownQuarter)
    return 0;
  *shownQuarter = tile;
  decodedTile = HeapSlot_Alloc(56, 0x400);
  if (tileOffsets[tile] != 0)
    Resource_DecodeByteLzInRam((u8 *)tileOffsets + tileOffsets[tile],
                               decodedTile);
  else
    Iwram_ClearWords(decodedTile, 0x400);

  sourceTiles = decodedTile;
  // The block table at its fixed address: as the symbol gMapBlocks, its
  // address forms in r2 rather than r1 and update reloads into r3, not r2.
  quarterCells = Ram_MapBlocks + (((layer * 2 + y) * 32 + x) << 6);
  for (rowIndex = 0; rowIndex < 16; rowIndex++) {
    Dma_Set(sourceTiles, quarterCells, 0x84000010, REG_DMA3);
    sourceTiles += 32;
    quarterCells += 128;
  }

  if (update != 0) {
    screenTiles = (u16 *)(0x06004000 + ((((layer * 2 + y) << 6) + x) << 5));
    sourceTiles = decodedTile;
    for (rowIndex = 0; rowIndex < 16; rowIndex++) {
      for (columnIndex = 0; columnIndex < 16; columnIndex++) {
        u16 *bottomTiles;

        cellPictureId = *sourceTiles;
        screenTiles[0] = *(u16 *)(Ram_MapCellBuffer + cellPictureId * 4);
        bottomTiles = (u16 *)(Ram_MapCellBuffer + 2);
        bottomTiles = (u16 *)((u8 *)bottomTiles + cellPictureId * 4);
        screenTiles[32] = *bottomTiles;
        screenTiles++;
        sourceTiles += 2;
      }
      screenTiles += 48;
    }
  }
  HeapSlot_Release(56);
  return 1;
}
#undef MAP_CELL_TILE_MASK
#undef MAP_CELL_ATTRIBUTE_MASK

// ---- games/tla/src/field/map/f02b998.c ----
extern char ResourceId_PerspectiveDataA;
extern char ResourceId_DefaultMapCells;
extern char ResourceId_Data199;
extern u8 gData_0202e000[];
extern u32 gCameraSceneParameters[];
extern const u8 Data_0802146c[];
extern u8 Data_0802146cSize[];
extern u32 gData_03001144;
extern u32 Data_03001244;

void Fade_FromBlack(s32 vblanks);
s32 Trig_Cos(s32 angle);
s32 Trig_Sin(s32 angle);
void Projection_Set(s32 distance, s32 nearPlane, s32 farPlane);
/* VIEW(f02b998): routine Render_ResetTransformState as u32 Render_ResetTransformState(void); graphics.h has void Render_ResetTransformState(void); tried as the definition's void Render_ResetTransformState(void), which the object already declares, Func_0802b998 moves: 1 of 372 instructions in tla-ja. */
extern u32 Render_ResetTransformState__f02b998(void) __asm__("Render_ResetTransformState"); // Returns void; see the tag.
void SceneTransform_ApplyPosition(void *position);
void SceneTransform_ApplyYaw(s32 yaw);
void SceneTransform_ApplyPitch(s32 pitch);
void Transform_SetLookAt(s32 *eye, s32 *target);
void Func_0802dd70(s32 ratio, s32 *position, void *affineWork);
void Func_0802c240(void);
void Func_0802c088(void);

struct WorldView {
  s32 *origin;
  s32 shakeX, shakeY, shakeDecay;
  u8 *unk010;
  u16 unk014;
  u8 unk016;
  u8 unk017[0xcd];
  s32 viewX;
  s32 viewY;
  u8 unk0ec[0x18];
  u16 windowTop;
  u16 windowBottom;
  u8 unk108[0x14];
  u16 pitch;
  u16 yaw;
  u8 unk120[0x83c];
  s32 farPlane;
  s32 distance;
  u8 unk964[4];
  s32 unk968;
  u16 unk96c;
  u8 unk96e[3];
  s8 flat;
  s8 paused;
  u8 unk973[3];
  s8 fixedEye;
};

struct EyeOffset {
  s32 x, y, z;
};

void Func_0802b998(void) {
  /* FAKEMATCH(signature): Render_ResetTransformState declared returning u32;
   * as void, 2 of 376 instructions differ in tla-en. */
  u8 *perspective;
  struct WorldView *view;
  u8 *camera;
  u8 *affineWork;
  u8 *affinePages;
  s32 *cameraPosition;
  volatile u32 fill;
  struct EyeOffset eyeOffset;
  s32 farPlane;
  s32 tileIndex;

  REG_DISPCNT &= 0xc1ff;
  Fade_FromBlack(0);
  perspective = HeapSlot_Alloc(32, 0x978);
  view = (struct WorldView *)perspective;
  Dma_Fill32Cell(fill, 0, perspective, 0x978);
  view->viewX = 0;
  view->viewY = 0;
  *(s32 *)(perspective + 0xec) = 0x200000;
  *(s32 *)(perspective + 0xf0) = 0x400000;
  *(s32 *)(perspective + 0xf4) = 0x3fe00000;
  *(s32 *)(perspective + 0xf8) = 0x3fe00000;
  view->unk010 = NULL;
  *(void **)(perspective + 0x114) =
      Resource_GetTableEntry((s32)&ResourceId_PerspectiveDataA);
  Io_Write16(0x3f9e, &REG_BLDCNT);
  Io_Write16(0x1010, &REG_BLDALPHA);
  REG_BLDY = 0;
  Resource_DecodeType01(
      Resource_GetTableEntry((s32)&ResourceId_DefaultMapCells), gMapCellBuffer);
  view->unk014 = 0x1f00;
  view->unk016 = 0x80;
  Resource_DecodeType01(Resource_GetTableEntry((s32)&ResourceId_Data199),
                        gData_0202e000);
  Io_Write16(0xa80a, &REG_BG3CNT);
  Io_Write16(0xaa0e, &REG_BG2CNT);
  Io_Write16(0x8501, &REG_BG1CNT);
  Io_Write16(0x100, &REG_BG2PA);
  REG_BG2PB = 0;
  REG_BG2PC = 0;
  Io_Write16(0x100, &REG_BG2PD);
  REG_BG2X = 0;
  REG_BG2Y = 0;
  Io_Write16(0x100, &REG_BG3PA);
  REG_BG3PB = 0;
  REG_BG3PC = 0;
  Io_Write16(0x100, &REG_BG3PD);
  REG_BG3X = 0;
  Io_Write16(0, &REG_BG3Y);

  camera = HeapSlot_AllocEwram(48, 76);
  affineWork = HeapSlot_Alloc(28, 0x3484);
  affinePages = affineWork + 0xc80;
  farPlane = 0x1fe0000;
  view->farPlane = farPlane;
  view->distance = farPlane;
  view->unk968 = 0x10000;
  cameraPosition = (s32 *)(camera + 12);
  view->unk96c = 0;
  *(s32 *)(camera + 24) = 0;
  *(s32 *)(camera + 28) = 0;
  gCameraSceneParameters[3] = 120;
  gCameraSceneParameters[4] = 96;
  Projection_Set(farPlane, farPlane >> 1, farPlane << 1);
  cameraPosition[0] = 0;
  cameraPosition[1] = 0;
  cameraPosition[2] = 0;
  Render_ResetTransformState__f02b998();
  SceneTransform_ApplyPosition(cameraPosition);
  SceneTransform_ApplyYaw(view->yaw);
  SceneTransform_ApplyPitch(view->pitch);
  eyeOffset.x = 0;
  eyeOffset.y = 0;
  eyeOffset.z = farPlane;
  Iwram_TransformVector(&eyeOffset, (s32 *)camera);
  Render_ResetTransformState__f02b998();
  Transform_SetLookAt((s32 *)camera, cameraPosition);
  RamCode_LoadSlot(104, Data_0802146c, Data_0802146cSize);
  Func_0802dd70(Iwram_RatioMulQ14(Trig_Cos(view->pitch), Trig_Sin(view->pitch)),
                cameraPosition, affineWork);
  Data_03001244 = 0;
  gData_03001144 = view->pitch;
  ((void (*)(void *, s32 *, void *, void *))Ram_HeapSlots->unk64[1])(
      camera, cameraPosition, affineWork,
      affinePages + (gFrameCount & 1) * 0x1400);
  cameraPosition[0] = 0;
  cameraPosition[1] = 0;
  cameraPosition[2] = 0;
  Render_ResetTransformState__f02b998();
  view->pitch = 0xe000;
  view->yaw = 0;
  Render_ResetTransformState__f02b998();
  SceneTransform_ApplyPosition(cameraPosition);
  SceneTransform_ApplyYaw(view->yaw);
  SceneTransform_ApplyPitch(view->pitch);
  if (view->fixedEye == 0) {
    eyeOffset.x = 0;
    eyeOffset.y = 0;
    eyeOffset.z = view->distance + 0x10000;
    Iwram_TransformVector(&eyeOffset, (s32 *)camera);
  }
  REG_MOSAIC = 0;
  Io_Write16(0x42, &REG_DISPCNT);
  gBgScroll[1].x = 0;
  gBgScroll[1].y = 0;
  gBgScroll[2].x = 0;
  gBgScroll[2].y = 0;
  gBgScroll[3].x = 0;
  gBgScroll[3].y = 0;
  view->windowTop = 0;
  view->windowBottom = 159;
  Task_Add(Func_0802c240, 0x485);
  Task_Add(Func_0802c088, 0xc80);
  for (tileIndex = 0; tileIndex <= 0x3ff; tileIndex++)
    ((u16 *)(perspective + 0x148))[tileIndex] = tileIndex;
}

#define ABS(v) ((v) < 0 ? -(v) : (v))

struct MapTileWindow_08010d48 {
  s32 *position;
  u8 unk004[0x144];
  u16 tiles[32][32];
};

s32 Map_WriteLayerCellTile(s32 layer, s32 x, s32 y, s32 tile, s32 update);

void Map_SetWindowCellTile(s32 x, s32 y, s32 px, s32 py) {
  struct MapTileWindow_08010d48 *window;
  s32 *position;
  s32 originX;
  s32 originY;
  s32 tile;

  window = (struct MapTileWindow_08010d48 *)Ram_HeapSlots->mapWork;
  originX = 0;
  originY = 0;
  position = window->position;
  if (position != 0) {
    originX = *position++;
    originY = position[1];
  }

  originX >>= 24;
  originY >>= 24;
  x >>= 4;
  y >>= 4;
  px >>= 3;
  py >>= 3;
  tile = (y << 5) + x;
  window->tiles[(py / 2) & 31][(px / 2) & 31] = tile;

  if (ABS(originX - px) <= 1 && ABS(originY - py) <= 1) {
    Map_WriteLayerCellTile(0, px / 2, py / 2, tile, 1);
    Map_WriteLayerCellTile(1, px / 2, py / 2, tile + 0x400, 1);
  }
}

/// Map_SetWindowCellTile without its view test: puts world map tile
/// (x / 16, y / 16) at cell (px / 16, py / 16) of the tile grid and draws it in
/// both layers.
void Func_0802bdd0(s32 x, s32 y, s32 px, s32 py) {
  struct MapTileWindow_08010d48 *window;
  s32 *position;
  s32 originX;
  s32 originY;
  s32 tile;

  window = (struct MapTileWindow_08010d48 *)Ram_HeapSlots->mapWork;
  /* FAKEMATCH(dead): the view's origin read as Map_SetWindowCellTile reads
   * it and never used, which emits nothing but steers register allocation;
   * without it, 57 of 59 instructions differ in tla-en. */
  position = window->position;
  if (position != 0) {
    originX = *position++;
    originY = position[1];
  }

  x >>= 4;
  y >>= 4;
  px >>= 3;
  py >>= 3;
  tile = (y << 5) + x;
  window->tiles[(py / 2) & 31][(px / 2) & 31] = tile;
  Map_WriteLayerCellTile(0, px / 2, py / 2, tile, 1);
  Map_WriteLayerCellTile(1, px / 2, py / 2, tile + 0x400, 1);
}

// ---- games/tla/src/field/map/f02be4c.c ----
/// One word of the table at Ram_MapBlocks, 32 words to a row.
struct Data02020000 {
  u32 unk0 : 24;
  /// Passed to Func_0802d088; region / 10 picks the set. The Broken Seal's
  /// WorldMapCell keeps the encounter region at these bits.
  u32 region : 6;
  u32 unk1e : 2;
};

extern const u32 gData_0802ed34[4][9];
extern u8 gData_02028000[];
/* VIEW(f02be4c): variable gWorldMapWork as extern u8 gWorldMapWork[]; battle.h has extern struct WorldMapWork gWorldMapWork; tried as the rest of the object declares it, it does not compile in tla-ja: games/tla/src/field/resource_slot.c:8903: `gWorldMapWork' has an incomplete type. */
extern u8 gWorldMapWork__f02be4c[] __asm__("gWorldMapWork");

void Func_0802d088(s32 region, s32 mode);
void Func_080c8970(void *palette);
void Func_0802cc9c(const u16 *script);

void Func_0802be4c(s32 x, s32 z) {
  u8 *buffer;
  struct MapViewWork *view;
  struct Data02020000 *block;
  const u32 *resources;
  s32 graphicsSet;
  s32 region;
  s16 backdropColor;
  u32 *screenTiles;
  u32 tilePair;
  s32 row;
  s32 column;

  buffer = Heap_Alloc(512);
  view = (struct MapViewWork *)Ram_HeapSlots->mapWork;
  block =
      &((struct Data02020000 *)Ram_MapBlocks)[((x / (32 << 16)) & 31) +
                                              (((z / (32 << 16)) & 31) << 5)];
  Func_0802d088(block->region, 0);
  // Reused: the region, then its group of ten.
  region = block->region;
  region /= 10;
  switch (region) {
  case 3:
    graphicsSet = 3;
    break;
  case 2:
    graphicsSet = 2;
    break;
  case 4:
    graphicsSet = 1;
    break;
  default:
    graphicsSet = 0;
    break;
  }
  resources = gData_0802ed34[graphicsSet];
  view->resources = (u32 *)resources;
  backdropColor = *(s16 *)BG_PLTT;
  Resource_DecodeByteLzInRam(Resource_GetTableEntry(resources[0]), buffer);
  *(s16 *)buffer = backdropColor;
  Dma_Set(buffer, (void *)BG_PLTT, DMA_COPY_WORDS(112), REG_DMA3);
  Func_080c8970(buffer);
  Resource_DecodeType01(Resource_GetTableEntry(resources[1]),
                        Ram_MapCollision + 0x1000);
  Func_0802cc9c((const u16 *)(Ram_MapCollision + 0x1000));
  Resource_DecodeType01(Resource_GetTableEntry(resources[2]), Ram_MapCollision);
  Resource_DecodeType01(Resource_GetTableEntry(resources[3]), Ram_BgTileBuffer);
  Dma_Set(Ram_BgTileBuffer, BG_CHAR_BLOCK(2), DMA_COPY_WORDS(2048), REG_DMA3);
  Resource_DecodeType01(Resource_GetTableEntry(resources[4]),
                        Ram_BgTileBuffer + 0x2000);
  Dma_Set(Ram_BgTileBuffer + 0x2000, BG_CHAR_BLOCK(2) + 0x2000,
          DMA_COPY_WORDS(2048), REG_DMA3);
  Resource_DecodeType01(Resource_GetTableEntry(resources[5]),
                        Ram_BgTileBuffer + 0x4000);
  Dma_Set(Ram_BgTileBuffer + 0x4000, BG_CHAR_BLOCK(3), DMA_COPY_WORDS(2048),
          REG_DMA3);
  Resource_DecodeType01(Resource_GetTableEntry(resources[6]),
                        Ram_BgTileBuffer + 0x6000);
  Dma_Set(Ram_BgTileBuffer + 0x6000, BG_CHAR_BLOCK(3) + 0x2000,
          DMA_COPY_WORDS(2048), REG_DMA3);
  Resource_DecodeType01(Resource_GetTableEntry(resources[7]), gData_02028000);
  Resource_DecodeType01(Resource_GetTableEntry(resources[8]), gWorldMapWork__f02be4c);
  Dma_Fill32(0xf07ff07f, BG_SCREEN_BLOCK(5), 1536);
  screenTiles = (u32 *)BG_SCREEN_BLOCK(6);
  tilePair = 0x01810180;
  for (row = 0; row < 20; row++) {
    for (column = 15; column >= 0; column--) {
      *screenTiles++ = tilePair;
      tilePair += 0x20002;
    }
  }
  tilePair = 0x01010100;
  for (row = 0; row < 24; row++) {
    for (column = 15; column >= 0; column--) {
      *screenTiles++ = tilePair;
      tilePair += 0x20002;
    }
  }
  tilePair = 0x01010100;
  for (row = 0; row < 4; row++) {
    for (column = 15; column >= 0; column--) {
      *screenTiles++ = tilePair;
      tilePair += 0x20002;
    }
  }
  Heap_Release(buffer);
}


struct MapScanline {
  u8 unk00[20];
};

struct MapAffinePair {
  u32 words[8];
};

struct MapAffineWork {
  struct MapScanline scanlines[160];
  struct MapAffinePair pages[2][160];
};

struct PerspectiveWindow {
  u8 unk000[0x104];
  u16 windowTop;
  u16 windowBottom;
  u16 shownTop;
  u16 shownBottom;
  u16 windowPending;
};

void Func_0802c088(void) {
  struct MapAffinePair(*affinePages)[160] =
      ((struct MapAffineWork *)Ram_HeapSlots->unk1c[0])->pages;
  s32 displayControl = (s16)(REG_DISPCNT & 0xfff8);
  u32 *affineRegisters = (u32 *)0x04000020;
  struct PerspectiveWindow *perspective =
      (struct PerspectiveWindow *)Ram_HeapSlots->mapWork;
  volatile u16 *dma0 = REG_DMA0;
  u32 *affineSource;
  u32 mode;
  u32 windowTop;
  u32 nextDisplayControl;

  dma0[5] &= 0xc5ff;
  dma0[5] &= 0x7fff;
  (void)dma0[5];
  if (affinePages != NULL) {
    affineSource = (u32 *)affinePages[gFrameCount & 1];
    *affineRegisters++ = *affineSource++;
    *affineRegisters++ = *affineSource++;
    *affineRegisters++ = *affineSource++;
    *affineRegisters++ = *affineSource++;
    *affineRegisters++ = *affineSource++;
    *affineRegisters++ = *affineSource++;
    *affineRegisters++ = *affineSource++;
    *affineRegisters = *affineSource++;
    Dma_Set(affineSource, (void *)0x04000020, 0xa6600008, (u32 *)dma0);
  }
  perspective->shownTop = perspective->windowTop;
  perspective->shownBottom = perspective->windowBottom;
  mode = 0;
  windowTop = perspective->shownTop;
  if (windowTop < 200) {
    u16 bottom = perspective->shownBottom;

    mode = (bottom != 0) << 1;
    if (windowTop <= bottom) {
      mode = 0;
      if (windowTop == 0)
        mode = 2;
    }
  }
  displayControl |= mode;
  nextDisplayControl = (u16)displayControl;
  REG_DISPCNT = nextDisplayControl;
  perspective->windowPending = 0;
}

/* VIEW(f02be4c): variable gMapBlocks as extern u8 gMapBlocks[]; f0298c0 has extern u32 gMapBlocks[]; tried as the rest of the object declares it, it does not compile in tla-ja: games/tla/src/field/resource_slot.c:9021: `gMapBlocks__f02be4c' undeclared (first use in this function). */
extern u8 gMapBlocks__f02be4c[] __asm__("gMapBlocks");

/// Copies one world map cell's two tile entries (top and bottom) to a BG map.
static __inline__ void CopyWorldMapTileColumn(u8 *cellBytes, u8 *screenTile) {
  u32 cellPictureIndex = *(u16 *)cellBytes;
  u16 *cellPictureTiles = (u16 *)Ram_MapCellBuffer;

  cellPictureTiles += cellPictureIndex * 2;
  *(u16 *)screenTile = *cellPictureTiles++;
  *(u16 *)(screenTile + 64) = *cellPictureTiles;
}

/// Draws world map tile row tileRow (two screen rows) into the BG map at
/// 0x06004000, from the 32 by 32 cell grid in gMapBlocks.
void Map_RenderPaletteMappedRow(u32 tileRow) {
  u8 *cellBytes;
  u8 *screenTile;
  u32 column;

  cellBytes = gMapBlocks__f02be4c + ((((s32)tileRow / 2) & 31) << 7);
  {
    /* FAKEMATCH(pin): the BG map address held in r5, loaded right after the
     * cell address is formed; as a plain local, 4 of 57 instructions differ in
     * tla-en. */
    register u32 screenBase asm("r5") = 0x06004000;

    /* FAKEMATCH(barrier): the empty asm holding the pin above, which keeps the
     * address's load where it is; without it, 10 of 57 instructions differ in
     * tla-en. */
    __asm__ volatile("" : "+l"(screenBase));
    screenTile = (u8 *)(((tileRow & 62) << 6) + screenBase);
  }
  column = 0;
  do {
    CopyWorldMapTileColumn(cellBytes, screenTile);
    column++;
    screenTile += 2;
    cellBytes += 4;
  } while (column <= 31);

  {
    /* FAKEMATCH(pin): the step to the next half's screen row held in r2; as a
     * plain local, built in r3, 6 of 57 instructions differ in tla-en. */
    register u32 screenStep asm("r2") = 0xfc0;

    /* FAKEMATCH(barrier): the empty asm holding the pin above; without it, 10
     * of 57 instructions differ in tla-en. */
    __asm__("" : "+l"(screenStep));
    screenTile += screenStep;
  }
  cellBytes += 0xf80;
  column = 0;
  do {
    CopyWorldMapTileColumn(cellBytes, screenTile);
    column++;
    screenTile += 2;
    cellBytes += 4;
  } while (column <= 31);
}

/// Draws world map tile column tileColumn (two screen columns) into the BG map
/// at 0x06004000.
void Map_RenderPaletteMappedColumn(u32 tileColumn) {
  u8 *cellBytes;
  u8 *screenTile;
  u32 row;
  /* FAKEMATCH(pin): the BG map address held in r5, loaded between the cell
   * offset's shift and add; as a plain local, 4 of 32 instructions differ in
   * tla-en. */
  register u32 screenBase asm("r5") = 0x06004000;

  cellBytes = gMapBlocks__f02be4c + ((((s32)tileColumn / 2) & 31) << 2);
  /* FAKEMATCH(barrier): the empty asm holding the pin above after the cell
   * address; without it, 4 of 32 instructions differ in tla-en. */
  __asm__ volatile("" : "+l"(screenBase));
  screenTile = (u8 *)((tileColumn & 62) + screenBase);
  row = 0;
  do {
    CopyWorldMapTileColumn(cellBytes, screenTile);
    row++;
    screenTile += 128;
    cellBytes += 128;
  } while (row <= 63);
}


extern u32 gData_03001144;
extern u32 Data_03001244;

u32 Random16(void);
s32 Trig_Cos(s32 angle);
s32 Trig_Sin(s32 angle);
void Projection_Set(s32 distance, s32 nearPlane, s32 farPlane);
void SceneTransform_ApplyPosition(void *position);
void SceneTransform_ApplyYaw(s32 yaw);
void SceneTransform_ApplyPitch(s32 pitch);
void Transform_SetLookAt(s32 *eye, s32 *target);
void Func_0802c58c(void);
void Func_0802dd70(s32 ratio, s32 *position, void *affineWork);



void Func_0802c240(void) {
  u8 *camera = Ram_HeapSlots->cameraWork;
  u8 *affineWork = Ram_HeapSlots->unk1c[0];
  struct WorldView *view = (struct WorldView *)Ram_HeapSlots->mapWork;
  s32 *cameraPosition = (s32 *)(camera + 12);
  s32 *followedPosition = view->origin;
  u8 *affinePages = affineWork + 0xc80;
  s32 distance = view->farPlane;
  s32 height = view->distance;
  s32 tileRow;
  s32 shakeOffsetX;
  s32 shakeOffsetY;
  struct EyeOffset eyeOffset;

  shakeOffsetX = 0;
  shakeOffsetY = 0;
  if (view->paused != 0) {
    if (view->shakeX == 0 && view->shakeY == 0)
      return;
  } else {
    Func_0802c58c();
  }
  if (followedPosition != NULL) {
    s32 x = followedPosition[0];
    s32 y = followedPosition[2];
    s32 tileColumn;

    if (view->shakeX != 0) {
      s32 shakeDifference = Random16();

      shakeDifference -= Random16();
      shakeOffsetX = Iwram_MulQ16(view->shakeX, shakeDifference);
      x += shakeOffsetX;
      view->shakeX = Iwram_MulQ16(view->shakeX, view->shakeDecay);
    }
    if (view->shakeY != 0) {
      s32 shakeDifference = Random16();

      shakeDifference -= Random16();
      shakeOffsetY = Iwram_MulQ16(view->shakeY, shakeDifference);
      y += shakeOffsetY;
      view->shakeY = Iwram_MulQ16(view->shakeY, view->shakeDecay);
    }
    tileColumn = x / 0x100000;
    tileRow = y / 0x100000;
    if ((view->viewX ^ x) & 0x100000) {
      if (view->viewX < x)
        Map_RenderPaletteMappedColumn(tileColumn + 16);
      else
        Map_RenderPaletteMappedColumn(tileColumn - 16);
    }
    if ((view->viewY ^ y) & 0x100000) {
      if (view->viewY < y)
        Map_RenderPaletteMappedRow(tileRow + 12);
      else
        Map_RenderPaletteMappedRow(tileRow - 18);
    }
    view->viewX = x;
    view->viewY = y;
  }
  cameraPosition[0] = view->viewX;
  cameraPosition[1] = 0;
  cameraPosition[2] = view->viewY;
  if (view->flat != 0) {
    gBgScroll[1].x = shakeOffsetX / 0x10000;
    gBgScroll[1].y = shakeOffsetY / 0x10000;
    return;
  }
  gCameraSceneParameters[3] = 120;
  gCameraSceneParameters[4] = 96;
  Projection_Set(distance, height / 2, height * 2);
  Render_ResetTransformState();
  SceneTransform_ApplyPosition(cameraPosition);
  SceneTransform_ApplyYaw(view->yaw);
  SceneTransform_ApplyPitch(view->pitch);
  if (view->fixedEye == 0) {
    eyeOffset.x = 0;
    eyeOffset.y = 0;
    eyeOffset.z = height + 0x10000;
    Iwram_TransformVector(&eyeOffset, (s32 *)camera);
  }
  Render_ResetTransformState();
  Transform_SetLookAt((s32 *)camera, cameraPosition);
  if (gData_03001144 != view->pitch) {
    s32 cosine = Trig_Cos(view->pitch);
    s32 sine = Trig_Sin(view->pitch);

    Func_0802dd70(Iwram_RatioMulQ14(cosine, sine), cameraPosition, affineWork);
    Data_03001244 = 0;
    gData_03001144 = view->pitch;
  }
  ((void (*)(void *, s32 *, void *, void *))Ram_HeapSlots->unk64[1])(
      camera, cameraPosition, affineWork,
      affinePages + (gFrameCount & 1) * 0x1400);
}

struct MapTileWindow {
  s32 *origin;
  u8 unk004[0x144];
  u16 tileIds[32 * 32];
};

s32 Map_WriteLayerCellTile(s32 layer, s32 x, s32 y, s32 tile, s32 update);

void Map_UpdateCurrentTileBlock(void) {
  struct MapTileWindow *window = (struct MapTileWindow *)Ram_HeapSlots->mapWork;
  s32 x0 = 0;
  s32 y0 = 0;
  u32 layer;
  u32 row;
  u32 col;
  s32 bias;
  s32 tile;

  if (window->origin != NULL) {
    s32 *p = window->origin;

    x0 = *p++;
    y0 = p[1];
  }
  x0 = (x0 - 0x1000000) >> 25;
  y0 = (y0 - 0x1400000) >> 25;
  for (layer = 0; layer < 2; layer++) {
    for (row = 0; row < 2; row++) {
      for (col = 0; col < 2; col++) {
        tile = (((y0 + row) & 31) << 5) + ((x0 + col) & 31);
        tile = window->tileIds[tile];
        tile += layer << 10;
        Map_WriteLayerCellTile(layer, x0 + col, y0 + row, tile, 1);
      }
    }
  }
}
