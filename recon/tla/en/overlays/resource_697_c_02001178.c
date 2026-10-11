/* Draft, differ 0 with an empty asm barrier after the height load (the native loads the leader height before it builds the threshold; without the barrier differ 2). Held back: the device ratchet (tla asm-barrier 197) may not rise. Tried without it: an object pointer, a limit temporary, a const height, a height inline, a subtraction. */
#include "../../../../games/tla/src/field/overlay_697/ov697.h"

static __inline__ void Map_CopyCells(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    Func_02005818(a, b, c, d, e, f);
}

void Func_02001178(void)
{
    struct ObjectRuntime *leader = Object_GetById(gPartyState.current_owner);
    struct ObjectRuntime *block = Object_GetById(15);

    if (block->x >> 20 == leader->x >> 20 && block->z >> 20 == leader->z >> 20) {
        s32 y = leader->y;

        /* FAKEMATCH: loads the leader height before it builds the threshold. */
        __asm__("");
        if (y < 0x200000)
            leader->y = 0x200000;
    }
}
