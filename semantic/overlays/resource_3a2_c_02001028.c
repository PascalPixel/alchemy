/*
 * resource_3a2 owner at 0x02001028, 56 bytes.  Complete owner: `push {lr}` /
 * `sub sp,#8` prologue at 0x02001028 and the interworking return
 * `add sp,#8 / pop {r0} / bx r0` at 0x0200105a.  r0 holds the popped return
 * address, so the owner returns nothing and takes no arguments.  No literal
 * pool sits inside the span.
 *
 * Byte-for-byte the sibling of Func_02000ff0: the two 56-byte owners differ
 * only in the two `movs r0` immediates (5 there, 6 here).  `bun
 * tools/overlay_call_targets.ts resource_3a2 1028 --json` reports the same
 * three veneer targets in the same order:
 *   0x103c -> veneer 0x14b0 -> Func_08009180
 *   0x1050 -> veneer 0x14b8 -> Func_080091c0
 *   0x1056 -> veneer 0x14e8 -> Func_0808a010
 *
 * Func_080091c0 is the six-argument collision/tile repaint (four register
 * arguments plus two words at [sp,#0] and [sp,#4]); Func_08009180 is reached
 * with the identical frame shape; Func_0808a010 is the frame-wait.
 *
 * Uncertainty: as in the sibling, Func_08009180's six-argument spelling is
 * inferred from its stack-argument frame.  Old-style declarations keep the
 * interfaces open.
 */

void Func_08009180();
void Func_080091c0();
void Func_0808a010();

void Func_02001028(void)
{
    Func_08009180(6, 28, 5, 13, 1, 2);
    Func_080091c0(6, 28, 1, 2, 5, 13);
    Func_0808a010(1);
}
