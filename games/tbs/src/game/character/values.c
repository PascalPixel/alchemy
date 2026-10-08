#include "owner_state.h"
#include "types.h"
#include "battle_unit.h"
#include "character.h"

s32 Owner_AdjustFirstValue(s32 owner, s32 delta)
{
    struct BtlUnit *state = UnitGet(owner);
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
    UnitRatio(owner);
    return state->hp;
}

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
