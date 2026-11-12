#include "types.h"
#include "resource.h"
#include "sprite.h"
#include "sprite_metadata.h"
#include "ram_buffer.h"
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

s32 ResourceMetadata_Register(struct ResourceSprite *sprite, s32 id);

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
