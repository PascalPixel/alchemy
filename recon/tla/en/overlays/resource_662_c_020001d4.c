// Differ 4 of 171 (tla-en, 2026-11-07): the final position[0] loads
// object->x after the multiply where the native loads it first, which also
// moves movs r0, #1.
// 7 November: an inline wrapper round the cell call (FAKEMATCH) makes the
// native rebuild add r1, sp, #28. A once-only block after position[0] moves
// movs r0, #1 behind the store but not the load; result locals, operand
// orders, types and an inline sum all leave 4.
// Finds how far a pushed object can slide in the leader's direction, probing
// its footprint cell by cell until Func_02001850 reports a wall.
// 6 November: the once-only block is gone. The head matches in plain order
// (dx, dz, position[5] = 0 before count = 0), and the outer loop computes
// probe[2] from offset * 4 + 1 between the two steps that set offset.
// Proven: offset is set in two steps at the top of the outer loop (one step,
// or both before probe[2], gives 113). The row loop must be goto or the size
// grows by 14. The cell loop's form changes nothing: for, while, do and goto
// all compile alike, and so do a struct probe, a pointer in the row, &probe[0],
// *probe and the step spelt as an assignment.
// Reading of the dumps (before 6 November): PRE and cprop put every sfp-12 on
// reg 100. Only r5 stays apart in the native: the row store and the cell step
// share a pseudo that the call's argument does not. Ours has the step reuse the
// argument's pseudo.
// Tried, no gain: declaration orders, a scoped pointer in the step, a union
// with direct y and z, y set inside the loop, kind read after the first
// section, result in a local in four placements, the tail's operands swapped,
// the tail as if/else.
#include "types.h"
#include "call.h"
#include "actor.h"

#define ABS(v) ((v) < 0 ? -(v) : (v))

extern s8 gData_02001974[];
extern s32 gData_0200198c[];

struct Actor *Func_0200033c(s32 *direction, s32 *slot, s32 *kind);
s32 Func_02001850(struct Actor *object, s32 *position);

static inline s32 Func_02001850_Call(struct Actor *object, s32 *position) {
  return Func_02001850(object, position);
}

s32 Func_020001d4(s32 *position) {
  s32 probe[3];
  s32 direction;
  struct Actor *object;
  s32 kind, w, h, dx, dz, count, i, j;
  u8 *state;
  s32 offset;

  object = Func_0200033c(&direction, &position[4], &position[3]);
  if (object != NULL) {
    kind = position[3];
    w = (ABS(gData_02001974[kind * 4]) + ABS(gData_02001974[kind * 4 + 2])) >>
        4;
    h = (ABS(gData_02001974[kind * 4 + 1]) +
         ABS(gData_02001974[kind * 4 + 3])) >>
        4;
    dx = gData_0200198c[direction] & 0xffff0000;
    dz = gData_0200198c[direction] << 16;
    position[5] = 0;
    count = 0;
    state = &object->terrainId;
    *state = 2;
    position[0] = object->x + dx;
    position[2] = object->z + dz;
    probe[1] = object->y;
  next:
    offset = kind;
    probe[2] = position[2] + (gData_02001974[offset * 4 + 1] << 16);
    offset *= 4;
    j = 0;
    if (j >= h)
      goto advance;
  row:
    probe[0] = position[0] + (gData_02001974[offset] << 16);
    for (i = 0; i < w; i++) {
      /* FAKEMATCH(wrapper): the call through Func_02001850_Call; as
         Func_02001850(object, probe), 6 of 171 instructions differ in tla-en.
       */
      if (Func_02001850_Call(object, probe) == 2)
        goto hit;
      probe[0] += 0x100000;
    }
    probe[2] += 0x100000;
    j++;
    if (j < h)
      goto row;
  advance:
    position[0] += dx;
    count++;
    position[2] += dz;
    goto next;
  hit:
    *state = 0;
    if (count == 0)
      return 0;
    position[0] = object->x + dx * count;
    position[1] = object->y;
    position[2] = object->z + dz * count;
    return 1;
  } else {
    return 0;
  }
}
