// Draft, differ 339 of 252: draws the rising platforms' columns as sprites,
// one 16-pixel piece per step of height and a bobbing top, up to 80. Left:
// register choice and the frame (the native keeps the view through one
// pointer and the entry pointer on the stack).
#include "types.h"
#include "ram_buffer.h"
#include "map.h"
#include "iwram_call.h"
#include "actor.h"

/// One rising platform and what rides it.
struct RideEntry {
  s16 id;
  s16 timer;
  s16 height;
  s16 period;
  s16 riseAt;
  s16 fallAt;
  s16 max;
  s16 min;
  s16 unk10;
  s16 step;
  s16 unk14;
  s16 phase;
  struct Actor *rider;
};

struct RideWork {
  s16 entry;
  s16 value;
  struct RideEntry entries[16];
  u32 *block;
};

struct ResourceTableEntry {
  u16 unk0;
  u16 offset;
};

extern u8 gData_020023c4[];
extern struct ResourceTableEntry gResourceTableEntries[];
extern u32 gFrameCount;

struct Actor *Actor_GetById(s32 id);
void Func_02003f88(u32 *oam, s32 depth);
s32 Math_Sine(s32 angle);

void Func_02003b80(void) {
  struct RideWork *work = (struct RideWork *)(gData_020023c4 + 0x188);
  struct RideEntry *entry = work->entries;
  u32 tile = gResourceTableEntries[work->entry].offset >> 5;
  u32 *oam = work->block;
  s32 count = 0;
  struct MapWork *map = Ram_HeapSlots->mapWork;
  s32 viewX = map->viewX & 0xffff0000;
  s32 viewZ = map->viewZ & 0xffff0000;
  s32 originY = map->origin[1];
  s32 i;

  for (i = 15; i >= 0; i--, entry++) {
    struct Actor *object;
    s32 frame, h, x, y, z, sx, sy, depth;

    if (entry->id == 0 || entry->height == 0)
      continue;
    object = Actor_GetById(entry->id);
    frame = (gFrameCount % 12) / 3 * 8 + 32;
    for (h = 0; h < entry->height; h += 16) {
      if (count > 79)
        continue;
      x = object->x - viewX;
      y = object->y + (h << 16) - originY;
      z = object->z - viewZ - originY;
      sy = ((z - y) >> 16) - 16;
      sx = (x >> 16) - 8;
      depth = ((y + z) >> 16) + 58;
      if ((u32)(sx + 15) > 334 || sy <= -16 || sy > 239)
        continue;
      *oam++ = 0;
      *oam++ = (sy & 255) | (sx & 511) << 16 | 0x40002000;
      *oam = (tile + frame) | 0x800;
      oam -= 2;
      Func_02003f88(oam, depth);
      count++;
      oam += 3;
    }
    if (count > 79)
      continue;
    x = object->x - viewX;
    y = object->y + (entry->height << 16) - originY;
    z = object->z - viewZ - originY;
    sy =
        ((z - y + Iwram_MulQ16(-0x20000, Math_Sine(entry->phase)) + -0x20000) >>
         16) -
        4;
    sx = (x >> 16) - 8;
    depth = ((y + z) >> 16) + 58;
    if ((u32)(sx + 15) > 334 || sy <= -16 || sy > 239)
      continue;
    *oam++ = 0;
    *oam++ = (sy & 255) | (sx & 511) << 16 | 0x40002000;
    *oam = (tile + frame - 32) | 0x800;
    oam -= 2;
    Func_02003f88(oam, depth);
    count++;
    oam += 3;
  }
}
