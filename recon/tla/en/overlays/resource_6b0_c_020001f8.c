// Draft of overlay 6b0's Func_020001f8, the twin of overlay 6ae's
// Func_020050b0: its draft (resource_6ae_c_020050b0.c) with this overlay's
// symbols: differ 90 of 120. Left: what that draft's header records.
#include "types.h"

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
  u16 *dst;
  s32 r, g, b;

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
    dst = (u16 *)0x5000000 + Data_02004438[gData_02004454 * gData_02004450 + i];
    r = *src++;
    g = *src++;
    b = *src++;
    *dst = (b << 10) | (g << 5) | r;
  }
  dst = (u16 *)0x5000000 + gData_02004444[gData_02004450];
  src = (gData_02004434 != 0 ? Data_02004448 : Data_0200444c) +
        gData_02004450 * 3;
  *dst = (src[2] << 10) | (src[1] << 5) | src[0];
  gData_02004434 ^= 1;
  if (++gData_02004454 == gData_02004450)
    gData_02004454 = 0;
}
