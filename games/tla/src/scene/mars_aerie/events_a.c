#include "mars_aerie.h"

const struct SceneEvent *Func_02000164(void)
{
    if (gPartyState.entrance == 2)
        return (const struct SceneEvent *)Data_0200644c;
    if ((u16)(gPartyState.entrance - 3) <= 2)
        return (const struct SceneEvent *)Data_020066bc;
    return (const struct SceneEvent *)Data_0200626c;
}

void Func_020001a0(void)
{
    Func_02004fac();
    Func_020057cc();
    {
        u8 *party = (u8 *)&gPartyState;

        /* FAKEMATCH: a one-pass block loads the party state before its offset is built (one reordered row). */
        do {
        } while (0);
        party[0x2c2] = 2;
    }
    Func_02005744(1);
    Battle_WaitMode0(40);
    Func_02005010();
}
