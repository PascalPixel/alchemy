/// @file
/// Items and inventories: equipping, adding, removing, breaking and counting.
// Part of game.c.

#include "game_internal.h"

/// Returns an item's definition; an inventory entry's other bits are ignored.
struct ItemDefinition *Item_GetDirect(s32 item) {
  return &gItem_DefinitionTable[item & INVENTORY_ITEM_ID_MASK];
}

/// Returns whether the character can equip the item.
s32 Item_CanUnitEquipDirect(s32 unitId, s32 itemId) {
  struct Unit *unit = Unit_Get(unitId);
  struct ItemDefinition *item = Item_GetDirect(itemId);
  s32 equipMask = item->equipMask;

  if (unit->id >= PARTY_UNIT_COUNT)
    return 0;
  return (equipMask >> unit->id) & 1;
}

/// Returns 1 for an item that is not a weapon or armor, and for those whether
/// the character can equip it.
s32 Item_IsCompatibleWithUnit(s32 unit, s32 item) {
  if (Item_GetEquipmentGroup(item) == EQUIPMENT_GROUP_NONE)
    return 1;
  return Item_CanUnitEquipDirect(unit, item);
}

/// Returns what kind of thing an item is, an enum EquipmentGroup.
s32 Item_GetEquipmentGroup(s32 itemId) {
  struct ItemDefinition *item = Item_GetDirect(itemId);

  s32 group = EQUIPMENT_GROUP_NONE;

  if (item->type == ITEM_TYPE_WEAPON)
    group = EQUIPMENT_GROUP_WEAPON;
  else if (item->type == ITEM_TYPE_ARMOR)
    group = EQUIPMENT_GROUP_ARMOR;
  else if (item->type == ITEM_TYPE_SHIELD)
    group = EQUIPMENT_GROUP_ARMOR;
  else if (item->type == ITEM_TYPE_HELM)
    group = EQUIPMENT_GROUP_ARMOR;
  else if (item->type == ITEM_TYPE_BOOTS)
    group = EQUIPMENT_GROUP_ARMOR;
  else if (item->type == ITEM_TYPE_SHIRT)
    group = EQUIPMENT_GROUP_ARMOR;
  return group;
}

/// Returns how many of the item in `slot` the unit carries (0 for an empty
/// slot). The entry stores one less than the count.
s32 Inventory_GetQuantity(s32 unitId, s32 slot) {
  u32 entry = Unit_Get(unitId)->inventory[slot];

  if ((entry & INVENTORY_ITEM_ID_MASK) == 0)
    return 0;
  return (entry >> INVENTORY_QUANTITY_SHIFT) + 1;
}

/// Counts filled inventory slots up to the first empty slot; the inventory is
/// kept closed up, so this is how many items the unit carries.
s32 Inventory_Count(s32 unitId) {
  struct Unit *unit = Unit_Get(unitId);
  s32 count;

  for (count = 0; count < INVENTORY_SLOTS; count++) {
    if (unit->inventory[count] == 0)
      break;
  }
  return count;
}

/// Returns 1 when the selected character or any party member has a free
/// inventory slot.
s32 PartyInventory_HasSpace(void) {
  s16 unitIds[10];
  s32 unitCount;
  s16 *unitId;
  s32 i;

  if (Inventory_Count(gMainRoutineWork.leaderActorId) != INVENTORY_SLOTS)
    return 1;
  unitCount = Party_ListMembers(unitIds);
  unitId = unitIds;
  for (i = 0; i < unitCount; i++) {
    if (Inventory_Count(*unitId++) != INVENTORY_SLOTS)
      return 1;
  }
  return 0;
}

/// Returns the free inventory slots of the whole party.
s32 PartyInventory_CountFreeSlots(void) {
  s16 unitIds[10];
  s32 unitCount = Party_ListMembers(unitIds);
  s32 freeSlots = 0;
  s16 *unitId = unitIds;
  s32 i;

  for (i = 0; i < unitCount; i++)
    freeSlots = freeSlots - Inventory_Count(*unitId++) + INVENTORY_SLOTS;
  return freeSlots;
}

/// Gives a unit one item, joining its first matching stack (up to 30) or taking
/// the first free slot. A full matching stack fails immediately, even if
/// another slot is free.
s32 Inventory_AddItem(s32 unitId, s32 itemId) {
  struct Unit *unit = Unit_Get(unitId);
  struct ItemDefinition *item = Item_GetDirect(itemId);
  s32 slot;

  if (item->flags & ITEM_STACKABLE) {
    for (slot = 0; slot < INVENTORY_SLOTS; slot++) {
      if (((unit->inventory[slot] ^ itemId) & INVENTORY_ITEM_ID_MASK) == 0)
        break;
    }
    if (slot != INVENTORY_SLOTS) {
      s32 entry = unit->inventory[slot];
      u32 count = ((u32)entry >> INVENTORY_QUANTITY_SHIFT) + 1;

      // A stack holds 30 at most.
      if (count > 29)
        return -1;
      {
        s32 value = INVENTORY_ENTRY_MASK;

        value &= entry;
        value |= count << INVENTORY_QUANTITY_SHIFT;
        unit->inventory[slot] = value;
      }
      return slot;
    }
  }

  for (slot = 0; slot < INVENTORY_SLOTS; slot++) {
    if (unit->inventory[slot] == 0) {
      unit->inventory[slot] = itemId;
      return slot;
    }
  }
  return -1;
}

/// Gives the item to the first party member with room. Returns who took it, or
/// -1.
s32 PartyInventory_Add(s32 itemId) {
  s16 unitIds[10];
  s32 unitCount = Party_ListMembers(unitIds);
  s16 *cursor = unitIds;
  s32 i;

  for (i = 0; i < unitCount; i++) {
    s16 unitId = *cursor++;

    if (Inventory_AddItem(unitId, itemId) >= 0)
      return unitId;
  }
  return -1;
}

/// Returns the first slot holding `itemId` (any quantity, equipped or not), or
/// -1.
s32 Inventory_Find(s32 unitId, s32 itemId) {
  struct Unit *unit = Unit_Get(unitId);
  s32 slot;

  for (slot = 0; slot < INVENTORY_SLOTS; slot++) {
    if ((unit->inventory[slot] & INVENTORY_ITEM_ID_MASK) == itemId)
      return slot;
  }
  return -1;
}

/// Returns who carries `itemId`, asking the selected character first, or -1.
s32 PartyInventory_FindMember(s32 itemId) {
  s16 unitIds[10];
  s32 unitCount;
  s16 *cursor;
  s32 i;

  if (Inventory_Find(gMainRoutineWork.leaderActorId, itemId) != -1)
    return gMainRoutineWork.leaderActorId;
  unitCount = Party_ListMembers(unitIds);
  cursor = unitIds;
  for (i = 0; i < unitCount; i++) {
    s16 unitId = *cursor++;

    if (Inventory_Find(unitId, itemId) != -1)
      return unitId;
  }
  return -1;
}

/// Equips the item in `slot`, clearing the equipped bit of the item of the same
/// type it replaces. Psynergy items may coexist. Recomputes the unit's Psynergy
/// and stats. Returns 0, -1 when the unit cannot equip it, or -2 when the item
/// it would replace is cursed on.
s32 Inventory_Equip(s32 unitId, s32 slot) {
  struct Unit *unit = Unit_Get(unitId);
  u32 inventoryEntry = unit->inventory[slot];
  struct ItemDefinition *item;
  u8 type;
  s32 equippedSlot;

  if (Item_CanUnitEquipDirect(unitId, inventoryEntry) == 0)
    return -1;
  if (inventoryEntry & INVENTORY_EQUIPPED)
    return 0;

  item = Item_GetDirect(inventoryEntry);
  type = item->type;
  if (type != ITEM_TYPE_PSYNERGY) {
    for (equippedSlot = 0; equippedSlot < INVENTORY_SLOTS; equippedSlot++) {
      if ((unit->inventory[equippedSlot] & INVENTORY_EQUIPPED) == 0)
        continue;
      if (Item_GetDirect(unit->inventory[equippedSlot])->type == type)
        break;
    }

    if (equippedSlot != INVENTORY_SLOTS) {
      item = Item_GetDirect(unit->inventory[equippedSlot]);
      if (item->flags & ITEM_CANNOT_UNEQUIP)
        return -2;
      unit->inventory[equippedSlot] &= (u16)~INVENTORY_EQUIPPED;
    }
  }

  unit->inventory[slot] |= INVENTORY_EQUIPPED;
  Unit_RefreshPsynergy(unitId);
  Unit_Calculate(unitId);
  return 0;
}

/// Returns the slot of the equipped item of `type`, or -1.
s32 Inventory_FindEquipped(s32 unitId, s32 type) {
  struct Unit *base = Unit_Get(unitId);
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

/// Returns the definition of the unit's equipped item of `type`, or NULL.
/// Nothing calls it by name.
struct ItemDefinition *Inventory_GetEquippedDefinition(struct Unit *unit,
                                                       s32 type) {
  s32 slot;
  struct ItemDefinition *item;

  for (slot = 0; slot < INVENTORY_SLOTS; slot++) {
    if (unit->inventory[slot] & INVENTORY_EQUIPPED) {
      item = Item_GetDirect(unit->inventory[slot]);
      if (item->type == type) {
        return item;
      }
    }
  }
  return 0;
}

/// Returns the id of the unit's equipped item of `type`, or 0.
s32 Inventory_GetEquippedItem(struct Unit *unit, s32 type) {
  s32 slot;

  for (slot = 0; slot < INVENTORY_SLOTS; slot++) {
    if (unit->inventory[slot] & INVENTORY_EQUIPPED) {
      struct ItemDefinition *item = Item_GetDirect(unit->inventory[slot]);

      if (item->type == type) {
        return unit->inventory[slot] & INVENTORY_ITEM_ID_MASK;
      }
    }
  }
  return 0;
}

/// Takes one item out of slot, closing up the inventory when it was the last.
/// The Psynergy list is not rebuilt, so a removed item's Psynergy stays listed
/// until something rebuilds it.
s32 Inventory_Remove(s32 unitId, s32 slot) {
  struct Unit *unit = Unit_Get(unitId);
  u16 item = unit->inventory[slot];
  s32 result = INVENTORY_REMOVE_EMPTY_SLOT;

  if (item != 0) {
    if (item & INVENTORY_QUANTITY_MASK) {
      unit->inventory[slot] = item - INVENTORY_QUANTITY_STEP;
      result = INVENTORY_REMOVE_ONE;
    } else {
      unit->inventory[slot] = 0;
      Inventory_CompactSlots(unit->inventory);
      result = INVENTORY_REMOVE_LAST;
    }
  }
  Unit_Calculate(unitId);
  return result;
}

/// Removes one item as Inventory_Remove does, and puts a rare item back into
/// circulation (its counter goes up by one) for the shops to sell.
s32 Inventory_Discard(s32 unitId, s32 slot) {
  s32 item = Unit_Get(unitId)->inventory[slot];
  s32 removalResult = Inventory_Remove(unitId, slot);

  if (removalResult != INVENTORY_REMOVE_EMPTY_SLOT) {
    Item_AdjustCounter(item, 1);
    Party_ClearUnusableShortcutsFar();
  }
  return removalResult;
}

/// Returns whether the item in `slot` may be dropped: 0 yes, -1 the slot is
/// empty, -4 the item is indispensable, -3 it is equipped and cursed on.
s32 Inventory_CheckDiscard(s32 unitId, s32 slot) {
  struct Unit *unit = Unit_Get(unitId);
  s32 itemId = unit->inventory[slot] & INVENTORY_ITEM_ID_MASK;
  struct ItemDefinition *item = Item_GetDirect(itemId);

  if (itemId == 0) {
    return -1;
  }
  if ((item->flags & ITEM_INDISPENSABLE) != 0) {
    return -4;
  }
  if ((unit->inventory[slot] & INVENTORY_EQUIPPED) != 0 &&
      (item->flags & ITEM_CANNOT_UNEQUIP) != 0) {
    return -3;
  }
  return 0;
}

/// Removes one `itemId` from whoever carries it first; always returns 0.
s32 PartyInventory_Remove(s32 itemId) {
  s32 unitId = PartyInventory_FindMember(itemId);

  if (unitId == -1)
    return 0;
  Inventory_Remove(unitId, Inventory_Find(unitId, itemId));
  return 0;
}

/// Discards one `itemId` from whoever carries it first, as Inventory_Discard
/// does; always returns 0.
s32 PartyInventory_Discard(s32 itemId) {
  s32 unitId = PartyInventory_FindMember(itemId);

  if (unitId == -1)
    return 0;
  Inventory_Discard(unitId, Inventory_Find(unitId, itemId));
  return 0;
}

/// Marks the item in `slot` broken; returns -1 for an empty slot.
s32 Inventory_Break(s32 unitId, s32 slot) {
  struct Unit *unit = Unit_Get(unitId);

  if (unit->inventory[slot] == 0)
    return -1;
  unit->inventory[slot] |= INVENTORY_BROKEN;
  return 0;
}

/// Clears the broken mark of the item in `slot`; returns -1 for an empty slot.
s32 Inventory_Repair(s32 unitId, s32 slot) {
  struct Unit *unit = Unit_Get(unitId);

  if (unit->inventory[slot] == 0)
    return -1;
  unit->inventory[slot] &= ~INVENTORY_BROKEN;
  return 0;
}

/// Returns how the action an item performs when used picks its targets. Only
/// game.s's bare veneer reaches it; nothing calls it by name.
u8 Item_GetTargetMode(s32 itemId) {
  return Ability_GetDirect(Item_GetDirect(itemId)->abilityId)->targetMode;
}

/// Adds `delta` to rare item counter `index`, kept between 0 and 99, and
/// returns the new count (0 for an index past 127).
s32 RareItemStock_Adjust(s32 index, s32 delta) {
  s32 counter_slot = index;
  u8 *data = gRareItemStock;

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

/// Adds delta to how many of a rare item the shops may sell, and returns the
/// new count; ordinary items have none.
s32 Item_AdjustCounter(s32 itemId, s32 delta) {
  // The table holds a rare item's counter number plus one, and 0 for an
  // ordinary item.
  s32 count = 0;
  u8 counter = gItem_ArtifactSlotTable[itemId & INVENTORY_ITEM_ID_MASK];

  if (counter != 0)
    count = RareItemStock_Adjust(counter - 1, delta);
  return count;
}

/// Counts matching nonstackable items, or the quantity in the first matching
/// stack.
s32 Inventory_CountItem(s32 unitId, s32 itemId) {
  struct Unit *unit = Unit_Get(unitId);
  s32 count = 0;
  s32 slot;

  itemId &= INVENTORY_ITEM_ID_MASK;
  for (slot = 0; slot < INVENTORY_SLOTS; slot++) {
    if ((unit->inventory[slot] & INVENTORY_ITEM_ID_MASK) == itemId) {
      if (Item_GetDirect(itemId)->flags & ITEM_STACKABLE) {
        count = (unit->inventory[slot] >> INVENTORY_QUANTITY_SHIFT) + 1;
        break;
      }
      count++;
    }
  }
  return count;
}

/// Counts `itemId` across the party.
s32 PartyInventory_CountItem(s32 itemId) {
  u16 unitIds[16];
  s32 count = 0;
  s32 unitCount = Party_ListMembers(unitIds);
  s32 i;

  for (i = 0; i < unitCount; i++)
    count += Inventory_CountItem(unitIds[i], itemId);
  return count;
}
