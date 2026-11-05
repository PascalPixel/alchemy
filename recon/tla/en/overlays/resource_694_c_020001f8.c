// Draft, differ 2: overlay 694's copy of the function drafted in
// resource_6ac_c_0200170c.c, whose header says what it is and what still differs.
#include "types.h"
#include "call.h"
#include "io_reg.h"

extern u16 gData_02006000;
extern u16 *gData_02006004;
extern u16 gData_02006008;
extern u16 gData_0200600c;
extern u16 *gData_02006010;
extern u16 *gData_02006014;
extern u16 *Data_02006018;
extern u16 gData_0200601c;
extern u16 gData_02006020;

void Func_020001f8(void) {
  s32 step;
  u32 i;
  u16 *src;
  volatile u16 *dst;
  int red, green, blue;

  step = 0;
  if (gData_02006008 != 0) {
    if (--gData_02006008 == 0) {
      step = 1;
      gData_02006008 = gData_0200600c;
      if (gData_0200600c > 2)
        gData_0200600c--;
    }
  }
  if (step == 0)
    return;
  src = Data_02006018;
  for (i = 0; i < gData_0200601c; i++) {
    /* FAKEMATCH(one-pass): the statement in a once-only block, whose end holds the
     * order around it; as the plain statement, 28 of 120 instructions differ
     * in tla-en. */
    do {
      dst = &BG_PLTT[gData_02006004[gData_02006020 * gData_0200601c + i]];
    } while (0);
    red = *src++;
    green = *src++;
    /* FAKEMATCH(one-pass): the statement in a once-only block, whose end holds the
     * order around it; as the plain statement, 6 of 120 instructions differ
     * in tla-en. */
    do {
      blue = *src++;
    } while (0);
    *dst = (blue << 10) | (green << 5) | red;
  }
  dst = &BG_PLTT[gData_02006010[gData_0200601c]];
  if (gData_02006000 != 0)
    src = gData_02006014 + gData_0200601c * 3;
  else
    src = Data_02006018 + gData_0200601c * 3;
  red = *src++;
  green = *src++;
  /* FAKEMATCH(one-pass): the statement in a once-only block, whose end holds the
   * order around it; as the plain statement, 22 of 120 instructions differ
   * in tla-en. */
  do {
    blue = *src++;
  } while (0);
  *dst = (blue << 10) | (green << 5) | red;
  gData_02006000 ^= 1;
  if (++gData_02006020 == gData_0200601c)
    gData_02006020 = 0;
}
