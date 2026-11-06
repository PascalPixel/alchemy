// Near miss, differ 6 of 171 (tla-en, 2026-11-06): the body inside one
// `if (object != 0)` with a single `return 0` after it gives the native's shared
// return block (23 to 6), and with that the three loops are plain for loops
// (6 as with labels). Left: the call takes &cell from r5, where the native
// recomputes sp + 28 (the cell.x stores use r5 in both), and movs r0, #1 is
// scheduled two rows early. A forwarding inline for Func_02004a64 gives 4
// (permuter), a wrapper device, so not kept. Older notes below predate this.
// Near miss, differ 219 of 171 (tla-en, 2026-10-31): the same
// control flow, calls, accesses and constants as the native; what is left is
// GCSE and allocation. Measures a push: finds the pushable object ahead of the
// leader (Func_020043a8, recording its number and kind in the push state), then
// steps the object's box one cell at a time along the leader's direction
// until a cell of it is blocked (Func_02004a64 returns 2); with at least one
// free step it records the end position in the push state and returns 1,
// else 0. The object stays on layer 2 while it is measured. The same listing
// as overlay 674's Func_020037d8 and 662's Func_020001d4.
// Left: gcse's PRE (07.gcse: expressions kind << 2 and sfp - 12) carries the
// box row's kind * 4 and the cell's address from before the loops through
// every pass, where the native recomputes kind * 4 at each pass's top (sp+4)
// and the cell's address in each row (r5), after the row (r2) and for the
// call (r1), and keeps kind itself on the stack (sp+20). The long-lived
// copies then take the native's registers: the native keeps the push state
// in r6, the object in r4 (saved around the cell test), j and i in r8 and r7,
// dx, dz and the step count in r9, r10 and r11, and spills the box sizes;
// here the push state takes r7, the object r8, kind * 4 r10, the cell's
// address r6, j r4 (saved around the call), the box sizes r9 and r11, and
// dx, dz and the step count are spilled. An
// analysis-only compile with -fno-gcse keeps the recomputations but not the
// native's registers (push state r8, object r10, frame 48 bytes).
// Tried 31 October: the loops as for loops (235) or only the outer one as
// for (;;) (221); the loops reading push->kind (213, but the native reads
// its stack copy, not the push state); the cell as an s32[3] or through a
// pointer (223, 229, with push->kind); steps * dx (no change).
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

extern s8 gData_02004cb0[][4];
extern s32 gData_02004cc8[];
struct Actor *Func_020043a8(s32 *direction, s32 *number, s32 *kind);
s32 Func_02004a64(struct Actor *object, struct Vector3 *cell);

static __inline__ s32 Abs(s32 value) { return value < 0 ? -value : value; }

s32 Func_02004240(struct PushState *push) {
  struct Actor *object;
  struct Vector3 cell;
  s32 direction, kind, width, height, dx, dz, steps, i, j, offset;
  u8 *layer;

  object = Func_020043a8(&direction, &push->number, &push->kind);
  if (object != 0) {
    kind = push->kind;
    width = (Abs(gData_02004cb0[kind][0]) + Abs(gData_02004cb0[kind][2])) >> 4;
    height = (Abs(gData_02004cb0[kind][1]) + Abs(gData_02004cb0[kind][3])) >> 4;
    offset = gData_02004cc8[direction];
    dx = offset & 0xffff0000;
    dz = offset << 16;
    push->unk14 = 0;
    steps = 0;
    layer = &object->terrainId;
    *layer = 2;
    push->x = object->x + dx;
    push->z = object->z + dz;
    cell.y = object->y;
    for (;;) {
      cell.z = push->z + (gData_02004cb0[kind][1] << 16);
      for (j = 0; j < height; j++) {
        cell.x = push->x + (gData_02004cb0[kind][0] << 16);
        for (i = 0; i < width; i++) {
          if (Func_02004a64(object, &cell) == 2)
            goto blocked;
          cell.x += 0x100000;
        }
        cell.z += 0x100000;
      }
      push->x += dx;
      steps++;
      push->z += dz;
    }
  blocked:
    *layer = 0;
    if (steps == 0)
      return 0;
    push->x = object->x + dx * steps;
    push->y = object->y;
    push->z = object->z + dz * steps;
    return 1;
  }
  return 0;
}
