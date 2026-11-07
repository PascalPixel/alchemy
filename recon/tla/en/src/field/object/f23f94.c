#include "types.h"
#include "actor.h"

s32 Actor_IsTargetUnset(struct Actor *object) {
  s32 first;
  s32 second;

  if (object->motionFlags == 0) {
    second = object->targetX;
    if (second != (s32)0x80000000)
      return 0;
    first = object->targetY;
  } else {
    first = object->targetX;
    second = (s32)0x80000000;
  }
  if (first != second || object->targetZ != first)
    return 0;
  return 1;
}

s32 Func_08023f94(struct Actor *object) {
  return Actor_IsTargetUnset(object);
}
