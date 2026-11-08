// Matches (tla-en, 2026-11-08, -mtune=gba); also differ 0 in tla-ja, tla-de, tla-es, tla-fr and tla-it.
// Before: differ 26 of 126 (tla-en, 2026-11-08). Closed by three plain-C changes, no device:
// both box sums (x and z before their >> 20) computed ahead of the map reads; the
// found index held in fromX (s32) instead of a u16 found; and the map's 0x148 read
// shifted into b, which is dead by then, before fromX narrows it. Each local reuse is
// ordinary C (S2). The same listing as overlay 674's Func_02003684, matched the same way.
// For a pushable object (its animation one of the six kinds in gData_02004ca4)
// copies the cells under its box (gData_02004cb0: four signed bytes a kind, in
// sixteenths of a cell) from the map's spare area (Func_02004a54) and fills both
// layers' byte 2 there with 255; returns 1, or 0 for an object of no known kind.
// The search stays a goto loop: for, while and break hoist the step (a for loop
// gives differ 29); (i + 1) * 65536 and (i << 16) + 65536 both match.
#include "types.h"
#include "call.h"
#include "actor.h"
#include "ram_buffer.h"

extern u16 gData_02004ca4[];
extern s8 gData_02004cb0[][4];
struct Actor *Actor_GetById(u32 id);
void Func_02004a54(s32 x, s32 z, s32 width, s32 height, s32 fromX, s32 fromZ);
void Map_FillCellRectByte2(s32 layer, s32 x, s32 z, s32 width, s32 height,
                           s32 value);

static __inline__ s32 Abs(s32 value) { return value < 0 ? -value : value; }

s32 Func_020040ec(s32 id) {
  u8 *map = (u8 *)Ram_HeapSlots->mapWork;
  struct Actor *object = Actor_GetById(id);
  s16 kind = *((struct ObjectAnimationState *)object->animation)->value28;
  s16 i = 0;
  s32 next;
  u32 index;
  s32 a, b;
  s32 fromX, fromZ;
  s32 width, height, x, z;

  if ((u16)kind != gData_02004ca4[(u16)i]) {
  again:
    next = (i + 1) * 65536;
    index = (u32)next >> 16;
    i = next >> 16;
    if (index <= 5 && (u16)kind != gData_02004ca4[index])
      goto again;
  }
  fromX = (u16)i;
  if (fromX == 6)
    return 0;
  a = gData_02004cb0[fromX][0];
  width = (Abs(a) + Abs(gData_02004cb0[fromX][2])) >> 4;
  b = gData_02004cb0[fromX][1];
  height = (Abs(b) + Abs(gData_02004cb0[fromX][3])) >> 4;
  x = object->x + (a << 16);
  z = object->z + (b << 16);
  b = *(s32 *)(map + 0x148) >> 20;
  x >>= 20;
  z >>= 20;
  fromZ = *(s32 *)(map + 0x14c) >> 20;
  fromX = (u16)b;
  Func_02004a54(x, z, width, height, x + fromX, z + (u16)fromZ);
  Map_FillCellRectByte2(0, x, z, width, height, 255);
  Map_FillCellRectByte2(2, x, z, width, height, 255);
  return 1;
}
