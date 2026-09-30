/*
 * Draft: Party_AdjustSixDigitCounterA does not yet match; the reference loads
 * 999999 before the coins (1 swap), in plain and in ☀️'s pointer form.
 * Links as recon/tla/raw/080afbec.s.
 */
#include "TYPES.H"
#include "PARTY_STATE.H"

/* Adds to the party's coins, kept within 0..999999. */
s32 Party_AdjustSixDigitCounterA(s32 amount)
{
    s32 value;

    struct PartyState *work;
    struct PartyState *store;

    work = &gPartyState;
    value = work->coins;
    value = (s32)((u32)value + (u32)amount);
    store = work;
    if (value > 0xf423f)
        value = 0xf423f;
    if (value < 0)
        value = 0;
    work = store;
    work->coins = value;
    return value;
}
