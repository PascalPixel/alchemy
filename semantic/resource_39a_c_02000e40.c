#include "types.h"

extern void Func_080f9010(s32);
extern void Func_02000180(s32, s32, s32);
extern void Func_080770d0(s32);
extern void Func_080000c0(s32);
extern void Func_020015dc(void);

void Func_02000e40(void)
{
    Func_080f9010(241);
    Func_02000180(10, -160, 0);
    Func_080f9010(0x121);
    Func_080770d0(0x315);
    Func_080000c0(2);
    Func_020015dc();
}
