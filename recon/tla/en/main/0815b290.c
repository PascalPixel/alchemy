/* Draft of Func_0815b290: differ 68 of 131 (tla-en, 2026-10-15). The Lost
 * Age's own, no Broken Seal twin (the nearest, RenderResource_CreatePair,
 * shares only the free-entry search). It makes an effect sprite with tiles
 * of its own in object VRAM: it takes a free VRAM cache entry and the first
 * free effect sprite of the 64 (as Func_0815b3b0 does), sets the sprite up
 * for Animation_ApplyChildArgumentFar, then reserves its tiles, width *
 * height bytes, halved unless the attributes ask for 256 colours (0x2000),
 * and adds them to the object system's tile count.
 *
 * Found so far: the cache entry is loaded (VramBlock_LoadCached(entry, 0,
 * 0)) before any store; the first three words are written through a word
 * cursor (stmia r3!); the halfword clear comes before the byte clears, so
 * the bytes share a halfword zero, which the native loads from the pool
 * (and dumps that pool mid-function, as here); the 93rd entry's offset is
 * read as halfword 187 of the table.
 *
 * Left: the table's base, which the native keeps in r9 across both calls
 * (here reloaded); the word at +28, which the native addresses with an add
 * of its own; height, which the native spills to the stack and reloads as
 * a byte; the end's tile field merge (0xfc00 from the pool). Tried: the
 * table through a local (70), u8 parameters (zero-extended: 76), the +28
 * word through a cast pointer (68), the byte clears first (72, 79).
 */
#include "types.h"
#include "ram_buffer.h"
#include "vram_tab.h"

struct AnimationEntry;

struct EffectSprite {
  s32 unk00;
  s32 attributes;
  union {
    u32 word;
    u16 tile;
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
s32 VramBlock_LoadCached(u32 slot, u32 size, const void *source);
struct AnimationEntry *Func_0815b24c(void);
s32 Animation_ApplyChildArgumentFar(void *object, s32 argument);

struct EffectSprite *Func_0815b290(s32 width, s32 height, s32 attributes,
                                   s32 flags) {
  struct EffectSprite *found = NULL;
  s32 vramEntry = Resource_FindFreeEntry();
  struct EffectSprite *sprite = Ram_HeapSlots->unk0c[1];
  s32 i = 0;
  u16 *tilesUsed;
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
  tile = VramBlock_LoadCached(vramEntry, 0, 0);
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
  found->unk1c = 0;
  found->unk20 = 0x6000;
  entries = gResourceTableEntries;
  found->unk24 = (((u16 *)entries)[187] >> 5) | 0x800;
  sprite->entry = Func_0815b24c();
  sprite->unk0c = 0x10000;
  sprite->width = width;
  sprite->height = height;
  sprite->unk17 = 0;
  sprite->unk16 = 0;
  sprite->unk1b = 1;
  sprite->unk1a = 0;
  Animation_ApplyChildArgumentFar(sprite, 0);

  tilesUsed = (u16 *)Ram_HeapSlots->objectSystem;
  size = sprite->width * sprite->height;
  if (!(attributes & 0x2000))
    size = (u32)size >> 1;
  VramBlock_LoadCached(sprite->vramEntry, size, 0);
  *tilesUsed += size;
  sprite->oam.tile =
      (sprite->oam.tile & 0xfc00) |
      (entries[sprite->vramEntry].offset >> 5 & 0x3ff);
  return found;
}
