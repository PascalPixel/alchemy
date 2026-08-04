#include "types.h"

extern void Func_080f9010(s32);
extern s32 Func_080770c0(s32);
extern void Func_02000180(s32, s32, s32);
extern void Func_080770c8(s32);
extern void Func_080770d0(s32);
extern void Func_080000c0(s32);
extern void Func_020012cc(void);

void Func_02000768(void)
{
    Func_080f9010(241);
    if (Func_080770c0(0x308) != 0) {
        Func_02000180(10, 0, -64);
        Func_080770d0(0x30b);
        Func_080770c8(0x30c);
        Func_080770d0(0x30d);
        Func_080770d0(0x30e);
    } else {
        Func_02000180(10, 0, -128);
        Func_080770c8(0x30b);
        Func_080770d0(0x30c);
        Func_080770d0(0x30d);
        Func_080770d0(0x30e);
    }
    Func_080f9010(0x121);
    Func_080000c0(2);
    Func_020012cc();
}
