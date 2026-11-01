// Draft, differ 160 of 171, skeleton gap 46 (tla-en, 2026-11-01): overlay 674's
// copy of overlay 668's Func_02004240 (the same listing); what it measures is
// in that draft's header.
// Agrees now: the frame (40), every spill slot, and the whole box measurement
// through the direction lookup. What got there, each proven by measurement:
// width and height are one 8-byte local (+12, +16): a DImode pseudo that loses
// the two-register allocation, which is why the hottest counters' bounds are
// spilled (memory locals or a taken address instead give base pseudos and frame
// 44 or 48). The kind index and layer are a second pair (+4, +8), the only way
// found to spill the index while dx, dz and steps keep registers (plain local:
// differ 185). The index is set each pass from kind's address-taken pseudo,
// which keeps PRE from merging it with the box measurement's shift. kind's
// address is taken (LoadKind) before the pairs' block so its pseudo, and slot
// +20, come first. These inlines and the block are shaping, not known source.
// Left, all allocation plus one reload form:
// - sfp - 12 stays one PRE-carried pseudo (25 refs) and takes r6; the native
//   has no such register: cell.y and cell.z go straight to sp, r5 is set per
//   row, r2 after the row, and the call takes add r1, sp. Ending the CSE block
//   before cell.x += (do/while round the test) makes loop.c hoist a row pseudo
//   into r5, as the native, but adds a spill and 10 instructions (differ 182).
//   Lower that pseudo's priority and r5, r6, r7, r8 should follow: here i r5,
//   push r7, j r4, object r8; native r5 row cell, push r6, i r7, j r8, object
//   r4; dx, dz, steps fp, r9, sl against r9, sl, fp.
// - The row's table read goes through the pair's subreg, so reload emits adds
//   and movs #0 where the native has ldrsb r2, [r3, r1]. BoxByte(index) fixed
//   the pass top; a row-local copy loses the slots (173).
// Tried, no gain: real for loops for the row (196) or pass (192), do/while row
// (196), goto inner loop (160, same), a row pointer to the cell (cprop removes
// it), cell fields stored as s32 array elements (191), every order of dx, dz,
// steps and of i, j (same), by-value box inlines (PRE merges the shift, 213),
// a struct table of s8 fields (235).
#include "types.h"
#include "actor.h"
#include "system/vector3.h"

struct PushState {
  s32 x;
  s32 y;
  s32 z;
  s32 kind;
  s32 number;
  s32 unk14;
};

extern s8 gData_0200407c[][4];
extern s32 gData_02004094[];
struct Actor *Func_02003940(s32 *direction, s32 *number, s32 *kind);
s32 Func_02003e2c(struct Actor *object, struct Vector3 *cell);

static __inline__ s32 Abs(s32 value) { return value < 0 ? -value : value; }

static __inline__ s32 BoxByte(s32 index) {
  return ((s8 *)gData_0200407c)[index];
}

static __inline__ s32 KindIndex(s32 *kind) { return *kind * 4; }

static __inline__ s32 BoxSpan(s32 *kind, s32 start) {
  return (Abs(gData_0200407c[*kind][start]) +
          Abs(gData_0200407c[*kind][start + 2])) >>
         4;
}

static __inline__ void LoadKind(s32 *kind, struct PushState *push) {
  *kind = push->kind;
}

s32 Func_020037d8(struct PushState *push) {
  struct Actor *object;
  struct Vector3 cell;
  s32 direction, kind;
  s32 dx, dz, steps, i, j, offset, result;

  object = Func_02003940(&direction, &push->number, &push->kind);
  if (object != 0) {
    LoadKind(&kind, push);
    {
      struct {
        s32 height;
        s32 width;
      } size;
      struct {
        s32 index;
        u8 *layer;
      } box;
    size.width = BoxSpan(&kind, 0);
    size.height = BoxSpan(&kind, 1);
    offset = gData_02004094[direction];
    dx = offset & 0xffff0000;
    dz = offset << 16;
    push->unk14 = 0;
    steps = 0;
    box.layer = &object->terrainId;
    *box.layer = 2;
    push->x = object->x + dx;
    push->z = object->z + dz;
    cell.y = object->y;
  nextStep:
    box.index = KindIndex(&kind);
    cell.z = push->z + (BoxByte(box.index + 1) << 16);
    j = 0;
    if (j >= size.height)
      goto stepFree;
  nextRow:
    cell.x = push->x + (BoxByte(box.index) << 16);
    for (i = 0; i < size.width; i++) {
      if (Func_02003e2c(object, &cell) == 2)
        goto blocked;
      cell.x += 0x100000;
    }
    cell.z += 0x100000;
    j++;
    if (j < size.height)
      goto nextRow;
  stepFree:
    push->x += dx;
    steps++;
    push->z += dz;
    goto nextStep;
  blocked:
    *box.layer = 0;
    result = 0;
    if (steps != 0) {
      push->x = object->x + dx * steps;
      push->y = object->y;
      push->z = object->z + dz * steps;
      result = 1;
    }
    }
  } else
    result = 0;
  return result;
}
