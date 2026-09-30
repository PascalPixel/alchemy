#include "TYPES.H"
#include "SCENE.H"
#include "ITEM.H"
#include "INVENTORY.H"
#include "BATTLE_RANDOM.H"

/* battle/random16.c */

s32 Item_GetEquippedElement(void)
{
    struct ItemDefinition *item;
    void *owner;

    owner = Owner_GetState();
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
