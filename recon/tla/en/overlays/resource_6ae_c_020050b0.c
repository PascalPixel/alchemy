// Draft, differ 2: overlay 6ae's copy of the function drafted in
// resource_6ac_c_0200170c.c, whose header says what it is and what still differs.
#include "types.h"
#include "call.h"
#include "io_reg.h"

extern u16 gData_02006cd0;
extern u16 *Data_02006cd4;
extern u16 gData_02006cd8;
extern u16 gData_02006cdc;
extern u16 *gData_02006ce0;
extern u16 *Data_02006ce4;
extern u16 *Data_02006ce8;
extern u16 gData_02006cec;
extern u16 gData_02006cf0;

void Func_020050b0(void) {
  s32 step;
  u32 i;
  u16 *src;
  volatile u16 *dst;
  int red, green, blue;

  step = 0;
  if (gData_02006cd8 != 0) {
    if (--gData_02006cd8 == 0) {
      step = 1;
      gData_02006cd8 = gData_02006cdc;
      if (gData_02006cdc > 2)
        gData_02006cdc--;
    }
  }
  if (step == 0)
    return;
  src = Data_02006ce8;
  for (i = 0; i < gData_02006cec; i++) {
    /* FAKEMATCH(one-pass): the statement in a once-only block, whose end holds the
     * order around it; as the plain statement, 28 of 120 instructions differ
     * in tla-en. */
    do {
      dst = &BG_PLTT[Data_02006cd4[gData_02006cf0 * gData_02006cec + i]];
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
  dst = &BG_PLTT[gData_02006ce0[gData_02006cec]];
  if (gData_02006cd0 != 0)
    src = Data_02006ce4 + gData_02006cec * 3;
  else
    src = Data_02006ce8 + gData_02006cec * 3;
  red = *src++;
  green = *src++;
  /* FAKEMATCH(one-pass): the statement in a once-only block, whose end holds the
   * order around it; as the plain statement, 22 of 120 instructions differ
   * in tla-en. */
  do {
    blue = *src++;
  } while (0);
  *dst = (blue << 10) | (green << 5) | red;
  gData_02006cd0 ^= 1;
  if (++gData_02006cf0 == gData_02006cec)
    gData_02006cf0 = 0;
}
