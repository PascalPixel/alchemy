// Draft, differ 71 of 120 (tla-en, 2026-11-05). Older notes below may predate this figure.
// Draft of overlay 6aa's Func_020047b8, the twin of overlay 6ae's
// Func_020050b0: its draft (resource_6ae_c_020050b0.c) with this overlay's
// symbols: differ 90 of 120. Left: what that draft's header records.
#include "types.h"
#include "call.h"

extern u16 gData_0200687c;
extern u16 *Data_02006880;
extern u16 gData_02006884;
extern u16 gData_0200e888;
extern u16 *gData_0200e88c;
extern u16 *gData_02006890;
extern u16 *Data_02006894;
extern u16 gData_02006898;
extern u16 gData_0200689c;

void Func_020047b8(void) {
  s32 step;
  u32 i;
  u16 *src;
  u16 *dst;
  s32 r, g, b;

  step = 0;
  if (gData_02006884 != 0) {
    if (--gData_02006884 == 0) {
      step = 1;
      gData_02006884 = gData_0200e888;
      if (gData_0200e888 > 2)
        gData_0200e888--;
    }
  }
  if (step == 0)
    return;
  src = Data_02006894;
  for (i = 0; i < gData_02006898; i++) {
    dst = (u16 *)0x5000000 + Data_02006880[gData_0200689c * gData_02006898 + i];
    /* FAKEMATCH(barrier): the empty asm on dst changes its allocation rank; as no
     * asm, 95 of 120 instructions differ in tla-en. */
    asm("" : : "r"(dst));
    r = *src++;
    g = *src++;
    b = *src++;
    /* FAKEMATCH(barrier): the empty asm on b changes its allocation rank; as no
     * asm, 75 of 120 instructions differ in tla-en. */
    asm("" : "=r"(b) : "0"(b));
    *dst = (b << 10) | (g << 5) | r;
  }
  dst = (u16 *)0x5000000 + gData_0200e88c[gData_02006898];
  src = (gData_0200687c != 0 ? gData_02006890 : Data_02006894) +
        gData_02006898 * 3;
  *dst = (src[2] << 10) | (src[1] << 5) | src[0];
  /* FAKEMATCH(one-pass): the statement in a once-only block, whose end holds the
   * order around it; as the plain statement, 73 of 120 instructions differ
   * in tla-en. */
  do {
    gData_0200687c ^= 1;
  } while (0);
  if (++gData_0200689c == gData_02006898)
    gData_0200689c = 0;
}
