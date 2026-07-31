typedef unsigned char u8;
typedef unsigned int u32;

/*
 * resource_3a4 owner at 0x02002a10, 50 bytes: clear a record byte, flip
 * two bits of its handle's flags byte at +9 -- the SceneHandle
 * flags-family field resource_3bc_c_02003bd0.c and resource_3a8_c_
 * 02003a3c.c already establish nearby offsets of -- run two presentation
 * primitives, then stamp two fixed words into the record.
 *
 * Complete owner: `push {r5, lr}` at 0x02002a10 through the matching
 * interworking return `pop {r5} / pop {r0} / bx r0` at 0x02002a3e-
 * 0x02002a42, followed by the one-word literal pool 0x02002a44-0x02002a47
 * that closes the 50-byte span. One argument (r0), void.
 *
 * Not found by the structural inventory walk (unindexed); one incoming
 * call site per overlay_unindexed.ts. This owner makes two outgoing
 * calls, both resolved with the `+2` rule against the raw image (this
 * owner starts outside a recorded row, so overlay_call_targets.ts's own
 * table does not cover it -- resolved with `bun tools/overlay_show.ts
 * resource_3a4 2a10 | bun tools/overlay_call_targets.ts resource_3a4
 * 2a10 2a42 --annotate`):
 *   0x2002a2c -> veneer -> Func_0808a160(record, 3)   old-style shape (object, mode), established elsewhere (resource_39a_c_02001c18.c, resource_3c8_c_02000118.c)
 *   0x2002a34 -> veneer -> Func_080091e0(record, 0)   established, set presentation phase
 *
 * The handle field at +9 is masked `(handle[9] & ~0x0c) | 0x04` -- close
 * to but not identical to resource_3a8_c_02003a3c.c's flags09 update
 * (that one also masks the result with 0x0f afterward; this owner does
 * not), so treated as a distinct observation rather than the same field.
 */

void Func_0808a160();          /* old-style shape (object, mode), established */
void Func_080091e0();          /* established (record, phase) */

void Func_02002a10(u8 *record)
{
    u8 *handle;

    record[0x55] = 0;

    handle = *(u8 **)(record + 80);
    handle[9] = (u8)((handle[9] & ~0x0c) | 0x04);

    Func_0808a160(record, 3);
    Func_080091e0(record, 0);

    *(u32 *)(record + 24) = 0x4ccc;
    *(u32 *)(record + 28) = 0x4ccc;
}
