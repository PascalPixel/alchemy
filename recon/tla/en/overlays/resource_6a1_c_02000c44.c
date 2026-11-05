// Draft, differ 6 (tla-en, 2026-11-05). Func_02000c44 of overlay 6a1.
// Overlay 6ad's Func_02004a8c with this overlay's symbols; see that draft.
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

extern s8 gData_020022dc[][4];
extern s32 gData_020022f4[];

struct Actor *Func_02000dac(s32 *step, s32 *a, s32 *direction);
s32 Func_020021b8(struct Actor *object, s32 *position);

#define ABS(x) ((x) < 0 ? -(x) : (x))

s32 Func_02000c44(struct PushMove *move) {
  s32 stepIndex;
  s32 position[3];
  s32 result;
  struct Actor *object = Func_02000dac(&stepIndex, &move->unk10, &move->direction);

  if (object != 0) {
    s32 direction = move->direction;
    s32 width = (ABS(gData_020022dc[direction][0]) +
                 ABS(gData_020022dc[direction][2])) >>
                4;
    s32 height = (ABS(gData_020022dc[direction][1]) +
                  ABS(gData_020022dc[direction][3])) >>
                 4;
    s32 dx = gData_020022f4[stepIndex] & 0xffff0000;
    s32 dz = gData_020022f4[stepIndex] << 16;
    s32 count;
    u8 *layer;

    move->unk14 = 0;
    count = 0;
    layer = &object->terrainId;
    *layer = 2;
    move->x = object->x + dx;
    move->z = object->z + dz;
    position[1] = object->y;
    for (;;) {
      s32 row;

      position[2] = move->z + (gData_020022dc[direction][1] << 16);
      for (row = 0; row < height; row++) {
        s32 col;
        s32 *cellX = position;

        *cellX = move->x + (gData_020022dc[direction][0] << 16);
        for (col = 0; col < width; col++) {
          if (Func_020021b8(object, position) == 2)
            goto blocked;
          *cellX += 0x100000;
        }
        position[2] += 0x100000;
      }
      move->x += dx;
      count++;
      move->z += dz;
    }
  blocked:
    *layer = 0;
    result = 0;
    if (count != 0) {
      move->x = object->x + dx * count;
      move->y = object->y;
      move->z = object->z + dz * count;
      result = 1;
    }
    return result;
  }
  return 0;
}
