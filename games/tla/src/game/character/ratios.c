#include "owner_state.h"
#include "fixed_math.h"

void UnitRatio(s32 owner_no)
{
    s32 first;
    s32 second;
    s32 first_value;
    s32 second_value;
    struct BtlUnit *owner;

    owner = UnitGet(owner_no);
    first = (owner->hp << 14) / owner->max_hp;
    first_value = 0x4000;
    if (first <= 0x4000) {
        first_value = 0;
        if (first >= 0) {
            first_value = first;
        }
    }
    owner->hp_gauge = first_value;
    if (owner->hp_gauge == 0 && owner->hp != 0) {
        first_value = 1;
        owner->hp_gauge = first_value;
    }
    second = (owner->ep << 14) / owner->max_ep;
    second_value = 0x4000;
    if (second <= 0x4000) {
        second_value = 0;
        if (second >= 0) {
            second_value = second;
        }
    }
    owner->ep_gauge = second_value;
    if (owner->ep_gauge == 0 && owner->ep != 0) {
        second_value = 1;
        owner->ep_gauge = second_value;
    }
}

void Owner_UpdateRatioPair(struct BtlUnit *state, s32 input)
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

    value = (state->ep << 14) / state->max_ep;
    {
        s32 output = 0x4000;

        if (value <= output) {
            output = 0;
            if (value >= 0) {
                output = value;
            }
        }
        state->ep_gauge = output;
        if ((output << 16) == 0 && state->ep != 0) {
            state->ep_gauge = 1;
        }
    }
}

