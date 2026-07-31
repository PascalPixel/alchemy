/*
 * resource_3a7 one-shot scene trigger at 0x0200048c, 64 bytes.
 *
 * Complete owner: `push {lr}` at 0x0200048c through `pop {r0} / bx r0` at
 * 0x020004be.  The return address is popped into r0, so the owner is `void`.
 * Two literal-pool words sit inside the span at 0x020004c4 (0x9aa) and
 * 0x020004c8 (0x207); both are past the epilogue and are data, not code.  The
 * two-byte `bx lr` leaf at 0x020004cc is NOT part of this owner - 0x0200048c +
 * 64 = 0x020004cc, so the span ends exactly before it.
 *
 * Same family as 0x02000368 and its own twin 0x020004d0.  The pair 048c/04d0
 * is the cheapest kind of evidence in this overlay: 0x020004d0 is this owner
 * with the leading `Func_02000170()` call removed, and nothing else changed -
 * 60 bytes against 64, calls=6 against 7, identical pool words, identical
 * branch offsets after the first instruction.  Each proves the other.
 *
 * Gate shape, matching the byte-exact sibling
 * assets/code/resource_3a7_c_020003e0.c (which spells the same import pair with
 * the printed pre-correction names Func_020014e6 / Func_02001500):
 * test a one-shot flag, run the step only while it is clear, and raise it on
 * the way out.  Here the raise happens LAST, after the scene body, whereas
 * 0x02000368 raises it before - the two orders are both present in this
 * overlay and are transcribed as found.
 *
 * Call targets resolved with
 *   bun tools/overlay_show.ts resource_3a7 048c -n 64 |
 *   bun tools/overlay_call_targets.ts resource_3a7 --annotate
 * Per-target multiset over the 7 sites, matching the inventory's calls=7:
 *   Func_02000170 x1  (this overlay's own prologue)
 *   Func_080770c0 x2 <- veneer 0x02001100  (flag 0x9aa, then flag 0x207)
 *   Func_02000458 x1  (this overlay's own prologue; result tested)
 *   Func_080f9010 x1 <- veneer 0x02001150
 *   Func_02000410 x1  (this overlay's own prologue - the scene body converted
 *                      alongside this file)
 *   Func_080770c8 x1 <- veneer 0x02001108
 *
 * Note the second flag, 0x207, is one of the completion flags the
 * 0x0200050c..0x02000690 presentation family raises (0x200..0x205 there); it is
 * read here as a guard, so this step waits for that group rather than setting
 * its own.  0x9aa is this owner's own one-shot.
 *
 * Uncertainty: Func_02000170's result is discarded and it is called before the
 * flag test's dependent work, so whether it is a precondition of
 * Func_02000458's predicate is not established.
 */

/* Old-style declarations: arities vary between call sites in this overlay.
 * The imports used in conditions must still declare a return type. */
typedef signed int s32;

void Func_02000170();
s32 Func_080770c0();    /* test a scene completion flag */
s32 Func_02000458();
void Func_080f9010();   /* scripted delay, in frames */
void Func_02000410();
void Func_080770c8();   /* raise a scene completion flag */

void Func_0200048c(void)
{
    Func_02000170();

    if (Func_080770c0(0x9aa) != 0) {
        return;
    }
    if (Func_02000458() == 0) {
        return;
    }
    if (Func_080770c0(0x207) != 0) {
        return;
    }

    Func_080f9010(0x50);
    Func_02000410();
    Func_080770c8(0x9aa);
}
