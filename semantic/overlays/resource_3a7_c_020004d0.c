/*
 * resource_3a7 one-shot scene trigger at 0x020004d0, 60 bytes.
 *
 * TRANSPOSED from semantic/overlays/resource_3a7_c_0200048c.c, its twin four
 * bytes shorter in the same overlay.  0x0200048c is exactly this owner with one
 * extra leading `bl Func_02000170`; every other halfword, both pool words
 * (0x9aa at 0x02000504 and 0x207 at 0x02000508) and every branch displacement
 * are identical.  That equality is the correctness proof for this file.
 *
 * Complete owner: `push {lr}` at 0x020004d0 through `pop {r0} / bx r0` at
 * 0x020004fe; the return address is popped into r0, so the owner is `void`.
 * The two pool words are past the epilogue, inside the span, and are data.
 *
 * Call targets resolved with `bun tools/overlay_call_targets.ts resource_3a7
 * 04d0`.  Per-target multiset over the 6 sites, matching the inventory's
 * calls=6:
 *   Func_080770c0 x2 <- veneer 0x02001100  (flag 0x9aa, then flag 0x207)
 *   Func_02000458 x1  (this overlay's own prologue; result tested)
 *   Func_080f9010 x1 <- veneer 0x02001150
 *   Func_02000410 x1  (this overlay's own prologue)
 *   Func_080770c8 x1 <- veneer 0x02001108
 *
 * Direction of the flag pair is fixed by the byte-exact sibling
 * assets/code/resource_3a7_c_020003e0.c; see the twin's header.
 *
 * Uncertainty: this owner and 0x0200048c raise the SAME one-shot flag 0x9aa and
 * run the SAME body 0x02000410, so only one of them can ever take its taken
 * path in a given playthrough.  Which is the live entry point, and why the
 * overlay carries both, is not established from this row.
 */

/* Old-style declarations: arities vary between call sites in this overlay.
 * The imports used in conditions must still declare a return type. */
typedef signed int s32;

s32 Func_080770c0();    /* test a scene completion flag */
s32 Func_02000458();
void Func_080f9010();   /* scripted delay, in frames */
void Func_02000410();
void Func_080770c8();   /* raise a scene completion flag */

void Func_020004d0(void)
{
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
