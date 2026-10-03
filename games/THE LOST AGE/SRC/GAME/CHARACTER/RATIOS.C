#include "OWNER_STATE.H"
#include "FIXED_MATH.H"


void Owner_RecalculateRatios(s32 owner_no)
{
    s32 first;
    s32 second;
    s32 first_value;
    s32 second_value;
    struct BattleUnit *owner;

    owner = Owner_GetState(owner_no);
    first = (s32)((u32)(s32)owner->hp << 14) / owner->max_hp;
    first_value = 0x4000;
    if (first <= 0x4000) {
        first_value = 0;
        if (first >= 0) {
            first_value = first;
        }
    }
    owner->hp_gauge = first_value;
    if ((((u32)first_value << 16) == 0) && (owner->hp != 0)) {
        first_value = 1;
        owner->hp_gauge = first_value;
    }
    second = (s32)((u32)(s32)owner->pp << 14) / owner->max_pp;
    second_value = 0x4000;
    if (second <= 0x4000) {
        second_value = 0;
        if (second >= 0) {
            second_value = second;
        }
    }
    owner->pp_gauge = second_value;
    if ((((u32)second_value << 16) == 0) && (owner->pp != 0)) {
        second_value = 1;
        owner->pp_gauge = second_value;
    }
}

void Owner_UpdateRatioPair(struct BattleUnit *state, s32 input)
{
    s32 value;

    if (input > state->max_hp) {
        value = state->max_hp;
    } else {
        value = 0;
        if (input >= 0) {
            value = input;
        }
    }
    state->hp = value;
    value = ((value << 16) >> 2) / state->max_hp;

    {
        s32 output = 0x4000;

        if (value <= output) {
            output = 0;
            if (value >= 0) {
                output = value;
            }
        }
        state->hp_gauge = output;
        if ((output << 16) == 0 && state->hp != 0) {
            state->hp_gauge = 1;
        }
    }

    value = (state->pp << 14) / state->max_pp;
    {
        s32 output = 0x4000;

        if (value <= output) {
            output = 0;
            if (value >= 0) {
                output = value;
            }
        }
        state->pp_gauge = output;
        if ((output << 16) == 0 && state->pp != 0) {
            state->pp_gauge = 1;
        }
    }
}

