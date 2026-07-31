/*
 * resource_3a7 one-shot scene trigger at 0x02000368, 48 bytes.
 *
 * Complete owner: `push {lr}` at 0x02000368 through `pop {r0} / bx r0` at
 * 0x02000390.  The return address is popped into r0, so the owner is `void`.
 * The word at 0x02000394 (0x000009a9) is this owner's literal pool - the flag
 * id - and is branched past by the epilogue; it is data, not code.
 *
 * This is the same gate shape the byte-exact sibling
 * assets/code/resource_3a7_c_020003e0.c already spells:
 *
 *     if (Func_020014e6(0x9A9) == 0 && Func_02000722() != 0) {
 *         Func_02001500(0x9A9); Func_0200154e(0x50); Func_0200079a();
 *     }
 *
 * That file was written with the PRINTED (pre-correction) `bl` names.  Resolved
 * with `tools/overlay_call_targets.ts` the same two imports are Func_080770c0
 * (test a flag) and Func_080770c8 (raise it), and the delay is Func_080f9010.
 * The exact sibling therefore fixes the direction of the pair for free, which
 * is why the flag test is written `== 0` and the setter is on the taken side.
 *
 * Note the flag ids: this owner gates on 0x9a9, the exact sibling on 0x9A9 too,
 * and the neighbouring 0x0200048c / 0x020004d0 pair on 0x9aa.  Consecutive
 * one-shot ids for consecutive scene steps.
 *
 * Call targets resolved with
 *   bun tools/overlay_show.ts resource_3a7 0368 -n 48 |
 *   bun tools/overlay_call_targets.ts resource_3a7 --annotate
 * Per-target multiset over the 6 sites, matching the inventory's calls=6:
 *   Func_080770c0 x1 <- veneer 0x02001100
 *   Func_02000170 x1  (this overlay's own prologue)
 *   Func_02000334 x1  (this overlay's own prologue; result tested)
 *   Func_080770c8 x1 <- veneer 0x02001108
 *   Func_080f9010 x1 <- veneer 0x02001150
 *   Func_02000398 x1  (this overlay's own prologue - the scene body converted
 *                      alongside this file)
 *
 * Uncertainty: Func_02000170 is called for effect only, before the second test,
 * and its result is discarded; whether it is a precondition for Func_02000334's
 * predicate or an unrelated per-frame step is not established here.
 */

/* Old-style declarations: arities vary between call sites in this overlay.
 * The two used in conditions must still declare a return type. */
s32_unused_guard:;
