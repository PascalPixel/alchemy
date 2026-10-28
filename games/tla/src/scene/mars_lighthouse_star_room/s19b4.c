#include "mars_lighthouse_star_room.h"

void Func_020019b4(s32 *position, s32 speed, s32 flag) {
  struct Actor *object = (struct Actor *)Value4(
      (s32 (*)())Func_020041e4, 366, position[0], position[1], position[2]);

  if (object != NULL) {
    object->bounce = 0;
    object->gravity = 0;
    object->unk4c = speed;
    Func_02001928(object, 3);
    object->update = Func_020017c8;
    if (flag != 0)
      object->unk5c[0] = 2;
    Func_020041cc(object, 2);
  }
}

void Func_020019fc(s32 *position, s32 speed, s32 flag) {
  struct Actor *object = (struct Actor *)Value4(
      (s32 (*)())Func_020041e4, 366, position[0], position[1], position[2]);

  if (object != NULL) {
    object->bounce = 0;
    object->gravity = 0;
    object->unk4c = -speed;
    Func_02001928(object, 3);
    object->update = Func_020017c8;
    if (flag != 0)
      object->unk5c[0] = 2;
    Func_020041cc(object, 2);
  }
}

s32 Func_02001a44(struct Actor *a, struct Actor *b) {
  if (DIFF(a->x, b->x) < 0xc0000) {
    if ((u32)(a->y - b->y - 1) <= 0xffffe) {
      if (DIFF(a->z, b->z) < 0x80000)
        return 1;
    }
  }
  return 0;
}
