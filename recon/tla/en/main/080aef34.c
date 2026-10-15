/* Near miss (differ 2): the native loads the slot before it builds the
 * 0xfdff unequip mask; a barrier does not hold because the load moves to
 * its single use. */
#include "inventory.h"

s32 Item_CanOwnerEquipDirect(s32 owner, s32 item);
void Unit_RefreshPsynergy(s32 owner);
void UnitCalc(s32 owner);

s32 Inventory_Equip(s32 owner, s32 slot)
{
    struct OwnerInventoryState *inv = UnitGet(owner);
    u32 item_id = inv->inventory[slot];
    struct ItemDefinition *item;
    u8 type;
    s32 other;

    if (Item_CanOwnerEquipDirect(owner, item_id) == 0)
        return -1;
    if (item_id & INVENTORY_EQUIPPED)
        return 0;

    item = Item_GetDirect(item_id);
    type = item->type;
    if (type != 6 && type != 11) {
        for (other = 0; other <= 14; other++) {
            if ((inv->inventory[other] & INVENTORY_EQUIPPED) == 0)
                continue;
            if (Item_GetDirect(inv->inventory[other])->type == type)
                break;
        }

        if (other != 15) {
            item = Item_GetDirect(inv->inventory[other]);
            if (item->flags & 2)
                return -2;
            inv->inventory[other] &= 0xfdff;
        }
    }

    inv->inventory[slot] |= INVENTORY_EQUIPPED;
    Unit_RefreshPsynergy(owner);
    UnitCalc(owner);
    return 0;
}
