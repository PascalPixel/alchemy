/*
 * resource_3a2 owner at 0x02000870, 54 bytes.  Complete owner: `push {lr}` /
 * `sub sp,#8` prologue at 0x02000870 and the interworking return
 * `add sp,#8 / pop {r0} / bx r0` at 0x020008a0.  r0 holds the popped return
 * address, so the owner returns nothing and takes no arguments (r0-r3 are all
 * written before any read).  No literal pool sits inside the span.
 *
 * A short cutscene beat: repaint one collision cell, then emit two scene cues.
 *
 * Calls resolved with `bun tools/overlay_call_targets.ts resource_3a2 0870
 * --json` (three sites, three distinct veneer targets; the disassembler's own
 * `bl` annotations are wrong on every overlay):
 *   0x0884 -> veneer 0x14b8 -> Func_080091c0
 *   0x088e -> veneer 0x15f8 -> Func_0808a3c0
 *   0x089c -> veneer 0x1550 -> Func_0808a0f0
 *
 * Func_080091c0 is the six-argument collision/tile repaint already established
 * by semantic/overlays/resource_3b4_c_02001c6c.c: four register arguments plus
 * two words pushed at [sp,#0] and [sp,#4].  The `sub sp,#8` exists solely to
 * carry them.
 *
 * Func_0808a0f0 is the scene-cue emitter used across this overlay family with a
 * selector and a 16.16 coordinate pair; here 172<<17 = 344.0 and 152<<16 =
 * 152.0.
 *
 * Uncertainty: Func_0808a3c0 has no other converted call site in the tree, so
 * only its three set argument registers are asserted.  Old-style declarations
 * keep every import's interface open, as required for overlay sources.
 */

void Func_080091c0();
void Func_0808a3c0();
void Func_0808a0f0();

void Func_02000870(void)
{
    Func_080091c0(85, 9, 1, 1, 21, 9);
    Func_0808a3c0(100, 0, 0);
    Func_0808a0f0(14, 344 << 16, 152 << 16);
}
