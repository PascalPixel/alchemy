#include "local.h"

/// When object 10 reaches row 12, or object 11 column 51, stops its motion,
/// lifts it one pixel and sets its flag; object 10 also redraws two blocks of
/// row 12.
void Func_0200042c(s32 unused, s32 id) {
  struct Actor *object = Actor_GetById(id);

  if (id == 10) {
    s32 row = object->z >> 20;

    if (row == 12) {
      object->motionFlags = 0;
      {
        /* FAKEMATCH(pin): the step in r1, as the native loads it; as a plain
         * local held by the barrier below, 34 of 61 instructions differ in
         * tla-en (r1 and r2 swap). */
        register s32 step asm("r1") = -0x10000;

        /* FAKEMATCH(barrier): holding the step loads -0x10000 from the pool
         * before y, as trial_road's Func_0200185c does; as object->y +=
         * -0x10000 at both lifts, 4 of 61 instructions differ in tla-en. */
        asm("" : : "l"(step));
        object->y += step;
      }
      GameFlag_SetBit(0x95a);
      Func_02003d1c(0, 12, 1, 1, 9, row);
      Func_02003d1c(0, 12, 1, 1, 13, row);
    }
  } else if (id == 11) {
    if (object->x >> 20 == 51) {
      object->motionFlags = 0;
      {
        /* FAKEMATCH(pin): as above. */
        register s32 step asm("r1") = -0x10000;

        /* FAKEMATCH(barrier): as above. */
        asm("" : : "l"(step));
        object->y += step;
      }
      GameFlag_SetBit(0x95b);
    }
  }
}
