#include "types.h"

extern void Func_0200287a(s32);
extern s32 Func_02002788(s32);
extern void Func_020005fe(s32, s32, s32);
extern void Func_020027ac(s32);
extern void Func_02000610(s32, s32, s32);
extern void Func_020027b8(s32);
extern void Func_020028ae(s32);
extern void Func_02002704(s32);
extern void Func_020015fc(void);

void Func_02000464(void)
{
    Func_0200287a((s32) 0xF1);

    if (Func_02002788((s32) 0x306) != 0) {
        Func_020005fe(8, 16, 0);
        Func_020027ac((s32) 0x305);
    } else {
        Func_02000610(8, 144, 0);
        /* movs r0,#0xc1 / lsls r0,#2 builds 0x304. */
        Func_020027b8((s32) 0x304);
    }

    Func_020028ae((s32) 0x121);
    Func_02002704(2);
    Func_020015fc();
}
