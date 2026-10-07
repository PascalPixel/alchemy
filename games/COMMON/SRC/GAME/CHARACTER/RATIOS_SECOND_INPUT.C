#include "BATTLE_UNIT.H"

void Owner_UpdateSecondInputAndRatios(
    struct BtlUnit *state, s32 input)
{
    s32 clamped;
    s32 value;

    if (input > state->max_ep) {
        clamped = state->max_ep;
    } else {
        clamped = 0;
        if (input >= 0) {
            clamped = input;
        }
    }
    /* FAKEMATCH: the one-pass loop keeps the store before the ratio-input reads. */
    do {
        state->ep = clamped;
    } while (0);
    value = state->hp;
    value <<= 14;
    value = value / state->max_hp;

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

    {
        s32 numerator;
        s32 divisor;

        numerator = state->ep;
        divisor = state->max_ep;
        value = (numerator << 14) / divisor;
    }
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

