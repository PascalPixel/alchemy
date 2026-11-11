#include "types.h"
#include "actor.h"

#define BYTE_AT(base, offset) (*(u8 *)((u8 *)(base) + (offset)))

s32 Func_020014d0(struct Actor *object) {
  struct Actor *target = object->watchedObject;
  s32 steps;
  s32 from;

  object->bounce = (target->x - object->x) / (steps = 10);
  /* FAKEMATCH(dead): an empty once-only block, the first of a pair of fences,
   * so the x step is stored, then the object height read, then the target
   * height; without the pair, the target height is read first. */
  do {
  } while (0);
  from = object->y;
  /* FAKEMATCH(barrier): an empty asm, the second fence of the pair above. */
  asm volatile("");
  object->unk4c = (target->y - from + 0x100000) / steps;
  object->gravity = (target->z - object->z) / steps;
  BYTE_AT(object, 98) = steps;
  return 0;
}

s32 Func_0200151c(struct Actor *object) {
  u8 left;

  object->x += object->bounce;
  object->y += object->unk4c;
  object->z += object->gravity;
  left = --BYTE_AT(object, 98);
  return left != 0;
}
