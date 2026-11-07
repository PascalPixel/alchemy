#include "types.h"
#include "sprite.h"
#include "ram_buffer.h"
#include "vram_tab.h"

struct AnimationEntry;

struct EffectSprite {
  s32 unk00;
  s32 attributes;
  union {
    u32 word;
    struct {
      u32 tile : 10;
      u32 rest : 22;
    } bits;
  } oam;
  s32 unk0c;
  u8 vramEntry;
  u8 unk11_0 : 1;
  u8 unk11_1 : 7;
  u16 unk12;
  u8 width;
  u8 height;
  u8 unk16;
  u8 unk17;
  u8 unk18[2];
  u8 unk1a;
  u8 unk1b;
  s32 unk1c;
  s32 unk20;
  s32 unk24;
  struct AnimationEntry *entry;
  u8 unk2c[0x0c];
};

s32 Resource_FindFreeEntry(void);
s32 VramCache_Load(u32 slot, u32 size, const void *source);
struct AnimationEntry *AnimationObject_AllocateEmpty(void);
s32 Animation_ApplyChildArgumentFar(void *object, s32 argument);

/// Makes an effect sprite with tiles of its own in object VRAM: a free VRAM
/// cache entry and the first free effect sprite of the 64, as Func_0815b3b0
/// takes, set up for Animation_ApplyChildArgumentFar. The Lost Age's own.
struct EffectSprite *Func_0815b290(s32 width, s32 height, s32 attributes,
                                   s32 flags) {
  struct EffectSprite *found = NULL;
  s32 vramEntry = Resource_FindFreeEntry();
  struct EffectSprite *sprite = Ram_HeapSlots->unk0c[1];
  s32 i = 0;
  struct ObjectSystemWork *objects;
  s32 size;
  s32 tile;
  struct VramBlockCacheEntry *entries;

  while (i <= 63) {
    if (sprite->width == 0) {
      found = sprite;
      break;
    }
    i++;
    sprite++;
  }
  tile = VramCache_Load(vramEntry, 0, 0);
  found->vramEntry = vramEntry;
  found->unk12 = 0;
  found->unk1a = 0;
  found->unk11_0 = 0;
  {
    u32 *words = (u32 *)found;

    *words++ = 0;
    *words++ = attributes;
    *words = flags | tile | 0x800;
  }
  {
    s32 *p = &found->unk1c;

    *p = 0;
  }
  found->unk20 = 0x6000;
  entries = gResourceTableEntries;
  found->unk24 = (((u16 *)entries)[187] >> 5) | 0x800;
  sprite->entry = AnimationObject_AllocateEmpty();
  sprite->width = width;
  sprite->unk0c = 0x10000;
  sprite->height = height;
  sprite->unk17 = 0;
  sprite->unk16 = 0;
  sprite->unk1b = 1;
  sprite->unk1a = 0;
  Animation_ApplyChildArgumentFar(sprite, 0);

  objects = Ram_HeapSlots->objectSystem;
  size = sprite->width * sprite->height;
  if (!(attributes & 0x2000))
    size = (u32)size >> 1;
  VramCache_Load(sprite->vramEntry, size, 0);
  objects->used += size;
  sprite->oam.bits.tile = entries[sprite->vramEntry].offset >> 5;
  return found;
}
