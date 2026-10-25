// Draft, differ 235 of 171: measures a push: finds the pushable object ahead of
// the leader (Func_020043a8, recording its number and kind in the push
// state), then steps the object's box one cell at a time along the leader's
// direction until a cell of it is blocked (Func_02004a64 returns 2); with
// at least one free step it records the end position in the push state and
// returns 1, else 0. The object stays on layer 2 while it is measured. The
// same listing as overlay 674's Func_020037d8 and 662's Func_020001d4.
// Left: registers from the start (the native keeps the push state in r6 and
// the object in r4, spilled around the cell test, and the box sizes and the
// kind on the stack), which moves nearly every row.
#include "types.h"
#include "object_runtime.h"
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
struct ObjectRuntime *Func_020043a8(s32 *direction, s32 *number, s32 *kind);
s32 Func_02004a64(struct ObjectRuntime *object, struct Vector3 *cell);

static __inline__ s32 Abs(s32 value) { return value < 0 ? -value : value; }

s32 Func_02004240(struct PushState *push) {
  struct ObjectRuntime *object;
  struct Vector3 cell;
  s32 direction, kind, width, height, dx, dz, steps, i, j, offset;
  u8 *layer;

  object = Func_020043a8(&direction, &push->number, &push->kind);
  if (object == 0)
    return 0;
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
