#include "types.h"

/*
 * resource_380 owner at 0x020027ec, 12 bytes: a published callback that
 * forwards to a single imported function and nothing else.
 *
 * PUBLISHED, NOT CALLED. Its Thumb pointer is written into a script
 * record; no `bl` in the image reaches it. Found by
 * `bun tools/lib/overlay_published.ts resource_380`. It is the last member
 * of the contiguous published run 0x02002400-0x020027f8 and the only
 * one that is not a talk handler -- it carries no scene bracket and no
 * dialogue.
 *
 * Func_020072b4 takes no arguments here: nothing is loaded into r0
 * between the prologue and the `bl`. The tree's one existing use,
 * semantic/overlays/resource_3b4_c_020018e0.c, also calls it with none,
 * so the arity agrees across two independent readings.
 *
 * Complete owner: `push {lr}` at 0x020027ec, the veneer call at
 * 0x020027ee, `pop {r0} / bx r0` at 0x020027f2-0x020027f4, then a zero
 * alignment halfword at 0x020027f6; the next owner's prologue is at
 * 0x020027f8 (drafted, resource_380_c_020027f8.c). No literal pool.
 *
 * The `bl` target resolved through the overlay's import-veneer table
 * under the +2 rule (tools/lib/overlay_call_targets.ts): the veneer at
 * image 0x4ac4 loads 0x0808a2a1 and branches, so the callee is
 * Func_020072b4.
 *
 * Uncertainty: what Func_020072b4 does, and why the script installs a
 * whole callback to reach it rather than pointing at it directly, is
 * unestablished. The likeliest reason is that a script record's pointer
 * must be an overlay-image address, but this owner is no evidence for
 * that.
 */

extern void Func_020072b4(void);

void Func_020027ec(void)
{
    Func_020072b4();
}
