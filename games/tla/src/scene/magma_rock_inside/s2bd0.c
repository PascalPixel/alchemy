#include "local.h"

#define BYTE_AT(base, offset) (*(u8 *)((u8 *)(base) + (offset)))

s32 Func_02002bd0(struct Actor *object) {
  struct Actor *target = object->watchedObject;
  s32 steps;
  s32 from;

  object->bounce = (target->x - object->x) / (steps = 10);
  /* FAKEMATCH(dead): an empty once-only block, the first fence of a pair, so
   * the x step is stored, then the object height read, then the target
   * height; as no block, the target height is read first, not measured. */
  do {
  } while (0);
  from = object->y;
  /* FAKEMATCH(barrier): an empty asm, the second fence of the pair above; as
   * no asm, not measured. */
  asm volatile("");
  object->unk4c = (target->y - from + 0x100000) / steps;
  object->gravity = (target->z - object->z) / steps;
  BYTE_AT(object, 98) = steps;
  return 0;
}
