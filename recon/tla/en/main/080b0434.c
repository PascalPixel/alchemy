/* Trial 2026-10-03: express incoming r0 as owner_id and forward it through
 * the canonical getter declaration. This repairs the previous compile error.
 * Fresh EN score 120, 2 differing rows; Owner_GetDefaultElement remains
 * unresolved, so that call is compared by symbol name only.
 */
#include "OWNER_STATE.H"
#include "TYPES.H"
#include "SCENE.H"
#include "ITEM.H"
#include "INVENTORY.H"
#include "BATTLE_RANDOM.H"

/* battle/random16.c */

s32 Item_GetEquippedElement(s32 owner_id)
{
    struct ItemDefinition *item;
    void *owner;

    owner = Owner_GetState(owner_id);
    if (FIELD_AT_OFFSET(owner, u8 *, 0x129) == 0) {
        return Owner_GetDefaultElement(owner);
    }
    item = Inventory_GetEquippedDefinition(
        (struct OwnerInventoryState *)owner, 1);
    if (item != NULL) {
        return FIELD_AT_OFFSET(item, s32 *, 0x14);
    }
    return 4;
}
