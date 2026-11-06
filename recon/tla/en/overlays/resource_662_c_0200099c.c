// Differ 6 of 120 (tla-en, 2026-11-06): left, scheduling only. In the loop the native keeps color's third step right
// after b's load and shifts b before g; here sched2 fills b's load delay with g << 5 and leaves the step until the
// store. After the loop g << 5 is likewise scheduled before b << 10. Every order of the ors (16 to 36), u32 channels
// and the channels' declaration orders were measured on this base: no gain.
// 2026-11-06, from 49: the loop is a while with i++ in a once-only block, and the index reads fe8 * fe4. Plain, CSE
// follows the guard into the body (the multiply then reads the guard's address, r6, as the native does) but the
// rotated exit test leaves a copy cycle on the address that loop.c cannot hoist (95). The once-only block's loop-end
// note ends CSE's block before the test, so the test reads the address on its own, loop.c hoists it and cse2 makes it
// the copy the native keeps in ip; with fe4 * fe8 the two loads swap registers (24). The guarded do-while (49) and a
// once-only block in it (49) give one shared copy for the multiply and the test.
// Tried before: s32 and u32 and u16 channels, shifted loads, r |= steps, operand orders of the multiply,
// i s32 or u16, dst non-volatile, an index local, RGB(), other orders of the ors, for, while, for (;;) and goto loops,
// guards on the count alone; 5 and 8 minutes of permute (best differ 53 and 63).
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
  while (i < gData_02001fe4) {
    dst = &BG_PLTT[gData_02001fcc[gData_02001fe8 * gData_02001fe4 + i]];
    r = *color++;
    g = *color++;
    b = *color++;
    *dst = b << 10 | g << 5 | r;
    /* FAKEMATCH(one-pass): i++ in a once-only block; as a plain i++, 95 of 120 instructions differ in tla-en. */
    do {
      i++;
    } while (0);
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
