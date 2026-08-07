#include "types.h"

/* STILL-OPEN: adopt --where differing_bytes=13/80. alchemist.ts refused
 * (tiers: class, priority) -- matches the documented DEAD-END FINGERPRINT
 * (adjacent register swap in an asrs/str block), genuinely unfixed. */

/*
 * resource_3bb owner at 0x020030e8, 80 bytes: spawn an object and run a
 * short fixed setup sequence on it.
 *
 * Complete owner: `push {r5, r6, r7, lr}` at 0x020030e8 through `pop
 * {r5, r6, r7} / pop {r0} / bx r0` at 0x02003130-0x02003134, followed by
 * one alignment halfword; the next owner's prologue is at 0x02003138.
 * Three arguments (r0, r1, r2), void. `r0` is never freshly loaded before
 * the first call, so it is the forwarded first argument.
 *
 * Not found by the structural inventory walk (unindexed): reached only by
 * `bl`, resolved with `bun tools/lib/overlay_call_targets.ts`'s `+2` rule.
 *
 * Uncertainty: none of the six callees are identified beyond call shape;
 * `object`'s fields (0x30/0x34, set to a fixed 0x14000/0xa000 pair, 0x5b
 * cleared, 0xc read back for the position setup call) are inferred only
 * from this call shape.
 */

/* Per-site veneers (raw sub_ symbols from the overlay .s). */
extern u8 *Func_02007168(s32 arg0);
extern void Func_02006f74(void);
extern void Func_02006f5c(u8 *object, s32 arg1);
extern void Func_02006f98(u8 *object, s32 arg1, s32 arg2, s32 arg3);
extern void Func_02006fa6(u8 *object);
extern void Func_02006f76(u8 *object, s32 arg1);

void Func_020030e8(s32 arg0, s32 arg1, s32 arg2)
{
    u8 *object = Func_02007168(arg0);

    if (object == 0) {
        return;
    }

    {
        s32 v = 0x14000;
        u8 *p = object;
        u8 zero = 0;
        *(s32 *)(object + 0x30) = v;
        *(s32 *)(object + 0x34) = v >> 1;
        p += 0x5b;
        *p = zero;
    }

    Func_02006f74();
    Func_02006f5c(object, 5);
    Func_02006f98(object, arg1 << 16, *(s32 *)(object + 12), arg2 << 16);
    Func_02006fa6(object);
    Func_02006f76(object, 1);
}
