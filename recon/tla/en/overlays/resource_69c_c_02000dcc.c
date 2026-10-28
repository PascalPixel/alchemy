// Draft, differ 219 of 171: how far a pushed object slides: from the object
// Func_02000f34 finds, steps its footprint one step at a time in the push
// direction until a cell of it is blocked (Func_02002338 returns 2), and
// leaves the stopping point in move; returns 0 when it cannot move at all.
// Shared with overlay 6a1 and others.
#include "types.h"
#include "actor.h"

struct PushMove {
  s32 x; ///< 16.16 fixed-point pixels
  s32 y;
  s32 z;
  s32 direction;
  s32 unk10;
  s32 unk14;
};

extern const s8 gData_02002484[][4];
extern const s32 gData_0200249c[];

struct Actor *Func_02000f34(s32 *step, s32 *a, s32 *direction);
s32 Func_02002338(struct Actor *object, s32 *position);

#define ABS(x) ((x) < 0 ? -(x) : (x))

s32 Func_02000dcc(struct PushMove *move) {
  struct Actor *object;
  s32 stepIndex;
  s32 position[3];
  s32 direction;
  s32 width;
  s32 height;
  s32 dx;
  s32 dz;
  s32 count;
  s32 row;
  s32 col;
  u8 *layer;

  object = Func_02000f34(&stepIndex, &move->unk10, &move->direction);
  if (object == 0)
    return 0;
  direction = move->direction;
  width = (ABS(gData_02002484[direction][0]) +
           ABS(gData_02002484[direction][2])) >> 4;
  height = (ABS(gData_02002484[direction][1]) +
            ABS(gData_02002484[direction][3])) >> 4;
  dx = gData_0200249c[stepIndex] & 0xffff0000;
  dz = gData_0200249c[stepIndex] << 16;
  move->unk14 = 0;
  count = 0;
  layer = &object->terrainId;
  *layer = 2;
  move->x = object->x + dx;
  move->z = object->z + dz;
  position[1] = object->y;
step:
  position[2] = move->z + (gData_02002484[direction][1] << 16);
  row = 0;
  if (row < height) {
  nextRow:
    position[0] = move->x + (gData_02002484[direction][0] << 16);
    col = 0;
    if (col < width) {
    nextCol:
      if (Func_02002338(object, position) == 2)
        goto blocked;
      position[0] += 0x100000;
      col++;
      if (col < width)
        goto nextCol;
    }
    position[2] += 0x100000;
    row++;
    if (row < height)
      goto nextRow;
  }
  move->x += dx;
  count++;
  move->z += dz;
  goto step;
blocked:
  *layer = 0;
  if (count == 0)
    return 0;
  move->x = object->x + dx * count;
  move->y = object->y;
  move->z = object->z + dz * count;
  return 1;
}
