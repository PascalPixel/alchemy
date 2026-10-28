#include "types.h"
#include "actor.h"

s32 Func_080eab70(struct Actor *object) {
  return object->x / 0x100000 + (object->z / 0x100000) * 128;
}
