#include "TYPES.H"
#include "PARTY_STATE.H"

s32 Event_ValidatePackedId(u32 packed);

/* Clears each of the party's two packed event ids that validates. */
void Event_ClearValidPackedIds(void)
{
    if (Event_ValidatePackedId(gPartyState.packed_events[0]))
        gPartyState.packed_events[0] = 0;
    if (Event_ValidatePackedId(gPartyState.packed_events[1]))
        gPartyState.packed_events[1] = 0;
}
