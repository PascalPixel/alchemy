#include "types.h"
#include "party_state.h"

void Func_02003bac(s32 a, s32 b);
void Func_02003bb4(s32 owner, u16 value);
s32  Field_DispatchTypeHandler(s32 type);
s32  Func_02003bbc(void);
s32  Func_02003bc4(void);

void Func_02000c08(s16 value)
{
    struct PartyState *party;

    Func_02003bac(151, 1);
    party = &gPartyState;
    /* FAKEMATCH: a one-pass block holds the argument's sign extension until after the first call (one reordered row; alchemy permute). */
    do
    {
        Func_02003bb4(party->currentOwner, value);
    } while (0);
    Field_DispatchTypeHandler(1);
    Func_02003bbc();
    Func_02003bc4();
}
