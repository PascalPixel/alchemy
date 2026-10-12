/* Near miss of Func_02001c38 (listing 34 instructions, draft 30, 16 differ): the native builds
 * the address of gPartyState.special inside the loop (ldr, movs, lsls, adds, adds on every pass);
 * the draft's loop pass moves it out as one literal held in r6. Tried: the scene layer's inline
 * flag helpers, a one-pass block around the test. */
#include "../../../../games/tla/src/field/overlay_656/ov656.h"

s32 Func_02001c38(void)
{
    u32 i;

    i = 0;
next:
        if (GameFlag_Test(i + 0x300) && gPartyState.special == 1) {
            GameFlag_SetBit(i + 0x930);
            GameFlag_SetBit(i + 0x220);
        }
        GameFlag_ClearBit(i + 0x300);
    i++;
    if (i < 16)
        goto next;
    return 0;
}
