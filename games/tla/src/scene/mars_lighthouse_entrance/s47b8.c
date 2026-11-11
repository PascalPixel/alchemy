#include "types.h"
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
    dst = &BG_PLTT[Data_02006880[gData_0200689c * gData_02006898 + i]];
    red = *src++;
    green = *src++;
    blue = *src++;
    /* FAKEMATCH(dead): the store in a once-only block, whose end holds the
     * order around it; as the plain statement, 2 of 120 instructions differ in
     * tla-en. */
    do {
      *dst = (blue << 10) | (green << 5) | red;
    } while (0);
  }
  dst = &BG_PLTT[gData_0200e88c[gData_02006898]];
  if (gData_0200687c != 0)
    src = gData_02006890 + gData_02006898 * 3;
  else
    src = Data_02006894 + gData_02006898 * 3;
  red = *src++;
  green = *src++;
  /* FAKEMATCH(dead): the last read in a once-only block, whose end holds
   * the order around it; as the plain statement, 2 of 120 instructions differ
   * in tla-en. */
  do {
    blue = *src++;
  } while (0);
  *dst = (blue << 10) | (green << 5) | red;
  gData_0200687c ^= 1;
  if (++gData_0200689c == gData_02006898)
    gData_0200689c = 0;
}
