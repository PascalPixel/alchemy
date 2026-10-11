#include "ov697.h"

extern const u8 Data_02007734[];

void Func_02000c34(void)
{
    Func_02000b7c(18, 19, 0x335, 1);
}

void Func_02000c48(void)
{
    Func_02000b7c(19, 18, 0x335, 0);
}

void Func_02000c5c(void)
{
    Func_02000b7c(20, 21, 0x337, 1);
}

void Func_02000c70(void)
{
    Func_02000b7c(21, 20, 0x337, 0);
}

void Func_02000c84(void)
{
    s32 y = Object_GetById(gPartyState.current_owner)->y;

    /* FAKEMATCH: loads the leader height before it builds the threshold. */
    __asm__("");
    if (y >= 0x100000) {
        Func_020059c0();
    } else if (Func_02005a30()) {
        Func_02005a38();
        Func_020009ac(0, 0);
    }
}

void Func_02000cc0(void)
{
    Func_02004418(Data_02007734);
}

void Func_02000cd0(s32 phase)
{
    if (phase == 3)
        Func_020045ac(Data_02007734, 15);
}

void Func_02000ce4(s32 phase)
{
    if (phase == 3)
        Func_020045ac(Data_02007734, 16);
}
