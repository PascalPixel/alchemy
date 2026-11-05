// Under -Os, 5 November: differ 97 of 120 (tla-en); older figures below predate it.
// Draft, differ 81 of 120, skeleton gap 29 (tla-en, 2026-11-01)
// The palette cycle callback: on each step it writes the cycle's colours to
// their palette entries and flashes one more.
// Closed: BG_PLTT is volatile, and a volatile in the loop stops loop.c merging
// the three colour steps (has_volatile), so the stepped pointer now matches.
// Left: the native keeps 0x05000000 in the loop, but the second loop pass hoists
// it here (24 real insns, threshold not reached); the native reads the count
// through two registers (r6 in the body, ip at the test, one register here),
// which looks like CSE not joining the two reads; i and dst swap r4 and r5.
// Tried: goto, do/while (loop ignored: PRE gives it two entries, differ 95),
// while, break forms; declaration orders and scopes (no change); a count
// pointer local (83 to 98); one-pass blocks (103 to 122).
// Not kept: `u16 entry; dst = BG_PLTT; entry = ...; dst += entry;` reaches differ
// 69, gap 21, and keeps the constant in the loop, but dst holds two values
// (S2 temporary), so it is a new device. Pascal could rule on it.
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
    b = *color++;
    g = *color++;
    *dst = b << 10 | g << 5 | r;
  }
  dst = &BG_PLTT[gData_02001fd8[gData_02001fe4]];
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
