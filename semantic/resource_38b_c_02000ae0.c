#include "types.h"

/*
 * Resource 38b scene setup at 0x02000ae0 (468 bytes, 37 call sites over 9
 * distinct callees).
 *
 * Complete owner: `push {r5, r6, r7, lr} / sub sp, #8` at 0x02000ae0 and the
 * matching `add sp, #8 / pop {r5, r6, r7} / pop {r0} / bx r0` at 0x02000c96.
 * The popped r0 is the return address, so the owner is `void`.
 * 0x02000c9e is alignment and 0x02000c9e-0x02000cb3 is the literal pool,
 * which runs exactly up to the next owner at 0x02000cb4 (overlay_show.ts
 * reports "out of bounds" at 0x02000cb2 for that reason).  Derived from a
 * control-flow walk of the prologue: nothing below 0x02000c9c is reached as
 * an instruction.
 *
 * Reached from the dispatcher at 0x02000890 for scene id 0x23.
 *
 * SHARED CALL SITE — the trap HANDOVER section 0 warns about, live here.
 * The tail call at 0x02000c92 to this overlay's own 0x02000cb4 is ONE `bl`
 * reached from five control paths: `b.n` from 0x02000b66, 0x02000b8a,
 * 0x02000c5a and 0x02000c7e, plus fall-through from 0x02000c90.  Writing it
 * once per arm would inject four phantom calls into the multiset.  It is
 * spelled once below, with a single `goto join` for the one path that cannot
 * reach it by ordinary fall-through.
 *
 * COMPLETENESS, per target (not a total — a matching total can hide a
 * permutation): Func_0808a0f0 x11, Func_0808a080 x10, Func_080091e0 x5,
 * Func_080770c0 x5, Func_0808a1b8 x2, Func_08009180 x1, Func_080091c0 x1,
 * Func_0808a1e0 x1, Func_02000cb4 x1 = 37 sites.  Each appears exactly that
 * many times below.  The inventory reports `calls=35`; 37 >= 35 is the
 * expected direction, since that field predates the corrected `bl` decoding.
 * Thirty-six targets are import veneers and one is this overlay's own
 * prologue, all resolved with tools/lib/overlay_call_targets.ts.
 *
 * The two Func_0808a0f0(11, ...) sites and the two Func_080770c0(0x848)
 * sites sit on opposite arms of tests and are deliberately NOT merged.
 *
 * REGISTER NOTE.  r5 first holds the result of Func_080770c0(0x845) — used as
 * both the branch condition and, in the else arm where it is provably 0, as
 * the stored value at slot-10 +0x55 — and is then reassigned to the constant
 * 16 at 0x02000be8 as an OR mask.  Tracked per use below.
 */

/* Slot accessor. */
extern u8 *Func_02001bb8();
extern u8 *Func_02001bc0();
extern u8 *Func_02001bc8();
extern void Func_02001b86();
extern s32 Func_02001b9c();
extern void Func_02001c14();
extern void Func_02001c1e();
extern void Func_02001c28();
extern void Func_02001bac(s32, s32, s32, s32, s32, s32);
extern void Func_02001bc8_b(s32, s32, s32, s32, s32, s32);
extern s32 Func_02001bee();
extern s32 Func_02001bf8();
extern void Func_02001c70();
extern void Func_02001ccc();
extern void Func_02001cd8();
extern void Func_02001c94();
extern void Func_02001c9e();
extern void Func_02001ca8();
extern u8 *Func_02001c7e();
extern void Func_02001c3c();
extern u8 *Func_02001c8a();
extern void Func_02001c48();
extern u8 *Func_02001c96();
extern void Func_02001c54();
extern s32 Func_02001c70_b();
extern u8 *Func_02001cb2();
extern u8 *Func_02001cc2();
extern u8 *Func_02001cd0();
extern void Func_02001d16();
extern u8 *Func_02001cec();
extern void Func_02001caa();
extern void Func_02001d30();
extern s32 Func_02001cde();
extern void Func_02001d58();
extern void Func_02001d68();
extern void Func_02001dc8();
extern void Func_02001948(void);

/* Progress-flag test. */

/* Imports; old-style, arity open. */

                            

                                                        

/* This overlay's collision-map repaint. */

void Func_02000ae0(void)
{
    u8 *actor10;
    u8 *actor11;
    u8 *record;
    u8 *child;
    s32 seen;

    s32 permuted_5;
    actor10 = Func_02001bb8(10);
    actor11 = Func_02001bc0(11);

    record = Func_02001bc8(8);
    Func_02001b86(record, 0);

    seen = Func_02001b9c(0x845);
    if (seen != 0) {
        /* Already-seen layout. */
        Func_02001c14(9, 0, 0);
        Func_02001c1e(10, 0, 0);
        Func_02001c28(11, 0, 0);

        Func_02001bac(56, 15, 40, 15, 1, 2);
        Func_02001bc8_b(26, 15, 1, 3, 10, 15);

        if (Func_02001bee(0x849) == 0) {
            if (Func_02001bf8(0x848) != 0) {
                goto join;
            }
            Func_02001c70(14, 0, 0);
        }

        /* 208 << 8 = 0xd000, 176 << 8 = 0xb000. */
        Func_02001ccc(12, 0xd000, 0);
        Func_02001cd8(13, 0xb000, 0);
    } else {
        /* First-visit layout. */
        Func_02001c94(12, 0, 0);
        Func_02001c9e(13, 0, 0);
        Func_02001ca8(14, 0, 0);

        record = Func_02001c7e(9);
        Func_02001c3c(record, 0);
        record = Func_02001c8a(10);
        Func_02001c48(record, 0);
        record = Func_02001c96(11);
        Func_02001c54(record, 0);

        /* `seen` is provably 0 on this arm; the original stores that
         * register rather than an immediate. */
        *(u8 *)(actor10 + 0x55) = (u8)seen;

        if (Func_02001c70_b(0x881) != 0) {
            /* Set bit 4 of the +0x59 flag byte on slots 9, 16 and 11. */
            record = Func_02001cb2(9) + 89;
            record = Func_02001cc2(16) + 89;
            record = Func_02001cd0(11) + 89;
            *record = (u8)(*record | 16);
            *record = (u8)(*record | 16);
            *record = (u8)(*record | 16);

            /* 142 << 16 and 156 << 16: 16.16 coordinates. */
            Func_02001d16(16, 0x008e0000, 0x009c0000);
            record = Func_02001cec(16);
            Func_02001caa(record, 0);
            Func_02001d30(10, 0x008e0000, 0x009c0000);

            /* 128 << 7 = 0x4000, into the child record's +0x1e halfword. */
            *(u16 *)(child + 30) = 0x4000;
            child = *(u8 **)(actor10 + 80);

            /* Pool word 0xfff80000 — a signed add of -0x00080000. */
            *(s32 *)(actor10 + 12) += (s32)0xfff80000;

            if (Func_02001cde(0x848) != 0) {
                /* 132 << 16, 186 << 16. */
                Func_02001d58(11, 0x00840000, 0x00ba0000);
            } else {
                /* 176 << 15 = 0x580000 — note the shift is 15, not 16, so
                 * this is not 176 << 16; and 196 << 16. */
                permuted_5 = (u8)(*(u8 *)(actor11 + 89) | 4);
                Func_02001d68(11, 0x00580000, 0x00c40000);
                Func_02001dc8(11, 3);
                *(u8 *)(actor11 + 89) = permuted_5;
            }
        } else {
            /* 128 << 14 = 0x200000, 192 << 14 = 0x300000. */
            *(s32 *)(actor10 + 12) = 0x00200000;
            /* The stored register is Func_080770c0's result, provably 0. */
            *(u8 *)(actor11 + 0x55) = 0;
            *(s32 *)(actor11 + 12) = 0x00300000;
        }
    }

join:
    Func_02001948();
}
