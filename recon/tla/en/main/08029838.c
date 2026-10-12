/*
 * Draft of Func_08029838. 14 instructions differ, all one register choice:
 * the listing moves the argument to r6 at entry and keeps the pressed flag
 * in r0 (adds r6, r0; movs r0, #0 before the null test), the draft keeps the
 * argument in r0 and the flag in r6. Tried: the flag as a local set before
 * the null test, returned at the null test, as the one result variable of
 * the function, and as the result of an inline (the zero then comes after
 * the null test). The three key fields at 0x230, 0x234 and 0x236 of the
 * party state are named keys_230, keys_234 and keys_236 for it.
 */
#include "types.h"
#include "ram_buffer.h"
#include "party_state.h"
#include "input.h"

/* FAKEMATCH: the pressed keys are read again for each of the three tests; as a plain field one read serves all three (47 differ) */
#define INPUT_PRESSED (((volatile struct InputState *)&gInput)->pressed)

struct EventWork {
    u8 unknown_000[0x166];
    s16 request;
};

s32 Func_080c8828();
void Audio_PlayCue();
s32 GameFlag_Test();

s32 Func_08029838(s32 arg)
{
    struct EventWork *work = Ram_HeapSlots->event_work;
    s32 pressed = 0;

    if (work == NULL)
        return 0;
    if ((INPUT_PRESSED & gPartyState.keys_234) || (INPUT_PRESSED & gPartyState.keys_230)
        || (INPUT_PRESSED & gPartyState.keys_236))
        pressed = 1;
    if (pressed) {
        if (Func_080c8828(arg)) {
            Audio_PlayCue(113);
        } else if (!GameFlag_Test(0x146)) {
            work->request = 0x208b;
            return 1;
        }
    }
    return 0;
}
