#include "types.h"

/*
 * resource_3bc owner at 0x02003b40, 64 bytes: a sibling of
 * resource_3bc_c_020022c4.c -- same "if the entity exists, run the
 * unestablished no-arg helper, set presentation mode 5, teleport
 * horizontally keeping height" sequence, plus three extra field resets
 * (+48, +52, +0x5b) run first.
 *
 * Complete owner: `push {r5, r6, r7, lr}` at 0x02003b40 through the matching
 * interworking return `pop {r5, r6, r7} / pop {r0} / bx r0` at
 * 0x2003b7a-0x2003b7e. No literal pool. Three arguments (r0-r2), void.
 *
 * Not found by the structural inventory walk (unindexed); one incoming call
 * site per overlay_unindexed.ts. This owner makes four outgoing calls, all
 * resolved with the `+2` rule against the raw image (this owner starts
 * outside a recorded row, so overlay_call_targets.ts's own table does not
 * cover it -- resolved with `bun tools-rs/overlay-show resource_3bc 3b40 |
 * bun tools/lib/overlay_call_targets.ts resource_3bc 3b40 3b7e --annotate`):
 *   0x2003b46 -> veneer -> Func_0808a400(selector)   entity by selector, established (resource_383_c_020047bc.c)
 *   0x2003b62 -> veneer -> Func_08009140()           unestablished, same call as resource_3bc_c_020038dc.c / 020022c4.c
 *   0x2003b6a -> veneer -> Func_08009080(entity, 5)  established (record, mode)
 *   0x2003b76 -> veneer -> Func_08009150(entity, x, y, z)   established, "move a record to a position" (resource_393/39a/3bb_c_02003b90.c); z argument is the entity's own +12 field unchanged, same horizontal-only-move idiom as 020022c4.c
 *
 * The two word resets (+48 = 0x20000, +52 = 0x20000 >> 1 = 0x10000) and the
 * byte reset (+0x5b = 0) are not otherwise cross-referenced from this owner
 * alone.
 */

s32 *Func_020086b0();          /* entity by selector, established */
void Func_02008464();          /* unestablished */
void Func_0200844c();          /* established (record, mode) */
void Func_02008488();          /* established (record, x, y, z) */

void Func_02003b40(s32 selector, s32 x, s32 z)
{
    s32 *record;
    s32 field48;

    record = Func_020086b0(selector);
    if (record != 0) {
        field48 = 0x20000;
        *(u32 *)((u8 *)record + 48) = field48;
        *(u32 *)((u8 *)record + 52) = field48 >> 1;
        *((u8 *)record + 0x5b) = 0;

        Func_02008464();
        Func_0200844c(record, 5);
        Func_02008488(record, x << 16, record[3], z << 16);
    }
}
