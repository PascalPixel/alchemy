#include "shaman_village.h"

void Func_0200430c(s32 x, s32 y, s32 z);

void Func_02004394(void) {
  if ((gFrameCount & 63) == 0) {
    Func_0200430c(0x480000, 0x420000, 0x1780000);
    Func_0200430c(0x680000, 0x620000, 0x980000);
  }
}
