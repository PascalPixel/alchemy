/* Draft, differ 0 with an empty asm barrier after the height load (the native loads the leader height before it builds the threshold; without the barrier differ 2). Held back: the device ratchet (tla asm-barrier 197) may not rise. Tried without it: an object pointer, a limit temporary, a const height, a height inline, a subtraction. */
#include "../../../../games/tla/src/field/overlay_697/ov697.h"

extern const u8 Data_02007734[];

void Func_02000c84(void)
{
    s32 y = Object_GetById(gPartyState.current_owner)->y;

    /* FAKEMATCH: loads the leader height before it builds the threshold. */
    __asm__("");
    if (y >= 0x100000) {
        Func_020059c0();
    } else if (Func_02005a30()) {
        Func_02005a38();
        Func_020009ac(0, 0);
    }
}
