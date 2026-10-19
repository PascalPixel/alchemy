/// @file
/// Items and inventories: equipping, adding, removing, breaking and counting.
/// Part of game.c, the game object, compiled only through it.

#include "../game_internal.h"

/// Returns item `item`'s definition; an inventory entry may be passed, its
/// equipped and quantity bits are ignored.
struct ItemDefinition *Item_GetDirect(s32 item) {
  return gItem_DefinitionTable + (item & ITEM_ID_MASK);
}

/// Returns whether the character can equip `item`: bit classId of the item's
/// equipMask. Units past the eight characters cannot.
s32 Item_CanOwnerEquipDirect(s32 owner, s32 item) {
  struct BattleUnit *unit = UnitGet(owner);
  struct ItemDefinition *itemDefinition = Item_GetDirect(item);
  s32 equipMask = itemDefinition->equipMask;

  if (unit->classId >= 8) {
    return 0;
  }
  return (equipMask >> unit->classId) & 1;
}

/// Returns 1 for an item that is not equipment, and for equipment whether the
/// character can equip it.
s32 Item_IsCompatibleWithOwner(s32 unit, s32 item) {
  if (Item_GetEquipmentGroup(item) == 0) {
    return 1;
  }
  return Item_CanOwnerEquipDirect(unit, item);
}

/// Returns what kind of thing an item is: 1 a weapon, 2 armor (types 2 to 5
/// and 9), 0 anything else.
s32 Item_GetEquipmentGroup(s32 itemId) {
  s32 group;
  struct ItemDefinition *item;

  item = Item_GetDirect(itemId);
  group = 0;
  if (item->type == ITEM_TYPE_WEAPON)
    group = 1;
  else if (item->type == 2)
    group = 2;
  else if (item->type == 3)
    group = 2;
  else if (item->type == 4)
    group = 2;
  else if (item->type == 5)
    group = 2;
  else if (item->type == 9)
    group = 2;
  return group;
}

/// Returns how many of the item in `slot` the unit carries (0 for an empty
/// slot). The entry stores one less than the count.
s32 Inventory_GetQuantity(s32 owner, s32 slot) {
  s32 itemId;

  owner = UnitGet(owner)->inventory[slot];
  itemId = ITEM_ID_MASK;
  itemId &= owner;
  owner = (u32)owner >> INVENTORY_QUANTITY_SHIFT;
  owner++;
  if (itemId == 0) {
    owner = 0;
  }
  return owner;
}

/// Counts filled inventory slots up to the first empty slot.
s32 Inventory_Count(s32 owner) {
  struct BattleUnit *unit = UnitGet(owner);
  s32 count = 0;

  if (unit->inventory[count] != 0) {
    do {
      count++;
      if (count > 14)
        break;
    } while (unit->inventory[count] != 0);
  }
  return count;
}

/// Returns 1 when the selected character or any party member has a free
/// inventory slot.
s32 PartyInventory_HasSpace(void) {
  s16 owners[10];
  s32 ownerCount;
  s32 ownerIndex;
  s16 *ownerCursor;

  if (Inventory_Count(gGameState.selectedActor) != INVENTORY_SLOTS)
    return 1;
  ownerCount = Party_ListActiveOwners(owners);
  ownerCursor = owners;
  ownerIndex = 0;
  if (ownerIndex < ownerCount) {
    do {
      if (Inventory_Count(*ownerCursor++) != INVENTORY_SLOTS)
        return 1;
      ownerIndex++;
    } while (ownerIndex < ownerCount);
  }
  return 0;
}

/// Returns the number of free inventory slots across the party.
s32 PartyInventory_CountFreeSlots(void) {
  s16 owners[10];
  s32 ownerCount = Party_ListActiveOwners(owners);
  s32 freeSlots = 0;
  s16 *ownerCursor = owners;

  if (freeSlots < ownerCount) {
    s32 remainingOwners = ownerCount;

    do {
      freeSlots = freeSlots - Inventory_Count(*ownerCursor++) + 15;
      remainingOwners--;
    } while (remainingOwners != 0);
  }
  return freeSlots;
}

/// Gives a unit one item, joining its first matching stack (up to 30) or
/// taking the first free slot. A full matching stack fails immediately, even
/// if another slot is free. Returns the slot, or -1 when there was no room.
s32 Inventory_AddItem(s32 owner, s32 itemId) {
  struct BattleUnit *unit = UnitGet(owner);
  struct ItemDefinition *item = Item_GetDirect(itemId);
  s32 slot;

  if ((item->flags & ITEM_STACKABLE) != 0) {
    slot = 0;
    if (((unit->inventory[slot] ^ itemId) & ITEM_ID_MASK) != 0) {
      do {
        slot++;
        if (slot >= INVENTORY_SLOTS)
          break;
      } while (((unit->inventory[slot] ^ itemId) & ITEM_ID_MASK) != 0);
    }
    if (slot != INVENTORY_SLOTS) {
      s32 entry = unit->inventory[slot];
      u32 count = ((u32)entry >> INVENTORY_QUANTITY_SHIFT) + 1;

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

  slot = 0;
  do {
    if (unit->inventory[slot] == 0) {
      unit->inventory[slot] = itemId;
      return slot;
    }
    slot++;
  } while (slot < INVENTORY_SLOTS);
  return -1;
}

/// Gives the item to the first party member with room. Returns who took it,
/// or -1.
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

/// Returns the first slot holding `itemId`, or -1.
s32 Inventory_Find(s32 owner, s32 itemId) {
  struct BattleUnit *inv = UnitGet(owner);
  s32 slot = 0;
  u16 *entry = inv->inventory;

  do {
    if (((*entry++) & ITEM_ID_MASK) == itemId) {
      return slot;
    }
    slot++;
  } while (slot < INVENTORY_SLOTS);
  return -1;
}

/// Returns who carries `itemId`, asking the selected character first, or -1.
s32 PartyInventory_FindOwner(s32 itemId) {
  s16 owners[10];
  s32 ownerCount;
  s32 ownerIndex;
  s16 *ownerCursor;
  s16 owner;

  if (Inventory_Find(gGameState.selectedActor, itemId) != -1)
    return gGameState.selectedActor;
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

/// Equips the item in `slot`, replacing equipment of the same type. Type 6
/// items grant Psynergy and may coexist. Returns 0, -1 when the unit cannot
/// equip it, or -2 when the item it would replace is cursed on.
s32 Inventory_Equip(s32 owner, s32 slot) {
  struct BattleUnit *unit = UnitGet(owner);
  u32 inventoryEntry = unit->inventory[slot];
  struct ItemDefinition *item;
  u8 type;
  s32 equippedSlot;

  if (Item_CanOwnerEquipDirect(owner, inventoryEntry) == 0)
    return -1;
  if (inventoryEntry & INVENTORY_EQUIPPED)
    return 0;

  item = Item_GetDirect(inventoryEntry);
  type = item->type;
  if (type != 6) {
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
      unit->inventory[equippedSlot] &= 0xfdff;
    }
  }

  unit->inventory[slot] |= INVENTORY_EQUIPPED;
  Unit_RefreshPsynergy(owner);
  UnitCalc(owner);
  return 0;
}

/// Returns the slot of the equipped item of `type`, or -1.
s32 Inventory_FindEquipped(s32 owner, s32 type) {
  struct BattleUnit *base = UnitGet(owner);
  s32 index;
  struct ItemDefinition *item;

  for (index = 0; index < INVENTORY_SLOTS; index++) {
    if (base->inventory[index] & INVENTORY_EQUIPPED) {
      item = Item_GetDirect(base->inventory[index]);
      if (item->type == type)
        break;
    }
  }
  if (index == 15)
    index = -1;
  return index;
}

/// Returns the definition of the equipped item of `type`, or NULL.
struct ItemDefinition *Inventory_GetEquippedDefinition(struct BattleUnit *unit,
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

/// Returns the id of the equipped item of `type`, or 0.
s32 Inventory_GetEquippedItem(struct BattleUnit *unit, s32 type) {
  s32 slot;

  for (slot = 0; slot < INVENTORY_SLOTS; slot++) {
    if (unit->inventory[slot] & INVENTORY_EQUIPPED) {
      struct ItemDefinition *item = Item_GetDirect(unit->inventory[slot]);

      if (item->type == type) {
        return unit->inventory[slot] & ITEM_ID_MASK;
      }
    }
  }
  return 0;
}

/// Takes one item out of `slot`: a stack shrinks by one (returns 1), a last
/// item empties the slot and the inventory closes up (returns 2). Returns -1
/// for an empty slot. The unit's stats are recomputed either way.
s32 Inventory_Remove(s32 owner, s32 slot) {
  struct BattleUnit *unit = UnitGet(owner);
  u16 item = unit->inventory[slot];
  s32 result = -1;

  if (item != 0) {
    if (item & INVENTORY_QUANTITY_MASK) {
      unit->inventory[slot] = item - INVENTORY_QUANTITY_STEP;
      result = 1;
    } else {
      unit->inventory[slot] = 0;
      Inventory_CompactSlots(unit->inventory);
      result = 2;
    }
  }
  UnitCalc(owner);
  return result;
}

/// Removes one item as Inventory_Remove does, and puts a rare item back into
/// circulation (its counter goes up by one) for the shops to sell.
s32 Inventory_Discard(s32 owner, s32 slot) {
  s32 item = UnitGet(owner)->inventory[slot];
  s32 removalResult = Inventory_Remove(owner, slot);

  if (removalResult != -1) {
    Item_AdjustCounter(item, 1);
    Event_ClearInvalidPackedValuesFar();
  }
  return removalResult;
}

/// Returns whether the item in `slot` may be dropped: 0 yes, -1 the slot is
/// empty, -4 the item is indispensable, -3 it is equipped and cursed on.
s32 Inventory_CheckDiscard(s32 owner, s32 slot) {
  struct BattleUnit *unit = UnitGet(owner);
  s32 itemId = unit->inventory[slot] & ITEM_ID_MASK;
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

/// Removes one `itemId` from whoever in the party carries it.
s32 PartyInventory_Remove(s32 itemId) {
  s32 owner = PartyInventory_FindOwner(itemId);

  if (owner == -1)
    return 0;
  Inventory_Remove(owner, Inventory_Find(owner, itemId));
  return 0;
}

/// Discards one `itemId` from whoever in the party carries it.
s32 PartyInventory_Discard(s32 itemId) {
  s32 owner = PartyInventory_FindOwner(itemId);

  if (owner == -1)
    return 0;
  Inventory_Discard(owner, Inventory_Find(owner, itemId));
  return 0;
}

/// Marks the item in `slot` broken. Returns -1 for an empty slot.
s32 Inventory_Break(s32 owner, s32 slot) {
  struct BattleUnit *inv = UnitGet(owner);
  if (inv->inventory[slot] == 0) {
    return -1;
  }
  inv->inventory[slot] |= INVENTORY_BROKEN;
  return 0;
}

/// Repairs the item in `slot`. Returns -1 for an empty slot.
s32 Inventory_Repair(s32 owner, s32 slot) {
  struct BattleUnit *inv = UnitGet(owner);
  if (inv->inventory[slot] == 0) {
    return -1;
  }
  inv->inventory[slot] &= ~INVENTORY_BROKEN;
  return 0;
}

/// Returns how the action an item performs when used picks its targets.
u8 Item_GetTargetMode(s32 itemId) {
  return BattleAction_GetDirect(Item_GetDirect(itemId)->actionId)->targetMode;
}

/// Adds `delta` to rare item counter `index`, kept between 0 and 99, and
/// returns the new count.
s32 ItemCounter_Adjust(s32 index, s32 delta) {
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

/// Adds `delta` to the counter of a rare item; ordinary items have none.
/// Returns the new count, or 0.
s32 Item_AdjustCounter(s32 itemId, s32 delta) {
  s32 itemIdMask = ITEM_ID_MASK;
  u8 counter;
  s32 result = 0;

  counter = gItem_ArtifactSlotTable[itemId & itemIdMask];
  if (counter != 0) {
    result = ItemCounter_Adjust(counter - 1, delta);
  }
  return result;
}

/// Counts matching nonstackable items, or the quantity in the first matching
/// stack.
s32 Inventory_CountItem(s32 owner, s32 itemId) {
  struct BattleUnit *unit = UnitGet(owner);
  s32 count = 0;
  s32 itemIdOnly = itemId & ITEM_ID_MASK;
  s32 slot = 0;

  do {
    if ((unit->inventory[slot] & ITEM_ID_MASK) == itemIdOnly) {
      struct ItemDefinition *item = Item_GetDirect(itemIdOnly);

      if (item->flags & ITEM_STACKABLE) {
        count = (unit->inventory[slot] >> INVENTORY_QUANTITY_SHIFT) + 1;
        break;
      }
      count++;
    }
    slot++;
  } while (slot < INVENTORY_SLOTS);
  return count;
}

/// Returns how many of `itemId` the party carries.
s32 PartyInventory_CountItem(s32 itemId) {
  u16 owners[16];
  s32 itemCount = 0;
  s32 ownerCount = Party_ListActiveOwners(owners);

  if (itemCount < ownerCount) {
    u16 *ownerCursor = owners;
    s32 remainingOwners = ownerCount;

    do {
      itemCount += Inventory_CountItem(*ownerCursor++, itemId);
      remainingOwners--;
    } while (remainingOwners != 0);
  }
  return itemCount;
}
