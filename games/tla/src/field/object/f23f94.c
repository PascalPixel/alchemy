#include "types.h"
#include "actor.h"

s32 Actor_IsTargetUnset(struct Actor *object) {
  s32 first;
  s32 second;

  if (object->motionFlags == 0) {
    /* FAKEMATCH: the one-pass block's end loads targetX before 0x80000000 is
     * built, as native, here and in the other arm; without it, 4 instructions
     * differ in tla-en. */
    do {
      second = object->targetX;
    } while (0);
    if (second != (s32)0x80000000)
      return 0;
    first = object->targetY;
  } else {
    /* FAKEMATCH: as above. */
    do {
      first = object->targetX;
    } while (0);
    second = (s32)0x80000000;
  }
  if (first != second || object->targetZ != first)
    return 0;
  return 1;
}

s32 Func_08023f94(struct Actor *object) {
  return Actor_IsTargetUnset(object);
}
