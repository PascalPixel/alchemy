#include "types.h"

/*
 * Resource 3ad, map-repaint sequence at 0x02000210 (300 bytes, 19 call sites).
 *
 * Complete owner.  Prologue at 0x02000210 saves r5/r6/lr, then sl/r9 and r8
 * through r5/r6, and opens an 8-byte outgoing-argument area with `sub sp, #8`.
 * The matching unwind at 0x02000328 restores all of them and returns through
 * `pop {r0} / bx r0`; the popped register is r0, so it holds the return address
 * and the owner is `void`.  One pool word (0x202) sits at 0x02000338, past the
 * return.
 *
 * Third member of this overlay's repaint family with 0x020000d4 and
 * 0x02000384: the same two six-argument imports, the same constants held in
 * callee-saved registers because six arguments do not fit in r0-r3, and the
 * same 4-frame waits between strips.
 *
 * The optional head is gated on story flag 0x202, which is the flag that
 * 0x0200033c sets and 0x02000460 clears - so this owner repaints the extra pair
 * of strips only in the 0x201 state of that two-valued selector.
 *
 * Register constants: r5 = 21 throughout, r6 = 57 then 58 then 59 (reassigned
 * twice), sl = 24, r9 = 62, r8 = 55.  r6's reassignments sit between call sites
 * rather than at the top, so it is tracked per-use below.
 *
 * Call targets resolved with `tools/lib/overlay_call_targets.ts`; all are import
 * veneers.  Per-target multiset over the 19 sites: Func_080091b8 x10,
 * Func_080000c0 x6, and one each of Func_080770c0, Func_0808a1e0,
 * Func_080091c0 - 19 C call expressions below, matching the row's calls=19.
 *   Func_080770c0 <- veneer 0x02001b04;  Func_080091b8 <- 0x02001ae4;
 *   Func_080000c0 <- 0x02001ad4;         Func_0808a1e0 <- 0x02001bc4;
 *   Func_080091c0 <- 0x02001aec.
 */

/* Old-style declarations: overlay import arities vary per call site. */
s32 Func_080770c0();    /* test a story flag (used in a condition) */
void Func_080000c0();   /* frame wait */
void Func_080091b8();   /* six-argument tile/strip request, last two on the stack */
void Func_080091c0();   /* six-argument collision repaint, last two on the stack */
void Func_0808a1e0();

void Func_02000210(void)
{
    int rowA = 21;      /* r5 */
    int rowB = 24;      /* sl */
    int colB = 62;      /* r9 */
    int colC = 55;      /* r8 */

    if (Func_080770c0(0x202) != 0) {
        Func_080091b8(41, 86, 2, 6, rowA, 57);
        Func_080000c0(4);
        Func_080091b8(43, 86, 2, 6, rowA, 57);
        Func_080000c0(4);

        Func_080091b8(41, 86, 2, 6, rowA, 58);
        Func_080000c0(4);
        Func_080091b8(43, 86, 2, 6, rowA, 58);
        Func_080000c0(4);
    }

    /* 0x02000286: run unconditionally, whichever way the gate went. */
    Func_080091b8(2, 93, 1, 1, rowB, colB);
    Func_080091b8(2, 94, 1, 1, rowA, colC);
    Func_080091b8(41, 86, 2, 6, rowA, 59);
    Func_080000c0(4);

    Func_080091b8(1, 93, 1, 1, rowB, colB);
    Func_080091b8(3, 94, 1, 1, rowA, colC);
    Func_080091b8(43, 86, 2, 6, rowA, 59);
    Func_080000c0(4);

    Func_0808a1e0(10, 3);
    Func_080091c0(19, 17, 1, 1, 22, 15);
}
