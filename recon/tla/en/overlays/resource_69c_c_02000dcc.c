// Draft, differ 6 of 171 (tla-en, 5 November 2026), under -Os. How far a pushed object slides: from the object Func_02000f34 finds, steps its footprint one step at a time in the push direction until a cell of it is blocked (Func_02002338 returns 2), and leaves the stopping point in move; returns 0 when it cannot move at all. Shared with overlay 6a1 and others.
// Left: the call's add r1, sp, #28 (GCSE gives it the row's pointer here) and the load of object->x ahead of the multiply, with the return value's 1 after the x store.
#include "types.h"
#include "call.h"
#include "actor.h"

struct PushMove {
  s32 x; ///< 16.16 fixed-point pixels
  s32 y;
  s32 z;
  s32 direction;
  s32 unk10;
  s32 unk14;
};

extern s8 gData_02002484[][4];
extern s32 gData_0200249c[];

struct Actor *Func_02000f34(s32 *step, s32 *a, s32 *direction);
s32 Func_02002338(struct Actor *object, s32 *position);

#define ABS(x) ((x) < 0 ? -(x) : (x))

s32 Func_02000dcc(struct PushMove *move) {
  struct Actor *object;
  s32 stepIndex;
  s32 position[3];
  s32 direction;
  s32 *p;
  s32 width;
  s32 height;
  s32 dx;
  s32 dz;
  s32 count;
  s32 row;
  s32 col;
  u8 *layer;

  object = Func_02000f34(&stepIndex, &move->unk10, p = (s32 *)&move->direction);
  if (object != 0) {
    direction = *p;
    width = (ABS(gData_02002484[direction][0]) +
             ABS(gData_02002484[direction][2])) >>
            4;
    height = (ABS(gData_02002484[direction][1]) +
              ABS(gData_02002484[direction][3])) >>
             4;
    dx = gData_0200249c[stepIndex] & 0xffff0000;
    dz = gData_0200249c[stepIndex] << 16;
    move->unk14 = 0;
    count = 0;
    layer = &object->terrainId;
    *layer = 2;
    move->x = object->x + dx;
    move->z = object->z + dz;
    position[1] = object->y;
    for (;;) {
      position[2] = move->z + (gData_02002484[direction][1] << 16);
      for (row = 0; row < height; row++) {
        p = position;
        *p = move->x + (gData_02002484[direction][0] << 16);
        for (col = 0; col < width; col++) {
          if (Func_02002338(object, position) == 2)
            goto blocked;
          *p += 0x100000;
        }
        position[2] += 0x100000;
      }
      move->x += dx;
      count++;
      move->z += dz;
    }
  blocked:
    *layer = 0;
    if (count == 0)
      return 0;
    {
      s32 x = object->x;
      move->x = x + dx * count;
    }
    move->y = object->y;
    move->z = object->z + dz * count;
    return 1;
  }
  return 0;
}
