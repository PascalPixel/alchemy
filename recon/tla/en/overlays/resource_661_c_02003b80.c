// Near miss, differ 196 of 252 (tla-en, 2026-10-31): draws the rising
// platforms' columns as sprites, one 16-pixel piece per step of height and a
// bobbing top, up to 80. The shape is the native's (declaration order gives
// its stack slots; the masks come before the stores). Left: GCSE keeps
// entry->height in a register through the inner loop and reloads it only
// after the call (the native reloads it with ldrsh at every test), and the
// register allocation follows from that: originY takes r11 instead of a stack
// slot, h r10 and the actor r8 (native: h r11, actor r10, y r8, x r7, z r5).
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
  s32 i;
  u32 tile = gResourceTableEntries[work->entry].offset >> 5;
  s32 frame;
  u32 *oam = work->block;
  s32 count = 0;
  struct MapWork *map = Ram_HeapSlots->mapWork;
  s32 *view = &map->viewX;
  s32 viewX, viewZ, originY;
  struct Actor *object;
  s32 h, x, y, z, sy, depth;
  u32 *p;

  viewX = view[0];
  viewX &= 0xffff0000;
  viewZ = view[1] & 0xffff0000;
  originY = map->origin[1];
  for (i = 15; i >= 0; i--, entry++) {
    if (entry->id == 0 || entry->height == 0)
      continue;
    object = Actor_GetById(entry->id);
    frame = (gFrameCount % 12) / 3 * 8 + 32;
    // A goto loop: the ROM moves nothing out of it; as a for loop, 272 of 252
    // instructions differ in tla-en (the sprite's third word moves out).
    h = 0;
    if (h >= entry->height)
      goto top;
  again:
    if (count > 79)
      goto next;
    x = object->x - viewX;
    y = object->y + (h << 16) - originY;
    z = object->z - viewZ - originY;
    sy = z - y;
    sy = (sy >> 16) - 16;
    x = (x >> 16) - 8;
    depth = ((y + z) >> 16) + 58;
    if ((u32)(x + 15) > 334 || sy <= -16 || sy > 239)
      goto next;
    x &= 511;
    sy &= 255;
    p = oam;
    *p++ = 0;
    *p++ = sy | x << 16 | 0x40002000;
    *p = (tile + frame) | 0x800;
    Func_02003f88(oam, depth);
    count++;
    oam += 3;
  next:
    h += 16;
    if (h < entry->height)
      goto again;
  top:
    if (count > 79)
      continue;
    x = object->x - viewX;
    y = object->y + (entry->height << 16) - originY;
    z = object->z - viewZ - originY;
    sy = z - y;
    sy = ((sy + Iwram_MulQ16(-0x20000, Math_Sine(entry->phase)) + -0x20000) >>
          16) -
         4;
    x = (x >> 16) - 8;
    depth = ((y + z) >> 16) + 58;
    if ((u32)(x + 15) > 334 || sy <= -16 || sy > 239)
      continue;
    x &= 511;
    sy &= 255;
    p = oam;
    *p++ = 0;
    *p++ = sy | x << 16 | 0x40002000;
    *p = (tile + frame - 32) | 0x800;
    Func_02003f88(oam, depth);
    count++;
    oam += 3;
  }
}
