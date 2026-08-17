#include "types.h"

/*
 * resource_3a4 owner at 0x020029dc, 46 bytes: fetch scene record 9, set
 * its presentation phase, and if a saved story value reads exactly 2,
 * place entity slot 9 at a fixed position.
 *
 * Complete owner: `push {lr}` at 0x020029dc through the matching
 * interworking return `pop {r0} / bx r0` at 0x02002a08-0x02002a0a,
 * followed by the two-word literal pool 0x02002a0c-0x02002a0f that closes
 * the 46-byte span. No arguments, void.
 *
 * Not found by the structural inventory walk (unindexed); one incoming
 * call site per overlay_unindexed.ts. This owner makes three outgoing
 * calls, all resolved with the `+2` rule against the raw image (this
 * owner starts outside a recorded row, so overlay_call_targets.ts's own
 * table does not cover it -- resolved with `bun tools/overlay-show
 * resource_3a4 29dc | cargo run --release --manifest-path tools/overlay-call-targets/Cargo.toml -- resource_3a4
 * 29dc 2a0a --annotate`):
 *   0x20029e0 -> veneer -> Func_0200657a(9)        scene-record accessor, established
 *   0x20029e6 -> veneer -> Func_02006518(record, 0)   established, set presentation phase
 *   0x2002a04 -> veneer -> Func_020065ee(9, x, z)  established, place an entity at (x, z)
 *
 * Data_02000240+450 (byte offset, index 225 << 1) is the established
 * word-blob idiom, read here as a signed halfword.
 */

extern u8 Data_02000240[];

u8 *Func_0200657a();           /* scene-record accessor, established */
void Func_02006518();          /* established (record, phase) */
void Func_020065ee();          /* established (slot, x, z) */

void Func_020029dc(void)
{
    u8 *record;

    record = Func_0200657a(9);
    Func_02006518(record, 0);

    {
        s32 off = 450;

        if (*(s16 *)&Data_02000240[off] == 2) {
        Func_020065ee(9, 0xb80000, 0x1480000);
    }
    }
}
