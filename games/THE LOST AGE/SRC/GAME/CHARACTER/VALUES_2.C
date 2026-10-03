#include "OWNER_STATE.H"

void Owner_RecalculateRatios(s32);

s16 Owner_AdjustSecondValue(s32 owner, s32 delta)
{
    struct BattleUnit *state = Owner_GetState(owner);
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
    Owner_RecalculateRatios(owner);
    return state->pp;
}
