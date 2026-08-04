#include "types.h"

extern void Func_02000978(void);
extern s32 Func_0200096c(void);
extern void Func_020009c8(s32, s32);
extern void Func_02000986(s32);
extern void Func_020009ec(s32);
extern void Func_02000a04(s32, s32);
extern void Func_02000a2e(s32, s32);
extern void Func_020009c6(s32, s32);
extern void Func_020009ba(void);

void Func_02000338(void)
{
    Func_02000978();

    if (Func_0200096c() == 0) {
        Func_020009c8(18, 4);
        Func_02000986(20);
        Func_020009ec(0x1384);
        Func_02000a04(18, 0);
    } else {
        Func_02000a2e(0xE7, 3);
        Func_020009c6(0xE7, 0);
    }

    Func_020009ba();
}
