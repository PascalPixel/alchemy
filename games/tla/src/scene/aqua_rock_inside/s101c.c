#include "local.h"

void Func_0200101c(s32 unused, s32 id) {
  if ((u32)(id - 13) <= 2) {
    struct Actor *object = Actor_Get(id);

    Func_02003d1c_(47, 45, 1, 1, object->x >> 20, object->z >> 20);
    Event_SetFlag(id + 0x244);
    object->priorityFlags = 0;
  }
}

void Func_02001060(void) { Func_02003d1c_(8, 63, 7, 1, 8, 34); }

/// When object 10 or 11 arrives, stops both and redraws row 33 at each one's
/// column.
void Func_0200107c(s32 unused, s32 id) {
  if ((u32)(id - 10) <= 1) {
    struct Actor *object = Actor_Get(10);

    object->motionFlags = 0;
    {
      s32 row = 33;

      Func_02003d1c_(8, row, 1, 1, object->x >> 20, 34);
    }
    object = Actor_Get(11);
    object->motionFlags = 0;
    {
      s32 row = 33;

      MapGrid_CopyCellAttributes(8, row, 1, 1, object->x >> 20, 34);
    }
  }
}
