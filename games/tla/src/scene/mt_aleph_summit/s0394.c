#include "types.h"
#include "event_inline.h"

extern u32 gFrameCount;

void Func_02000394(void) {
  u16 *src = (u16 *)0x050000c2;
  u16 *dst = (u16 *)0x05000082;
  s32 delta = (Math_Sine((gFrameCount << 11) >> 1) * 2) >> 16;
  s32 i = 0;

next:
  {
    s32 r = (*src & 31) + delta;
    s32 g = ((*src >> 5) & 31) + delta;
    s32 b = ((*src >> 10) & 31) + delta;

    if (r > 31)
      r = 31;
    if (r < 0)
      r = 0;
    if (g > 31)
      g = 31;
    if (g < 0)
      g = 0;
    if (b > 31)
      b = 31;
    if (b < 0)
      b = 0;
    *dst = b << 10 | g << 5 | r;
  }
  i++;
  src++;
  dst++;
  if (i != 15)
    goto next;
}
