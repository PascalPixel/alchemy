#include "types.h"

extern unsigned char Value_00004ccc;

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
 * table does not cover it -- resolved with `bun tools/overlay-show
 * resource_3a4 2a10 | cargo run --release --manifest-path tools/overlay-call-targets/Cargo.toml -- resource_3a4
 * 2a10 2a42 --annotate`):
 *   0x2002a2c -> veneer -> Func_0200664e(rec, 3)   old-style shape (object, mode), established elsewhere (resource_39a_c_02001c18.c, resource_3c8_c_02000118.c)
 *   0x2002a34 -> veneer -> Func_02006566(rec, 0)   established, set presentation phase
 *
 * The handle field at +9 is masked `(handle[9] & ~0x0c) | 0x04` -- close
 * to but not identical to resource_3a8_c_02003a3c.c's flags09 update
 * (that one also masks the result with 0x0f afterward; this owner does
 * not), so treated as a distinct observation rather than the same field.
 */

void Func_0200664e();          /* old-style shape (object, mode), established */
void Func_02006566();          /* established (rec, phase) */

void SceneActor_SetMode3AndRate4ccc(u8 *rec)
{
    u8 *p = rec + 0x55;
    s32 v = 0;
    u8 *h;

    *p = v;
    h = *(u8 **)(rec + 80);
    v -= 13;
    v &= h[9];
    v |= 4;
    h[9] = (u8)v;
    Func_0200664e(rec, 3);
    Func_02006566(rec, 0);
    {
        s32 rate = (s32)&Value_00004ccc;

        *(s32 *)(rec + 24) = rate;
        *(s32 *)(rec + 28) = rate;
    }
}
