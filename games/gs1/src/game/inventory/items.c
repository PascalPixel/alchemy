#include "inventory.h"

s32 Inventory_GetQuantity(s32 owner, s32 slot)
{
    s32 item_id;

    owner = ((struct OwnerInventoryState *)OwnerState_Get(owner))->inventory[slot];
    item_id = 0x1ff;
    item_id &= owner;
    owner = (u32)owner >> 11;
    owner++;
    if (item_id == 0) {
        owner = 0;
    }
    return owner;
}

s32 Inventory_Count(s32 owner)
{
    struct OwnerInventoryState *inv = OwnerState_Get(owner);
    s32 count = 0;

    if (inv->inventory[count] != 0) {
        do {
            count++;
            if (count > 14)
                break;
        } while (inv->inventory[count] != 0);
    }
    return count;
}

s32 PartyInventory_HasSpace(void)
{
    s16 owners[10];
    s32 owner_count;
    s32 owner_index;
    s16 *owner_cursor;

    if (Inventory_Count(Data_02000240.current_owner) != 15)
        return 1;
    owner_count = Party_ListActiveOwners(owners);
    owner_cursor = owners;
    owner_index = 0;
    if (owner_index < owner_count) {
        do {
            if (Inventory_Count(*owner_cursor++) != 15)
                return 1;
            owner_index++;
        } while (owner_index < owner_count);
    }
    return 0;
}

s32 PartyInventory_CountFreeSlots(void)
{
    s16 owners[10];
    s32 owner_count = Party_ListActiveOwners(owners);
    s32 cnt = 0;
    s16 *owner_cursor = owners;

    if (cnt < owner_count) {
        s32 n = owner_count;

        do {
            cnt = cnt - Inventory_Count(*owner_cursor++) + 15;
            n--;
        } while (n != 0);
    }
    return cnt;
}

/* 所持品追加。積み重ね可能な品は同一番号の枠を探して個数を増やし、
   そうでなければ空き枠へ入れる。戻り値は枠番号、失敗は -1。 */
s32 Inventory_Add(s32 owner_id, s32 item_id)
{
    struct OwnerInventoryState *inv = OwnerState_Get(owner_id);
    struct ItemDefinition *item = Item_GetDirect(item_id);
    s32 slot;

    if ((item->flags & 0x10) != 0) {
        slot = 0;
        if (((inv->inventory[slot] ^ item_id) & 0x1ff) != 0) {
            do {
                slot++;
                if (slot > 14)
                    break;
            } while (((inv->inventory[slot] ^ item_id) & 0x1ff) != 0);
        }
        if (slot != 15) {
            s32 entry = inv->inventory[slot];
            u32 count = ((u32)entry >> 11) + 1;

            if (count > 29)
                return -1;
            {
                s32 value = 0x7ff;

                value &= entry;
                value |= count << 11;
                inv->inventory[slot] = value;
            }
            return slot;
        }
    }

    slot = 0;
    do {
        if (inv->inventory[slot] == 0) {
            inv->inventory[slot] = item_id;
            return slot;
        }
        slot++;
    } while (slot <= 14);
    return -1;
}

s32 PartyInventory_Add(s32 item_id)
{
    s16 owners[10];
    s32 owner_count;
    s32 owner_index;
    s16 *owner_cursor;

    owner_count = Party_ListActiveOwners(owners);
    owner_cursor = owners;
    owner_index = 0;
    if (owner_index < owner_count) {
        do {
            s16 owner_id = *owner_cursor++;

            if (Inventory_Add(owner_id, item_id) >= 0)
                return owner_id;
            owner_index++;
        } while (owner_index < owner_count);
    }
    return -1;
}

s32 Inventory_Find(s32 owner, s32 item_id)
{
    struct OwnerInventoryState *inv = OwnerState_Get(owner);
    s32 slot = 0;
    u16 *entry = inv->inventory;

    do {
        if (((*entry++) & 0x1ff) == item_id) {
            return slot;
        }
        slot++;
    } while (slot <= 14);
    return -1;
}

s32 PartyInventory_FindOwner(s32 item_id)
{
    s16 owners[10];
    s32 owner_count;
    s32 owner_index;
    s16 *owner_cursor;
    s16 owner;

    if (Inventory_Find(Data_02000240.current_owner, item_id) != -1)
        return Data_02000240.current_owner;
    owner_count = Party_ListActiveOwners(owners);
    owner_cursor = owners;
    owner_index = 0;
    if (owner_index < owner_count) {
        do {
            owner = *owner_cursor++;
            if (Inventory_Find(owner, item_id) != -1)
                return owner;
            owner_index++;
        } while (owner_index < owner_count);
    }
    return -1;
}

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
            unsigned int m = mask;
            unsigned int flags = *(u16 *)(item_id + (unsigned int)inv);

            flags &= m;
            if (flags == 0)
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

s32 Inventory_FindEquipped(s32 owner, s32 type)
{
    u8 *base = OwnerState_Get(owner);
    s32 index;
    s32 offset;
    struct ItemDefinition *item;

    for (index = 0, offset = 216; index <= 14; index++) {
        if (*(u16 *)((u8 *)offset + (s32)base) & 0x200) {
            item = Item_GetDirect(
                *(u16 *)((u8 *)offset + (s32)base));
            if (item->type == type) break;
        }
        offset += 2;
    }
    if (index == 15) index = -1;
    return index;
}

struct ItemDefinition *Inventory_GetEquippedDefinition(
    struct OwnerInventoryState *inv,
    s32 type)
{
    s32 slot;
    struct ItemDefinition *item;

    for (slot = 0; slot <= 14; slot++) {
        if (inv->inventory[slot] & 0x200) {
            item = Item_GetDirect(inv->inventory[slot]);
            if (item->type == type) {
                return item;
            }
        }
    }
    return 0;
}

s32 Inventory_GetEquippedItem(struct OwnerInventoryState *inv, s32 type)
{
    s32 slot;

    for (slot = 0; slot <= 14; slot++) {
        if (inv->inventory[slot] & 0x200) {
            struct ItemDefinition *item =
                Item_GetDirect(inv->inventory[slot]);

            if (item->type == type) {
                return inv->inventory[slot] & 0x1ff;
            }
        }
    }
    return 0;
}
