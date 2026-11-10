#include "types.h"
#include "actor.h"
#include "event_inline.h"

s32 Func_02000038(void) {
  struct Actor *object = Actor_GetById(10);

  if ((u32)(object->x - 0x940000) <= 0x80000) {
    s32 z = object->z;

    if (z >= 0x26c0000) {
      if (z <= 0x2740000)
        return 0;
    }
  }
  return 1;
}

s32 Func_0200006c(void) {
  struct Actor *object = Actor_GetById(9);

  if ((u32)(object->x - 0x840000) <= 0x80000) {
    s32 z = object->z;

    if (z >= 0x26c0000) {
      if (z <= 0x2740000)
        return 0;
    }
  }
  return 1;
}
