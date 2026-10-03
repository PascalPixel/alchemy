/* Clamp the second input to 0..limitY and store it, then refresh both
   output ratios (input << 14 over its limit, clamped to 0..0x4000, and at
   least 1 while the input is nonzero). The first-input sibling is
   Owner_UpdateRatioPair in RATIOS.C. */
#include "BATTLE_UNIT.H"


void Owner_UpdateSecondInputAndRatios(
    struct BattleUnit *state, s32 input)
{
    s32 clamped;
    s32 value;

    if (input > state->max_pp) {
        clamped = state->max_pp;
    } else {
        clamped = 0;
        if (input >= 0) {
            clamped = input;
        }
    }
    /* FAKEMATCH: a one-pass loop around the store orders it before the
       ratio inputs are read, as in the ROM. */
    do {
        state->pp = clamped;
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

        numerator = state->pp;
        divisor = state->max_pp;
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
        state->pp_gauge = output;
        if ((output << 16) == 0 && state->pp != 0) {
            state->pp_gauge = 1;
        }
    }
}

