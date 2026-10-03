#include "OWNER_STATE.H"
#include "OWNERVAL.H"

void Owner_RecalculateRatios(s32);

s32 Owner_AdjustFirstValue(s32 owner, s32 delta)
{
    struct BattleUnit *state = Owner_GetState(owner);
    s32 cur = state->hp;
    s32 max = state->max_hp;
    s32 value = cur + delta;
    s32 result;

    if (value > max)
        result = max;
    else {
        result = 0;
        if (value >= 0)
            result = value;
    }
    state->hp = result;
    Owner_RecalculateRatios(owner);
    return state->hp;
}
