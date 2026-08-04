/*
 * resource_3b3 @ 0x020020b8 (56 bytes including trailing alignment).
 *
 * Compares an actor with slot zero.  When it is farther right, bit 1 at +35
 * is cleared and then restored only if the actor is also above slot zero.
 * The function always returns zero.
 */
#include "types.h"

u8 *Func_0808a080(s32 slot);

s32 Func_020020b8(u8 *actor)
{
    u8 *reference = Func_0808a080(0);

    if (*(s32 *)(actor + 16) > *(s32 *)(reference + 16)) {
        actor[35] = (u8)(actor[35] & 0xfd);
        if (*(s32 *)(actor + 12) < *(s32 *)(reference + 12))
            actor[35] = (u8)(actor[35] | 2);
    }

    return 0;
}
