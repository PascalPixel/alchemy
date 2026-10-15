#include "trial_road.h"

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
    s32 y = Object_GetById(gPartyState.currentOwner)->y;

    /* FAKEMATCH: loads the leader height before it builds the threshold (differ 2 without it, as in Func_020007b4). */
    do { } while (0);
    if (y >= 0x100000) {
        Func_020059c0();
    } else if (Func_02005a30()) {
        Func_02005a38();
        Func_020009ac(0, 0);
    }
}
