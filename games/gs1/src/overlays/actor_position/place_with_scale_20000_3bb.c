#include "types.h"

#define SceneActor_PlaceWithScale20000 Func_020030a8

/* STILL-OPEN: adopt --where differing_bytes=11/64. alchemist.ts refused
 * (tiers: class, priority) -- matches the documented DEAD-END FINGERPRINT
 * (adjacent register swap in an asrs/str block), genuinely unfixed. */

/* Per-site veneers (raw sub_ symbols from the overlay .s). */
extern s32 *Func_02007128(s32 selector);
extern void Func_02006f34(s32 *actor);
extern void Func_02006f1c(s32 *actor, s32 mode);
extern void Func_02006f58(s32 *actor, s32 x, s32 y, s32 z);

void SceneActor_PlaceWithScale20000(s32 selector, s32 x, s32 z)
{
    s32 *actor = Func_02007128(selector);

    if (actor != 0) {
        s32 scale = 0x20000;
        u8 *p = (u8 *)actor;
        u8 zero = 0;
        *(s32 *)((u8 *)actor + 48) = scale;
        *(s32 *)((u8 *)actor + 52) = scale >> 1;
        p += 91;
        *p = zero;

        Func_02006f34(actor);
        Func_02006f1c(actor, 5);
        Func_02006f58(actor, x << 16, actor[3], z << 16);
    }
}
