/*
 * resource_3a2 owner at 0x020008a8, 54 bytes.  Complete owner: `push {lr}` /
 * `sub sp,#8` prologue at 0x020008a8 and the interworking return
 * `add sp,#8 / pop {r0} / bx r0` at 0x020008d8.  r0 holds the popped return
 * address, so the owner returns nothing and takes no arguments.  No literal
 * pool sits inside the span.
 *
 * Sibling of Func_02000870: the two owners are the same 54-byte skeleton with
 * the same three callees in the same order, differing only in the immediates.
 * Both were confirmed with `bun tools/overlay_call_targets.ts resource_3a2
 * 08a8 --json`, which reports the identical three veneer targets:
 *   0x08bc -> veneer 0x14b8 -> Func_080091c0
 *   0x08ca -> veneer 0x15f8 -> Func_0808a3c0
 *   0x08d4 -> veneer 0x1550 -> Func_0808a0f0
 *
 * Func_080091c0 is the six-argument collision/tile repaint (four register
 * arguments plus two words at [sp,#0] and [sp,#4]); the `sub sp,#8` carries
 * them.  The two stack words 21 and 9 are identical to Func_02000870's.
 *
 * The second and third calls here use plain small immediates rather than 16.16
 * coordinates: `negs` produces -1 for both of Func_0808a3c0's trailing
 * arguments, and Func_0808a0f0's coordinate pair is 0.
 *
 * Uncertainty: Func_0808a3c0 has no other converted call site in the tree, so
 * only its three set argument registers are asserted.  Old-style declarations
 * keep every import's interface open.
 */

void Func_080091c0();
void Func_0808a3c0();
void Func_0808a0f0();

void Func_020008a8(void)
{
    Func_080091c0(21, 73, 1, 1, 21, 9);
    Func_0808a3c0(100, -1, -1);
    Func_0808a0f0(14, 0, 0);
}
