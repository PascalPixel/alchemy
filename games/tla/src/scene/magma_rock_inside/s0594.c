#include "local.h"

void Func_02000594(void) { Func_080d2b4c(15, 56); }

void Func_020005a0(void) { Func_080d7788(17, 2, 19); }

s32 Func_020005b0(s32 *a, s32 *b) {
  s32 dx = (a[0] - b[0]) >> 16;
  s32 dz = (a[2] - b[2]) >> 16;

  return Iwram_Sqrt(dx * dx + dz * dz);
}
