// Draft, differ 129 of 120: the palette cycle callback: on each step it
// writes the cycle's colours to their palette entries and flashes one more.
// Left: the native reads each colour through a stepped pointer and keeps
// 0x05000000 inside the loop.
#include "../../../../games/tla/src/scene/yampi_desert_cave/yampi_desert_cave.h"

extern u16 *gData_02001fcc;
extern u16 *gData_02001fdc;
extern u16 *gData_02001fe0;

void Func_0200099c(void) {
  s32 step = 0;
  u16 *color;
  u16 *dst;
  u32 i;
  s32 r, g, b;

  if (gData_02001fd0 != 0 && --gData_02001fd0 == 0) {
    step = 1;
    gData_02001fd0 = gData_02001fd4;
    if (gData_02001fd4 > 2)
      gData_02001fd4--;
  }
  if (!step)
    return;
  color = gData_02001fe0;
  for (i = 0; i < gData_02001fe4; i++) {
    dst = (u16 *)(0x05000000 +
                  gData_02001fcc[gData_02001fe8 * gData_02001fe4 + i] * 2);
    r = *color++;
    g = *color++;
    b = *color++;
    *dst = b << 10 | g << 5 | r;
  }
  dst = (u16 *)(0x05000000 + gData_02001fd8[gData_02001fe4] * 2);
  color =
      (gData_02001fc8 ? gData_02001fdc : gData_02001fe0) + gData_02001fe4 * 3;
  r = *color++;
  g = *color++;
  b = *color;
  *dst = b << 10 | g << 5 | r;
  gData_02001fc8 ^= 1;
  if (++gData_02001fe8 == gData_02001fe4)
    gData_02001fe8 = 0;
}
