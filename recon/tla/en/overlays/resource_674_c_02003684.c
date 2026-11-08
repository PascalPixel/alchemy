// Matches (tla-en, 2026-11-08, -mtune=gba); also differ 0 in tla-ja, tla-de, tla-es, tla-fr and tla-it.
// Before: differ 26 of 126 (tla-en, 2026-11-08). Closed by three plain-C changes, no device:
// both box sums (x and z before their >> 20) computed ahead of the map reads (alone,
// differ 40: the schedule matched, allocation did not); the found index held in fromX
// (s32) instead of a u16 found (permuter's lead, 4); and the map's 0x148 read shifted
// into b, which is dead by then, before fromX narrows it (0). Each local reuse is
// ordinary C (S2).
// Overlay 674's copy of overlay 668's Func_020040ec (the same listing, calling
// Func_02003790 where 668 calls Map_FillCellRectByte2). The search is a goto loop
// because any C loop gets loop.c's strength reduction, which the native lacks (a for
// loop gives differ 29); (i + 1) * 65536 and (i << 16) + 65536 both match.
#include "types.h"
#include "call.h"
#include "actor.h"
#include "ram_buffer.h"

extern u16 gData_02004070[];
extern s8 gData_0200407c[][4];
struct Actor *Actor_GetById(u32 id);
void Func_02003e24(s32 x, s32 z, s32 width, s32 height, s32 fromX, s32 fromZ);
void Func_02003790(s32 layer, s32 x, s32 z, s32 width, s32 height,
                           s32 value);

static __inline__ s32 Abs(s32 value) { return value < 0 ? -value : value; }

s32 Func_02003684(s32 id) {
  u8 *map = (u8 *)Ram_HeapSlots->mapWork;
  struct Actor *object = Actor_GetById(id);
  s16 kind = *((struct ObjectAnimationState *)object->animation)->value28;
  s16 i = 0;
  s32 next;
  u32 index;
  s32 a, b;
  s32 fromX, fromZ;
  s32 width, height, x, z;

  if ((u16)kind != gData_02004070[(u16)i]) {
  again:
    next = (i + 1) * 65536;
    index = (u32)next >> 16;
    i = next >> 16;
    if (index <= 5 && (u16)kind != gData_02004070[index])
      goto again;
  }
  fromX = (u16)i;
  if (fromX == 6)
    return 0;
  a = gData_0200407c[fromX][0];
  width = (Abs(a) + Abs(gData_0200407c[fromX][2])) >> 4;
  b = gData_0200407c[fromX][1];
  height = (Abs(b) + Abs(gData_0200407c[fromX][3])) >> 4;
  x = object->x + (a << 16);
  z = object->z + (b << 16);
  b = *(s32 *)(map + 0x148) >> 20;
  x >>= 20;
  z >>= 20;
  fromZ = *(s32 *)(map + 0x14c) >> 20;
  fromX = (u16)b;
  Func_02003e24(x, z, width, height, x + fromX, z + (u16)fromZ);
  Func_02003790(0, x, z, width, height, 255);
  Func_02003790(2, x, z, width, height, 255);
  return 1;
}
