#include "local.h"

void Func_02004e70(s32 a, s32 b);
void Func_02004e78(s32 owner, u16 value);
s32  Field_DispatchTypeHandler(s32 type);
s32  Func_02004e80(void);
s32  Func_02004e88(void);

void Func_02000ce4(s16 value)
{
    struct PartyState *party;

    Func_02004e70(151, 1);
    party = &gPartyState;
    /* FAKEMATCH: a one-pass block holds the argument's sign extension until after the first call (one reordered row; alchemy permute). */
    do
    {
        Func_02004e78(party->currentOwner, value);
    } while (0);
    Field_DispatchTypeHandler(1);
    Func_02004e80();
    Func_02004e88();
}
