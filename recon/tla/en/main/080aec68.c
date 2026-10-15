/* Near miss, differ 19 of 28, 2026-10-15: one instruction short. The type
 * pinned to r1 gives the native's registers (unpinned it takes r2: differ
 * 37); every branch offset then differs by two bytes because the tail is
 * one instruction shorter. Remaining: for type == 10 the native builds a
 * fresh 1 (lsrs r0, r2, #31; movs r3, #1; subs r0, r3, r0); this reuses the
 * 1 already in r0 from the type 7 test (lsrs r2, r2, #31; subs r0, r0, r2).
 * Tried: an if/return 0 tail, ?:, !(type ^ 10), 1 - (type != 10), a result
 * local, a one-pass block or an empty asm before the tail, and a 1 pinned
 * to r3 (differ 24). The Broken Seal's form (one group variable, else-ifs)
 * lays the tests out otherwise (differ 45).
 */
#include "types.h"
#include "item.h"

struct ItemDefinition *Item_GetDirect(s32 item_id);

s32 Item_GetEquipmentGroup(s32 item_id)
{
    register s32 type asm("r1") = Item_GetDirect(item_id)->type;

    if (type == 1)
        return 1;
    if (type == 2)
        return 2;
    if (type == 3)
        return 2;
    if (type == 4)
        return 2;
    if (type == 5)
        return 2;
    if (type == 9)
        return 2;
    if (type == 7)
        return 1;
    return type == 10;
}
