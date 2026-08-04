#include "types.h"

extern void Func_080f9010(s32);
extern void Func_02000180(s32, s32, s32);
extern void Func_02000bc8(void);
extern void Func_080000c0(s32);
extern void Func_020012cc(void);

void Func_02000d08(void)
{
    Func_080f9010(241);
    Func_02000180(11, 0, 48);
    Func_080f9010(0x121);
    Func_02000bc8();
    Func_080000c0(2);
    Func_020012cc();
}
