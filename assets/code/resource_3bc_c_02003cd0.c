#include "types.h"

/*
 * resource_3bc owner at 0x02003cd0, 34 bytes: spin the task scheduler until
 * the balance snapshot reads 9.
 *
 * Complete owner: `push {r5, lr}` at 0x02003cd0 through the matching
 * interworking return `pop {r5} / pop {r0} / bx r0` at 0x02003cea-
 * 0x02003cee, followed by the one-word literal pool 0x02003cf0-0x02003cf3
 * that closes the 34-byte span. No arguments, void.
 *
 * Not found by the structural inventory walk (unindexed); one incoming call
 * site per overlay_unindexed.ts. This owner makes calls in a loop to a
 * single target, resolved with the `+2` rule against the raw image (this
 * owner starts outside a recorded row, so overlay_call_targets.ts's own
 * table does not cover it -- resolved with `bun tools/overlay_show.ts
 * resource_3bc 3cd0 | bun tools/overlay_call_targets.ts resource_3bc 3cd0
 * 3cee --annotate`):
 *   0x2003cde -> veneer -> Func_080000c0(1)   established, advance the task scheduler (resource_3cd_c_020000ec.c)
 *
 * Data_02001000 is the established "balance snapshot, written by
 * Func_020009f8" (semantic/overlays/resource_3b7_c_020009f8.c); it is a
 * full s32 there, read here through its low halfword only, the same
 * narrower-view idiom used elsewhere in this codebase (e.g.
 * Data_02000240's mixed byte/halfword/word views).
 *
 * The test-then-loop shape (skip the loop entirely if already 9, otherwise
 * call-then-retest) collapses to a plain `while`; both compile to the same
 * observable behaviour.
 */

extern s16 Data_02001000;

void Func_02008520();          /* advance the task scheduler, established (veneer to Func_080000c0) */

void Func_02003cd0(void)
{
    s16 *status = &Data_02001000;

    while (*status != 9) {
        Func_02008520(1);
    }
}
