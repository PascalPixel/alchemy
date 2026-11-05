// Draft, differ 34 of 171 (tla-en, 5 November 2026), from 74: no call for the kind, the
// layer set before it is written. Left: kind sits at sp+20 in the native, ahead of the
// spilled locals, and (ashift kind 2) is recomputed at nextStep; read through a pointer
// it lands at sp+4 in the frame, by value gcse shares row (131). An array of one is the
// same as the pointer.
// Under -Os, 5 November: differ 74 of 171 (tla-en); older figures below predate it.
// Draft, differ 156 of 171, skeleton gap 23 (tla-en, 2026-11-01): overlay 674's
// copy of overlay 668's Func_02004240 (the same listing); what it measures is
// in that draft's header.
// Agrees now: the frame (40), every spill slot, the whole box measurement
// through the direction lookup, and the cell's address in r5. What got there,
// each proven by measurement: width and height are one 8-byte local (+12, +16):
// a DImode pseudo that loses the two-register allocation, which is why the
// hottest counters' bounds are spilled (memory locals or a taken address
// instead give base pseudos and frame 44 or 48). The kind index and layer are a
// second pair (+4, +8), the only way found to spill the index while dx, dz and
// steps keep registers (plain local: differ 185). The index is set each pass
// from kind's address-taken pseudo, which keeps PRE from merging it with the
// box measurement's shift. kind's address is taken (LoadKind) before the pairs'
// block so its pseudo, and slot +20, come first. i = 0 above cell.x lengthens
// i's live range (26 to 40), so sfp - 12 outranks i and takes r5 (differ 160 to
// 156). These inlines and the block are shaping, not known source: a plain
// kind = push->kind costs 10 (166), plain table casts for BoxByte 7 (163). Under
// the new compiler the pairs declared at the top give 172.
// Left, not closable in C so far:
// - gcse's PRE carries sfp - 12 as one pseudo across the whole pass loop; the
//   native sets r5 per row, r2 after the row, and takes add r1, sp for the
//   call, and stores cell.y and cell.z straight to sp. gcse.c exempts the frame
//   pointer from the call clobber, so the expression stays transparent across
//   calls. No plain C stops it here; this is the frame-address-in-a-loop-with-
//   calls family, and a compiler question, so out of scope for a draft.
// - Priorities: draft i r6, push r7, j r4, object r8, dx, dz, steps fp, r9, sl;
//   native push r6, i r7, object r4, j r8, dx, dz, steps r9, sl, fp. The dumps
//   give i 18 refs/40, j 13/54, object 13/102, steps 8/138: the native's order
//   needs the loop-held pseudos lower than this compiler's loop-weighted refs
//   put them (with one ref fewer steps would fall below dx, as native). Lead
//   only: unproven, and again a compiler question.
// - The row's table read goes through the pair's subreg, so reload emits adds
//   and movs #0 where the native has ldrsb r2, [r3, r1]. Spelling it
//   gData_0200407c[0][box.index] closes that (gap 21 + 19, 169 instructions) but
//   costs differ 172, so it is left out; ((s8 *)gData_0200407c)[box.index] by
//   hand instead of BoxByte is the same code.
// Tried, no gain: real for loops for the row (196) or pass (192), do/while row
// (196), goto inner loop (160, same), a while inner loop (same), a row pointer
// to the cell (cprop removes it), cell fields stored as s32 array elements
// (191), every order of dx, dz, steps and of i, j (same), either pair's fields
// swapped (164, 166), j = 0 above cell.z (same), the operands of cell.x swapped
// (174), by-value box inlines (PRE merges the shift, 213), a struct table of s8
// fields (235), wholly plain locals and loops (243, frame 44).
#include "types.h"
#include "call.h"
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

static __inline__ s32 BoxSpan(s32 *kind, s32 start) {
  return (Abs(gData_0200407c[*kind][start]) +
          Abs(gData_0200407c[*kind][start + 2])) >>
         4;
}

s32 Func_020037d8(struct PushState *push) {
  struct Actor *object;
  struct Vector3 cell;
  s32 direction, kind;
  s32 dx, dz, steps, i, j, offset, result;

  object = Func_02003940(&direction, &push->number, &push->kind);
  if (object != 0) {
    s32 height, width, index;
    u8 *layer;

    kind = push->kind;
    width = BoxSpan(&kind, 0);
    height = BoxSpan(&kind, 1);
    offset = gData_02004094[direction];
    dx = offset & 0xffff0000;
    dz = offset << 16;
    push->unk14 = 0;
    steps = 0;
    layer = &object->terrainId;
    *layer = 2;
    push->x = object->x + dx;
    push->z = object->z + dz;
    cell.y = object->y;
  nextStep:
    index = kind * 4;
    cell.z = push->z + (BoxByte(index + 1) << 16);
    j = 0;
    if (j >= height)
      goto stepFree;
  nextRow:
    cell.x = push->x + (BoxByte(index) << 16);
    i = 0;
    for (; i < width; i++) {
      if (Func_02003e2c(object, &cell) == 2)
        goto blocked;
      cell.x += 0x100000;
    }
    cell.z += 0x100000;
    j++;
    if (j < height)
      goto nextRow;
  stepFree:
    push->x += dx;
    steps++;
    push->z += dz;
    goto nextStep;
  blocked:
    *layer = 0;
    result = 0;
    if (steps != 0) {
      push->x = object->x + dx * steps;
      result = 1;
      push->y = object->y;
      push->z = object->z + dz * steps;
    }
  } else
    result = 0;
  return result;
}
