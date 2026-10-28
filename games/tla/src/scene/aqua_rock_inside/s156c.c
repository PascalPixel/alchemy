#include "local.h"

#define BYTE_AT(base, offset) (*(u8 *)((u8 *)(base) + (offset)))

s32 Func_0200156c(struct Actor *object) {
  u8 left;

  object->x += object->bounce;
  object->y += object->unk4c;
  object->z += object->gravity;
  left = --BYTE_AT(object, 98);
  return left != 0;
}
