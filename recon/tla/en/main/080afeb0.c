/*
 * Draft: Party_AdjustSixDigitCounterA does not yet match; 4 halfwords differ from ☀️'s C, first at +0x4 (ldr r2, [pc, #24]).
 * Links as recon/tla/raw/080afbec.s.
 */
#include "GLOBAL_PROGRESS.H"

extern struct GameState gGameState;

struct PartyCounterWork {
    u8 unknown_00[0x10];
    s32 value;
};

s32 Party_AdjustSixDigitCounterA(s32 amount)
{
    s32 value;
    struct PartyCounterWork *work;
    struct PartyCounterWork *store;

    work = (struct PartyCounterWork *)&gGameState;
    value = work->value;
    value = (s32)((u32)value + (u32)amount);
    store = work;
    if (value > 0xF423F) {
        value = 0xF423F;
    }
    if (value < 0) {
        value = 0;
    }
    work = store;
    work->value = value;
    return value;
}
