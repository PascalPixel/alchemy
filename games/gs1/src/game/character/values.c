#include "types.h"

struct OwnerValueState {
    u8 unknown[0x34];
    s16 first_max;
    s16 second_max;
    s16 first;
    s16 second;
};

void *Owner_GetState(s32);
void Owner_RecalculateRatios(s32);

s16 Owner_AdjustFirstValue(s32 owner, s32 delta)
{
    struct OwnerValueState *state = Owner_GetState(owner);
    s32 cur = state->first;
    s32 max = state->first_max;
    s32 value = cur + delta;
    s32 result;

    if (value > max)
        result = max;
    else {
        result = 0;
        if (value >= 0)
            result = value;
    }
    state->first = result;
    Owner_RecalculateRatios(owner);
    return state->first;
}

s16 Owner_AdjustSecondValue(s32 owner, s32 delta)
{
    struct OwnerValueState *state = Owner_GetState(owner);
    s32 cur = state->second;
    s32 max = state->second_max;
    s32 value = cur + delta;
    s32 result;

    if (value > max)
        result = max;
    else {
        result = 0;
        if (value >= 0)
            result = value;
    }
    state->second = result;
    Owner_RecalculateRatios(owner);
    return state->second;
}
