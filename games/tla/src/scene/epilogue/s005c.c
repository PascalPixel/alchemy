#include "types.h"

extern s8 gData_02000a34;
extern s16 gData_02000a38;
extern s16 gData_02000a2c;
extern s32 gData_02000a00[];

void Func_0200083c();

void Func_0200005c(void) {
  if (gData_02000a34 != 0) {
    gData_02000a00[3] = gData_02000a38 << 16;
    gData_02000a00[4] = gData_02000a2c << 16;
    Func_0200083c(gData_02000a00);
  }
}
