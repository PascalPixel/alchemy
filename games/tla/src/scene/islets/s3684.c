/// @file
/// Rewrites the map cells under a pushable actor. The actor's sprite
/// shape picks one of the six footprints in gData_0200407c (four signed bytes
/// each, in sixteenths of a cell); the cells under it are copied from the
/// map's spare area (Map_CopyCellAttributeRect) and byte 2 of both collision
/// layers there is filled with 255 (Func_02003790). Returns 1, or 0 for a shape
/// with no footprint. The twin of overlay 668's Func_020040ec.

#include "islets.h"
#include "event_inline.h"

extern s8 gData_0200407c[][4];
void Func_02003790(s32 layer, s32 x, s32 z, u32 width, u32 height, s32 value);

static __inline__ s32 Abs(s32 value) { return value < 0 ? -value : value; }

s32 Func_02003684(s32 id) {
  u8 *map = (u8 *)Ram_HeapSlots->mapWork;
  struct Actor *actor = Actor_GetById(id);
  s16 kind = *((struct ObjectAnimationState *)actor->animation)->value28;
  s16 i = 0;
  s32 next;
  u32 index;
  s32 a, b;
  s32 fromX, fromZ;
  s32 width, height, x, z;

  // A goto loop: a C loop is strength-reduced, which the original's is not.
  if ((u16)kind != gData_02004070[(u16)i]) {
  again:
    next = (i + 1) * 65536;
    index = (u32)next >> 16;
    i = next >> 16;
    if (index <= 5 && (u16)kind != gData_02004070[index])
      goto again;
  }
  // fromX and b are reused for the next values they hold.
  fromX = (u16)i;
  if (fromX == 6)
    return 0;
  a = gData_0200407c[fromX][0];
  width = (Abs(a) + Abs(gData_0200407c[fromX][2])) >> 4;
  b = gData_0200407c[fromX][1];
  height = (Abs(b) + Abs(gData_0200407c[fromX][3])) >> 4;
  x = actor->x + (a << 16);
  z = actor->z + (b << 16);
  b = *(s32 *)(map + 0x148) >> 20;
  x >>= 20;
  z >>= 20;
  fromZ = *(s32 *)(map + 0x14c) >> 20;
  fromX = (u16)b;
  Map_CopyCellAttributeRect(x, z, width, height, x + fromX, z + (u16)fromZ);
  Func_02003790(0, x, z, width, height, 255);
  Func_02003790(2, x, z, width, height, 255);
  return 1;
}
