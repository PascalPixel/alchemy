#include "owner_state.h"

void UnitRatio(s32);

s16 UnitEpAdd(s32 owner, s32 delta)
{
    struct BtlUnit *state = UnitGet(owner);
    s32 cur = state->ep;
    s32 max = state->max_ep;
    s32 value = cur + delta;
    s32 result;

    if (value > max)
        result = max;
    else {
        result = 0;
        if (value >= 0)
            result = value;
    }
    state->ep = result;
    UnitRatio(owner);
    return state->ep;
}
