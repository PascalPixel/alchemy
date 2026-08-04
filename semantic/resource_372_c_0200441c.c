#include "types.h"

/*
 * Resource 372 two-slot actor spawner at 0x0200441c (308 bytes, 4 calls).
 *
 * Complete owner: `push {r5, r6, r7, lr}` + the high-register saves at
 * 0x0200441c, matching `add sp, #8 / pop {r3, r5, r6, r7} / mov r8..fp /
 * pop {r5, r6, r7} / pop {r0} / bx r0` at 0x02004534.  `pop {r0} ; bx r0` —
 * r0 is the popped return address, so the owner returns nothing.  There are
 * two interior literal pools, at 0x02004476-0x0200447f and
 * 0x020044e6-0x020044ef, each branched over (`b.n` at 0x02004474 and 0x020044e4
 * respectively) and each genuinely referenced by `ldr rN, [pc, #imm]`; the pool
 * word 0x02004478 is the constant 0 that is loaded into r8 and stored three
 * times.  `a prior working note` parked this row for the
 * byte-exact reconstruction over that mid-function pool.  That is a code-placement
 * obstacle, not a semantic one.
 *
 * LINK BASE — second independent witness for 0x02008000.  The two pool words
 * installed into field +0x6c of the spawned objects are 0x0200c3c9 and
 * 0x0200c379.  Minus the 0x8000 link shift and the Thumb bit those are exactly
 * 0x020043c8 and 0x02004378, both real function starts with byte-exact sources
 * in `assets/code/`, and both of the right shape: one-argument per-frame
 * updaters over the same object type.  (The jump table in
 * `semantic/overlays/resource_372_c_02003ce4.c` is the first witness.)
 *
 * `assets/code/resource_372_c_020043c8.c` also fixes two field meanings used
 * here: +0x68 (104) is the parent/descriptor back-pointer and +0x64 (100) is a
 * u16 frame counter, both of which this owner initialises.
 *
 * Observations / uncertainties:
 *  - r4 is written at 0x020044fe and 0x0200451a but is never saved by the
 *    prologue.  The bytes are not in doubt (`6d14`, `6d04`); it is recorded
 *    here as an oddity of the original codegen and has no effect on the
 *    reconstructed behaviour.
 *  - the tail dereferences both stack slots unconditionally even though the
 *    loop stores a null when 0x02008ac0 fails; that is faithful to the code.
 *  - the return value of 0x02008cec is unused.
 *  - 0x03001b10 is a RAM table of 4-byte entries indexed by the sprite index
 *    byte; only its second halfword is read.  0x03001f30 holds a pointer to a
 *    global record whose halfword at +70 supplies that index.
 */

/* Imports, old-style: interfaces unknown.  Named by the address each call site
 * computes, per this overlay's established convention. */
void Func_080f9010();
u8 *Func_080090c8();
void Func_08009020();
void Func_080001b8();

/* In-image per-frame updaters installed as callbacks (pool words 0x0200c3c9
 * and 0x0200c379 = these entry points plus the Thumb bit). */
void Func_020043c8();
void Func_02004378();

void Func_0200441c(u8 *desc)
{
    u8 *slot[2];
    u8 *gp;          /* fp: *(void **)0x03001f30 */
    u8 *obj;
    u8 *spr;
    const u16 *tbl;
    s32 i;
    u32 v;

    gp = *(u8 **)0x03001f30;
    Func_080f9010(152);

    for (i = 0; i <= 1; i++) {
        obj = Func_080090c8(26, *(s32 *)(desc + 8), *(s32 *)(desc + 12),
                            *(s32 *)(desc + 16));
        slot[i] = obj;
        if (obj == 0) {
            continue;
        }

        *(s32 *)(obj + 20) = *(s32 *)(desc + 20);
        spr = *(u8 **)(obj + 80);
        obj[85] = 0;
        *(u16 *)(obj + 100) = 0;      /* frame counter, f64 in 0x020043c8 */
        *(u8 **)(obj + 104) = desc;   /* parent back-pointer, f68 */
        if (spr == 0) {
            continue;
        }

        Func_08009020(spr, 0);
        spr[38] = 0;
        Func_080001b8(spr[28]);

        spr[28] = (u8)*(u16 *)(gp + 70);
        spr[29] |= 1;

        tbl = (const u16 *)0x03001b10;             /* 4 bytes per entry */
        v = ((u32)tbl[spr[28] * 2 + 1] << 17) >> 22;  /* (field & 0x7fff) >> 5 */
        *(u16 *)(spr + 8) = (u16)((*(u16 *)(spr + 8) & 0xfc00) | v);

        /* 0xdf and 63 are applied as two separate masks by the original. */
        spr[5] = (u8)(((spr[5] & 0xdf) & 63) | 64);
        spr[7] = (u8)((spr[7] & 63) | 128);

        (*(u8 **)(spr + 40))[22] = 0;
    }

    /* Slot 0 */
    *(void **)(slot[0] + 108) = (void *)Func_020043c8;
    {
        u8 *src = *(u8 **)(desc + 80);
        u8 *dst = *(u8 **)(slot[0] + 80);
        dst[9] = (u8)((dst[9] & ~12) | (src[9] & 12));
    }

    /* Slot 1 */
    {
        u8 *src = *(u8 **)(desc + 80);
        u8 *dst = *(u8 **)(slot[1] + 80);
        u8 merged = (u8)((dst[9] & ~12) | (src[9] & 12));
        *(void **)(slot[1] + 108) = (void *)Func_02004378;
        dst[9] = merged;
        slot[1][35] = 2;
    }
}
