#include "types.h"
#include "io_reg.h"

extern u16 gData_02003918;
extern u16 *Data_0200391c;
extern u16 gData_02003920;
extern u16 gData_02003924;
extern u16 *gData_02003928;
extern u16 *Data_0200392c;
extern u16 *Data_02003930;
extern u16 gData_02003934;
extern u16 gData_02003938;

void Func_0200170c(void) {
  s32 step;
  u32 i;
  u16 *src;
  volatile u16 *dst;
  int red, green, blue;

  step = 0;
  if (gData_02003920 != 0) {
    if (--gData_02003920 == 0) {
      step = 1;
      gData_02003920 = gData_02003924;
      if (gData_02003924 > 2)
        gData_02003924--;
    }
  }
  if (step == 0)
    return;
  src = Data_02003930;
  for (i = 0; i < gData_02003934; i++) {
    dst = &BG_PLTT[Data_0200391c[gData_02003938 * gData_02003934 + i]];
    red = *src++;
    green = *src++;
    blue = *src++;
    /* FAKEMATCH(one-pass): the store in a once-only block, whose end holds the
     * order around it; as the plain statement, 2 of 120 instructions differ in
     * tla-en. */
    do {
      *dst = (blue << 10) | (green << 5) | red;
    } while (0);
  }
  dst = &BG_PLTT[gData_02003928[gData_02003934]];
  if (gData_02003918 != 0)
    src = Data_0200392c + gData_02003934 * 3;
  else
    src = Data_02003930 + gData_02003934 * 3;
  red = *src++;
  green = *src++;
  /* FAKEMATCH(one-pass): the last read in a once-only block, whose end holds
   * the order around it; as the plain statement, 2 of 120 instructions differ
   * in tla-en. */
  do {
    blue = *src++;
  } while (0);
  *dst = (blue << 10) | (green << 5) | red;
  gData_02003918 ^= 1;
  if (++gData_02003938 == gData_02003934)
    gData_02003938 = 0;
}
