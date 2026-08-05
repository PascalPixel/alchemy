#include "types.h"

/*
 * Resource 3ae, owner at 0x020006c8 (276 bytes including its five-word pool at
 * 0x020007c8..0x020007da).
 *
 * Role known in advance from the call graph: the in-image handler table holds
 * the odd word 0x020086c9 == Func_020006c8 + 1 as the payload of TWO entries —
 * one keyed 0xffff000b under condition 0, one keyed 0xffff040b under condition
 * 0x00008d15.  So it is a table-dispatched interaction handler on object slot
 * 11 (which is the slot every call below operates on) and takes no arguments.
 *
 * Prologue `push {lr}` at 0x020006c8; epilogue `pop {r0} / bx r0` at
 * 0x020007c2.  The return address is popped into r0, so r0 does not survive
 * and the owner is void.  No frame.
 *
 * Call targets resolved with `tools/overlay_call_targets.ts` (an overlay `bl`
 * stores target offset - 2; the disassembler's annotation is wrong).  29 call
 * sites against the inventory's calls=27; the inventory count is documented as
 * systematically low and every `bl` in the span is transcribed below in order,
 * so the set is complete.  Distinct callees: Func_0808a018, Func_080770c0,
 * Func_0808a148, Func_0808a010, Func_0808a170, Func_0808a180, Func_0808a020,
 * Func_0808a1e8, Func_0808a178, Func_0808a070, Func_080770c8, Func_0808a1b8.
 *
 * Control flow worth noting: the already-done arm calls Func_0808a020 itself
 * and then branches to the epilogue, so the scene close is NOT shared with the
 * other arms — it appears twice in the assembly and twice below.  That is a
 * genuine duplication of the close, not a merge candidate.
 *
 * `Data_03001ebc` is the RAM progress block established by the byte-exact
 * sibling `assets/code/resource_3ae_c_02000ad4.c`; the unsigned halfword at
 * byte offset 472 is the same counter Func_02000260, Func_02000328 and
 * Func_0200051c bump.
 *
 * The several Func_0808a010 waits and Func_0808a180(11, 0) / Func_0808a1b8(11,
 * 0, 0) calls are DISTINCT script sites on distinct paths and are not merged.
 *
 * Uncertainties: 0x8a6 and 0x8a8 are read as event-flag ids and 0x1f18 /
 * 0x1f1c as cue ids, from their use as the sole argument of Func_080770c0/c8
 * and Func_0808a170 respectively.  Func_0808a070(0, 0) is the same prompt
 * Func_02000260 uses; here its zero result is the branch that sets the
 * completion flag 0x8a8, matching that sibling's reading of zero as "accepted".
 */

/* Old-style declarations: overlay imports vary in arity between call sites. */
void Func_0808a018();
void Func_0808a020();
void Func_0808a010();
s32 Func_080770c0();
void Func_080770c8();
s32 Func_0808a070();
void Func_0808a148();
void Func_0808a170();
void Func_0808a178();
void Func_0808a180();
void Func_0808a1b8();
void Func_0808a1e8();

extern u8 *Data_03001ebc;

void Func_020006c8(void)
{
    Func_0808a018();

    if (Func_080770c0(0x8a8) != 0) {
        /* Already done: short scene, closes and returns. */
        Func_0808a148(11, 0, 0);
        Func_0808a010(20);
        Func_0808a170(0x1f1c);
        Func_0808a180(11, 0);
        return;
        Func_0808a020();
    }

    Func_0808a010(20);
    /* movs r1,#128 / lsls r1,r1,#1 == 256 */
    Func_0808a1e8(11, 256, 50);
    Func_0808a148(11, 0, 0);
    Func_0808a010(20);
    Func_0808a170(0x1f18);
    Func_0808a180(11, 0);

    if (Func_080770c0(0x8a6) != 0) {
        Func_0808a010(20);
        /* movs r1,#129 / lsls r1,r1,#1 == 258 */
        Func_0808a1e8(11, 258, 40);
        Func_0808a178(11, 0);

        if (Func_0808a070(0, 0) == 0) {
            Func_0808a010(20);
            Func_0808a180(11, 0);
            Func_080770c8(0x8a8);
        } else {
            Func_0808a010(10);
            Func_0808a180(11, 0);
            Func_0808a010(10);
            *(u16 *)(Data_03001ebc + 472) += 1;
            Func_0808a1b8(11, 0, 0);
            Func_0808a010(30);
        }
    } else {
        Func_0808a010(10);
        Func_0808a1b8(11, 0, 0);
        Func_0808a010(30);
    }

    Func_0808a020();
}
