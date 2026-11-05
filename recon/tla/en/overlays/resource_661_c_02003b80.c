// Under -Os, 5 November: differ 107 of 252 (tla-en); older figures below predate it.
// Draft, differ 196 of 252, skeleton gap 44 (tla-en, 2026-11-01): 24 removed,
// 20 added. Draws the rising platforms' columns as sprites, one 16-pixel piece
// per step of height and a bobbing top, up to 80. No device in the draft.
// Left, first: PRE (gcse.c) removes the entry->height loads at the inner loop's
// bottom test and in the top piece, inserting one after the sprite call, which
// gives the ldrh and shifts; the native keeps all four ldrsh loads, and the
// top's load is fully redundant, so no ordinary reading of the field survives.
// That PRE is the only one in the function. A diagnostic compile with -fno-gcse
// gives the native's skeleton there (differ about 151), so what stops PRE is
// the open question; a flag is out of scope here (K1).
// Ruled out at the same bytes or worse: volatile height (221, ldrh and shifts,
// so the native is not volatile); a for loop (272); entering at the bottom test
// (244); indexing work->entries[i] (303, entry is a hand-stepped pointer); a
// copy of entry read at the bottom test (196, cprop runs before PRE).
// Left, second: allocation. Native sy r6, x r7, z r5, y r8, actor r10, h r11,
// oam r9, originY on the stack. Here x r5, z r7, y r4, actor r8, h r10,
// originY r11. y takes r4 because 15 refs beat 4 * 2 calls (caller-save), which
// shifts every high register; the native's y must fail that test, and oam must
// outrank h (refs over live, h 16/100 against oam 21/143). Splitting x, y, z,
// sy, depth or p between the two pieces is worse in all 64 combinations (best
// 245 with p split); y and z split go block-local in low registers. All 720
// orders of the s32 declarations stay at 196. A -0x20000 local (270) and a
// split sum (268) are worse; the native loads that constant twice.
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
  s32 i;
  struct RideEntry *entry = work->entries;
  s32 frame;
  u32 tile = gResourceTableEntries[work->entry].offset >> 5;
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
    x = (x >> 16) - 8;
    sy = (sy >> 16) - 16;
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
    oam += 3;
    count++;
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
    depth = ((y + z) >> 16) + 58;
    x = (x >> 16) - 8;
    if ((u32)(x + 15) > 334 || sy <= -16 || sy > 239)
      continue;
    x &= 511;
    sy &= 255;
    p = oam;
    *p++ = 0;
    *p++ = sy | x << 16 | 0x40002000;
    *p = (tile + frame - 32) | 0x800;
    Func_02003f88(oam, depth);
    oam += 3;
    count++;
  }
}
