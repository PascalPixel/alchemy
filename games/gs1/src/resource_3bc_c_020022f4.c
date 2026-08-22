#include "types.h"

/*
 * resource_3bc owner at 0x020022f4, 60 bytes: a sibling of
 * resource_3bc_c_020022c4.c -- the same "if the entity exists, run the
 * unestablished no-arg helper, set presentation mode 5, teleport
 * horizontally keeping height" sequence, plus two extra trailing calls.
 *
 * Complete owner: `push {r5, r6, r7, lr}` at 0x020022f4 through the matching
 * interworking return `pop {r5, r6, r7} / pop {r0} / bx r0` at
 * 0x200232a-0x200232e. No literal pool. Three arguments (r0-r2), void.
 *
 * Not found by the structural inventory walk (unindexed); two incoming call
 * sites per overlay_unindexed.ts. This owner makes five outgoing calls, all
 * resolved with the `+2` rule against the raw image (this owner starts
 * outside a recorded row, so overlay_call_targets.ts's own table does not
 * cover it -- resolved with `bun tools/overlay-show resource_3bc 22f4 |
 * cargo run --release --manifest-path tools/overlay-call-targets/Cargo.toml -- resource_3bc 22f4 232e --annotate`):
 *   0x20022fa -> veneer -> Func_0808a400(selector)   entity by selector, established
 *   0x2002304 -> veneer -> Func_08009140()           unestablished, same call as resource_3bc_c_020038dc.c and siblings
 *   0x200230c -> veneer -> Func_08009080(entity, 5)  established (record, mode)
 *   0x2002318 -> veneer -> Func_08009150(entity, x, y, z)   established, "move a record to a position"; z argument is the entity's own +12 field unchanged, same horizontal-only-move idiom as 020022c4.c
 *   0x200231e -> veneer -> Func_08009158(entity)     unestablished single-argument call, immediately followed by Func_08009080(entity, 1)
 *   0x2002326 -> veneer -> Func_08009080(entity, 1)  mode changed to 1 after the move settles
 */

s32 *Func_02006e64();          /* entity by selector, established */
void Func_02006c06();          /* unestablished */
void Func_02006bee();          /* established (record, mode) */
void Func_02006c2a();          /* established (record, x, y, z) */
void Func_02006c38();          /* unestablished, single argument */
void Func_02006c08();          /* established (record, mode) */

void Func_020022f4(s32 selector, s32 x, s32 z)
{
    s32 *record;

    record = Func_02006e64(selector);
    if (record != 0) {
        Func_02006c06();
        Func_02006bee(record, 5);
        Func_02006c2a(record, x << 16, record[3], z << 16);
        Func_02006c38(record);
        Func_02006c08(record, 1);
    }
}
