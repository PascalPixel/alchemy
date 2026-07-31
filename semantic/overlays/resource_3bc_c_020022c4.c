typedef unsigned char u8;
typedef signed int s32;

/*
 * resource_3bc owner at 0x020022c4, 44 bytes: if the caller's entity
 * exists, run the same unestablished no-arg helper as
 * resource_3bc_c_020038dc.c, set its presentation mode to 5, then teleport
 * it horizontally to the caller's (x, z) while keeping its current height.
 *
 * Complete owner: `push {r5, r6, r7, lr}` at 0x020022c4 through the matching
 * interworking return `pop {r5, r6, r7} / pop {r0} / bx r0` at
 * 0x20022ec-0x20022f0. No literal pool. Three arguments (r0-r2), void.
 *
 * Not found by the structural inventory walk (unindexed); two incoming call
 * sites per overlay_unindexed.ts. This owner makes four outgoing calls,
 * all resolved with the `+2` rule against the raw image (this owner starts
 * outside a recorded row, so overlay_call_targets.ts's own table does not
 * cover it -- resolved with `bun tools/overlay_show.ts resource_3bc 22c4 |
 * bun tools/overlay_call_targets.ts resource_3bc 22c4 22f0 --annotate`):
 *   0x20022ca -> veneer -> Func_0808a400(selector)   entity by selector, established (resource_383_c_020047bc.c)
 *   0x20022d4 -> veneer -> Func_08009140()           unestablished, same call as resource_3bc_c_020038dc.c
 *   0x20022dc -> veneer -> Func_08009080(record, 5)  established (record, mode)
 *   0x20022e8 -> veneer -> Func_08009150(record, x, y, z)   established, "move a record to a position" (resource_393/39a/3bb_c_02003b90.c)
 *
 * The z argument passed to Func_08009150 is this owner's own +12 (y) field,
 * unchanged -- the horizontal-only-move idiom is not otherwise established
 * from this owner alone.
 */

s32 *Func_0808a400();          /* entity by selector, established */
void Func_08009140();          /* unestablished */
void Func_08009080();          /* established (record, mode) */
void Func_08009150();          /* established (record, x, y, z) */

void Func_020022c4(s32 selector, s32 x, s32 z)
{
    s32 *record;

    record = Func_0808a400(selector);
    if (record != 0) {
        Func_08009140();
        Func_08009080(record, 5);
        Func_08009150(record, x << 16, record[3], z << 16);
    }
}
