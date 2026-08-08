#include "types.h"

/*
 * resource_3bc owner at 0x02002e18, 60 bytes: resolve an item against a
 * party/item record (side effect only, its result is discarded), then scan
 * the record's 15-entry u16 table at +216 for a slot equal to that item and
 * fire the table-entry handler for the first match.
 *
 * Complete owner: `push {r5, r6, r7, lr} / mov r7,r8 / push {r7}` at
 * 0x02002e18 through the matching interworking return `pop {r3} / mov
 * r8,r3 / pop {r5, r6, r7} / pop {r0} / bx r0` at 0x2002e4a-0x2002e52. No
 * literal pool. Two arguments (r0, r1), void.
 *
 * Not found by the structural inventory walk (unindexed); two incoming call
 * sites per overlay_unindexed.ts. This owner makes three outgoing calls,
 * all resolved with the `+2` rule against the raw image (this owner starts
 * outside a recorded row, so overlay_call_targets.ts's own table does not
 * cover it -- resolved with `bun tools/overlay-show resource_3bc 2e18 |
 * cargo run --release --manifest-path tools/overlay-call-targets/Cargo.toml -- resource_3bc 2e18 2e52 --annotate`):
 *   0x2002e22 -> veneer -> Func_08077008(handle)         item/party record by id, established (resource_3cd_c_020000ec.c, semantic/main/080b5368.c)
 *   0x2002e2c -> veneer -> Func_08077028(handle, item)   established, `s32 Func_08077028(s32 handle, s32 item);` (semantic/main/080b5368.c); its return is loaded then never used, so it is called for its side effect only
 *   0x2002e40 -> veneer -> Func_08077050(handle, slot)   established, `void Func_08077050(s32 handle, s32 resolved);` (same file); here the second argument is this owner's own loop index, not Func_08077028's discarded return -- the two established callees are not chained the way that main-image sibling chains them
 */

u8 *Func_020077bc();           /* veneer, item/party record by id, established */
s32 Func_020077ce();           /* veneer, established (handle, item) */
void Func_020077ea();          /* veneer, established (handle, slot) */

void Func_02002e18(s32 handle, s32 item)
{
    u8 *record;
    s32 slot;

    record = Func_020077bc(handle);
    Func_020077ce(handle, item);

    for (slot = 0; slot <= 14; slot++) {
        if (*(u16 *)(record + 216 + slot * 2) == item) {
            Func_020077ea(handle, slot);
        }
    }
}
