#include "INVENTORY.H"
#include "BATTLE_RUNTIME.H"
#include "ITEM.H"
#include "SCENE.H"

void Owner_RefreshClassActions(s32 owner);
void Owner_RecalculateStats(s32 owner);
void Event_ClearInvalidPackedValuesFar(s32);

extern u8 gItemCounters[128];
extern u8 Item_ArtifactSlotTable[];

extern const u8 BattleAction_DefinitionTable[];

s32 Inventory_GetQuantity(s32 owner, s32 slot)
{
    s32 item_id;

    owner = ((struct OwnerInventoryState *)Owner_GetState(owner))->inventory[slot];
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
    struct OwnerInventoryState *inv = Owner_GetState(owner);
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

    if (Inventory_Count(gGameState.current_owner) != 15)
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
s32 Inventory_AddItem(s32 owner_id, s32 item_id)
{
    struct OwnerInventoryState *inv = Owner_GetState(owner_id);
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

            if (Inventory_AddItem(owner_id, item_id) >= 0)
                return owner_id;
            owner_index++;
        } while (owner_index < owner_count);
    }
    return -1;
}

s32 Inventory_Find(s32 owner, s32 item_id)
{
    struct OwnerInventoryState *inv = Owner_GetState(owner);
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

    if (Inventory_Find(gGameState.current_owner, item_id) != -1)
        return gGameState.current_owner;
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

s32 Inventory_Equip(s32 owner, s32 slot)
{
    struct OwnerInventoryState *inv = Owner_GetState(owner);
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
    Owner_RefreshClassActions(owner);
    Owner_RecalculateStats(owner);
    return 0;
}

s32 Inventory_FindEquipped(s32 owner, s32 type)
{
    u8 *base = Owner_GetState(owner);
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

/* Takes one item from an owner's slot: a stacked item loses one from its
   count, a single item is removed and the list is compacted so the empty
   slots follow the rest. The compaction reads the slots as signed
   halfwords. Returns 1 or 2 for those cases, -1 for an empty slot. */
s32 Inventory_Remove(s32 owner, s32 slot)
{
    struct OwnerInventoryState *inv = Owner_GetState(owner);
    u16 item = inv->inventory[slot];
    s32 result = -1;

    if (item != 0) {
        if (item & 0xf800) {
            inv->inventory[slot] = item - 0x800;
            result = 1;
        } else {
            s16 *list;
            s32 count;
            s32 i;

            inv->inventory[slot] = 0;
            list = (s16 *)inv->inventory;
            count = 0;
            for (i = 0; i < 15; i++) {
                if (list[i] != 0)
                    list[count++] = list[i];
            }
            for (; count < 15; count++)
                list[count] = 0;
            result = 2;
        }
    }
    Owner_RecalculateStats(owner);
    return result;
}

s32 Inventory_Discard(s32 owner, s32 slot)
{
    s32 item =
        ((struct OwnerInventoryState *)Owner_GetState(owner))->inventory[slot];
    s32 removed_slot = Inventory_Remove(owner, slot);

    if (removed_slot != -1) {
        Event_ClearInvalidPackedValuesFar(Item_AdjustCounter(item, 1));
    }
    return removed_slot;
}

s32 Inventory_CheckDiscard(s32 owner, s32 slot)
{
    struct OwnerInventoryState *inv = Owner_GetState(owner);
    s32 item_id = inv->inventory[slot] & 0x1ff;
    struct ItemDefinition *item = Item_GetDirect(item_id);

    if (item_id == 0) {
        return -1;
    }
    if ((item->flags & 8) != 0) {
        return -4;
    }
    if ((inv->inventory[slot] & 0x200) != 0 &&
        (item->flags & 2) != 0) {
        return -3;
    }
    return 0;
}

s32 PartyInventory_Remove(s32 item_id)
{
    s32 owner = PartyInventory_FindOwner(item_id);

    if (owner == -1)
        return 0;
    Inventory_Remove(owner, Inventory_Find(owner, item_id));
    return 0;
}

s32 PartyInventory_Discard(s32 item_id)
{
    s32 owner = PartyInventory_FindOwner(item_id);

    if (owner == -1)
        return 0;
    Inventory_Discard(owner, Inventory_Find(owner, item_id));
    return 0;
}

s32 Inventory_Break(s32 owner, s32 slot)
{
    struct OwnerInventoryState *inv = Owner_GetState(owner);
    if (inv->inventory[slot] == 0) {
        return -1;
    }
    inv->inventory[slot] |= 0x400;
    return 0;
}

s32 Inventory_Repair(s32 owner, s32 slot)
{
    struct OwnerInventoryState *inv = Owner_GetState(owner);
    if (inv->inventory[slot] == 0) {
        return -1;
    }
    inv->inventory[slot] &= ~0x400;
    return 0;
}

u8 Item_GetTargetMode(s32 item_id)
{
    return BattleAction_GetDirect(
        Item_GetDirect(item_id)->action_id)->target_mode;
}

s32 ItemCounter_Adjust(s32 index, s32 delta)
{
    s32 counter_slot = index;
    u8 *data = gItemCounters;

    index = 0;
    if (counter_slot <= 127) {
        s32 value = data[counter_slot];

        value += delta;

        if (value < 0) {
            value = 0;
        } else if (value > 99) {
            value = 99;
            index = 99;
        } else {
            index = value;
        }
        data[counter_slot] = value;
    }
    return index;
}

s32 Item_AdjustCounter(s32 item_id, s32 delta)
{
    s32 item_id_mask = 0x1ff;
    u8 counter;
    s32 result = 0;

    counter = Item_ArtifactSlotTable[item_id & item_id_mask];
    if (counter != 0) {
        result = ItemCounter_Adjust(counter - 1, delta);
    }
    return result;
}

s32 Inventory_CountItem(s32 owner, s32 item_id)
{
    u8 *base = Owner_GetState(owner);
    s32 count = 0;
    s32 target = item_id & 0x1ff;
    s32 index = 0;
    s32 offset = 216;

    do {
        if ((*(u16 *)((u8 *)offset + (s32)base) & 0x1FF) == target) {
            struct ItemDefinition *item = Item_GetDirect(target);

            if (item->flags & 0x10) {
                count = (*(u16 *)((u8 *)offset + (s32)base) >> 11) + 1;
                break;
            }
            count++;
        }
        index++;
        offset += 2;
    } while (index <= 14);
    return count;
}

s32 PartyInventory_CountItem(s32 item_id)
{
    u16 owners[16];
    s32 item_count = 0;
    s32 owner_count = Party_ListActiveOwners(owners);

    if (item_count < owner_count) {
        u16 *owner_cursor = owners;
        s32 n = owner_count;

        do {
            item_count += Inventory_CountItem(*owner_cursor++, item_id);
            n--;
        } while (n != 0);
    }
    return item_count;
}

struct BattleAction *BattleAction_GetDirect(s32 action_id) {
    u32 entry_index;

    entry_index = action_id & 0x3fff;
    if (entry_index >= 0x208U) {
        entry_index = 0;
    }
    return (struct BattleAction *)(BattleAction_DefinitionTable + entry_index * 0x10);
}

/* inventory/has_equipment_value.c */
s32 Equipment_HasValue(s32 owner, s32 value)
{
    u8 *entry = Owner_GetState(owner);
    s32 mask = 0x3fff;
    s32 index = 0;

    entry += 88;
    do {
        s32 current = *(u16 *)entry;

        current &= mask;
        entry += 4;
        if (current == value) {
            return 1;
        }
        index++;
    } while (index <= 31);
    return 0;
}
