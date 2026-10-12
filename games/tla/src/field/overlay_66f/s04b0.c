#include "types.h"
#include "call.h"

void Func_0200106c(s32 a, s32 b, s32 c, s32 d, s32 x, s32 z);

void Func_020004b0(s32 unused, s32 cell)
{
    if (cell == 16 || cell == 17)
        Call6((void (*)())Func_0200106c, 120, 33, 8, 2, 20, 33);
    if (cell == 14 || cell == 15)
        Call6((void (*)())Func_0200106c, 112, 15, 16, 4, 8, 15);
}
