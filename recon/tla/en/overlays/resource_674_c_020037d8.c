// Draft, differ 199 of 171, skeleton gap 65 (tla-en, 2026-11-01): overlay 674's
// copy of overlay 668's Func_02004240 (the same listing); what it measures is
// in that draft's header.
// The native keeps kind on the stack (sp+20) and rereads it each pass, which
// PRE would otherwise hoist: taking its address in an inline (&kind, gone by
// the addressof pass) keeps kind << 2 out of PRE, and the shape then agrees.
// Left: the native computes kind * 4 once per pass into a spill (sp+4) and
// uses it in the row; a box local for it (gap 82) or the plain table reads
// here (gap 67, differ 181, native's 171 less three) do not reach that. PRE
// still carries sfp - 12 from before the loops, where the native sets it in
// each row (r5), after the row (r2) and for the call (r1); a cell pointer per
// row or per use, inline row helpers and s32 cell[3] leave that as it is.
// Allocation is the rest: the native gives r9, r10, r11 to dx, dz, steps, r8
// to j and r4 to the object (saved around the test), and spills width, height
// and layer; here width and height outrank dx and dz (the lreg dump's refs
// over live length), which the priorities do not explain for the native.
// Tried: stack struct or array for kind (gaps 93, 91; the address sits in a
// base pseudo, frame 44); a nested reference to the locals (frame 52); for
// loops for the row or the pass (gaps 85, 90: loop.c reduces the row's read).
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

static __inline__ s32 RowStart(s32 *kind, s32 z) {
  return z + (gData_0200407c[*kind][1] << 16);
}

static __inline__ s32 CellStart(s32 *kind, s32 x) {
  return x + (gData_0200407c[*kind][0] << 16);
}

static __inline__ void BoxSize(s32 *kind, s32 *width, s32 *height) {
  *width = (Abs(gData_0200407c[*kind][0]) + Abs(gData_0200407c[*kind][2])) >> 4;
  *height =
      (Abs(gData_0200407c[*kind][1]) + Abs(gData_0200407c[*kind][3])) >> 4;
}

s32 Func_020037d8(struct PushState *push) {
  struct Actor *object;
  struct Vector3 cell;
  s32 direction, kind, width, height;
  s32 dx, dz, steps, i, j, offset, result;
  u8 *layer;

  object = Func_02003940(&direction, &push->number, &push->kind);
  if (object != 0) {
    kind = push->kind;
    BoxSize(&kind, &width, &height);
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
    cell.z = RowStart(&kind, push->z);
    j = 0;
    if (j >= height)
      goto stepFree;
  nextRow:
    cell.x = CellStart(&kind, push->x);
    for (i = 0; i < width; i++) {
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
      push->y = object->y;
      push->z = object->z + dz * steps;
      result = 1;
    }
  } else
    result = 0;
  return result;
}
