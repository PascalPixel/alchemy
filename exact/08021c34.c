#include "types.h"

s32 Func_080162d4(s32, s32, s32, s32, s32);
void Func_0801e940(u8 *s, s32 arg1, u32 arg2, u32 arg3);
s32 Func_08021c34(void)
{
    s32 handle = Func_080162d4(0, 0, 6, 4, 6);

    Func_0801e940((u8 *)0x08037300, handle, 0, 0);
    return handle;
}
