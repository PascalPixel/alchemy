#include "types.h"

/*
 * resource_380 owner at 0x02004248, 24 bytes including the trailing
 * alignment halfword: the overlay's 97x-called workhorse turns out to
 * be a two-call convenience wrapper.  Complete owner: `push {r5, lr}`
 * at 0x02004248 through `pop {r0} / bx r0` at 0x0200425c; next owner's
 * prologue at 0x02004260.
 *
 * CORRECTION (name sweep): the claim that "this overlay executes at
 * 0x02000000 because its bl targets land in a 0x02008xxx resident
 * module" is STRUCK.  There is no such module: the targets were a
 * naive pc-relative misdecode.  Under the +2 rule
 * (tools/overlay_call_targets.ts) every bl here resolves through the
 * overlay's own import-veneer table to main-ROM code, and the pool
 * pointers fit the standard 0x02008000 link base.  This wrapper's two
 * calls are Func_02008bea and Func_02008cb4.
 *
 * Uncertainty: callee roles beyond call shape remain open.
 */

extern void Func_02008bea(s32 arg0);
extern void Func_02008cb4(s32 arg0, s32 arg1);

void Func_02004248(s32 arg0, s32 arg1)
{
    Func_02008cb4(arg0, 0);
    Func_02008bea(arg1);
}
