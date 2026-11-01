// Draft, differ 319 of 252, skeleton gap 61 (tla-en, 2026-11-01): 31 listing
// instructions missing, 30 extra; what is left is allocation. x and height
// have each other's registers (x r8, height r7; the native has x r7, height
// r8), which costs the x-8 form (movs #8, negs, adds), and lift and entry are
// swapped (r9, r11), z and sy too (r6, r5). The frame is 32 bytes to 36.
// x's priority (refs over live length) sits just under height's; the orders of
// the x, height, z, sy and depth statements were searched in both sprites and
// this is the best skeleton (a lower differ, 281, gives gap 67).
// Found by this draft, each measured: movers[i] rather than a pointer stops
// GCSE's PRE keeping offset in a register and loop.c turns i into the native's
// pointer and down-count (the count falls to 15..0 only while nothing hoists
// i * 28 out of the inner loop); sx is x itself, and x and sy are masked in
// place. Three locals steer loop.c and allocation, untagged, and need judging
// against S2 before this adopts: attr is set twice so loop.c cannot hoist
// tile + frame; word, a constant, is the first thing hoisted, which lowers
// loop.c's threshold so i * 28 stays; rise is shared so it is rematerialized
// at both uses instead of holding r5 across Iwram_MulQ16.
// The native computes movers + 4 first and stores it at once; here the loop's
// pointer is set just before the loop. Also open: a real pointer variable
// would explain that store, but PRE then returns.
// Draws the Trial Road movers' columns: for each raised mover (of 16) it
// queues one 16 by 16 sprite per 16 pixels of its offset and a bobbing cap on
// top, animated over 4 frames every 3, projected from the map's view and kept
// to the screen, at most 80 sprites (Func_02005790 queues each three-word
// entry at its depth, as Trial Road's cell sprite does).
#include "../../../../games/tla/src/scene/trial_road/trial_road.h"
#include "iwram_call.h"
#include "frame.h"
#include "map.h"

struct ResourceTableEntry {
  u16 unk0;
  u16 offset;
};

struct TrialRoadMover {
  s16 id;
  s16 phase;
  s16 offset;
  s16 unk06[8];
  s16 bob;
  struct Actor *marker;
};

struct TrialRoadMovers {
  s16 vramEntry;
  s16 shape;
  struct TrialRoadMover movers[16];
  u32 *entries;
};

extern struct ResourceTableEntry gResourceTableEntries[];

s32 Math_Sine(s32 angle);
void Func_02005790(void *entry, s32 priority);

void Func_02003f68(void) {
  struct TrialRoadMovers *base =
      (struct TrialRoadMovers *)(gData_020023c4 + 0x188);
  struct TrialRoadMover *movers = base->movers;
  s32 tile = gResourceTableEntries[base->vramEntry].offset >> 5;
  u32 *entry = base->entries;
  s32 count = 0;
  s32 rise = -0x20000;
  struct MapWork *map = Ram_HeapSlots->mapWork;
  s32 *view = &map->viewX;
  s32 viewX = view[0] & 0xffff0000;
  s32 viewZ = view[1] & 0xffff0000;
  s32 viewY = map->origin[1];
  struct Actor *object;
  u32 *cursor;
  s32 frame;
  s32 lift;
  s32 word;
  s32 attr;
  s32 x;
  s32 height;
  s32 z;
  s32 sy;
  s32 depth;
  s32 i;

  for (i = 0; i < 16; i++) {
    if (movers[i].id == 0 || movers[i].offset == 0)
      continue;
    object = Actor_GetById(movers[i].id);
    frame = ((gFrameCount % 12) / 3) * 8 + 32;
    for (lift = 0; lift < movers[i].offset; lift += 16) {
      if (count > 79)
        continue;
      x = object->x - viewX;
      height = object->y + (lift << 16) - viewY;
      z = object->z - viewZ - viewY;
      x = (x >> 16) - 8;
      sy = z - height;
      sy = (sy >> 16) - 16;
      depth = ((height + z) >> 16) + 58;
      if ((u32)(x + 15) > 334 || sy <= -16 || sy > 239)
        continue;
      word = 0x40002000;
      x &= 0x1ff;
      sy &= 0xff;
      cursor = entry;
      *cursor++ = 0;
      *cursor++ = sy | (x << 16) | word;
      attr = tile + frame;
      attr |= 0x800;
      *cursor = attr;
      Func_02005790(entry, depth);
      count++;
      entry += 3;
    }
    if (count > 79)
      continue;
    x = object->x - viewX;
    height = object->y + (movers[i].offset << 16) - viewY;
    z = object->z - viewZ - viewY;
    sy = z - height;
    sy += Iwram_MulQ16(rise, Math_Sine(movers[i].bob));
    depth = ((height + z) >> 16) + 58;
    x = (x >> 16) - 8;
    sy = ((sy + rise) >> 16) - 4;
    if ((u32)(x + 15) > 334 || sy <= -16 || sy > 239)
      continue;
    x &= 0x1ff;
    sy &= 0xff;
    cursor = entry;
    *cursor++ = 0;
    *cursor++ = sy | (x << 16) | 0x40002000;
    *cursor = (tile + frame - 32) | 0x800;
    Func_02005790(entry, depth);
    count++;
    entry += 3;
  }
}
