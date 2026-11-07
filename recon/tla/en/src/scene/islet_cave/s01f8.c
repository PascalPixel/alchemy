#include "types.h"
#include "call.h"
#include "io_reg.h"

extern u16 gData_02001a14;
extern u16 *gData_02001a18;
extern u16 gData_02001a1c;
extern u16 gData_02001a20;
extern u16 *gData_02001a24;
extern u16 *gData_02001a28;
extern u16 *Data_02001a2c;
extern u16 gData_02001a30;
extern u16 gData_02001a34;

void Func_020001f8(void) {
  s32 step;
  u32 i;
  u16 *src;
  volatile u16 *dst;
  int red, green, blue;

  step = 0;
  if (gData_02001a1c != 0) {
    if (--gData_02001a1c == 0) {
      step = 1;
      gData_02001a1c = gData_02001a20;
      if (gData_02001a20 > 2)
        gData_02001a20--;
    }
  }
  if (step == 0)
    return;
  src = Data_02001a2c;
  for (i = 0; i < gData_02001a30; i++) {
    dst = &BG_PLTT[gData_02001a18[gData_02001a34 * gData_02001a30 + i]];
    red = *src++;
    green = *src++;
    blue = *src++;
    /* FAKEMATCH(one-pass): the store in a once-only block, whose end holds the
     * order around it; as the plain statement, 93 of 120 instructions differ
     * in tla-en. */
    do {
      *dst = (blue << 10) | (green << 5) | red;
    } while (0);
  }
  dst = &BG_PLTT[gData_02001a24[gData_02001a30]];
  if (gData_02001a14 != 0)
    src = gData_02001a28 + gData_02001a30 * 3;
  else
    src = Data_02001a2c + gData_02001a30 * 3;
  red = *src++;
  green = *src++;
  /* FAKEMATCH(one-pass): the last read in a once-only block, whose end holds
   * the order around it; as the plain statement, 2 of 120 instructions differ
   * in tla-en. */
  do {
    blue = *src++;
  } while (0);
  *dst = (blue << 10) | (green << 5) | red;
  gData_02001a14 ^= 1;
  if (++gData_02001a34 == gData_02001a30)
    gData_02001a34 = 0;
}
