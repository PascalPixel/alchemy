#include "types.h"

/*
 * resource_380 owner at 0x0200278c, 32 bytes: the talk handler for
 * actor 13, one of four byte-identical 32-byte stubs in this
 * overlay's per-actor published-callback bank.
 *
 * PUBLISHED, NOT CALLED. Its Thumb pointer is written into a script
 * record; no `bl` in the image reaches it, so neither the call-graph
 * sweep nor `tools-rs/overlay-unindexed` lists it. Found by
 * `bun tools/lib/overlay_published.ts resource_380`.
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
 * Func_02007202 tail with a call to the overlay's own Func_02004248.
 * It was read separately rather than folded in.
 *
 * The bank as a whole, and the way it corroborates the six-actor scene
 * wiring at 0x0200227c, is written up in resource_380_c_02002674.c.
 *
 * Shape: the scripted-scene bracket (Func_0200712c / Func_02007146)
 * around one line of dialogue and one Func_02007202 call. No branch,
 * no state.
 *
 * Complete owner: `push {lr}` at 0x0200278c through `pop {r0} / bx r0`
 * at 0x020027a4-0x020027a6, then the one-word literal pool at
 * 0x020027a8 and a zero alignment halfword at 0x020027aa; the
 * next owner's prologue begins at 0x020027ac.
 *
 * All three `bl` targets resolved through the overlay's import-veneer
 * table under the +2 rule (tools/lib/overlay_call_targets.ts).
 *
 * Uncertainty: Func_02007202's second argument is zero at every site in
 * this bank and its role is not established.
 */

extern void Func_0200712c(void);
extern void Func_02007146(void);
extern void Func_020071ea(s32 dialogueId);
extern void Func_02007202(s32 id, s32 arg1);

void Func_0200278c(void)
{
    Func_0200712c();
    Func_020071ea(0x10c8);
    Func_02007202(13, 0);
    Func_02007146();
}
