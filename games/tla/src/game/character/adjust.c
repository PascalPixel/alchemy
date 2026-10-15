#include "owner_state.h"

void UnitRatio(s32);

s16 UnitPpAdd(s32 owner, s32 delta)
{
    struct BtlUnit *state = UnitGet(owner);
    s32 cur = state->pp;
    s32 max = state->max_pp;
    s32 value = cur + delta;
    s32 result;

    if (value > max)
        result = max;
    else {
        result = 0;
        if (value >= 0)
            result = value;
    }
    state->pp = result;
    UnitRatio(owner);
    return state->pp;
}
