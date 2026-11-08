#include "local.h"

/// When object 10 reaches row 12, or object 11 column 51, stops its motion,
/// lifts it one pixel and sets its flag; object 10 also redraws two blocks of
/// row 12.
void Func_0200042c(s32 unused, s32 id) {
  /* FAKEMATCH(barrier): SPLIT_CONSTANT builds each constant as the game does, a
   * shifted byte plus an addend; as plain constants, 8 of 61 instructions
   * differ in tla-en. */
  struct Actor *object = Actor_GetById(id);

  if (id == 10) {
    s32 row = object->z >> 20;

    if (row == 12) {
      object->motionFlags = 0;
      {
        s32 step = -0x10000;

        object->y += step;
      }
      GameFlag_SetBit(SPLIT_CONSTANT(0x900, 90));
      Func_02003d1c(0, 12, 1, 1, 9, row);
      Func_02003d1c(0, 12, 1, 1, 13, row);
    }
  } else if (id == 11) {
    if (object->x >> 20 == 51) {
      object->motionFlags = 0;
      {
        s32 step = -0x10000;

        object->y += step;
      }
      GameFlag_SetBit(SPLIT_CONSTANT(0x900, 91));
    }
  }
}
