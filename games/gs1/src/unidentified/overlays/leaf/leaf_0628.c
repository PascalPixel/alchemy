#include "types.h"

/*
 * resource_380 owner at 0x0200274c, 32 bytes: the talk handler for
 * actor 10, one of four byte-identical 32-byte stubs in this
 * overlay's per-actor published-callback bank.
 *
 * PUBLISHED, NOT CALLED. Its Thumb pointer is written into a script
 * record; no `bl` in the image reaches it, so neither the call-graph
 * sweep nor `tools/overlay-unindexed` lists it. Found by
 * `cargo run --release --manifest-path tools/overlay-published/Cargo.toml -- resource_380`.
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
 * Func_020071c2 tail with a call to the overlay's own Func_02004248.
 * It was read separately rather than folded in.
 *
 * The bank as a whole, and the way it corroborates the six-actor scene
 * wiring at 0x0200227c, is written up in resource_380_c_02002674.c.
 *
 * Shape: the scripted-scene bracket (Func_020070ec / Func_02007106)
 * around one line of dialogue and one Func_020071c2 call. No branch,
 * no state.
 *
 * Complete owner: `push {lr}` at 0x0200274c through `pop {r0} / bx r0`
 * at 0x02002764-0x02002766, then the one-word literal pool at
 * 0x02002768 and a zero alignment halfword at 0x0200276a; the
 * next owner's prologue begins at 0x0200276c.
 *
 * All three `bl` targets resolved through the overlay's import-veneer
 * table under the +2 rule (cargo run --release --manifest-path tools/overlay-call-targets/Cargo.toml --).
 *
 * Uncertainty: Func_020071c2's second argument is zero at every site in
 * this bank and its role is not established.
 */

extern void Func_020070ec(void);
extern void Func_02007106(void);
extern void Func_020071aa(s32 dialogueId);
extern void Func_020071c2(s32 id, s32 arg1);

void Func_0200274c(void)
{
    Func_020070ec();
    Func_020071aa(0x10ca);
    Func_020071c2(10, 0);
    Func_02007106();
}
