// Draft, differ 2: overlay 6b0's copy of the function drafted in
// resource_6ac_c_0200170c.c, whose header says what it is and what still differs.
#include "types.h"
#include "call.h"
#include "io_reg.h"

extern u16 gData_02004434;
extern u16 *Data_02004438;
extern u16 gData_0200443c;
extern u16 gData_02004440;
extern u16 *gData_02004444;
extern u16 *Data_02004448;
extern u16 *Data_0200444c;
extern u16 gData_02004450;
extern u16 gData_02004454;

void Func_020001f8(void) {
  s32 step;
  u32 i;
  u16 *src;
  volatile u16 *dst;
  int red, green, blue;

  step = 0;
  if (gData_0200443c != 0) {
    if (--gData_0200443c == 0) {
      step = 1;
      gData_0200443c = gData_02004440;
      if (gData_02004440 > 2)
        gData_02004440--;
    }
  }
  if (step == 0)
    return;
  src = Data_0200444c;
  for (i = 0; i < gData_02004450; i++) {
    /* FAKEMATCH(one-pass): the statement in a once-only block, whose end holds the
     * order around it; as the plain statement, 28 of 120 instructions differ
     * in tla-en. */
    do {
      dst = &BG_PLTT[Data_02004438[gData_02004454 * gData_02004450 + i]];
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
  dst = &BG_PLTT[gData_02004444[gData_02004450]];
  if (gData_02004434 != 0)
    src = Data_02004448 + gData_02004450 * 3;
  else
    src = Data_0200444c + gData_02004450 * 3;
  red = *src++;
  green = *src++;
  /* FAKEMATCH(one-pass): the statement in a once-only block, whose end holds the
   * order around it; as the plain statement, 22 of 120 instructions differ
   * in tla-en. */
  do {
    blue = *src++;
  } while (0);
  *dst = (blue << 10) | (green << 5) | red;
  gData_02004434 ^= 1;
  if (++gData_02004454 == gData_02004450)
    gData_02004454 = 0;
}
