#include "types.h"

s32 Func_08017dd4(void *, s32, s32);
void Func_0801e940(u8 *s, s32 arg1, u32 arg2, u32 arg3);

void Func_0801ea08(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4)
{
    u8 data[16];
    Func_0801e940(Func_08017dd4(data, arg0, arg1), arg2, arg3, arg4);
}
