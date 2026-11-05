// Draft, differ 2: overlay 6aa's copy of the function drafted in
// resource_6ac_c_0200170c.c, whose header says what it is and what still differs.
#include "types.h"
#include "call.h"
#include "io_reg.h"

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
  volatile u16 *dst;
  int red, green, blue;

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
    /* FAKEMATCH(one-pass): the statement in a once-only block, whose end holds the
     * order around it; as the plain statement, 28 of 120 instructions differ
     * in tla-en. */
    do {
      dst = &BG_PLTT[Data_02006880[gData_0200689c * gData_02006898 + i]];
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
  dst = &BG_PLTT[gData_0200e88c[gData_02006898]];
  if (gData_0200687c != 0)
    src = gData_02006890 + gData_02006898 * 3;
  else
    src = Data_02006894 + gData_02006898 * 3;
  red = *src++;
  green = *src++;
  /* FAKEMATCH(one-pass): the statement in a once-only block, whose end holds the
   * order around it; as the plain statement, 22 of 120 instructions differ
   * in tla-en. */
  do {
    blue = *src++;
  } while (0);
  *dst = (blue << 10) | (green << 5) | red;
  gData_0200687c ^= 1;
  if (++gData_0200689c == gData_02006898)
    gData_0200689c = 0;
}
