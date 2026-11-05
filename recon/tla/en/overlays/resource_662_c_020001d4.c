// Draft, differ 34 of 171 (tla-en, 2026-11-05). Older notes below may predate this figure.
// Under -Os, 5 November: differ 36 of 171 (tla-en); older figures below predate it.
// Draft, differ 173 of 171, skeleton gap 34 (tla-en, 2026-11-01)
// Finds how far a pushed object can slide in the leader's direction, probing
// its footprint cell by cell until Func_02001850 reports a wall.
// Draft is 172 instructions. Control flow matches: the failed search is an else
// at the end (the far b to the shared movs r0, #0), and count is set before
// position[5] takes it. The compiler change did not close this one. Left:
// allocation, and the one extra instruction goes with it.
// Proven: offset is set in two steps at the top of the outer loop, so PRE
// neither merges it with the first section nor hoists it (native sp+4). The row
// loop must be goto or the size grows by 14. The cell loop's form changes
// nothing: for, while, do and goto all compile alike, and so do a struct probe
// and a pointer in the row.
// Reading of the dumps: PRE and cprop put every sfp-12 on reg 100 (25 refs,
// taken in r6). The native reg 100 is unallocated, and its reg_equiv explains
// [sp,#32], [sp,#36], add r1, sp, #28 and, through a two-use copy that combine
// leaves, add r2, sp, #28. Only r5 stays apart: the row store and the cell step
// share a pseudo that the call's argument does not. Ours has the step reuse the
// argument's reg 121, which cprop 2 turns into reg 100. A copy of the address
// in the step folds in cse1. Next: anything that leaves the step outside the
// argument's extended block with no extra code, so loop.c can hoist it (the
// label would have to survive jump 1). Native also has r7 for i after r5 and
// r6, which priorities alone do not explain: i outranks position.
// Tried, no gain: declaration orders, a scoped pointer in the step, a union
// with direct y and z, y set inside the loop, kind read after the first
// section, result in a local (same code as the returns).
#include "types.h"
#include "call.h"
#include "actor.h"

#define ABS(v) ((v) < 0 ? -(v) : (v))

extern s8 gData_02001974[];
extern s32 gData_0200198c[];

struct Actor *Func_0200033c(s32 *direction, s32 *slot, s32 *kind);
s32 Func_02001850(struct Actor *object, s32 *position);

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
    count = 0;
    dz = gData_0200198c[direction] << 16;
    position[5] = count;
    state = &object->terrainId;
    /* FAKEMATCH(one-pass): the statement in a once-only block, whose end holds the
     * order around it; as the plain statement, 36 of 171 instructions differ
     * in tla-en. */
    do {
      dx = gData_0200198c[direction] & 0xffff0000;
    } while (0);
    *state = 2;
    position[0] = object->x + dx;
    position[2] = object->z + dz;
    probe[1] = object->y;
  next:
    offset = kind;
    offset *= 4;
    probe[2] = position[2] + (gData_02001974[offset + 1] << 16);
    j = 0;
    if (j >= h)
      goto advance;
  row:
    probe[0] = position[0] + (gData_02001974[offset] << 16);
    for (i = 0; i < w; i++) {
      if (Func_02001850(object, probe) == 2)
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
