#include "types.h"

/// One halfword read as a struct's field.
struct HalfwordField {
  s16 value;
};

extern s16 gData_02006868;
extern s16 gData_020068a0;

void Func_0200246c(s32 a, s32 b);

void Func_02001254(void) {
  s16 *state = &gData_02006868;

  if (gData_020068a0 < 60) {
    if (*state == 0) {
      Func_0200246c(0, 3);
      *state = 1;
    }
  } else {
    // Read through a struct, so PRE does not merge it with the first arm's
    // read.
    if (((struct HalfwordField *)state)->value == 1) {
      Func_0200246c(0, 67);
      *state = 0;
    }
  }
  if (++gData_020068a0 > 300)
    gData_020068a0 = 0;
}
