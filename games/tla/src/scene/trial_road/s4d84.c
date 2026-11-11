#include "trial_road.h"

extern const u8 gData_020063ac[];

void Func_02004d84(u8 *text, u32 value) {
  s32 i;
  const u8 *digits = gData_020063ac;

  text += 8;
  *text = 0;
  text--;
  i = 7;
  // A goto loop, which builds the 15 on every pass: as a for loop, the 15 is
  // moved out of it (differ 13).
next:
  *text = digits[value & 15];
  value >>= 4;
  text--;
  i--;
  if (i >= 0)
    goto next;
}
