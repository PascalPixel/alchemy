#include "types.h"

void Func_0815b410(void) {
  s32 i = 0;
  u16 *color = (u16 *)0x05000002;

next:
  {
    s32 level = i / 2;

    i++;
    *color = level << 10 | level << 5 | level;
  }
  color++;
  if (i != 63)
    goto next;
}
