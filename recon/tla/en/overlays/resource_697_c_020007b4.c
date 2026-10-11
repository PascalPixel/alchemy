/* Draft, differ 0 with an empty asm barrier after the height load (the native loads the leader height before it builds the threshold; without the barrier differ 2). Held back: the device ratchet (tla asm-barrier 197) may not rise. Tried without it: an object pointer, a limit temporary, a const height, a height inline, a subtraction. */
#include "../../../../games/tla/src/field/overlay_697/ov697.h"

void Func_020007b4(void)
{
    s32 y = Object_GetById(gPartyState.current_owner)->y;

    /* FAKEMATCH: loads the leader height before it builds the threshold. */
    __asm__("");
    if (y == 0x200000)
        GameFlag_SetBit(0x208);
    else
        GameFlag_ClearBit(0x208);
}
