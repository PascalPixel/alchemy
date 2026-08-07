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
 * Call targets resolved with `tools/overlay_call_targets.ts`; all are import
 * veneers.  Per-target multiset over the 15 sites: Func_080091b8 x5,
 * Func_080000c0 x4, Func_080091c0 x3, and one each of Func_0808a080,
 * Func_0808a100, Func_080091e0 - 15 C call expressions below, matching the
 * row's calls=15.
 *   Func_0808a080 <- veneer 0x02001b3c;  Func_0808a100 <- 0x02001b74;
 *   Func_080091e0 <- 0x02001af4;         Func_080091b8 <- 0x02001ae4;
 *   Func_080000c0 <- 0x02001ad4;         Func_080091c0 <- 0x02001aec.
 */

/* Old-style declarations: overlay import arities vary per call site. */
u8 *Func_0808a080();    /* scene entity record by selector */
void Func_0808a100();
void Func_080091e0();
void Func_080000c0();   /* frame wait */
void Func_080091b8();   /* six-argument tile/strip request, last two on the stack */
void Func_080091c0();   /* six-argument collision repaint, last two on the stack */

void Func_020000d4(void)
{
    u8 *entity;
    int rowA = 21;      /* r6, live for the whole body */

    entity = Func_0808a080(10);
    Func_0808a100(10, 5);

    if (entity != 0) {
        Func_080091e0(entity, 0);
        entity[0x23] = 1;
    }

    Func_080091b8(41, 87, 2, 5, rowA, 59);
    Func_080000c0(4);

    Func_080091b8(3, 93, 1, 1, 24, 62);
    Func_080091b8(1, 94, 1, 1, rowA, 55);
    Func_080091b8(43, 87, 2, 5, rowA, 58);
    Func_080000c0(4);

    Func_080091b8(41, 87, 2, 5, rowA, 58);
    Func_080000c0(4);
    Func_080000c0(4);

    Func_080091c0(21, 11, 2, 2, rowA, 13);
    Func_080091c0(21, 11, 1, 1, 22, 15);
    Func_080091c0(19, 17, 1, 1, rowA, 14);
}
