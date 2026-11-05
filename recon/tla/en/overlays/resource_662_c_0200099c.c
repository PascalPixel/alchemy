// Draft, differ 95 of 120 (tla-en, 5 November).
// Left: loop.c leaves the bottom test's copy of the count pointer in the loop (adds r1, r7, #0), where the native moves it out (mov ip, r6),
// so r1 is taken and colour, r and dst rotate. Move decisions depend on the loop's insn count: with all three reads as one expression
// (wrong program) it moves and the registers agree (77), so the native's loop has fewer insns here: find which.
// Tried: s32 and u32 and u16 channels, shifted loads, r |= steps, operand orders of the multiply, i s32, dst non-volatile.
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
  for (i = 0; i < gData_02001fe4; i++) {
    dst = &BG_PLTT[gData_02001fcc[gData_02001fe8 * gData_02001fe4 + i]];
    r = *color++;
    g = *color++;
    b = *color++;
    *dst = b << 10 | g << 5 | r;
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
