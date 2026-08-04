typedef signed int s32;

/*
 * Resource 3b3 scene at 0x0200253c (66 bytes, 7 calls).
 *
 * Derived span, not an inventory row: this owner has no row in
 * out/decomp/overlays.json and no exact sibling.  It was found by sweeping the
 * two-byte gaps in metrics/gs1-en-executable.json for `push {..,lr}`
 * prologues.  Walking from the prologue at 0x0200253c, the two branches
 * (`beq` at 0x0200254e to 0x02002566 and `b` at 0x02002564 to 0x02002572) stay
 * inside the body, and the walk stops at the interworking return
 * `add sp,#32 / pop {r5} / pop {r0} / bx r0` at 0x02002576-0x0200257c.  So the
 * executable extent is exactly 0x0200253c-0x0200257d (66 bytes).  The popped
 * register is r0, so the popped value is the return address and the owner is
 * `void`.
 *
 * Pool map: none — this owner loads no literal; its 32-byte frame is built
 * with `sub sp,#32`.  The halfword at 0x0200257e is `0x0000` alignment before
 * the next prologue at 0x02002580.
 *
 * All seven call sites were resolved with
 * `bun tools/overlay_call_targets.ts resource_3b3 253c 257e`, assigning the
 * i-th occurrence in the source to the i-th site in address order:
 *   0x02002540 -> veneer 0x02002b0c -> Func_0808a018
 *   0x02002548 -> prologue 0x02000758
 *   0x02002560 -> 0x020008ec
 *   0x02002566 -> prologue 0x02002498
 *   0x0200256a -> prologue 0x020003a8
 *   0x0200256e -> prologue 0x020024cc
 *   0x02002572 -> veneer 0x02002b14 -> Func_0808a020
 *
 * `overlay_call_targets.ts` reports 0x020008ec as `unknown` rather than
 * `prologue`, and that is a classifier limitation rather than a bad target:
 * 0x020008ec opens with `sub sp,#16` BEFORE its `push {r5, r6, r7, lr}`, so a
 * classifier keyed on a leading `push` does not recognise it.  Disassembling
 * there shows an ordinary frame, so it is a real in-overlay function.  Because
 * the tool declines to name it, `overlay_multiset_check.ts` also drops the
 * site, and this file's Func_020008ec call is therefore checked by hand rather
 * than by the tool: the histogram is one site per target for all seven.
 *
 * First `Func_0808a018` and last `Func_0808a020` is the scene bracket, and it
 * is symmetric here — both arms of the `beq` converge on 0x02002572, so there
 * is exactly one close and it must NOT be copied into the two arms.
 *
 * The 32-byte frame is two records: the probe at sp+8 that 0x02000758 fills,
 * and a two-word pair copied from sp+24 down to sp+0 before the call at
 * 0x02002560 — so 0x020008ec takes four register arguments plus two stack
 * words, and the copy is what supplies them.
 */

void Func_0200504e();          /* scene open */
s32 Func_02002ca2();           /* fills the probe record, returns a flag */
void Func_02002e4e();          /* four register arguments plus two stack words */
void Func_02004a00();
void Func_02002914();
void Func_02004a3c();
void Func_02005088();          /* scene close */

void Func_0200253c(void)
{
    s32 frame[8];
    s32 *probe = &frame[2];

    Func_0200504e();

    if (Func_02002ca2(probe) != 0) {
        Func_02002e4e(probe[0], probe[1], probe[2], probe[3], probe[4], probe[5]);
    } else {
        Func_02004a00();
        Func_02002914();
        Func_02004a3c();
    }

    Func_02005088();
}
