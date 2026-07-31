typedef unsigned char u8;
typedef signed int s32;
typedef unsigned int u32;

/*
 * resource_3bc owner at 0x02003b80, 76 bytes: a third sibling in this
 * family -- resource_3bc_c_02003b40.c's field-reset-then-teleport sequence
 * (different reset constants: +48 = 0x14000, +52 = 0x14000 >> 1 = 0xa000)
 * plus resource_3bc_c_020022f4.c's two trailing calls.
 *
 * Complete owner: `push {r5, r6, r7, lr}` at 0x02003b80 through the matching
 * interworking return `pop {r5, r6, r7} / pop {r0} / bx r0` at
 * 0x2003bc8-0x2003bcc. No literal pool. Three arguments (r0-r2), void.
 *
 * Not found by the structural inventory walk (unindexed); two incoming call
 * sites per overlay_unindexed.ts. This owner makes six outgoing calls, all
 * resolved with the `+2` rule against the raw image (this owner starts
 * outside a recorded row, so overlay_call_targets.ts's own table does not
 * cover it -- resolved with `bun tools/overlay_show.ts resource_3bc 3b80 |
 * bun tools/overlay_call_targets.ts resource_3bc 3b80 3bcc --annotate`):
 *   0x2003b86 -> veneer -> Func_0808a400(selector)   entity by selector, established
 *   0x2003ba2 -> veneer -> Func_08009140()           unestablished, same call as resource_3bc_c_020038dc.c and siblings
 *   0x2003baa -> veneer -> Func_08009080(entity, 5)  established (record, mode)
 *   0x2003bb6 -> veneer -> Func_08009150(entity, x, y, z)   established, "move a record to a position"; z argument is the entity's own +12 field unchanged
 *   0x2003bbc -> veneer -> Func_08009158(entity)     unestablished, same call as resource_3bc_c_020022f4.c
 *   0x2003bc4 -> veneer -> Func_08009080(entity, 1)  mode changed to 1 after the move settles
 *
 * The two word resets (+48, +52) and the byte reset (+0x5b = 0) are not
 * otherwise cross-referenced from this owner alone.
 */

s32 *Func_0808a400();          /* entity by selector, established */
void Func_08009140();          /* unestablished */
void Func_08009080();          /* established (record, mode) */
void Func_08009150();          /* established (record, x, y, z) */
void Func_08009158();          /* unestablished, single argument */

void Func_02003b80(s32 selector, s32 x, s32 z)
{
    s32 *record;

    record = Func_0808a400(selector);
    if (record != 0) {
        *(u32 *)((u8 *)record + 48) = 0x14000;
        *(u32 *)((u8 *)record + 52) = 0xa000;
        *((u8 *)record + 0x5b) = 0;

        Func_08009140();
        Func_08009080(record, 5);
        Func_08009150(record, x << 16, record[3], z << 16);
        Func_08009158(record);
        Func_08009080(record, 1);
    }
}
