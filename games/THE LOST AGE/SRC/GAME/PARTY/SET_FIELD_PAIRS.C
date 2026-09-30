#include "TYPES.H"
#include "PARTY_STATE.H"

void Party_SetFields1eeAnd1f0(u16 first, u16 second)
{
    gPartyState.pair_1ee[0] = first;
    gPartyState.pair_1ee[1] = second;
}

void Party_SetFields1f2And1f4(u16 first, u16 second)
{
    gPartyState.pair_1f2[0] = first;
    gPartyState.pair_1f2[1] = second;
}
