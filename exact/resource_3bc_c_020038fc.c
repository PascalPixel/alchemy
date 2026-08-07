#include "types.h"

/*
 * resource_3bc owner at 0x020038fc, 30 bytes: lazily allocate the handle
 * resource_3bc_c_02003b18.c tears down -- if Data_0200dace still holds the
 * empty sentinel (-1, the exact value that teardown owner resets it to),
 * allocate a new one and store it.
 *
 * Complete owner: `push {r5, lr}` at 0x020038fc through the matching
 * interworking return `pop {r5} / pop {r0} / bx r0` at 0x02003912-
 * 0x02003916, followed by the one-word literal pool 0x02003918-0x0200391b
 * that closes the 30-byte span. No arguments, void.
 *
 * Not found by the structural inventory walk (unindexed); this owner is
 * itself called from sibling item-28 owner 0x02003a58
 * (resource_3bc_c_02003a58.c). It makes one outgoing call, resolved with
 * the `+2` rule against the raw image (this owner starts outside a
 * recorded row, so overlay_call_targets.ts's own table does not cover it
 * -- resolved with `bun tools/overlay/overlay_show.ts resource_3bc 38fc | bun
 * tools/lib/overlay_call_targets.ts resource_3bc 38fc 3916 --annotate`):
 *   0x200390c -> veneer -> Func_080153b8()   established, no-arg, u16 return (semantic/main/0809bb64.c)
 */

extern s16 Data_0200dace;

s32 Func_0200829e(void);       /* established (veneer to Func_080153b8) */

void Func_020038fc(void)
{
    s16 *cursor = &Data_0200dace;

    if (*cursor == -1) {
        *cursor = Func_0200829e();
    }
}
