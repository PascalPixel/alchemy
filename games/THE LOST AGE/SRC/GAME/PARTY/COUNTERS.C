#include "TYPES.H"
#include "PARTY_STATE.H"

/* Adds to the party's second six-digit counter, kept within 0..999999. */
s32 Party_AdjustSixDigitCounterB(s32 amount)
{
    s32 value;

    value = gPartyState.counter_b;
    value = (s32)((u32)value + (u32)amount);
    if (value > 0xf423f)
        value = 0xf423f;
    if (value < 0)
        value = 0;
    gPartyState.counter_b = value;
    return value;
}
