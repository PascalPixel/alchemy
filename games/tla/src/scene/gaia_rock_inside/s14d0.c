#include "types.h"
#include "object_runtime.h"

#define BYTE_AT(base, offset) (*(u8 *)((u8 *)(base) + (offset)))

s32 Func_020014d0(struct ObjectRuntime *object) {
  struct ObjectRuntime *target = object->linkedObject;
  s32 steps;
  s32 from;

  object->bounce = (target->x - object->x) / (steps = 10);
  /* FAKEMATCH: ⚓️ stores the x step, then reads the object height, then the
   * target height; unfenced, the target height is read first */
  asm volatile("");
  from = object->y;
  asm volatile(""); /* FAKEMATCH: second fence of the pair above */
  object->unk4c = (target->y - from + 0x100000) / steps;
  object->gravity = (target->z - object->z) / steps;
  BYTE_AT(object, 98) = steps;
  return 0;
}

s32 Func_0200151c(struct ObjectRuntime *object) {
  u8 left;

  object->x += object->bounce;
  object->y += object->unk4c;
  object->z += object->gravity;
  left = --BYTE_AT(object, 98);
  return left != 0;
}
