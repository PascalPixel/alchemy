typedef unsigned char u8;
typedef signed int s32;
typedef unsigned int u32;

/*
 * resource_3a4 owner at 0x02002e1c, 104 bytes: bracket a scene setup --
 * place entity slots 9 and 8, set a story flag, run three unresolved
 * no-arg primitives and the task scheduler, stamp the overlay workspace's
 * +448 field once with 0x201 and again with 0x204 around two more
 * primitives, then close the bracket.
 *
 * Complete owner: `push {r5, r6, lr}` at 0x02002e1c through the matching
 * interworking return `pop {r5, r6} / pop {r0} / bx r0` at 0x2002e72-
 * 0x02002e76, followed by the three-word literal pool 0x02002e78-0x02002e83
 * that closes the 104-byte span. No arguments, void.
 *
 * Not found by the structural inventory walk (unindexed); one incoming
 * call site per overlay_unindexed.ts. This owner makes eleven outgoing
 * calls over nine distinct targets, all resolved with the `+2` rule
 * against the raw image (this owner starts outside a recorded row, so
 * overlay_call_targets.ts's own table does not cover it -- resolved with
 * `bun tools/overlay_show.ts resource_3a4 2e1c | bun
 * tools/overlay_call_targets.ts resource_3a4 2e1c 2e76 --annotate`):
 *   0x2002e1e -> veneer -> Func_0808a018()          established no-arg scene bracket
 *   0x2002e28, 0x2002e36 -> veneer -> Func_0808a0f0(slot, x, z)   established, place an entity
 *   0x2002e3c -> veneer -> Func_080770c8(0x323)     established, set a story flag
 *   0x2002e40 -> raw prologue -> Func_20009ec()      in-overlay, no args, not itself in this overlay's item-28 list
 *   0x2002e44 -> veneer -> Func_08009128()          unresolved no-arg primitive (resource_37f_c_02000a24.c, resource_38d_c_020021bc.c)
 *   0x2002e4a -> veneer -> Func_080000c0(1)         established, advance the task scheduler
 *   0x2002e5a -> veneer -> Func_0808a360()          established no-arg scene helper
 *   0x2002e5e -> veneer -> Func_0808a370()          established no-arg scene helper
 *   0x2002e62 -> raw prologue -> Func_020007e8()     in-overlay, sibling item-28 owner, same overlay, undrafted at time of writing
 *   0x2002e6e -> veneer -> Func_0808a020()          established no-arg scene bracket, pairs with Func_0808a018
 *
 * 0x03001ebc is the overlay workspace pointer, same global documented in
 * resource_3bc_c_0200288c.c / resource_37b_c_02000150.c; this owner writes
 * its +448 field twice with different literal values, not read back
 * in between.
 */

void Func_0808a018();          /* no-arg scene bracket, established */
void Func_0808a0f0();          /* place an entity, established */
void Func_080770c8();          /* set a story flag, established */
void Func_020009ec();          /* in-overlay, undocumented */
void Func_08009128();          /* unresolved no-arg primitive */
void Func_080000c0();          /* advance the task scheduler, established */
void Func_0808a360();
void Func_0808a370();
void Func_020007e8();          /* sibling item-28 owner, same overlay */
void Func_0808a020();          /* no-arg scene bracket, established */

void Func_02002e1c(void)
{
    u8 *workspace;

    Func_0808a018();

    Func_0808a0f0(9, 0, 0);
    Func_0808a0f0(8, 0x1480000, 0x1a80000);

    Func_080770c8(0x323);

    Func_020009ec();
    Func_08009128();
    Func_080000c0(1);

    workspace = *(u8 **)0x03001ebc;
    *(u32 *)(workspace + 448) = 0x201;

    Func_0808a360();
    Func_0808a370();
    Func_020007e8();

    workspace = *(u8 **)0x03001ebc;
    *(u32 *)(workspace + 448) = 0x204;

    Func_0808a020();
}
