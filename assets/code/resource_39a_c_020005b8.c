#include "types.h"

extern void Func_020029ce(s32);
extern s32 Func_020028de(s32);
extern s32 Func_020028e8(s32);
extern void Func_02000760(s32, s32, s32);
extern void Func_02002910(s32);
extern void Func_0200290e(s32);
extern void Func_0200077c(s32, s32, s32);
extern void Func_02002924(s32);
extern void Func_02002932(s32);
extern void Func_02002a20(s32);
extern void Func_02002876(s32);
extern void Func_020018e6(void);

void Func_020005b8(void)
{
    Func_020029ce((s32) 0xF1);

    /* movs r0,#0xc4 / lsls r0,#2 builds 0x310. The second test is only reached
     * when the first fails, and both truths take the same path. */
    if (Func_020028de((s32) 0x310) != 0 || Func_020028e8((s32) 0x30D) != 0) {
        Func_02000760(8, -48, 0);
        Func_02002910((s32) 0x308);
        Func_0200290e((s32) 0x309);
    } else {
        Func_0200077c(8, -96, 0);
        Func_02002924((s32) 0x308);
        Func_02002932((s32) 0x309);
    }

    Func_02002a20((s32) 0x121);
    Func_02002876(2);
    Func_020018e6();
}
