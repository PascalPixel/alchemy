#include "types.h"

/*
 * resource_3c5 owner at 0x02000ae8, 472 bytes: code 0x02000ae8-0x02000cb3 and
 * the three pool words 0x0200b134, 0x02008ab1 and 0xffff0000 at
 * 0x02000cb4-0x02000cbf.  The overlay's effect spawner.
 *
 * TRANSPOSED from semantic/overlays/resource_39f_c_02000ae8.c.  The two owners
 * are the same routine shared verbatim: over all 236 halfwords they differ in
 * 11 places, 10 of which are BL halfwords and exactly one of which is the low
 * half of the pool word at +0x1cc naming the descriptor table.  Notably the
 * second pool word 0x02008ab1 is byte-identical, i.e. this overlay's per-frame
 * integrator also sits at 0x02000ab0.
 *
 * What was changed:
 *  - Data_0200b058 becomes Data_0200b134 (file offset 0x3134 under the
 *    proven 0x02008000 link base);
 *  - the calls were re-resolved with 'bun tools/overlay_call_targets.ts
 *    resource_3c5 0ae8': the same 10 sites, 6 distinct targets, all veneers
 *    publishing Func_03000380, Func_08009080, Func_08009098, Func_080090c8,
 *    Func_0808a080 and Func_0808a160 with the same multiplicities as in
 *    resource_39f.  Import names are therefore unchanged; only the veneer
 *    offsets differ, and those do not appear in C.
 *
 * It creates an effect record at (x, y, z), gives it the constant velocity
 * (vx, vy, vz) that the per-frame handler at 0x02000ab0 integrates, and then
 * applies whichever of the optional fields 'flags' selects from 'options'.
 *
 * Func_03000380 is an ARM-mode helper relocated into IWRAM; it is called with a
 * distance and the descriptor's word at +12, and its result becomes a per-frame
 * step, so it is the division helper.
 *
 * The epilogue is 'add sp, #8 / pop {r3, r5, r6, r7} / ... / pop {r0} / bx r0',
 * so the owner is void.
 */

extern u32 Data_0200b134[];   /* first three words are descriptor pointers */

/* Returns the party record; only its presentation block at +80 is read. */


/* Creates the effect record and returns it, or 0 on failure. */






/* Relocated IWRAM helper: turns a distance and a descriptor duration into a
 * per-frame step. */


extern u8 * Func_0200395a();
extern u8 * Func_020038d8();
extern void Func_020038e2();
extern void Func_020038fc();
extern void Func_02003a96();
extern s32 Func_020039a0();
extern s32 Func_020039b8();
extern s32 Func_020039c6();
extern void Func_020039fc();
extern void Func_02003a0c();
void Func_02000ae8(s32 x, s32 y, s32 z, s32 vx, s32 vy, s32 vz,
                   u32 flags, const u8 *options)
{
    u8 *party;
    u8 *effect;
    u8 *block;
    u16 *tag;
    s32 kind;

    party = Func_0200395a(0);

    /* 128 << 13.  With that bit set and an options block present the effect's
     * kind comes from the options rather than from the default 222. */
    if ((flags & 0x100000) != 0 && options != 0) kind = *(s16 *)(options + 24);
    else kind = 222;

    effect = Func_020038d8(kind, x, y, z);
    if (effect == 0) return;

    block = *(u8 **)(effect + 80);

    Func_020038e2(effect, (flags + 1) & 15);
    Func_020038fc(effect, Data_0200b134[flags & 15]);

    effect[85] = 0;
    block[38] = 0;

    /* 0x02008ab1 is Func_02000ab0 with the Thumb bit: the per-frame
     * integrator. */
    *(u32 *)(effect + 108) = 0x02008ab1;

    *(s32 *)(effect + 68) = vx;
    *(s32 *)(effect + 72) = vy;
    *(s32 *)(effect + 76) = vz;

    /* Bits 2 and 3 of the effect's mode byte are copied from the party's. */
    block[9] = (u8)((block[9] & 0xf3) | ((*(u8 **)(party + 80))[9] & 0x0c));

    *(s32 *)(effect + 48) = 0;
    *(s32 *)(effect + 52) = 0;

    tag = (u16 *)(effect + 100);
    *tag = 0;

    /* Everything below is optional detail: the whole block is skipped unless
     * some high flag bit is set and an options record was supplied. */
    if ((flags & 0xffff0000) == 0 || options == 0) return;

    if ((flags & 0x10000) != 0) {                   /* 128 << 9 */
        Func_02003a96(effect, *(s32 *)(options + 4));
    }

    if ((flags & 0x20000) != 0) {                   /* 128 << 10 */
        effect[35] = (u8)(effect[35] & 0xfe);
        block[9] = (u8)((block[9] & 0xf3) | ((options[0] & 3) << 2));
    }

    if ((flags & 0x80000) != 0) {                   /* 128 << 12 */
        *(s32 *)(effect + 24) = *(s32 *)(options + 8);
        *(s32 *)(effect + 28) = *(s32 *)(options + 12);
    }

    if ((flags & 0x40000) != 0) {                   /* 128 << 11 */
        const s32 *descriptor = (const s32 *)Data_0200b134[flags & 15];
        s32 delta;

        /* The 0x80000 test is the same register the previous block left live:
         * with a destination supplied the step is measured from it, otherwise
         * the target is biased by -1.0 in 16.16. */
        if ((flags & 0x80000) != 0) {
            *(s32 *)(effect + 48) =
                Func_020039a0(*(s32 *)(options + 16) - *(s32 *)(effect + 24),
                              descriptor[3]);
            delta = *(s32 *)(options + 20) - *(s32 *)(effect + 28);
        } else {
            *(s32 *)(effect + 48) =
                Func_020039b8(*(s32 *)(options + 16) + (s32)0xffff0000,
                              descriptor[3]);
            delta = *(s32 *)(options + 20) + (s32)0xffff0000;
        }

        /* Only the FIRST call is per-arm.  The `b.n 0x02000c4c` at the end of
         * the first arm joins both arms onto the single second call site, so
         * the second delta is computed in each arm and the call is spelled
         * once. */
        *(s32 *)(effect + 52) = Func_020039c6(delta, descriptor[3]);
    }

    if ((flags & 0x200000) != 0) {                  /* 128 << 14 */
        Func_020039fc(effect, 1);
        Func_02003a0c(effect, *(s32 *)(options + 28));
    }

    if ((flags & 0x400000) != 0) {                  /* 128 << 15 */
        *(u16 *)(block + 30) = *(u16 *)(options + 32);
    }

    if ((flags & 0x800000) != 0) {                  /* 128 << 16 */
        *tag = *(u16 *)(options + 34);
    }

    if ((flags & 0x1000000) != 0) {                 /* 128 << 17 */
        *(u32 *)(effect + 108) = *(u32 *)(options + 36);
    }
}
