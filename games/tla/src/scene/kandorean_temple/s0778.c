#include "local.h"

struct EventWork1cc {
  u8 unk000[0x1cc];
  struct Actor *focus;
};

void Func_02000778(struct Actor *object, s32 dz) {
  struct Actor *focus =
      ((struct EventWork1cc *)Ram_HeapSlots->eventWork)->focus;
  s32 position[3];

  position[0] = object->x;
  position[1] = object->y;
  position[2] = object->z + dz;
  Func_02005180(object, position);
  position[0] = object->x;
  position[1] = object->y;
  position[2] = object->z + 0x80000;
  if (Func_02005180(object, position) > 0)
    return;
  position[0] = object->x + 0x5b333;
  position[1] = object->y;
  position[2] = object->z + 0x5b333;
  if (Func_02005180(object, position) > 0)
    return;
  position[0] = object->x - 0x5b333;
  position[1] = object->y;
  position[2] = object->z + 0x5b333;
  if (Func_02005180(object, position) > 0)
    return;
  focus->z += dz;
  object->z += dz;
}
