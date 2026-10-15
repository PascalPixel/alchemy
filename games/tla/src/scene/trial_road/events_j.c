#include "trial_road.h"

void Func_0200106c(void)
{
    Func_02000e40(16, -1, 0, 0);
}

void Func_02001080(void)
{
    Func_02000e40(16, 1, 0, 0);
}

void Func_02001090(void)
{
    Func_02000e40(17, -1, 0, 0);
}

void Func_020010a4(void)
{
    Func_02000e40(17, 1, 0, 0);
}

void Func_020010b4(void)
{
    Func_02000e40(18, 0, -1, 1);
}

void Func_020010c8(void)
{
    Func_02000e40(18, 0, 1, 1);
}

void Func_020010d8(void)
{
    struct PartyState *party = &gPartyState;

    /* FAKEMATCH: loads the party base before it builds the render-mode offset (differ 2 without it). */
    do { } while (0);
    if (party->render_mode == 2)
        Func_020059c8();
    else
        Func_02000e40(18, 0, 1, 1);
}
