#include "types.h"
#include "party_state.h"

s32 Party_SetPackedEvent(s32 kind, s32 id, s32 second)
{
    u32 packed = (kind << 10) | (id & 0x3fff);

    if (second == 0)
        gPartyState.packedEvents[0] = packed;
    else
    {
        struct PartyState *party = &gPartyState;

        /* FAKEMATCH: a one-pass block loads the party state before its offset is built (one reordered row). */
        do
        {
        } while (0);
        party->packedEvents[1] = packed;
    }
    return 1;
}
