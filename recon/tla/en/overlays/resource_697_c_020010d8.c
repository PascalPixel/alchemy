/* Draft, differ 0 with an empty asm barrier after the party base (the native loads the base before it builds the render-mode offset; without it differ 2). Held back: the device ratchet (tla asm-barrier 197) may not rise. */
#include "../../../../games/tla/src/field/overlay_697/ov697.h"

static __inline__ void Map_CopyCells(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    Func_02005818(a, b, c, d, e, f);
}

void Func_020010d8(void)
{
    struct PartyState *party = &gPartyState;

    /* FAKEMATCH: loads the party base before it builds the field offset. */
    __asm__("");
    if (party->render_mode == 2)
        Func_020059c8();
    else
        Func_02000e40(18, 0, 1, 1);
}
