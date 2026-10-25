// Draft of overlay 6aa's Func_020047b8, the twin of overlay 6ae's
// Func_020050b0: its draft (resource_6ae_c_020050b0.c) with this overlay's
// symbols: differ 90 of 120. Left: what that draft's header records.
#include "types.h"

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
  u16 *dst;
  s32 r, g, b;

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
    dst = (u16 *)0x5000000 + Data_02006880[gData_0200689c * gData_02006898 + i];
    r = *src++;
    g = *src++;
    b = *src++;
    *dst = (b << 10) | (g << 5) | r;
  }
  dst = (u16 *)0x5000000 + gData_0200e88c[gData_02006898];
  src = (gData_0200687c != 0 ? gData_02006890 : Data_02006894) +
        gData_02006898 * 3;
  *dst = (src[2] << 10) | (src[1] << 5) | src[0];
  gData_0200687c ^= 1;
  if (++gData_0200689c == gData_02006898)
    gData_0200689c = 0;
}
