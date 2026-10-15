/* Near miss, differ 2 of 21 (ported from The Broken Seal's
   BattleFx_FindDescriptorWithOverride): the native loads the party state's
   address before it builds the offset 0x26a; this draft builds the offset
   first. A one-pass block or barrier around the address load fixes that pair
   but then moves the result's copy (adds r6, r0) ahead of the load; a forced
   temporary for the result is coalesced. Permute (3 min) found nothing. */
#include "types.h"
#include "party_state.h"

void *Func_080ccd78(s32 group, s32 id);

// Finds the talk event of object id; when the party has stopped that object,
// an event of kind 7 comes first.
void *Func_080cce94(s32 id)
{
    void *event = Func_080ccd78(0, id);
    struct PartyState *party = &gPartyState;
    s32 stopped = *(s16 *)party->unknown_26a;

    if (stopped == id) {
        void *alt = Func_080ccd78(7, stopped);

        if (alt != NULL)
            return alt;
    }
    return event;
}
