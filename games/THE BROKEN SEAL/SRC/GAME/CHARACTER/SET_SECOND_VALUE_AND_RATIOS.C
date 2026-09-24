#include "TYPES.H"

struct OwnerRatioState {
    u8 unknown_00[20];
    s16 first_ratio;
    s16 second_ratio;
    u8 unknown_18[28];
    s16 first_max;
    s16 second_max;
    s16 first;
    s16 second;
};

s32 FixedPoint_Ratio(s32 numerator, s32 denominator);

/* Sets the second value, clamped to 0..second_max, and refreshes both
   Q14 ratios; a nonzero value never shows as a zero ratio. */
void Owner_SetSecondValueAndRatios(struct OwnerRatioState *state, s32 value)
{
    s32 clamped;
    s32 ratio;

    if (value > state->second_max) {
        clamped = state->second_max;
    } else {
        clamped = 0;
        if (value >= 0)
            clamped = value;
    }
    /* FAKEMATCH: the do-while keeps the store ahead of the first value's
       load. */
    do {
        state->second = clamped;
    } while (0);
    ratio = state->first;
    ratio <<= 14;
    ratio = FixedPoint_Ratio(ratio, state->first_max);
    {
        s32 output = 0x4000;

        if (ratio <= output) {
            output = 0;
            if (ratio >= 0)
                output = ratio;
        }
        state->first_ratio = output;
        if ((output << 16) == 0 && state->first != 0)
            state->first_ratio = 1;
    }
    {
        s32 numerator;
        s32 divisor;

        numerator = state->second;
        divisor = state->second_max;
        ratio = FixedPoint_Ratio(numerator << 14, divisor);
    }
    {
        s32 output = 0x4000;

        if (ratio <= output) {
            output = 0;
            if (ratio >= 0)
                output = ratio;
        }
        state->second_ratio = output;
        if ((output << 16) == 0 && state->second != 0)
            state->second_ratio = 1;
    }
}
