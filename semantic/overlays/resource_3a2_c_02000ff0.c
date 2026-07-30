/*
 * resource_3a2 owner at 0x02000ff0, 56 bytes.  Complete owner: `push {lr}` /
 * `sub sp,#8` prologue at 0x02000ff0 and the interworking return
 * `add sp,#8 / pop {r0} / bx r0` at 0x02001022.  r0 holds the popped return
 * address, so the owner returns nothing and takes no arguments (r0-r3 are all
 * written before any read).  No literal pool sits inside the span.
 *
 * Two six-argument map edits followed by a one-frame wait.
 *
 * Calls resolved with `bun tools/overlay_call_targets.ts resource_3a2 0ff0
 * --json` (three sites, three distinct veneer targets):
 *   0x1004 -> veneer 0x14b0 -> Func_08009180
 *   0x1018 -> veneer 0x14b8 -> Func_080091c0
 *   0x101e -> veneer 0x14e8 -> Func_0808a010
 *
 * Func_080091c0 is the six-argument collision/tile repaint established by
 * semantic/overlays/resource_3b4_c_02001c6c.c (four register arguments plus two
 * words at [sp,#0] and [sp,#4]).  Func_08009180 is called with the identical
 * frame shape, so it is spelled with six arguments too.  Func_0808a010 is the
 * frame-wait used throughout this overlay family.
 *
 * Note the stack slots are rewritten between the two calls -- 1,2 for the first
 * and 5,13 for the second -- so the `sub sp,#8` frame serves both.
 *
 * Sibling of Func_02001028, which is the same 56-byte body with 5 replaced by
 * 6 in the two leading register arguments.
 *
 * Uncertainty: Func_08009180 has no other converted call site, so its six-word
 * argument list is inferred from the identical stack-argument frame rather than
 * from a second witness.  Old-style declarations keep the interfaces open.
 */

void Func_08009180();
void Func_080091c0();
void Func_0808a010();

void Func_02000ff0(void)
{
    Func_08009180(5, 28, 5, 13, 1, 2);
    Func_080091c0(5, 28, 1, 2, 5, 13);
    Func_0808a010(1);
}
