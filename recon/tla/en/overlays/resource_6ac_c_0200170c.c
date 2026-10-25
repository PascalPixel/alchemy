// Draft of overlay 6ac's Func_0200170c, the twin of overlay 6ae's
// Func_020050b0: its draft (resource_6ae_c_020050b0.c) with this overlay's
// symbols: differ 90 of 120. Left: what that draft's header records.
#include "types.h"

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
  u16 *dst;
  s32 r, g, b;

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
    dst = (u16 *)0x5000000 + Data_0200391c[gData_02003938 * gData_02003934 + i];
    r = *src++;
    g = *src++;
    b = *src++;
    *dst = (b << 10) | (g << 5) | r;
  }
  dst = (u16 *)0x5000000 + gData_02003928[gData_02003934];
  src = (gData_02003918 != 0 ? Data_0200392c : Data_02003930) +
        gData_02003934 * 3;
  *dst = (src[2] << 10) | (src[1] << 5) | src[0];
  gData_02003918 ^= 1;
  if (++gData_02003938 == gData_02003934)
    gData_02003938 = 0;
}
