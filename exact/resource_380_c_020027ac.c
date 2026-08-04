#include "types.h"

/*
 * resource_380 owner at 0x020027ac, 32 bytes: the talk handler for
 * actor 14, one of four byte-identical 32-byte stubs in this
 * overlay's per-actor published-callback bank.
 *
 * PUBLISHED, NOT CALLED. Its Thumb pointer is written into a script
 * record; no `bl` in the image reaches it, so neither the call-graph
 * sweep nor `tools/overlay_unindexed.ts` lists it. Found by
 * `bun tools/overlay_published.ts resource_380`.
 *
 * DIFFED, NOT ASSUMED. The four stubs at 0x0200274c, 0x0200276c,
 * 0x0200278c and 0x020027ac differ in exactly two places: the actor id
 * in the `movs r0` and the single pool word carrying the line id.
 *
 *     0x0200274c   actor 10   line 0x10ca
 *     0x0200276c   actor 11   line 0x10c7
 *     0x0200278c   actor 13   line 0x10c8
 *     0x020027ac   actor 14   line 0x10cc
 *
 * A fifth 32-byte member sits at 0x020027cc and is NOT part of this
 * set -- it keeps the same bracket and line call but replaces the
 * Func_02007222 tail with a call to the overlay's own Func_02004248.
 * It was read separately rather than folded in.
 *
 * The bank as a whole, and the way it corroborates the six-actor scene
 * wiring at 0x0200227c, is written up in resource_380_c_02002674.c.
 *
 * Shape: the scripted-scene bracket (Func_0200714c / Func_02007166)
 * around one line of dialogue and one Func_02007222 call. No branch,
 * no state.
 *
 * Complete owner: `push {lr}` at 0x020027ac through `pop {r0} / bx r0`
 * at 0x020027c4-0x020027c6, then the one-word literal pool at
 * 0x020027c8 and a zero alignment halfword at 0x020027ca; the
 * next owner's prologue begins at 0x020027cc.
 *
 * All three `bl` targets resolved through the overlay's import-veneer
 * table under the +2 rule (tools/overlay_call_targets.ts).
 *
 * Uncertainty: Func_02007222's second argument is zero at every site in
 * this bank and its role is not established.
 */

extern void Func_0200714c(void);
extern void Func_02007166(void);
extern void Func_0200720a(s32 dialogueId);
extern void Func_02007222(s32 id, s32 arg1);

void Func_020027ac(void)
{
    Func_0200714c();
    Func_0200720a(0x10cc);
    Func_02007222(14, 0);
    Func_02007166();
}
