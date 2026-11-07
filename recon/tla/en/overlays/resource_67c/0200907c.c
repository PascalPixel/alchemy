// Matches (tla-en, 2026-11-07).
// Taken from its matched twin in games/tla/src/scene/aqua_rock_inside/s101c.c.
#include "../../../../../games/tla/src/scene/aqua_rock_inside/local.h"

/// When object 10 or 11 arrives, stops both and redraws row 33 at each one's
/// column.
void Func_0200107c(s32 unused, s32 id) {
  if ((u32)(id - 10) <= 1) {
    struct Actor *object = Actor_GetById(10);

    object->motionFlags = 0;
    {
      s32 row = 33;

      Func_02003d1c_(8, row, 1, 1, object->x >> 20, 34);
    }
    object = Actor_GetById(11);
    object->motionFlags = 0;
    {
      s32 row = 33;

      Func_02003d1c(8, row, 1, 1, object->x >> 20, 34);
    }
  }
}
