// Near miss, differ 20 of 37: the native keeps 0 in r5 and 34 in r6 across
// the two calls and builds r1 = 33 before it reads the object x; this draft
// has the two saved registers the other way round and reads x first.
#include "../../../../../games/tla/src/scene/aqua_rock_inside/local.h"

void Func_0200107c(s32 unused, s32 id) {
  struct Actor *object;

  if ((u32)(id - 10) <= 1) {
    object = Actor_GetById(10);
    object->motionFlags = 0;
    Func_02003d1c(8, 33, 1, 1, object->x >> 20, 34);
    object = Actor_GetById(11);
    object->motionFlags = 0;
    Func_02003d1c(8, 33, 1, 1, object->x >> 20, 34);
  }
}
