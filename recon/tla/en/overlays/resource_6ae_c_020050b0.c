// Draft of overlay 6ae's Func_020050b0, a palette cycle: differ 90 of 120.
// Left: the native keeps the colour pointer's three post-increments and builds
// the palette address inside the loop, where this strength-reduces the pointer
// and hoists the address; a goto loop keeps both but hoists nothing (differ
// 118).
#include "types.h"

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
  u16 *dst;
  s32 r, g, b;

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
    dst = (u16 *)0x5000000 + Data_02006cd4[gData_02006cf0 * gData_02006cec + i];
    r = *src++;
    g = *src++;
    b = *src++;
    *dst = (b << 10) | (g << 5) | r;
  }
  dst = (u16 *)0x5000000 + gData_02006ce0[gData_02006cec];
  src = (gData_02006cd0 != 0 ? Data_02006ce4 : Data_02006ce8) +
        gData_02006cec * 3;
  *dst = (src[2] << 10) | (src[1] << 5) | src[0];
  gData_02006cd0 ^= 1;
  if (++gData_02006cf0 == gData_02006cec)
    gData_02006cf0 = 0;
}
