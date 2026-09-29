#include "TYPES.H"
#include "FIXED_MATH.H"

void *Owner_GetState(s32);

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
    value = Math_Div((value << 16) >> 2, state->divisor_34);

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

    value = Math_Div(state->value_3a << 14, state->divisor_36);
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

