#include "scene.h"

s32 Func_020021f0(struct FieldActor *object) {
  s32 x = object->targetX;

  asm volatile(
      ""); /* FAKEMATCH: ⚓️ loads the target before it builds 0x80000000 */
  if (x == ACTOR_NO_TARGET && object->targetZ == x)
    return 1;
  return 0;
}
