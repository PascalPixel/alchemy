// Near miss, differ 49 of 120 (tla-en, 2026-11-06): the guarded do-while is the native loop (95 as a for);
// left, in the loop, the count address: the native keeps the guard's pseudo for the multiply (r6) and a hoisted copy
// for the bottom test (mov ip, r6), where this shares one pseudo between both (adds r7, r2, #0 before the loop), so fe8
// goes to ip in place of lr and the colour read and the 0x5000000 base rotate; and g << 5 is scheduled before b << 10 at
// both stores. In the for form gcse leaves a copy cycle on the count address (69 = 67 at the top, 67 = 69 at the bottom)
// that loop.c cannot move.
// Tried: s32 and u32 and u16 channels, shifted loads, r |= steps, operand orders of the multiply (fe4 * fe8 + i is best),
// i s32 or u16, dst non-volatile, an index local, RGB(), other orders of the ors, for, while, for (;;) and goto loops,
// guards on the count alone.
#include "../../../../games/tla/src/scene/yampi_desert_cave/yampi_desert_cave.h"
#include "system/io_reg.h"

extern u16 *gData_02001fcc;
extern u16 *gData_02001fdc;
extern u16 *gData_02001fe0;

void Func_0200099c(void) {
  s32 step = 0;
  u16 *color;
  volatile u16 *dst;
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
  i = 0;
  if (i < gData_02001fe4) {
    do {
      dst = &BG_PLTT[gData_02001fcc[gData_02001fe4 * gData_02001fe8 + i]];
      r = *color++;
      g = *color++;
      b = *color++;
      *dst = b << 10 | g << 5 | r;
      i++;
    } while (i < gData_02001fe4);
  }
  dst = &BG_PLTT[gData_02001fd8[gData_02001fe4]];
  if (gData_02001fc8)
    color = gData_02001fdc + gData_02001fe4 * 3;
  else
    color = gData_02001fe0 + gData_02001fe4 * 3;
  r = *color++;
  g = *color++;
  b = *color;
  *dst = b << 10 | g << 5 | r;
  gData_02001fc8 ^= 1;
  if (++gData_02001fe8 == gData_02001fe4)
    gData_02001fe8 = 0;
}
