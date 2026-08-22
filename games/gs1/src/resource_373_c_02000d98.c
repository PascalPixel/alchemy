#include "types.h"

extern void Func_02006d4e(s32);
extern void Func_02006cf2(s32, s32, s32, s32, s32, s32);

void Func_02000d98(void)
{
    s32 m, n;
    Func_02006d4e(0x210);
    m = 10;
    n = 84;
    Func_02006cf2(40, 89, 7, 4, m, n);
}
