/*
 * Resource 3ad, map-repaint sequence at 0x02000384 (218 bytes, 11 call sites).
 *
 * Complete owner.  Prologue at 0x02000384 saves r5/r6/lr, then sl/r9 and r8
 * through r5/r6 (`mov r6,sl / mov r5,r9 / push {r5,r6} / mov r6,r8 /
 * push {r6}`) and opens an 8-byte outgoing-argument area with `sub sp, #8`.
 * The matching unwind at 0x0200044e restores all of them and returns through
 * `pop {r0} / bx r0`; the popped register is r0, so it holds the return address
 * and the owner is `void`.  There is no literal pool inside the span - every
 * constant is a Thumb immediate.
 *
 * The high registers are pure constant holders, kept live because the six
 * argument slots do not fit: r5 = 21, sl = 24, r9 = 62, r8 = 55, r6 = 58.  They
 * are written once and read at several later call sites, so each is spelled as
 * a named local below rather than repeated as a literal.
 *
 * Call targets resolved with `tools/lib/overlay_call_targets.ts`; all are import
 * veneers:
 *   0x020003a2, 0x020003c4, 0x020003d8, 0x020003ea, 0x02000404, 0x02000416,
 *   0x02000426  -> veneer 0x02001ae4 -> Func_080091b8   (x7)
 *   0x020003a8, 0x020003f0                             -> veneer 0x02001ad4 -> Func_080000c0 (x2)
 *   0x02000438, 0x0200044a                             -> veneer 0x02001aec -> Func_080091c0 (x2)
 * Per-target multiset: Func_080091b8 x7, Func_080000c0 x2, Func_080091c0 x2 -
 * 11 sites against 11 C call expressions, matching the row's calls=11.
 *
 * Both imports take six arguments, four in r0-r3 and two pushed at [sp,#0] and
 * [sp,#4]; Func_080091c0 is the established collision-repaint entry and
 * Func_080091b8 its sibling.  The two stack words are written in either order
 * across the sequence, which is why they are listed explicitly per call.
 */

/* Old-style declarations: overlay import arities vary per call site. */
void Func_080000c0();   /* frame wait */
void Func_080091b8();   /* six-argument tile/strip request, last two on the stack */
void Func_080091c0();   /* six-argument collision repaint, last two on the stack */

void Func_02000384(void)
{
    /* The five constants held in callee-saved registers for the whole body. */
    int rowA = 21;      /* r5 */
    int rowB = 24;      /* sl */
    int colB = 62;      /* r9 */
    int colC = 55;      /* r8 */
    int colD = 58;      /* r6 */

    Func_080091b8(41, 87, 2, 5, rowA, 59);
    Func_080000c0(4);

    Func_080091b8(2, 93, 1, 1, rowB, colB);
    Func_080091b8(2, 94, 1, 1, rowA, colC);
    Func_080091b8(43, 87, 2, 5, rowA, colD);
    Func_080000c0(4);

    Func_080091b8(3, 93, 1, 1, rowB, colB);
    Func_080091b8(1, 94, 1, 1, rowA, colC);
    Func_080091b8(41, 87, 2, 5, rowA, colD);

    Func_080091c0(21, 11, 2, 2, rowA, 13);
    Func_080091c0(19, 17, 1, 1, rowA, 14);
}
