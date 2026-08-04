#include "types.h"

extern s32 Func_02003c78(s32, s32);

void Func_02000158(void)
{
    s32 value = 66;
    s32 *mode = (s32 *)0x0200C41C;

    *mode = value;
    Func_02003c78(0x0200804D, 0xC80);
}
