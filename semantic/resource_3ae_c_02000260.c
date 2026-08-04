#include "types.h"

/*
 * Resource 3ae, owner at 0x02000260 (124 bytes including its four-word pool at
 * 0x020002cc..0x020002da).
 *
 * Role known in advance from the call graph: the in-image handler table holds
 * the odd word 0x02008261 == Func_02000260 + 1 as the payload of the entry
 * keyed 0xffff000b, so this is a table-dispatched interaction handler invoked
 * with no arguments.
 *
 * Prologue `push {lr}` at 0x02000260; epilogue `pop {r0} / bx r0` at
 * 0x020002c6.  The return address is popped into r0, so r0 does not survive
 * and the owner is void.
 *
 * Call targets resolved with `tools/overlay_call_targets.ts` (an overlay `bl`
 * stores target offset - 2; the disassembler's annotation is wrong):
 *
 *   0x02000262 -> Func_0808a018   (scene open)
 *   0x02000268 -> Func_080770c0   (event-flag test, id 0x8a6)
 *   0x02000272 -> Func_0808a170   (cue 0x1cfd)
 *   0x0200027a -> Func_0808a178
 *   0x02000282 -> Func_0808a070   (prompt; its result selects the branch)
 *   0x0200028e -> Func_0808a180
 *   0x02000294 -> Func_080770c8   (event-flag set, id 0x8a6)
 *   0x020002ae -> Func_0808a180
 *   0x020002b6 -> Func_0808a170   (cue 0x1cfe)
 *   0x020002be -> Func_0808a180
 *   0x020002c2 -> Func_0808a020   (scene close)
 *
 * Eleven call sites against the inventory's calls=11, so the call set is
 * complete.
 *
 * `Data_03001ebc` is the RAM save/progress block already established by the
 * byte-exact sibling `assets/code/resource_3ae_c_02000ad4.c`, which declares it
 * as `extern u8 *Data_03001ebc` and indexes through the same single
 * indirection.  Here the unsigned halfword at byte offset 472
 * (`movs r3,#236 / lsls r3,r3,#1`) is incremented once — a counter bumped only
 * on the "declined" arm of the prompt.
 *
 * The three Func_0808a180(11, 0) calls are DISTINCT sites on three distinct
 * paths and are not merged, even though they are textually identical.
 *
 * Uncertainties: 0x8a6 is read as an event-flag id and 0x1cfd / 0x1cfe as cue
 * ids, from their use as the sole argument of Func_080770c0/c8 and
 * Func_0808a170 respectively.  Func_0808a070's two zero arguments are read as
 * a default prompt configuration; its non-zero return is treated as "the
 * player declined", from the fact that the zero branch is the one that sets
 * the completion flag.
 */

/* Old-style declarations: overlay imports vary in arity between call sites. */
void Func_0808a018();
void Func_0808a020();
s32 Func_080770c0();
void Func_080770c8();
s32 Func_0808a070();
void Func_0808a170();
void Func_0808a178();
void Func_0808a180();

extern u8 *Data_03001ebc;

void Func_02000260(void)
{
    Func_0808a018();

    if (Func_080770c0(0x8a6) == 0) {
        Func_0808a170(0x1cfd);
        Func_0808a178(11, 0);

        if (Func_0808a070(0, 0) == 0) {
            Func_0808a180(11, 0);
            Func_080770c8(0x8a6);
        } else {
            *(u16 *)(Data_03001ebc + 472) += 1;
            Func_0808a180(11, 0);
        }
    } else {
        Func_0808a170(0x1cfe);
        Func_0808a180(11, 0);
    }

    Func_0808a020();
}
