// Draft, differ 235 of 171: overlay 674's copy of overlay 668's
// Func_02004240 (the same listing), whose draft holds its notes.
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

extern s8 gData_0200407c[][4];
extern s32 gData_02004094[];
struct ObjectRuntime *Func_02003940(s32 *direction, s32 *number, s32 *kind);
s32 Func_02003e2c(struct ObjectRuntime *object, struct Vector3 *cell);

static __inline__ s32 Abs(s32 value) { return value < 0 ? -value : value; }

s32 Func_020037d8(struct PushState *push) {
  struct ObjectRuntime *object;
  struct Vector3 cell;
  s32 direction, kind, width, height, dx, dz, steps, i, j, offset;
  u8 *layer;

  object = Func_02003940(&direction, &push->number, &push->kind);
  if (object == 0)
    return 0;
  kind = push->kind;
  width = (Abs(gData_0200407c[kind][0]) + Abs(gData_0200407c[kind][2])) >> 4;
  height = (Abs(gData_0200407c[kind][1]) + Abs(gData_0200407c[kind][3])) >> 4;
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
  for (;;) {
    cell.z = push->z + (gData_0200407c[kind][1] << 16);
    for (j = 0; j < height; j++) {
      cell.x = push->x + (gData_0200407c[kind][0] << 16);
      for (i = 0; i < width; i++) {
        if (Func_02003e2c(object, &cell) == 2)
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
