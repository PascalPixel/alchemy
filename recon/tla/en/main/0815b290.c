// Near miss of Func_0815b290: differ 4 of 131 (tla-en, 2026-10-19). The
// Lost Age's own, no Broken Seal twin (the nearest, RenderResource_CreatePair,
// shares only the free-entry search). It makes an effect sprite with tiles
// of its own in object VRAM: it takes a free VRAM cache entry and the first
// free effect sprite of the 64 (as Func_0815b3b0 does), sets the sprite up
// for Animation_ApplyChildArgumentFar, then reserves its tiles, width *
// height bytes, halved unless the attributes ask for 256 colours (0x2000),
// and adds them to the object system's tile count.

// Found: the cache entry is loaded (VramCache_Load(entry, 0, 0)) before
// any store; the first three words are written through a word cursor
// (stmia r3!); the halfword clear comes before the byte clears, so the
// bytes share a halfword zero, which the native loads from the pool (and
// dumps that pool mid-function, as here); the 93rd entry's offset is read
// as halfword 187 of the table.
// 2026-10-19 (68 -> 4): the tile number is a 10-bit field of the OAM
// word, so the end is a bitfield store (the 0xfffffc00 mask from the pool,
// and the table's base then stays in r9 with height spilled, as in the
// native); the tile count is written through struct ObjectSystemWork, so
// its store does not hold back the sprite's reads; the word at +28 is
// stored through a pointer of its own (the native's add, and the
// mid-function pool lands where it does); width is stored before 0x10000
// (alchemy-devtools permute).

// Left: two single-row order differences, each the native issuing a load
// one slot earlier, as a load latency of 3 would: the reload of r8 for the
// bitfield's ldrb before the zero's movs r1, and the spilled height's ldrb
// before the 0x10000 constant after AnimationObject_AllocateEmpty.
// Tried: the bit as a plain u8 cleared with &= ~1 (the first row matches,
// but the mask becomes 254: 9) or s8 (4); every order of the four first
// stores and of the four stores after the allocation; alchemy-devtools permute 11
// minutes, two seeds: nothing below 4.
// 2026-10-24: the flag byte read into a u8 held by an asm that clobbers r1
// (31: the shared zero in r1 is rebuilt, and the mask becomes 254); the
// height copied into a u8 held by an asm that clobbers r3 (48: height is no
// longer spilled).

#include "types.h"
#include "animspr.h"
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
