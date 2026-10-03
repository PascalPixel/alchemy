#include "TYPES.H"
#include "OWNER_STATE.H"

/* One party member's djinn count for an element, or of all four elements
   when the index is -1. */
s32 Owner_SumDjinnCounts(s32 owner, s32 index)
{
    struct OwnerDjinnState *state;
    s32 result;

    state = Owner_GetState(owner);
    if (index == -1) {
        result = state->counts.banks.owned[0];
        result += state->counts.banks.owned[1];
        result += state->counts.banks.owned[2];
        result += state->counts.banks.owned[3];
    } else {
        result = state->counts.banks.owned[index];
    }
    return result;
}
