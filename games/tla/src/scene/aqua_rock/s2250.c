#include "types.h"
#include "call.h"
#include "actor.h"
#include "map.h"
#include "ram_buffer.h"
#include "graphics/vram_tab.h"

struct Unk020034f0Entry {
  s32 x;
  s32 y;
  s32 z;
  // volatile: the listing reads age again after the OAM stores.
  volatile s32 age;
  u32 oam[3];
};

struct Unk020034f0 {
  struct Actor *source;
  u16 block;
  u16 next;
  u8 unk08[4];
  struct Unk020034f0Entry entries[64];
};

extern struct Unk020034f0 gData_020034f0;
u32 Random16Far(void);
s32 Map_GetTerrainHeight(s32 terrain, s32 x, s32 z);
void Func_020028d0(u32 *oam, s32 depth);

void Func_02002250(void) {
  struct MapWork *map = Ram_HeapSlots->mapWork;
#ifndef UBFIX
  s32 *view = &map->viewX;
#endif
  // UB: viewZ is a separate member, not a second element of viewX (as the
  // members, 69 of 169 instructions differ in tla-en).
#ifdef UBFIX
  s32 viewX = map->viewX & 0xffff0000;
  s32 viewZ = map->viewZ & 0xffff0000;
#else
  s32 viewX = view[0] & 0xffff0000;
  s32 viewZ = view[1] & 0xffff0000;
#endif
  s32 lift = map->origin[1];
  struct Unk020034f0 *work = &gData_020034f0;
  u32 tile = gResourceTableEntries[work->block].offset >> 5;
  struct Actor *source;
  struct Unk020034f0Entry *entry;
  s32 i, x, y, z, screenX, screenY;

  if (Random16Far() < 0x1000 && (source = work->source) != 0) {
    entry = &work->entries[work->next];
    entry->x = source->x + Random16Far() * 3 * 16 - (24 << 16);
    entry->y = source->y;
    entry->z = source->z;
    entry->age = 0;
    if (++work->next > 63)
      work->next = 0;
  }
  entry = work->entries;
  for (i = 63; i >= 0; i--, entry++) {
    if (entry->x != 0) {
      // screenX holds the ground's height here, before its own value.
      screenX = Map_GetTerrainHeight(0, entry->x, entry->z);
      if (screenX > entry->y)
        entry->y += 2 << 16;
      else
        entry->z -= 2 << 16;
      if (entry->y >> 20 == 23)
        entry->x = 0;
      if (entry->z >> 20 == 48)
        entry->x = 0;
      entry->age++;
      x = entry->x - viewX;
      y = entry->y;
      z = entry->z;
      y -= lift;
      z -= viewZ;
      z -= lift;
      screenY = z - y;
      y += z;
      y >>= 16;
      screenX = x >> 16;
      x = screenX - 4;
      screenY >>= 16;
      screenY -= 4;
      z = y + 58;
      // screenX now holds its offset into the range test (on screen from
      // -11 to 323).
      screenX += 11;
      if ((u32)screenX <= 334 && screenY > -16 && screenY <= 239) {
        x &= 0x1ff;
        screenY &= 255;
        entry->oam[0] = 0;
        entry->oam[1] = screenY | (x << 16) | 0x2000;
        entry->oam[2] = (tile + (entry->age & 3) * 2) | 0x400;
        Func_020028d0(entry->oam, z);
      }
    }
  }
}
