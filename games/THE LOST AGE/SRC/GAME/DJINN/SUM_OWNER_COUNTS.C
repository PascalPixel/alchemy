#include "TYPES.H"
#include "OWNER_STATE.H"

/* One party member's djinn count for an element, or of all four elements
   when the index is -1. */
s32 Owner_SumDjinnCounts(s32 owner, s32 index)
{
    struct OwnerValueState *state;
    s32 result;

    state = Owner_GetState(owner);
    if (index == -1) {
        result = state->values[0];
        result += state->values[1];
        result += state->values[2];
        result += state->values[3];
    } else {
        result = state->values[index];
    }
    return result;
}
