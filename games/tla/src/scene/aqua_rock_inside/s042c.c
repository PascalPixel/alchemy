#include "local.h"

/// When object 10 reaches row 12, or object 11 column 51, stops its motion,
/// lifts it one pixel and sets its flag; object 10 also redraws two blocks of
/// row 12.
void Func_0200042c(s32 unused, s32 id) {
  struct Actor *object = Actor_Get(id);

  if (id == 10) {
    s32 row = object->z >> 20;

    if (row == 12) {
      object->motionFlags = 0;
      {
        s32 step = -0x10000;

        object->y += step;
      }
      Event_SetFlag(0x95a);
      MapGrid_CopyCellAttributes(0, 12, 1, 1, 9, row);
      MapGrid_CopyCellAttributes(0, 12, 1, 1, 13, row);
    }
  } else if (id == 11) {
    if (object->x >> 20 == 51) {
      object->motionFlags = 0;
      {
        s32 step = -0x10000;

        object->y += step;
      }
      Event_SetFlag(0x95b);
    }
  }
}
