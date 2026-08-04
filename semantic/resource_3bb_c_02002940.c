#include "types.h"

/*
 * resource_3bb owner at 0x02002940, 144 bytes: seed a set of fixed IWRAM
 * cells with the caller's mode/id pair, pick one of several handler
 * addresses by that same mode, and install a task through this overlay's
 * own task-install wrapper.
 *
 * Complete owner: `push {r5, r6, lr}` at 0x02002940 through `pop {r5, r6}
 * / pop {r0} / bx r0` at 0x020029c8-0x020029cc, followed by one
 * alignment halfword; the next owner's prologue is at 0x020029d0. Two
 * arguments (r0, r1), void.
 *
 * Not found by the structural inventory walk (unindexed): reached only by
 * `bl`, resolved with `bun tools/overlay_call_targets.ts`'s `+2` rule.
 *
 * `Func_080000d0(0x0200a451, 0xc80)` matches the `Func_080000d0(taskAddress,
 * frameBudget)` task-install shape this project already documents
 * (resource_3bc_c_02003abc.c's header), just reached through this
 * overlay's own local wrapper at 0x02006700 instead of the shared
 * veneer.
 *
 * The handler selection is three independent, non-exclusive `if`s in the
 * compiled order (mode==2, then mode==4, then mode==3 with a further
 * sub-case on the second argument), kept sequential here rather than
 * rewritten as an if/else chain, since the owner itself never assumes
 * they are mutually exclusive.
 *
 * Uncertainty: none of the seven `Data_0200ccXX` cells or the five
 * handler addresses are identified; they are recorded as opaque
 * addresses per this project's address-taken-literal convention.
 */

extern void Func_080000d0(s32 taskAddress, s32 frameBudget);

void Func_02002940(s32 mode, s32 arg1)
{
    u16 *modeCell = (u16 *)0x0200cc30;
    u16 *scaledCell = (u16 *)0x0200cc04;
    s32 handler = 0x0200ca1e;

    *modeCell = (u16)mode;
    *scaledCell = (u16)(arg1 << 4);

    Func_080000d0(0x0200a451, 0xc80);

    if (mode == 2) {
        handler = 0x0200c0ce;
    }
    if (mode == 4) {
        handler = 0x0200ca4a;
    }
    if (mode == 3) {
        if (arg1 == 0) {
            handler = 0x0200cac8;
        } else {
            handler = 0x0200c0f6;
        }
    }

    *(u16 *)0x0200cc3c = 0;
    *(u32 *)0x0200cc40 = (u32)handler;
    *(u16 *)0x0200cc98 = 0;
    *(u16 *)0x0200cc0c = 0;
    *(u32 *)0x0200cc10 = 0;
}
