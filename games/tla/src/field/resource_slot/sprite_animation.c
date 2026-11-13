/// @file
/// Sprites, their animations and the picture caches they draw from.
// Part of field.c.

#include "../resource_slot_internal.h"
#include "module/field/field.h"
#include "module/field/resource_slot.h"
#include "module/system/system.h"
#include "module/tla.h"

struct ChildObject {
  u8 unk00[2];
  u16 value;
  u8 unk04[8];
  s32 active;
  u8 unk10[5];
  u8 mode;
};

/// The sprite as Sprite_Create fills it: the image's three attribute
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

/* VIEW(resource_slot): routine SpriteAnimation_Alloc as s32
 * SpriteAnimation_Alloc(s32 id); the definition has struct AnimationEntry
 * *SpriteAnimation_Alloc(s32 id); tried as the definition's struct
 * AnimationEntry *SpriteAnimation_Alloc(s32 id), which the object already
 * declares, SpriteAnimation_Alloc (gone) moves: 60 of 60 instructions in
 * tla-ja. */
extern s32 AnimationObject_Allocate__resource_slot(s32 id) __asm__(
    "SpriteAnimation_Alloc");
#define SpriteAnimation_Alloc AnimationObject_Allocate__resource_slot
#include "system/system.h"
#undef SpriteAnimation_Alloc
#include "types.h"
#include "graphics/resource.h"
#include "field/sprite.h"
#include "system/sprite_metadata.h"
#include "system/ram_buffer.h"

s32 SpritePictureCache_Load(u32 slot, u32 *buffer, s32 number, u32 variant) {
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
/* VIEW(resource_slot): tag struct ActorSpriteView as struct
 * ActorSpriteView__resource_slot; actor_view.h has another definition; tried
 * with the definition the rest of the object has, it does not compile in
 * tla-ja: games/tla/src/field/field.c:1026: structure has no member named
 * `width'. */
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
/* VIEW(resource_slot): routine InitializeAnimationObjects as s32
 * InitializeAnimationObjects(struct ActorSpriteView__resource_slot *state);
 * actor_view.h has s32 InitializeAnimationObjects(struct ActorSpriteView *);
 * tried as the definition's s32 InitializeAnimationObjects(struct
 * ActorSpriteView__resource_slot *state), which the object already declares, it
 * does not compile in tla-ja: games/tla/src/field/field.c:1027: conflicting
 * types for `InitializeAnimationObjects'. */
s32 InitializeAnimationObjects__resource_slot(
    struct ActorSpriteView__resource_slot
        *state) __asm__("InitializeAnimationObjects");
#define InitializeAnimationObjects InitializeAnimationObjects__resource_slot
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

void SpriteAnimation_Reset(struct AnimationEntry *work) {
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

/* VIEW(resource_slot): routine ResourceMetadata_Register as s32
 * ResourceMetadata_Register(struct MetadataSlotState *state, s32 id);
 * actor_view.h has s32 ResourceMetadata_Register(struct ActorSpriteView *,
 * s32); tried as the definition's s32 ResourceMetadata_Register(struct
 * MetadataSlotState *state, s32 id), which the object already declares, it does
 * not compile in tla-ja: games/tla/src/field/field.c:1114: conflicting types
 * for `ResourceMetadata_Register'. */
s32 ResourceMetadata_Register__resource_slot(
    struct MetadataSlotState *state,
    s32 id) __asm__("ResourceMetadata_Register");
#define ResourceMetadata_Register ResourceMetadata_Register__resource_slot
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

void Sprite_RemoveAnimation(struct MetadataSlotState *state, s32 handle) {
  s32 count;
  u32 slot;
  u32 i;

  if (state != NULL && handle != 0) {
    SpriteAnimation_Clear((void *)handle);
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

void Sprite_RemoveAnimationAt(struct MetadataSlotState *state, u32 no) {
  s32 slot;
  s32 count;
  u32 i;

  if (state != NULL && no <= 3) {
    slot = state->slots[no];
    if (slot != 0) {
      SpriteAnimation_Clear((void *)slot);
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

void SpriteAnimation_Start(struct AnimationEntry *work, s32 no) {
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

/* VIEW(resource_slot): routine Animation_ApplyChildArgument as s32
 * Animation_ApplyChildArgument(struct AnimationGroup *state, s32 flags);
 * actor_view.h has s32 Animation_ApplyChildArgument(struct ActorSpriteView *,
 * s32); tried as the definition's s32 Animation_ApplyChildArgument(struct
 * AnimationGroup *state, s32 flags), which the object already declares, it does
 * not compile in tla-ja: games/tla/src/field/field.c:1220: conflicting types
 * for `Animation_ApplyChildArgument'. */
s32 Animation_ApplyChildArgument__resource_slot(
    struct AnimationGroup *state,
    s32 flags) __asm__("Animation_ApplyChildArgument");
#define Animation_ApplyChildArgument Animation_ApplyChildArgument__resource_slot
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

struct ChildGroup {
  u8 unk00[27];
  u8 count;
  u8 unk1c[12];
  struct ChildObject *children[1];
};

s32 Sprite_SetAnimationTimers(struct ChildGroup *group, s32 value) {
  s32 i;

  for (i = 0; i < group->count; i++) {
    struct ChildObject *child = group->children[i];

    if (child != NULL && child->active != 0) {
      child->value = value << 4;
    }
  }
  return 0;
}

/* VIEW(resource_slot): routine Animation_ApplyChildValue as void
 * Animation_ApplyChildValue(struct ChildGroup *group, s32 value); actor_view.h
 * has void Animation_ApplyChildValue(struct ActorSpriteView *, s32); tried as
 * the definition's void Animation_ApplyChildValue(struct ChildGroup *group, s32
 * value), which the object already declares, it does not compile in tla-ja:
 * games/tla/src/field/field.c:1294: conflicting types for
 * `Animation_ApplyChildValue'. */
void Animation_ApplyChildValue__resource_slot(
    struct ChildGroup *group, s32 value) __asm__("Animation_ApplyChildValue");
#define Animation_ApplyChildValue Animation_ApplyChildValue__resource_slot
void Animation_ApplyChildValue(struct ChildGroup *group, s32 value) {
  s32 i;

  for (i = 0; i < group->count; i++) {
    struct ChildObject *child = group->children[i];

    if (child != NULL && child->active != 0)
      *((u8 *)child + 21) = value;
  }
}

#include "system/heap_release.h"
#include "system/io_reg.h"
#include "system/dma.h"

#define ANIMATION_OBJECT_COUNT 64

/// Takes the sprite and layer pools (from the fixed heap blocks in mode 3,
/// the heap otherwise) and clears them, loads the shadow tiles into VRAM cache
/// entry 93 and copies the strided tile copier into RAM.
// As ☀️'s function of the same name.
void ActorSystem_Configure(s32 mode) {
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
  Palette_LoadBasicSpriteColors();
  Dma_Fill32Cell(zero, 0, sprites,
                 ANIMATION_OBJECT_COUNT * sizeof(struct AnimationObject));
  Dma_Fill32Cell(zero, 0, layers,
                 ANIMATION_OBJECT_COUNT * sizeof(struct AnimationEntry));
  VramCache_Load(93, 128, gActor_ShadowTiles);
  {
    u32 size = (u32)Render_CopyStridedCodeSize;

    Dma_Set(Render_CopyStridedCode, HeapSlot_Alloc(84, size),
            DMA_COPY_WORDS(size >> 2), REG_DMA3);
  }
}

void VramBlock_Reset93(void) { VramCache_FreeEntry(93); }

void VramBlock_Load93(void) {
  const u8 *data = gActor_ShadowTiles;
  s32 size = 128;

  VramCache_Load(93, size, data);
}

/* VIEW(resource_slot): routine SpriteAnimation_Alloc as struct AnimationEntry
 * *SpriteAnimation_Alloc(s32 id); system/system.h has s32
 * SpriteAnimation_Alloc(s32 id); tried as the definition's struct
 * AnimationEntry *SpriteAnimation_Alloc(s32 id), which the object already
 * declares, it does not compile in tla-ja: games/tla/src/field/field.c:1358:
 * conflicting types for `SpriteAnimation_Alloc'. */
struct AnimationEntry *
SpriteAnimation_Alloc__resource_slot(s32 id) __asm__("SpriteAnimation_Alloc");
#define SpriteAnimation_Alloc SpriteAnimation_Alloc__resource_slot
struct AnimationEntry *SpriteAnimation_Alloc(s32 id) {
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
void SpriteAnimation_Clear(void *dst) {
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

typedef char
    ResourceObjectWork_size[sizeof(struct ResourceObjectWork) == 56 ? 1 : -1];

#include "graphics/vram_tab.h"

/// Creates an animated sprite object for animation id, using one of 64 free
/// objects and a VRAM cache entry. Returns the object, or NULL on failure.
// As ☀️'s function of the same name.
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
void Sprite_Destroy(struct ResourceObjectWork *work) {
  volatile u32 zero;
  s32 n;
  void **child;

  if (work) {
    if (!(work->flags & 1))
      VramCache_FreeEntry(work->resource);
    child = work->children;
    n = 3;
    do {
      SpriteAnimation_Clear(*child++);
    } while (--n >= 0);
    zero = 0;
    Dma_Set(&zero, work,
            ((DMA_ENABLE | DMA_32BIT | DMA_SRC_FIXED) << 16) |
                (sizeof(*work) / 4),
            REG_DMA3);
  }
}

#include "system/scene.h"

/// Sums the first cnt frame durations of animation id's entry number `no`.
/// Commands are two bytes (command, value); 254, 241, 253 and 239 end the
/// list, and the values of commands 245, 255 and those up to 238 count.
s32 SpriteAnimation_SumCommandLengths(s32 id, u32 no, s32 cnt) {
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
