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
 * `bl`, resolved with `cargo run --release --manifest-path tools/overlay-call-targets/Cargo.toml --`'s `+2` rule.
 *
 * Uncertainty: none of the six callees are identified beyond call shape;
 * `obj`'s fields (0x30/0x34, set to a fixed 0x14000/0xa000 pair, 0x5b
 * cleared, 0xc read back for the position setup call) are inferred only
 * from this call shape.
 */

/* Per-site veneers (raw sub_ symbols from the overlay .s). */
extern u8 *Func_02007168(s32 no);
extern void Func_02006f74(void);
extern void Func_02006f5c(u8 *obj, s32 x);
extern void Func_02006f98(u8 *obj, s32 x, s32 z, s32 arg3);
extern void Func_02006fa6(u8 *obj);
extern void Func_02006f76(u8 *obj, s32 x);

void SceneActor_PlaceWithScale14000(s32 no, s32 x, s32 z)
{
    u8 *obj = Func_02007168(no);

    if (obj == 0) {
        return;
    }

    {
        s32 scale = 0x14000;
        u8 *p = obj;
        u8 zero = 0;
        *(s32 *)(obj + 0x30) = scale;
        *(s32 *)(obj + 0x34) = scale >> 1;
        p += 0x5b;
        *p = zero;
    }

    Func_02006f74();
    Func_02006f5c(obj, 5);
    Func_02006f98(obj, x << 16, *(s32 *)(obj + 12), z << 16);
    Func_02006fa6(obj);
    Func_02006f76(obj, 1);
}
