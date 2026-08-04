#include "types.h"

extern void Func_02002fc6(s32);
extern s32 Func_02002fd6(s32, s32);
extern s32 Func_02002f66(s32, s32);

s32 Func_0200141c(void)
{
    Func_02002fc6(0x255C);
    Func_02002fd6(1, 0);
    return Func_02002f66(0, 0) == 0;
}
