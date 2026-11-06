// Differ 0 of 120 in all six editions (2026-11-06; the comparison omits the literal pool, so check it at adoption).
// Two forms make it. READ_COLOR reads the three channels in a do-while (0) macro, used at both stores: its loop-end
// note is a scheduling barrier between the loads and the shifts, so b << 10 is scheduled before g << 5 as in the
// native (inline reads: 6). i++ sits in a once-only block (one tagged device): its loop-end note ends CSE's block
// before the exit test, so the test reads the count's address on its own, loop.c hoists it and cse2 makes it the
// copy the native keeps in ip, and i++ is scheduled before that load. As a plain i++ the macro form differs in 2
// (i++ after the load); without the macro as well, 95 (CSE follows the guard into the body and the rotated test
// leaves a copy cycle on the address that loop.c cannot hoist). The index reads fe8 * fe4 (fe4 * fe8 swaps the
// two loads' registers). The guarded do-while of the earlier draft gave 49, with one copy of the address shared by
// the multiply and the test. A for loop with the macro also gives 2.
#include "../../../../games/tla/src/scene/yampi_desert_cave/yampi_desert_cave.h"
#include "system/io_reg.h"

extern u16 *gData_02001fcc;
extern u16 *gData_02001fdc;
extern u16 *gData_02001fe0;

/// Reads one colour's three channels, red first, stepping p past them.
#define READ_COLOR(r, g, b, p)                                                 \
  do {                                                                         \
    (r) = *(p)++;                                                              \
    (g) = *(p)++;                                                              \
    (b) = *(p)++;                                                              \
  } while (0)

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
    READ_COLOR(r, g, b, color);
    *dst = b << 10 | g << 5 | r;
    /* FAKEMATCH(one-pass): i++ in a once-only block; as a plain i++, 2 of 120 instructions differ in tla-en. */
    do {
      i++;
    } while (0);
  }
  dst = &BG_PLTT[gData_02001fd8[gData_02001fe4]];
  if (gData_02001fc8)
    color = gData_02001fdc + gData_02001fe4 * 3;
  else
    color = gData_02001fe0 + gData_02001fe4 * 3;
  READ_COLOR(r, g, b, color);
  *dst = b << 10 | g << 5 | r;
  gData_02001fc8 ^= 1;
  if (++gData_02001fe8 == gData_02001fe4)
    gData_02001fe8 = 0;
}
