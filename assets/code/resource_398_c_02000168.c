#include "types.h"

extern void Func_02000aae(s32, s32, s32, s32, s32, s32);

void Func_02000168(void)
{
    s32 width = 21;
    s32 height = 14;

    Func_02000aae(0, 0, 1, 1, width, height);
}
