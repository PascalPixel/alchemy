#include "types.h"

extern const u16 gData_0802f064[];

/// Steps a number from 0 to 715 by delta: a step of one wraps around the
/// range, a larger step forward goes to the next stop above value and one back
/// to the last stop below it (gData_0802f064, ended by 0xffff, from which a
/// step forward wraps to 0). A delta of 0 returns value unchanged.
s32 Func_0802e444(s32 value, s32 delta) {
  s32 step;

  if (delta != 0) {
    step = delta;
    if (delta < 0)
      step = -delta;
    if (step == 1) {
      value += delta;
      if (value < 0)
        value = 715;
      if (value >= 716)
        value = 0;
    } else if (delta > 0) {
      u32 i;

      for (i = 0; i < 10; i++) {
        if (value < gData_0802f064[i]) {
          value = gData_0802f064[i];
          if (value == 0xffff)
            value = 0;
          break;
        }
      }
    } else if (value == 0) {
      value = gData_0802f064[8];
    } else {
      s32 i;

      for (i = 8; i >= 0; i--) {
        if (value > gData_0802f064[i]) {
          value = gData_0802f064[i];
          break;
        }
      }
    }
    return value;
  }
#ifdef UBFIX
  return value;
#else
  // UB: no return statement for a delta of 0; the native leaves value in r0.
#endif
}
