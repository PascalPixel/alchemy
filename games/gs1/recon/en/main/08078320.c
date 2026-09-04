#include "types.h"

struct OwnerRatioPairState {
    u8 padding0[20];
    s16 outputX;
    s16 outputY;
    u8 padding1[28];
    s16 limitX;
    s16 limitY;
    s16 inputX;
    s16 inputY;
};

s32 FixedPoint_Ratio(s32, s32);

void Owner_UpdateSecondInputAndRatios(
    struct OwnerRatioPairState *state, s32 input)
{
    s32 clamped;
    s32 value;

    if (input > state->limitY) {
        clamped = state->limitY;
    } else {
        clamped = 0;
        if (input >= 0) {
            clamped = input;
        }
    }
    state->inputY = clamped;
    value = state->inputX;
    value <<= 14;
    value = FixedPoint_Ratio(value, state->limitX);

    {
        s32 output = 0x4000;

        if (value <= output) {
            output = 0;
            if (value >= 0) {
                output = value;
            }
        }
        state->outputX = output;
        if ((output << 16) == 0 && state->inputX != 0) {
            state->outputX = 1;
        }
    }

    {
        s32 numerator;
        s32 divisor;

        numerator = state->inputY;
        divisor = state->limitY;
        value = FixedPoint_Ratio(numerator << 14, divisor);
    }
    {
        s32 output = 0x4000;

        if (value <= output) {
            output = 0;
            if (value >= 0) {
                output = value;
            }
        }
        state->outputY = output;
        if ((output << 16) == 0 && state->inputY != 0) {
            state->outputY = 1;
        }
    }
}
