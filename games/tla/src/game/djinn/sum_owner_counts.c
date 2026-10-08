#include "types.h"
#include "owner_state.h"

s32 Owner_SumDjinnCounts(s32 owner, s32 index)
{
    struct OwnerDjinnState *state;
    s32 result;

    state = UnitGet(owner);
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
