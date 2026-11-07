#include "types.h"
#include "actor.h"

s32 Func_020006a4(struct Actor *object) {
  s32 x = object->targetX;

  if (x == 0x80000000 && object->targetZ == 0x80000000)
    return 1;
  return 0;
}
