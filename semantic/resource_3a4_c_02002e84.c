#include "types.h"

/*
 * resource_3a4 owner at 0x02002e84, 104 bytes: STRUCTURALLY IDENTICAL to
 * sibling item-28 owner resource_3a4_c_02002e1c.c -- the same bracketed
 * scene-setup shape (place two entity slots, set a story flag, run the
 * same unresolved primitives and workspace-448 double-stamp), just with
 * different literals: slot 10 instead of 9, a different placement target,
 * story flag 0x325 instead of 0x323, and two different in-overlay callees.
 *
 * Complete owner: `push {r5, r6, lr}` at 0x02002e84 through the matching
 * interworking return `pop {r5, r6} / pop {r0} / bx r0` at 0x02002eda-
 * 0x02002ede, followed by the three-word literal pool 0x02002ee0-0x02002eeb
 * that closes the 104-byte span. No arguments, void.
 *
 * Not found by the structural inventory walk (unindexed); one incoming
 * call site per overlay_unindexed.ts. This owner makes eleven outgoing
 * calls over nine distinct targets, all resolved with the `+2` rule
 * against the raw image (this owner starts outside a recorded row, so
 * overlay_call_targets.ts's own table does not cover it -- resolved with
 * `bun tools-rs/overlay-show resource_3a4 2e84 | bun
 * tools/lib/overlay_call_targets.ts resource_3a4 2e84 2ede --annotate`):
 *   0x2002e86 -> veneer -> Func_0808a018()          established no-arg scene bracket
 *   0x2002e90 -> veneer -> Func_0808a0f0(10, 0, 0)  established, place an entity
 *   0x2002e9e -> veneer -> Func_0808a0f0(8, 0x1e80000, 0x8a0000)   established
 *   0x2002ea4 -> veneer -> Func_080770c8(0x325)     established, set a story flag
 *   0x2002ea8 -> raw prologue -> Func_02000a94()      in-overlay, undocumented
 *   0x2002eac -> veneer -> Func_08009128()          unresolved no-arg primitive
 *   0x2002eb2 -> veneer -> Func_080000c0(1)         established, advance the task scheduler
 *   0x2002ec2 -> veneer -> Func_0808a360()          established no-arg scene helper
 *   0x2002ec6 -> veneer -> Func_0808a370()          established no-arg scene helper
 *   0x2002eca -> raw prologue -> Func_020008d4()      in-overlay, sibling item-28 owner, same overlay, undrafted at time of writing
 *   0x2002ed6 -> veneer -> Func_0808a020()          established no-arg scene bracket, pairs with Func_0808a018
 *
 * 0x03001ebc is the overlay workspace pointer; +448 is written 0x201 then
 * 0x204, same as resource_3a4_c_02002e1c.c.
 */

void Func_0808a018();          /* no-arg scene bracket, established */
void Func_0808a0f0();          /* place an entity, established */
void Func_080770c8();          /* set a story flag, established */
void Func_02000a94();          /* in-overlay, undocumented */
void Func_08009128();          /* unresolved no-arg primitive */
void Func_080000c0();          /* advance the task scheduler, established */
void Func_0808a360();
void Func_0808a370();
void Func_020008d4();          /* sibling item-28 owner, same overlay */
void Func_0808a020();          /* no-arg scene bracket, established */

void Func_02002e84(void)
{
    u8 *workspace;

    Func_0808a018();

    Func_0808a0f0(10, 0, 0);
    Func_0808a0f0(8, 0x1e80000, 0x8a0000);

    Func_080770c8(0x325);

    Func_02000a94();
    Func_08009128();
    Func_080000c0(1);

    workspace = *(u8 **)0x03001ebc;
    *(u32 *)(workspace + 448) = 0x201;

    Func_0808a360();
    Func_0808a370();
    Func_020008d4();

    workspace = *(u8 **)0x03001ebc;
    *(u32 *)(workspace + 448) = 0x204;

    Func_0808a020();
}
