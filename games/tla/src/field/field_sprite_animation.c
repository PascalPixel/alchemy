/// @file
/// Sprites, their animations and the picture caches they draw from.
// Part of field.c.

#include "field_internal.h"
#include "field/field_bank.h"
#include "field/resource_slot.h"
#include "system/system_bank.h"
#include "scene/shared.h"
#include "system/system.h"
#include "types.h"
#include "graphics/resource.h"
#include "field/sprite.h"
#include "system/sprite_metadata.h"
#include "system/ram_buffer.h"
#include "debug.h"
#include "system/heap_release.h"
#include "system/io_reg.h"
#include "system/dma.h"
#include "edition.h"
#include "graphics/vram_tab.h"
#include "system/scene.h"

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
  struct SpriteAnimation *entries[4];
};

#undef SpriteAnimation_Alloc

/// Loads the pictures of animation id number into picture cache slot's buffer:
/// the resource listed for it in gResourceSlot_NumberTable, decoded, its
/// picture offsets turned into pointers, and for variant 1-5 its pixel colours
/// (those up to 223, the basic sprite palettes 0 to 13) remapped through that
/// variant's conversion table; a variant above 5 uses variant 1's.
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
  work = Ram_HeapSlots->actorSystem;
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

/// Returns the buffer loaded into the sprite picture cache with this tag, or 0.
s32 SpritePictureCache_FindByTag(s32 key) {
  struct ObjectSystemWork *work = Ram_HeapSlots->actorSystem;
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
 * tla-ja: games/tla/src/field/field_sprite_animation.c:158: structure has no
 * member named `width'. */
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
  struct SpriteAnimation *entries[4];
};
/* VIEW(resource_slot): routine Sprite_ResetAnimations as s32
 * Sprite_ResetAnimations(struct ActorSpriteView__resource_slot *state);
 * actor_view.h has s32 Sprite_ResetAnimations(struct ActorSpriteView *); tried
 * as the definition's s32 Sprite_ResetAnimations(struct
 * ActorSpriteView__resource_slot *state), which the object already declares, it
 * does not compile in tla-ja: games/tla/src/field/field_sprite_animation.c:157:
 * conflicting types for `Sprite_ResetAnimations'. */
s32 InitializeAnimationObjects__resource_slot(
    struct ActorSpriteView__resource_slot
        *state) __asm__("Sprite_ResetAnimations");
#define Sprite_ResetAnimations InitializeAnimationObjects__resource_slot
/// Resets each sprite animation of a sprite from its animation's metadata: its
/// facing kind, pictures, scripts and codec, with no picture shown and no
/// script running.
s32 Sprite_ResetAnimations(struct ActorSpriteView__resource_slot *state) {
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
  s32 z;
  s32 animation;
  struct AnimationMetadata *info;

  if (work != NULL) {
    info = Resource_GetMetadataRecordFar(work->animationId);
    if (info->width != 0) {
      value = info->pictures;
      if (value == 0)
        value = SpritePictureCache_FindByTag(work->animationId);
      z = 0;
      work->kind = info->drawKind;
      work->script = (u8 *)z;
      /* FAKEMATCH(dead): an empty once-only block. */
      do {
      } while (0);
      animation = info->animation;
      work->pictures = (void **)value;
      work->scripts = (u8 **)animation;
      work->scriptOffset = z;
      work->mode = info->pictureCodec;
      work->picture = 0xff;
    }
  }
}

/* VIEW(resource_slot): routine Sprite_AddAnimation as s32
 * Sprite_AddAnimation(struct MetadataSlotState *state, s32 id); actor_view.h
 * has s32 Sprite_AddAnimation(struct ActorSpriteView *, s32); tried as the
 * definition's s32 Sprite_AddAnimation(struct MetadataSlotState *state, s32
 * id), which the object already declares, it does not compile in tla-ja:
 * games/tla/src/field/field_sprite_animation.c:248: conflicting types for
 * `Sprite_AddAnimation'. */
s32 ResourceMetadata_Register__resource_slot(
    struct MetadataSlotState *state, s32 id) __asm__("Sprite_AddAnimation");
#define Sprite_AddAnimation ResourceMetadata_Register__resource_slot
/// Adds a sprite animation of animation id to a sprite, in the first of its
/// four slots that is free; returns it, 0 when none could be taken, or -1 when
/// all four are in use.
s32 Sprite_AddAnimation(struct MetadataSlotState *state, s32 id) {
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
  value = SpriteAnimation_Alloc(id);
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

/// Removes the sprite animation handle from a sprite and clears it, trimming
/// the sprite's animation count when no later sprite animation remains.
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

/// Removes and clears a sprite's animation number no (0-3), trimming the
/// sprite's animation count when no later sprite animation remains.
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

/* VIEW(resource_slot): routine Sprite_StartAnimation as s32
 * Sprite_StartAnimation(struct AnimationGroup *state, s32 flags); actor_view.h
 * has s32 Sprite_StartAnimation(struct ActorSpriteView *, s32); tried as the
 * definition's s32 Sprite_StartAnimation(struct AnimationGroup *state, s32
 * flags), which the object already declares, it does not compile in tla-ja:
 * games/tla/src/field/field_sprite_animation.c:360: conflicting types for
 * `Sprite_StartAnimation'. */
s32 Animation_ApplyChildArgument__resource_slot(
    struct AnimationGroup *state, s32 flags) __asm__("Sprite_StartAnimation");
#define Sprite_StartAnimation Animation_ApplyChildArgument__resource_slot
/// Starts animation flags & 0x7f on every sprite animation of a sprite (bit 7
/// keeps each sprite animation's place and timer), unless the sprite plays it
/// already.
s32 Sprite_StartAnimation(struct AnimationGroup *state, s32 flags) {
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

struct ChildGroup {
  u8 unk00[27];
  u8 count;
  u8 unk1c[12];
  struct ChildObject *children[1];
};

/// Sets each playing sprite animation's timer to the given duration in frames.
/// Returns 0.
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

/* VIEW(resource_slot): routine Sprite_SetAnimationSpeed as void
 * Sprite_SetAnimationSpeed(struct ChildGroup *group, s32 value); actor_view.h
 * has void Sprite_SetAnimationSpeed(struct ActorSpriteView *, s32); tried as
 * the definition's void Sprite_SetAnimationSpeed(struct ChildGroup *group, s32
 * value), which the object already declares, it does not compile in tla-ja:
 * games/tla/src/field/field_sprite_animation.c:440: conflicting types for
 * `Sprite_SetAnimationSpeed'. */
void Animation_ApplyChildValue__resource_slot(
    struct ChildGroup *group, s32 value) __asm__("Sprite_SetAnimationSpeed");
#define Sprite_SetAnimationSpeed Animation_ApplyChildValue__resource_slot
/// Sets the timer step, the animation speed, of each playing sprite animation
/// of a sprite (16 is normal speed).
void Sprite_SetAnimationSpeed(struct ChildGroup *group, s32 value) {
  s32 i;

  for (i = 0; i < group->count; i++) {
    struct ChildObject *child = group->children[i];

    if (child != NULL && child->active != 0)
      *((u8 *)child + 21) = value;
  }
}

#define ANIMATION_OBJECT_COUNT 64

/// Takes the sprite and layer pools (from the fixed heap blocks in mode 3,
/// the heap otherwise) and clears them, loads the shadow tiles into VRAM cache
/// entry 93 and copies the strided tile copier into RAM.
void ActorSystem_Configure(s32 mode) {
  void *sprites;
  void *layers;
  volatile u32 zero;

  if (mode == 3) {
    sprites = HeapSlot_AllocEwram(HEAP_SLOT_SPRITES, ANIMATION_OBJECT_COUNT *
                                                         sizeof(struct Sprite));
    layers = HeapSlot_AllocEwram(HEAP_SLOT_SPRITE_ANIMATIONS,
                                 ANIMATION_OBJECT_COUNT *
                                     sizeof(struct SpriteAnimation));
  } else {
    sprites = HeapSlot_Alloc(HEAP_SLOT_SPRITES,
                             ANIMATION_OBJECT_COUNT * sizeof(struct Sprite));
    layers =
        HeapSlot_Alloc(HEAP_SLOT_SPRITE_ANIMATIONS,
                       ANIMATION_OBJECT_COUNT * sizeof(struct SpriteAnimation));
  }
  Palette_LoadBasicSpriteColors();
  Dma_Fill32Cell(zero, 0, sprites,
                 ANIMATION_OBJECT_COUNT * sizeof(struct Sprite));
  Dma_Fill32Cell(zero, 0, layers,
                 ANIMATION_OBJECT_COUNT * sizeof(struct SpriteAnimation));
  VramCache_Load(93, 128, gActor_ShadowTiles);
  RamCode_LoadSlot(HEAP_SLOT_PICTURE_UPLOADER, Render_CopyStridedCode,
                   Render_CopyStridedCodeSize);
}

/// Frees VRAM cache entry 93, the actors' shadow tiles.
void VramBlock_Reset93(void) { VramCache_FreeEntry(93); }

/// Loads the actors' shadow tiles into VRAM cache entry 93.
void VramBlock_Load93(void) {
  const u8 *data = gActor_ShadowTiles;
  s32 size = 128;

  VramCache_Load(93, size, data);
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

typedef char
    ResourceObjectWork_size[sizeof(struct ResourceObjectWork) == 56 ? 1 : -1];

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
  *attribute = (gVramCacheEntries[93].offset >> 5) | 0x800;
  if (Sprite_AddAnimation(sprite, id) == -1) {
    /* FAKEMATCH(dead): a store of the byte it reloads. */
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
