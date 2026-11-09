/// @file
/// Items and inventories: equipping, adding, removing, breaking and counting.
// Part of game.c.

#include "../game_internal.h"

/// Returns the item's definition.
/// item: item ID; the inventory's equipment and quantity bits are ignored.
struct ItemDefinition *Item_GetDirect(s32 item) {
  return (struct ItemDefinition *)gItem_DefinitionTable + (item & ITEM_ID_MASK);
}

/// Returns whether a character can equip the item: bit classId of its
/// equipMask, or 0 for classId 8 and up.
s32 Item_CanOwnerEquipDirect(s32 unit, s32 item) {
  struct OwnerInventoryState *st = Unit_Get(unit);
  struct ItemDefinition *def = Item_GetDirect(item);
  s32 mask = def->equipMask;

  if (st->classId >= 8) {
    return 0;
  }
  return (mask >> st->classId) & 1;
}

/// Returns 1 for an item that is not equipment, otherwise whether the
/// character can equip it.
s32 Item_IsCompatibleWithOwner(s32 unit, s32 item) {
  if (Item_GetEquipmentGroup(item) == 0) {
    return 1;
  }
  return Item_CanOwnerEquipDirect(unit, item);
}

struct ItemDefinition *Item_GetDirect(s32 item_id);

/// Returns an item's equipment group from its type: 1 for types 1, 7 and 10,
/// 2 for types 2 to 5 and 9, and 0 for any other type.
s32 Item_GetEquipmentGroup(s32 item_id) {
  s32 type = Item_GetDirect(item_id)->type;
  s32 group;

  if (type == 1)
    group = 1;
  else if (type == 2)
    group = 2;
  else if (type == 3)
    group = 2;
  else if (type == 4)
    group = 2;
  else if (type == 5)
    group = 2;
  else if (type == 9)
    group = 2;
  else if (type == 7)
    group = 1;
  else
    group = type == 10;
  return group;
}

s32 Inventory_GetQuantity(s32 owner, s32 slot) {
  s32 item_id;

  owner = ((struct OwnerInventoryState *)Unit_Get(owner))->inventory[slot];
  /* FAKEMATCH: keeps the mask after the slot load. */
  __asm__("");
  item_id = ITEM_ID_MASK;
  item_id &= owner;
  owner = (u32)owner >> INVENTORY_QUANTITY_SHIFT;
  owner++;
  if (item_id == 0) {
    owner = 0;
  }
  return owner;
}

s32 Inventory_Count(s32 owner) {
  struct OwnerInventoryState *inv = Unit_Get(owner);
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

s32 PartyInventory_HasSpace(void) {
  s16 owners[10];
  s32 ownerCount;
  s32 ownerIndex;
  s16 *ownerCursor;

  if (Inventory_Count(gMainRoutineWork.currentOwner) != 15)
    return 1;
  ownerCount = Party_ListActiveOwners(owners);
  ownerCursor = owners;
  ownerIndex = 0;
  if (ownerIndex < ownerCount) {
    do {
      if (Inventory_Count(*ownerCursor++) != 15)
        return 1;
      ownerIndex++;
    } while (ownerIndex < ownerCount);
  }
  return 0;
}

s32 PartyInventory_CountFreeSlots(void) {
  s16 owners[10];
  s32 ownerCount = Party_ListActiveOwners(owners);
  s32 cnt = 0;
  s16 *ownerCursor = owners;

  if (cnt < ownerCount) {
    s32 n = ownerCount;

    do {
      cnt = cnt - Inventory_Count(*ownerCursor++) + 15;
      n--;
    } while (n != 0);
  }
  return cnt;
}

s32 Inventory_AddItem(s32 owner_id, s32 item_id) {
  struct OwnerInventoryState *inv = Unit_Get(owner_id);
  struct ItemDefinition *item = Item_GetDirect(item_id);
  s32 slot;

  if ((item->flags & ITEM_STACKABLE) != 0) {
    slot = 0;
    if (((inv->inventory[slot] ^ item_id) & ITEM_ID_MASK) != 0) {
      do {
        slot++;
        if (slot >= INVENTORY_SLOTS)
          break;
      } while (((inv->inventory[slot] ^ item_id) & ITEM_ID_MASK) != 0);
    }
    if (slot != INVENTORY_SLOTS) {
      s32 entry = inv->inventory[slot];
      u32 count = ((u32)entry >> INVENTORY_QUANTITY_SHIFT) + 1;

      if (count > 29)
        return -1;
      {
        /* FAKEMATCH: a separate mask temporary preserves the and/store
         * registers. */
        s32 value = INVENTORY_ENTRY_MASK;

        value &= entry;
        value |= count << INVENTORY_QUANTITY_SHIFT;
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
  } while (slot < INVENTORY_SLOTS);
  return -1;
}

s32 PartyInventory_Add(s32 itemId) {
  s16 owners[10];
  s32 ownerCount;
  s32 ownerIndex;
  s16 *ownerCursor;

  ownerCount = Party_ListActiveOwners(owners);
  ownerCursor = owners;
  ownerIndex = 0;
  if (ownerIndex < ownerCount) {
    do {
      s16 ownerId = *ownerCursor++;

      if (Inventory_AddItem(ownerId, itemId) >= 0)
        return ownerId;
      ownerIndex++;
    } while (ownerIndex < ownerCount);
  }
  return -1;
}

s32 PartyInventory_FindRoom(s32 itemId) {
  s16 owners[10];
  s32 ownerCount;
  s32 ownerIndex;

  ownerCount = Party_ListActiveOwners(owners);
  for (ownerIndex = 0; ownerIndex < ownerCount; ownerIndex++) {
    s16 ownerId = owners[ownerIndex];
    s32 slot = Inventory_AddItem(ownerId, itemId);

    if (slot >= 0) {
      Inventory_Remove(ownerId, slot);
      return ownerId;
    }
  }
  return -1;
}

s32 Inventory_Find(s32 owner, s32 item_id) {
  struct OwnerInventoryState *inv = Unit_Get(owner);
  s32 slot;

  for (slot = 0; slot < INVENTORY_SLOTS; slot++) {
    if ((inv->inventory[slot] & ITEM_ID_MASK) == item_id) {
      return slot;
    }
  }
  return -1;
}

s32 PartyInventory_FindOwner(s32 itemId) {
  s16 owners[10];
  s32 ownerCount;
  s32 ownerIndex;
  s16 *ownerCursor;
  s16 owner;

  if (Inventory_Find(gMainRoutineWork.currentOwner, itemId) != -1)
    return gMainRoutineWork.currentOwner;
  ownerCount = Party_ListActiveOwners(owners);
  ownerCursor = owners;
  ownerIndex = 0;
  if (ownerIndex < ownerCount) {
    do {
      owner = *ownerCursor++;
      if (Inventory_Find(owner, itemId) != -1)
        return owner;
      ownerIndex++;
    } while (ownerIndex < ownerCount);
  }
  return -1;
}

s32 Item_CanOwnerEquipDirect(s32 owner, s32 item);
void Unit_RefreshPsynergy(s32 owner);
void Unit_Calculate(s32 owner);

s32 Inventory_Equip(s32 owner, s32 slot) {
  struct OwnerInventoryState *inv = Unit_Get(owner);
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
  Unit_Calculate(owner);
  return 0;
}

s32 Inventory_FindEquipped(s32 owner, s32 type) {
  struct OwnerInventoryState *base = Unit_Get(owner);
  s32 index;
  struct ItemDefinition *item;

  for (index = 0; index < INVENTORY_SLOTS; index++) {
    if (base->inventory[index] & INVENTORY_EQUIPPED) {
      item = Item_GetDirect(base->inventory[index]);
      if (item->type == type)
        break;
    }
  }
  if (index == INVENTORY_SLOTS)
    index = -1;
  return index;
}

struct ItemDefinition *
Inventory_GetEquippedDefinition(struct OwnerInventoryState *inv, s32 type) {
  s32 slot;
  struct ItemDefinition *item;

  for (slot = 0; slot < INVENTORY_SLOTS; slot++) {
    if (inv->inventory[slot] & INVENTORY_EQUIPPED) {
      item = Item_GetDirect(inv->inventory[slot]);
      if (item->type == type) {
        return item;
      }
    }
  }
  return 0;
}

s32 Inventory_GetEquippedItem(struct OwnerInventoryState *inv, s32 type) {
  s32 slot;

  for (slot = 0; slot <= 14; slot++) {
    if (inv->inventory[slot] & 0x200) {
      struct ItemDefinition *item = Item_GetDirect(inv->inventory[slot]);

      if (item->type == type) {
        return inv->inventory[slot] & 0x1ff;
      }
    }
  }
  return 0;
}

void Unit_Calculate(s32 owner);

/// Empties an inventory slot outright, closes the inventory up and
/// recalculates the unit; returns 2, as Inventory_Remove does for a last item.
s32 Func_080af0e4(s32 owner, s32 slot) {
  struct OwnerInventoryState *unit = Unit_Get(owner);

  unit->inventory[slot] = 0;
  Inventory_CompactSlots((s16 *)unit->inventory);
  Unit_Calculate(owner);
  return 2;
}

s32 Inventory_Remove(s32 owner, s32 slot) {
  struct OwnerInventoryState *unit = Unit_Get(owner);
  u16 item = unit->inventory[slot];
  s32 result = -1;

  if (item != 0) {
    if (item & INVENTORY_QUANTITY_MASK) {
      unit->inventory[slot] = item - INVENTORY_QUANTITY_STEP;
      result = 1;
    } else {
      unit->inventory[slot] = 0;
      Inventory_CompactSlots((s16 *)unit->inventory);
      result = 2;
    }
  }
  Unit_Calculate(owner);
  return result;
}

s32 Inventory_Discard(s32 owner, s32 slot) {
  s32 item = ((struct OwnerInventoryState *)Unit_Get(owner))->inventory[slot];
  s32 removed_slot = Inventory_Remove(owner, slot);

  if (removed_slot != -1) {
    Item_AdjustCounter(item, 1);
    Event_ClearInvalidPackedValuesFar();
  }
  return removed_slot;
}

s32 Inventory_CheckUnequip(s32 owner, s32 slot) {
  struct OwnerInventoryState *inv = Unit_Get(owner);
  s32 raw = inv->inventory[slot];
  s32 item_id;
  struct ItemDefinition *item;
  s32 result;

  asm volatile(
      ""); /* FAKEMATCH: ⚓️ loads the slot before it builds the item mask */
  /* FAKEMATCH: the mask is built in the register it masks into; raw &
   * ITEM_ID_MASK swaps r3 and r5 */
  item_id = ITEM_ID_MASK;
  item_id &= raw;
  item = Item_GetDirect(item_id);
  result = 0;

  if (item_id == 0)
    return 8;
  if ((inv->inventory[slot] & INVENTORY_EQUIPPED) != 0 &&
      (item->flags & ITEM_CANNOT_UNEQUIP) != 0)
    result = 1;
  return result;
}

s32 Inventory_CheckDiscard(s32 owner, s32 slot) {
  struct OwnerInventoryState *inv = Unit_Get(owner);
  s32 raw = inv->inventory[slot];
  s32 item_id;
  struct ItemDefinition *item;
  s32 result;

  asm volatile(
      ""); /* FAKEMATCH: ⚓️ loads the slot before it builds the item mask */
  /* FAKEMATCH: the mask is built in the register it masks into; raw &
   * ITEM_ID_MASK swaps r3 and r5 */
  item_id = ITEM_ID_MASK;
  item_id &= raw;
  item = Item_GetDirect(item_id);
  result = 0;

  if (item_id == 0)
    return 8;
  if ((item->flags & ITEM_INDISPENSABLE) != 0)
    result = 2;
  if ((inv->inventory[slot] & INVENTORY_EQUIPPED) != 0 &&
      (item->flags & ITEM_CANNOT_UNEQUIP) != 0)
    result |= 1;
  return result;
}

s32 PartyInventory_Remove(s32 item_id) {
  s32 owner = PartyInventory_FindOwner(item_id);

  if (owner == -1)
    return 0;
  Inventory_Remove(owner, Inventory_Find(owner, item_id));
  return 0;
}

s32 PartyInventory_Discard(s32 item_id) {
  s32 owner = PartyInventory_FindOwner(item_id);

  if (owner == -1)
    return 0;
  Inventory_Discard(owner, Inventory_Find(owner, item_id));
  return 0;
}

enum { ITEM_COUNTER_COUNT = 300 };

extern u8 gItemCounters[ITEM_COUNTER_COUNT];

s32 Inventory_Break(s32 owner, s32 slot) {
  struct OwnerInventoryState *inv = Unit_Get(owner);
  if (inv->inventory[slot] == 0) {
    return -1;
  }
  inv->inventory[slot] |= 0x400;
  return 0;
}

s32 Inventory_Repair(s32 owner, s32 slot) {
  struct OwnerInventoryState *inv = Unit_Get(owner);
  if (inv->inventory[slot] == 0) {
    return -1;
  }
  inv->inventory[slot] &= ~0x400;
  return 0;
}

u8 Item_GetTargetMode(s32 itemId) {
  return ((struct BattleAct *)BattleAction_GetDirect(
              Item_GetDirect(itemId)->actionId))
      ->targetMode;
}

s32 ItemCounter_Adjust(s32 index, s32 delta) {
  s32 counter_slot = index;
  u8 *data = gItemCounters;

  index = 0;
  if (counter_slot < ITEM_COUNTER_COUNT) {
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

extern u16 gData_080b1f40[];

s32 Item_AdjustCounter(s32 itemId, s32 delta) {
  s32 result;
  s32 index = ITEM_ID_MASK;
  u16 *table = gData_080b1f40;
  u16 counter;

  /* FAKEMATCH: the barrier keeps the table's load between the mask constant and
   * its and, the index doubled apart keeps the table the base register; without
   * them, 2 instructions differ in tla-en. */
  asm volatile("");
  index &= itemId;
  index <<= 1;
  counter = *(u16 *)((u8 *)table + index);
  result = 0;
  if (counter)
    result = ItemCounter_Adjust(counter - 1, delta);
  return result;
}

s32 Inventory_CountItem(s32 owner, s32 item_id) {
  struct OwnerInventoryState *base = Unit_Get(owner);
  s32 count = 0;
  s32 target = item_id & ITEM_ID_MASK;
  s32 index = 0;

  do {
    if ((base->inventory[index] & ITEM_ID_MASK) == target) {
      struct ItemDefinition *item = Item_GetDirect(target);

      if (item->flags & ITEM_STACKABLE) {
        count = (base->inventory[index] >> INVENTORY_QUANTITY_SHIFT) + 1;
        break;
      }
      count++;
    }
    index++;
  } while (index < INVENTORY_SLOTS);
  return count;
}

s32 PartyInventory_CountItem(s32 itemId) {
  u16 owners[16];
  s32 itemCount = 0;
  s32 ownerCount = Party_ListActiveOwners(owners);

  if (itemCount < ownerCount) {
    u16 *ownerCursor = owners;
    s32 n = ownerCount;

    do {
      itemCount += Inventory_CountItem(*ownerCursor++, itemId);
      n--;
    } while (n != 0);
  }
  return itemCount;
}
