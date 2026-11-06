// Differ 28 of 126 (tla-en, 2026-11-06), unchanged: x, z, fromX, fromZ in that order
// (the native's read order) gives 44; the permuter's best in 5 minutes was 66.
// Draft, differ 28 of 126 (tla-en, 2026-11-05). Older notes below may predate this figure.
// Under -Os, 5 November: differ 28 of 126 (tla-en); older figures below predate it.
// Draft, differ 60 of 126, skeleton gap 3 (tla-en, 2026-11-01)
// For a pushable object (its animation one of the six kinds in gData_02004ca4)
// copies the cells under its box (gData_02004cb0: four signed bytes a kind, in
// sixteenths of a cell) from the map's spare area (Func_02004a54) and fills both
// layers' byte 2 there with 255; returns 1, or 0 for an object of no known kind.
// The same listing as overlay 674's Func_02003684, whose search this takes.
// The compiler change closed nothing by itself. Rebuilt from 674's draft: 93 to
// 60, no device. The search stays a goto loop: for, while and break hoist the
// step (gap 12). (i << 16) + 65536 is what keeps the listing's step; i++ gives
// gap 5, next = i + 1 gap 3.
// Left, gap: the order of the map reads against x's >> 20 and their narrowing.
// fromX = (u16)fromX after both reads, in place of toX and toZ, reaches differ 56,
// gap 0, as 674 does. It is not used here: it reuses a local, which needs a
// ruling, and the native keeps the wide value and the narrowed one in different
// registers (r1, r2), so it is not the source's form either. A separate narrowed
// local gives gap 3 to 5.
// Left, allocation (read from the lreg and greg dumps at differ 56):
// 1. The native gives i r1 and kind << 16 r4; this gives them r4 and r1. Priority
// is i 7 refs over 28 insns, the shift 4 over 13, so the shift goes first. One
// more ref on i, or i shorter than 23 insns, would swap them. A later i = 0, a
// 32-bit kind (folds to ldrh) and declaration orders do not.
// 2. The native gives found r2, then found * 4 r2, abs(box 0) r1 and box 1 r1;
// this gives found r3 (it prefers r3 and does not conflict with it), the offset
// r1, and r2 for the other two. The offset (5 refs over 17) loses to box 1 (5
// over 15). The instruction stream fixes those counts, so the native looks like
// one pseudo for found and its offset. found *= 4 builds that (reg 45, 8 refs
// over 21) but is the same reuse, stays at differ 56 and takes r1, not r2, so
// it does not settle this. Tried, no gain: flat table, found as s32, u32 or
// (u16)i, box bytes and absolute values as locals, height in two steps, swapped
// compares, an ABS macro.
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
  u16 found;
  s32 fromX, fromZ, toX, toZ;
  s32 width, height, x, z;

  if ((u16)kind != gData_02004ca4[(u16)i]) {
  again:
    next = (i << 16) + 65536;
    index = (u32)next >> 16;
    i = next >> 16;
    if (index <= 5 && (u16)kind != gData_02004ca4[index])
      goto again;
  }
  found = i;
  if (found == 6)
    return 0;
  width = (Abs(gData_02004cb0[found][0]) + Abs(gData_02004cb0[found][2])) >> 4;
  height = (Abs(gData_02004cb0[found][1]) + Abs(gData_02004cb0[found][3])) >> 4;
  x = (object->x + (gData_02004cb0[found][0] << 16)) >> 20;
  fromX = *(s32 *)(map + 0x148) >> 20;
  z = (object->z + (gData_02004cb0[found][1] << 16)) >> 20;
  fromZ = *(s32 *)(map + 0x14c) >> 20;
  toX = x + (u16)fromX;
  toZ = z + (u16)fromZ;
  Func_02004a54(x, z, width, height, toX, toZ);
  Map_FillCellRectByte2(0, x, z, width, height, 255);
  Map_FillCellRectByte2(2, x, z, width, height, 255);
  return 1;
}
