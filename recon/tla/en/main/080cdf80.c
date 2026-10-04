/*
 * Draft: Item_MatchesFilter does not yet match; the reference sets the result
 * 1 before subtracting 1 from the type (1 swap). A u8 compare and a switch
 * both reshape it.
 * 2026-10-04: a type local, a result local set to 1 before the test, a
 * barrier after the subtraction, "+r" on the type or the result, and 300
 * device-search steps all keep or worsen the swap. It now directly follows
 * PENDING.C, which it would join.
 * Links as its recon/tla/raw listing.
 */
#include "TYPES.H"
#include "ITEM.H"

struct ItemDefinition *Item_Get(s32 item_id);

s32 Item_MatchesFilter(s32 filter, s32 item_id)
{
    if (filter == item_id)
        return 1;
    if (filter == 0x1ff)
        return 1;
    if (filter == 0x1fe && (u32)(Item_Get(item_id)->type - 1) <= 3)
        return 1;
    return 0;
}
