#include "types.h"

void *Owner_GetState(s32);
s32 FixedPoint_Ratio(s32, s32);

void Owner_RatioNoOp(void)
{
}

struct OwnerRatioState {
    u8 unknown_00[0x14];
    s16 value_14;
    s16 value_16;
    u8 unknown_18[0x1c];
    s16 divisor_34;
    s16 divisor_36;
    s16 value_38;
    s16 value_3a;
};

void Owner_RecalculateRatios(s32 owner_no)
{
    s32 first;
    s32 second;
    s32 first_value;
    s32 second_value;
    struct OwnerRatioState *owner;

    owner = Owner_GetState(owner_no);
    first = FixedPoint_Ratio(
        (s32)((u32)(s32)owner->value_38 << 14), owner->divisor_34);
    first_value = 0x4000;
    if (first <= 0x4000) {
        first_value = 0;
        if (first >= 0) {
            first_value = first;
        }
    }
    owner->value_14 = first_value;
    if ((((u32)first_value << 16) == 0) && (owner->value_38 != 0)) {
        first_value = 1;
        owner->value_14 = first_value;
    }
    second = FixedPoint_Ratio(
        (s32)((u32)(s32)owner->value_3a << 14), owner->divisor_36);
    second_value = 0x4000;
    if (second <= 0x4000) {
        second_value = 0;
        if (second >= 0) {
            second_value = second;
        }
    }
    owner->value_16 = second_value;
    if ((((u32)second_value << 16) == 0) && (owner->value_3a != 0)) {
        second_value = 1;
        owner->value_16 = second_value;
    }
}

void Owner_UpdateRatioPair(struct OwnerRatioState *state, s32 input)
{
    s32 value;

    if (input > state->divisor_34) {
        value = state->divisor_34;
    } else {
        value = 0;
        if (input >= 0) {
            value = input;
        }
    }
    state->value_38 = value;
    value = FixedPoint_Ratio((value << 16) >> 2, state->divisor_34);

    {
        s32 output = 0x4000;

        if (value <= output) {
            output = 0;
            if (value >= 0) {
                output = value;
            }
        }
        state->value_14 = output;
        if ((output << 16) == 0 && state->value_38 != 0) {
            state->value_14 = 1;
        }
    }

    value = FixedPoint_Ratio(state->value_3a << 14, state->divisor_36);
    {
        s32 output = 0x4000;

        if (value <= output) {
            output = 0;
            if (value >= 0) {
                output = value;
            }
        }
        state->value_16 = output;
        if ((output << 16) == 0 && state->value_3a != 0) {
            state->value_16 = 1;
        }
    }
}
