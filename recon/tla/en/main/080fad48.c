/* Trial 2026-10-03: use OWNER_STATE.H for the getter contract.
 * Complete object unchanged; fresh EN score 575, 22 differing rows.
 */
#include "OWNER_STATE.H"
/*
 * Draft: Inventory_CountStackedItem does not yet match; the reference keeps
 * the slot cursor in r0 and the count in r5, and copies the value and the
 * masks before each test. Three shapes tried.
 * Links as recon/tla/raw/080fad48.s.
 */
#include "TYPES.H"


/* Counts one party member's units of an item, a stacked slot counting its
   quantity field (bits 11-15) plus one. */
s32 Inventory_CountStackedItem(s32 owner, s32 item_id)
{
    u16 *slot;
    s32 count;
    s32 i;

    slot = (u16 *)((u8 *)Owner_GetState(owner) + 0xd8);
    count = 0;
    for (i = 14; i >= 0; i--) {
        u32 value = *slot++;

        if (value != 0 && (value & 0x1ff) == item_id) {
            count += (value & 0xf800) >> 11;
            count++;
        }
    }
    return count;
}
