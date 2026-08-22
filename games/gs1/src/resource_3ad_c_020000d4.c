#include "types.h"

/*
 * Resource 3ad, map-repaint sequence at 0x020000d4 (220 bytes, 15 call sites).
 *
 * Complete owner: `push {r5, r6, lr} / sub sp, #8` at 0x020000d4 and the
 * matching `add sp, #8 / pop {r5, r6} / pop {r0} / bx r0` at 0x020001a8.  The
 * popped branch register is r0, so it holds the return address and the owner is
 * `void`.  No literal pool lies inside the span.
 *
 * Close sibling of 0x02000384, which runs the same repaint sequence with the
 * same constants but a different lead-in and a different mix of frame waits.
 *
 * REGISTER TRAP worth recording: r5 is used for two unrelated things.  From
 * 0x020000e0 it holds the entity record returned by Func_0808a080(10); from
 * 0x0200013c it is reloaded with the constant 58 and is thereafter only a stack
 * argument.  The reassignment sits far from either set of uses, so r5 must be
 * tracked per-use rather than as one variable.  r6 is a plain constant holder
 * (21) for the whole body.
 *
 * Call targets resolved with `cargo run --release --manifest-path tools/overlay-call-targets/Cargo.toml --`; all are import
 * veneers.  Per-target multiset over the 15 sites: Func_080091b8 x5,
 * Func_080000c0 x4, Func_080091c0 x3, and one each of Func_0808a080,
 * Func_0808a100, Func_080091e0 - 15 C call expressions below, matching the
 * row's calls=15.
 *   Func_0808a080 <- veneer 0x02001b3c;  Func_0808a100 <- 0x02001b74;
 *   Func_080091e0 <- 0x02001af4;         Func_080091b8 <- 0x02001ae4;
 *   Func_080000c0 <- 0x02001ad4;         Func_080091c0 <- 0x02001aec.
 */

/* Old-style declarations: overlay import arities vary per call site. */
    /* scene entity record by selector */


   /* frame wait */
   /* six-argument tile/strip request, last two on the stack */
   /* six-argument collision repaint, last two on the stack */

extern u8 * Func_02001c18();
extern void Func_02001c5a();
extern void Func_02001be6();
extern void Func_02001bf2();
extern void Func_02001be8();
extern void Func_02001c0c();
extern void Func_02001c1e();
extern void Func_02001c30();
extern void Func_02001c26();
extern void Func_02001c46();
extern void Func_02001c3c();
extern void Func_02001c42();
extern void Func_02001c6c();
extern void Func_02001c80();
extern void Func_02001c92();
void Func_020000d4(void)
{
    u8 *entity;
    int rowA;           /* r6; assigned at its first use, as the reference does */

    entity = Func_02001c18(10);
    Func_02001c5a(10, 5);

    if (entity != 0) {
        Func_02001be6(entity, 0);
        entity[0x23] = 1;
    }

    rowA = 21;
    Func_02001bf2(41, 87, 2, 5, rowA, 59);
    Func_02001be8(4);

    { s32 f1 = 24; s32 g1 = 62; Func_02001c0c(3, 93, 1, 1,  f1, g1); }
    Func_02001c1e(1, 94, 1, 1, rowA, 55);
    Func_02001c30(43, 87, 2, 5, rowA, 58);
    Func_02001c26(4);

    Func_02001c46(41, 87, 2, 5, rowA, 58);
    Func_02001c3c(4);
    Func_02001c42(4);

    Func_02001c6c(21, 11, 2, 2, rowA, 13);
    { s32 f2 = 22; s32 g2 = 15; Func_02001c80(21, 11, 1, 1,  f2, g2); }
    Func_02001c92(19, 17, 1, 1, rowA, 14);
}
