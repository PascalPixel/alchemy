#include "types.h"

/*
 * resource_3a4 owner at 0x02003460, 34 bytes: clear bit 0 of the caller's
 * scene record's flags byte at +89 -- the same field resource_3bc_c_
 * 02004628.c and resource_3bb_c_02003b90.c test for the identical bit --
 * then drop a marker at the record's (x, z) position, level 255.
 *
 * Complete owner: `push {lr}` at 0x02003460 through the matching
 * interworking return `pop {r0} / bx r0` at 0x200347e-0x02003482. No
 * literal pool. One argument (r0), void.
 *
 * Not found by the structural inventory walk (unindexed); seven incoming
 * call sites per overlay_unindexed.ts. This owner makes two outgoing
 * calls, both resolved with the `+2` rule against the raw image (this
 * owner starts outside a recorded row, so overlay_call_targets.ts's own
 * table does not cover it -- resolved with `bun tools/overlay_show.ts
 * resource_3a4 3460 | bun tools/overlay_call_targets.ts resource_3a4 3460
 * 3480 --annotate`):
 *   0x2003462 -> veneer -> Func_02006ffc(selector)          scene-record accessor, established
 *   0x200347a -> veneer -> Func_02006fbc(0, x, z, 255)      established, "place a marker (kind, x, z, level)" (resource_37b_c_020016dc.c, resource_3b2_c_02000f70.c)
 */

u8 *Func_02006ffc();           /* scene-record accessor, established (veneer to Func_0808a080) */
void Func_02006fbc();          /* place a marker (kind, x, z, level), established (veneer to Func_08009278) */

void Func_02003460(s32 selector)
{
    u8 *record;

    record = Func_02006ffc(selector);
    record[89] &= 0xfe;

    Func_02006fbc(0, *(s32 *)(record + 8), *(s32 *)(record + 16), 255);
}
