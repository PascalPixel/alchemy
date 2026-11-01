// Draft, differ 182 of 171, skeleton gap 40 (tla-en, 2026-11-01)
// Finds how far a pushed object can slide in the leader's direction, probing
// its footprint cell by cell until Func_02001850 reports a wall.
// Size is 171 and the first section matches to its last ABS. Left: allocation.
// Proven: offset is set in two steps at the top of the outer loop, so its
// shift's operand changes in the loop and PRE neither merges it with the first
// section nor hoists it (native sp+4). The row loop must be goto or the size
// grows by 14; the cell loop may be either form.
// Reading of the listing: w, h, offset and the frame address pseudo reach
// global allocation in low registers by caller-save and are kicked out by
// reload, which is why [sp,#32], [sp,#36] and each add rN, sp, #28 are
// rematerialized, with object in r4. Ours keeps sfp-12 (reg 100) in r6 with 25
// refs, because cprop pass 2 replaces the row's copy in the cell block. The
// native r5 (set in the row, used by the row store and the cell step) is that
// copy surviving; it would take the cell refs off the frame pseudo.
// Tried, no gain: *(probe + n), a struct probe, a pointer set in the row, the
// same pointer shared with &position[3] (r5 fits, cprop still replaces it), a
// pointer reset in the cell, a one-pass block around the step, a kind = 0
// default, builtin abs, kind read after the first section. Next: anything that
// makes the row's copy unavailable to cprop in the cell block.
#include "types.h"
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
  if (object == NULL)
    return 0;
  kind = position[3];
  w = (ABS(gData_02001974[kind * 4]) + ABS(gData_02001974[kind * 4 + 2])) >> 4;
  h = (ABS(gData_02001974[kind * 4 + 1]) + ABS(gData_02001974[kind * 4 + 3])) >> 4;
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
}
