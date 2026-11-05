// Draft, differ 26 of 126 (tla-en, 2026-11-05). Older notes below may predate this figure.
// Under -Os, 5 November: differ 26 of 126 (tla-en); older figures below predate it.
// Draft, differ 56 of 126, skeleton gap 0 (tla-en, 2026-11-01): overlay 674's
// copy of overlay 668's Func_020040ec (the same listing, calling Func_02003790
// where 668 calls Map_FillCellRectByte2). The instruction stream matches; only
// allocation is left. The native keeps i in r1 and the shifted kind in r4 where
// this gives i r4 and the shift r1, then the found index in r2 where this
// gives it r3 with a separate r1 for index * 4, and r1 and r2 swap from there
// to the second Func_02003e24 argument. Declaration order, a late i = 0, a
// 32-bit kind or i, the search as gotos or a block, and locals for the box bytes
// and their absolute values all leave differ at 56 or worse; right and bottom as
// locals reach 50 to 52 with skeleton gap 2 to 4. The search is a goto loop
// because any C loop gets loop.c's strength reduction, which the native lacks;
// (i + 1) * 65536 is the only form found for its index + 0x10000 step.
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
  u16 found;
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
  found = i;
  if (found == 6)
    return 0;
  a = gData_0200407c[found][0];
  width = (Abs(a) + Abs(gData_0200407c[found][2])) >> 4;
  b = gData_0200407c[found][1];
  height = (Abs(b) + Abs(gData_0200407c[found][3])) >> 4;
  x = (object->x + (a << 16)) >> 20;
  fromX = *(s32 *)(map + 0x148) >> 20;
  z = (object->z + (b << 16)) >> 20;
  fromZ = *(s32 *)(map + 0x14c) >> 20;
  fromX = (u16)fromX;
  Func_02003e24(x, z, width, height, x + fromX, z + (u16)fromZ);
  Func_02003790(0, x, z, width, height, 255);
  Func_02003790(2, x, z, width, height, 255);
  return 1;
}
