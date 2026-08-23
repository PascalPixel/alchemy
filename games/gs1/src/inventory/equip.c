#include "inventory.h"

void Func_08078bf0(s32 owner);
void Func_08077428(s32 owner);

s32 Inventory_Equip(s32 owner, s32 slot)
{
    struct OwnerInventoryState *inv = OwnerState_Get(owner);
    unsigned int mask;
    unsigned int item_id = inv->inventory[slot];
    struct ItemDefinition *item;
    u8 type;
    s32 other;

    if (Item_CanOwnerEquipDirect(owner, item_id) == 0)
        return -1;
    mask = 0x200;
    if (item_id & mask)
        return 0;

    item = Item_GetDirect(item_id);
    type = item->type;
    if (type != 6) {
        for (other = 0, item_id = 0xd8;
             other <= 14;
             item_id += 2, other++) {
            unsigned int checkMask = mask;
            unsigned int bits = *(u16 *)(item_id + (unsigned int)inv);

            bits &= checkMask;
            if (bits == 0)
                continue;
            if (Item_GetDirect(
                    *(volatile u16 *)(item_id + (unsigned int)inv))->type
                == type)
                break;
        }

        if (other != 15) {
            item = Item_GetDirect(inv->inventory[other]);
            if (item->flags & 2)
                return -2;
            inv->inventory[other] &= 0xfdff;
        }
    }

    inv->inventory[slot] |= 0x200;
    Func_08078bf0(owner);
    Func_08077428(owner);
    return 0;
}
