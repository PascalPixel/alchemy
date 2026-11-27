// Items and inventories: equipping, adding, removing, breaking and counting. Part of game.c.

// Returns an item's definition; an inventory entry's other bits are ignored.
const ITEM_DEFINITION *gmItemGetDirect(Sint32 item)
{
	return &ItemDefinitionTable[item & INVENTORY_ITEM_ID_MASK];
}

// Returns whether the character can equip the item.
Sint32 gmItemCanUnitEquipDirect(Sint32 unitId, Sint32 itemId)
{
	UNIT                  *unit      = gmUnitGet(unitId);
	const ITEM_DEFINITION *item      = gmItemGetDirect(itemId);
	Sint32                 equipMask = item->equipMask;

	if (unit->id >= PARTY_UNIT_COUNT)
		return 0;
	return (equipMask >> unit->id) & 1;
}

// Returns 1 for an item that is not a weapon or armor, and for those whether the character can
// equip it.
Sint32 gmItemIsCompatibleWithUnit(Sint32 unitId, Sint32 itemId)
{
	if (gmItemGetEquipmentGroup(itemId) == EQUIPMENT_GROUP_NONE)
		return 1;
	return gmItemCanUnitEquipDirect(unitId, itemId);
}

// Returns what kind of thing an item is, an EQUIPMENT_GROUP.
Sint32 gmItemGetEquipmentGroup(Sint32 itemId)
{
	const ITEM_DEFINITION *item  = gmItemGetDirect(itemId);

	Sint32                 group = EQUIPMENT_GROUP_NONE;

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

// Returns how many of the item in `slot` the unit carries (0 for an empty slot). The entry stores
// one less than the count.
Sint32 gmInventoryGetQuantity(Sint32 unitId, Sint32 slot)
{
	Uint32 entry = gmUnitGet(unitId)->inventory[slot];

	if ((entry & INVENTORY_ITEM_ID_MASK) == 0)
		return 0;
	return (entry >> INVENTORY_QUANTITY_SHIFT) + 1;
}

// Counts filled inventory slots up to the first empty slot; the inventory is kept closed up, so
// this is how many items the unit carries.
Sint32 gmInventoryCount(Sint32 unitId)
{
	UNIT  *unit = gmUnitGet(unitId);
	Sint32 count;

	for (count = 0; count < INVENTORY_SLOTS; count++)
	{
		if (unit->inventory[count] == 0)
			break;
	}
	return count;
}

// Returns 1 when the leader or any party member has a free inventory slot.
Bool gmPartyInventoryHasSpace(void)
{
	Sint16  unitIds[10];
	Sint32  unitCount;
	Sint16 *unitId;
	Sint32  i;

	if (gmInventoryCount(MainRoutineStateWork.leaderActorId) != INVENTORY_SLOTS)
		return TRUE;
	unitCount = gmPartyListMembers(unitIds);
	unitId    = unitIds;
	for (i = 0; i < unitCount; i++)
	{
		if (gmInventoryCount(*unitId++) != INVENTORY_SLOTS)
			return TRUE;
	}
	return FALSE;
}

// Returns the free inventory slots of the whole party.
Sint32 gmPartyInventoryCountFreeSlots(void)
{
	Sint16  unitIds[10];
	Sint32  unitCount = gmPartyListMembers(unitIds);
	Sint32  freeSlots = 0;
	Sint16 *unitId    = unitIds;
	Sint32  i;

	for (i = 0; i < unitCount; i++)
		freeSlots = freeSlots - gmInventoryCount(*unitId++) + INVENTORY_SLOTS;
	return freeSlots;
}

// Gives a unit one item, joining its first matching stack (up to INVENTORY_STACK_MAX) or taking the
// first free slot, and returns the slot, or -1. A full matching stack fails at once, even if
// another slot is free.
Sint32 gmInventoryAddItem(Sint32 unitId, Sint32 itemId)
{
	UNIT                  *unit = gmUnitGet(unitId);
	const ITEM_DEFINITION *item = gmItemGetDirect(itemId);
	Sint32                 slot;

	if (item->flags & ITEM_STACKABLE)
	{
		for (slot = 0; slot < INVENTORY_SLOTS; slot++)
		{
			if (((unit->inventory[slot] ^ itemId) & INVENTORY_ITEM_ID_MASK) == 0)
				break;
		}
		if (slot != INVENTORY_SLOTS)
		{
			Sint32 entry = unit->inventory[slot];
			Uint32 count = ((Uint32)entry >> INVENTORY_QUANTITY_SHIFT) + 1;

			if (count >= INVENTORY_STACK_MAX)
				return -1;
			{
				Sint32 value           = INVENTORY_ENTRY_MASK;

				value                 &= entry;
				value                 |= count << INVENTORY_QUANTITY_SHIFT;
				unit->inventory[slot]  = value;
			}
			return slot;
		}
	}

	for (slot = 0; slot < INVENTORY_SLOTS; slot++)
	{
		if (unit->inventory[slot] == 0)
		{
			unit->inventory[slot] = itemId;
			return slot;
		}
	}
	return -1;
}

// Gives the item to the first party member with room. Returns who took it, or -1.
Sint32 gmPartyInventoryAdd(Sint32 itemId)
{
	Sint16  unitIds[10];
	Sint32  unitCount = gmPartyListMembers(unitIds);
	Sint16 *cursor    = unitIds;
	Sint32  i;

	for (i = 0; i < unitCount; i++)
	{
		Sint16 unitId = *cursor++;

		if (gmInventoryAddItem(unitId, itemId) >= 0)
			return unitId;
	}
	return -1;
}

// Returns the first slot holding `itemId` (any quantity, equipped or not), or -1.
Sint32 gmInventoryFind(Sint32 unitId, Sint32 itemId)
{
	UNIT  *unit = gmUnitGet(unitId);
	Sint32 slot;

	for (slot = 0; slot < INVENTORY_SLOTS; slot++)
	{
		if ((unit->inventory[slot] & INVENTORY_ITEM_ID_MASK) == itemId)
			return slot;
	}
	return -1;
}

// Returns who carries `itemId`, asking the leader first, or -1.
Sint32 gmPartyInventoryFindMember(Sint32 itemId)
{
	Sint16  unitIds[10];
	Sint32  unitCount;
	Sint16 *cursor;
	Sint32  i;

	if (gmInventoryFind(MainRoutineStateWork.leaderActorId, itemId) != -1)
		return MainRoutineStateWork.leaderActorId;
	unitCount = gmPartyListMembers(unitIds);
	cursor    = unitIds;
	for (i = 0; i < unitCount; i++)
	{
		Sint16 unitId = *cursor++;

		if (gmInventoryFind(unitId, itemId) != -1)
			return unitId;
	}
	return -1;
}

// Equips the item in `slot`, clearing the equipped bit of the item of the same type it replaces;
// any number of Psynergy items may be equipped. Recomputes the unit's Psynergy and stats. Returns
// an INVENTORY_EQUIP_RESULT.
Sint32 gmInventoryEquip(Sint32 unitId, Sint32 slot)
{
	UNIT                  *unit           = gmUnitGet(unitId);
	Uint32                 inventoryEntry = unit->inventory[slot];
	const ITEM_DEFINITION *item;
	Uint8                  type;
	Sint32                 equippedSlot;

	if (gmItemCanUnitEquipDirect(unitId, inventoryEntry) == 0)
		return INVENTORY_EQUIP_REFUSED;
	if (inventoryEntry & INVENTORY_EQUIPPED)
		return INVENTORY_EQUIP_DONE;

	item = gmItemGetDirect(inventoryEntry);
	type = item->type;
	if (type != ITEM_TYPE_PSYNERGY)
	{
		for (equippedSlot = 0; equippedSlot < INVENTORY_SLOTS; equippedSlot++)
		{
			if ((unit->inventory[equippedSlot] & INVENTORY_EQUIPPED) == 0)
				continue;
			if (gmItemGetDirect(unit->inventory[equippedSlot])->type == type)
				break;
		}

		if (equippedSlot != INVENTORY_SLOTS)
		{
			item = gmItemGetDirect(unit->inventory[equippedSlot]);
			if (item->flags & ITEM_CANNOT_UNEQUIP)
				return INVENTORY_EQUIP_STUCK;
			unit->inventory[equippedSlot] &= (Uint16)~INVENTORY_EQUIPPED;
		}
	}

	unit->inventory[slot] |= INVENTORY_EQUIPPED;
	gmUnitRefreshPsynergy(unitId);
	gmUnitCalculate(unitId);
	return INVENTORY_EQUIP_DONE;
}

// Returns the slot of the equipped item of `type`, or -1.
Sint32 gmInventoryFindEquipped(Sint32 unitId, Sint32 type)
{
	UNIT                  *unit = gmUnitGet(unitId);
	Sint32                 slot;
	const ITEM_DEFINITION *item;

	for (slot = 0; slot < INVENTORY_SLOTS; slot++)
	{
		if (unit->inventory[slot] & INVENTORY_EQUIPPED)
		{
			item = gmItemGetDirect(unit->inventory[slot]);
			if (item->type == type)
				break;
		}
	}
	if (slot == INVENTORY_SLOTS)
		slot = -1;
	return slot;
}

// Returns the definition of the unit's equipped item of `type`, or NULL. Nothing calls it by name.
const ITEM_DEFINITION *gmInventoryGetEquippedDefinition(UNIT *unit, Sint32 type)
{
	Sint32                 slot;
	const ITEM_DEFINITION *item;

	for (slot = 0; slot < INVENTORY_SLOTS; slot++)
	{
		if (unit->inventory[slot] & INVENTORY_EQUIPPED)
		{
			item = gmItemGetDirect(unit->inventory[slot]);
			if (item->type == type)
			{
				return item;
			}
		}
	}
	return 0;
}

// Returns the id of the unit's equipped item of `type`, or 0.
Sint32 gmInventoryGetEquippedItem(UNIT *unit, Sint32 type)
{
	Sint32 slot;

	for (slot = 0; slot < INVENTORY_SLOTS; slot++)
	{
		if (unit->inventory[slot] & INVENTORY_EQUIPPED)
		{
			const ITEM_DEFINITION *item = gmItemGetDirect(unit->inventory[slot]);

			if (item->type == type)
			{
				return unit->inventory[slot] & INVENTORY_ITEM_ID_MASK;
			}
		}
	}
	return 0;
}

// Takes one item out of slot, closing up the inventory when it was the last. The Psynergy list is
// not rebuilt, so a removed item's Psynergy stays listed until something rebuilds it.
Sint32 gmInventoryRemove(Sint32 unitId, Sint32 slot)
{
	UNIT  *unit   = gmUnitGet(unitId);
	Uint16 entry  = unit->inventory[slot];
	Sint32 result = INVENTORY_REMOVE_EMPTY_SLOT;

	if (entry != 0)
	{
		if (entry & INVENTORY_QUANTITY_MASK)
		{
			unit->inventory[slot] = entry - INVENTORY_QUANTITY_STEP;
			result                = INVENTORY_REMOVE_ONE;
		}
		else
		{
			unit->inventory[slot] = 0;
			gmInventoryCompactSlots(unit->inventory);
			result = INVENTORY_REMOVE_LAST;
		}
	}
	gmUnitCalculate(unitId);
	return result;
}

// Removes one item as gmInventoryRemove does, and gives the shops' Artifacts one more of it to
// sell, if they keep a stock of it.
Sint32 gmInventoryDiscard(Sint32 unitId, Sint32 slot)
{
	Sint32 item          = gmUnitGet(unitId)->inventory[slot];
	Sint32 removalResult = gmInventoryRemove(unitId, slot);

	if (removalResult != INVENTORY_REMOVE_EMPTY_SLOT)
	{
		gmItemAdjustRareItemStock(item, 1);
		fdPartyClearUnusableShortcutsStub();
	}
	return removalResult;
}

// Returns whether the item in `slot` may be dropped or sold, an enum INVENTORY_DISCARD_CHECK.
Sint32 gmInventoryCheckDiscard(Sint32 unitId, Sint32 slot)
{
	UNIT                  *unit   = gmUnitGet(unitId);
	Sint32                 itemId = unit->inventory[slot] & INVENTORY_ITEM_ID_MASK;
	const ITEM_DEFINITION *item   = gmItemGetDirect(itemId);

	if (itemId == 0)
	{
		return INVENTORY_DISCARD_EMPTY_SLOT;
	}
	if ((item->flags & ITEM_INDISPENSABLE) != 0)
	{
		return INVENTORY_DISCARD_INDISPENSABLE;
	}
	if ((unit->inventory[slot] & INVENTORY_EQUIPPED) != 0
	    && (item->flags & ITEM_CANNOT_UNEQUIP) != 0)
	{
		return INVENTORY_DISCARD_STUCK;
	}
	return INVENTORY_DISCARD_ALLOWED;
}

// Removes one `itemId` from whoever carries it first; always returns 0.
Sint32 gmPartyInventoryRemove(Sint32 itemId)
{
	Sint32 unitId = gmPartyInventoryFindMember(itemId);

	if (unitId == -1)
		return 0;
	gmInventoryRemove(unitId, gmInventoryFind(unitId, itemId));
	return 0;
}

// Discards one `itemId` from whoever carries it first, as gmInventoryDiscard does; always returns
// 0.
Sint32 gmPartyInventoryDiscard(Sint32 itemId)
{
	Sint32 unitId = gmPartyInventoryFindMember(itemId);

	if (unitId == -1)
		return 0;
	gmInventoryDiscard(unitId, gmInventoryFind(unitId, itemId));
	return 0;
}

// Marks the item in `slot` broken; returns -1 for an empty slot.
Sint32 gmInventoryBreak(Sint32 unitId, Sint32 slot)
{
	UNIT *unit = gmUnitGet(unitId);

	if (unit->inventory[slot] == 0)
		return -1;
	unit->inventory[slot] |= INVENTORY_BROKEN;
	return 0;
}

// Clears the broken mark of the item in `slot`; returns -1 for an empty slot.
Sint32 gmInventoryRepair(Sint32 unitId, Sint32 slot)
{
	UNIT *unit = gmUnitGet(unitId);

	if (unit->inventory[slot] == 0)
		return -1;
	unit->inventory[slot] &= ~INVENTORY_BROKEN;
	return 0;
}

// Returns how the ability an item uses picks its targets. Only game.s's bare stub reaches it;
// nothing calls it by name.
Uint8 gmItemGetTargetMode(Sint32 itemId)
{
	return gmAbilityGetDirect(gmItemGetDirect(itemId)->abilityId)->targetMode;
}

// Adds `delta` to the shops' stock of rare item `index`, kept between 0 and RARE_ITEM_STOCK_MAX,
// and returns the new count (0 for an index from RARE_ITEM_STOCK_COUNT on).
Sint32 gmRareItemStockAdjust(Sint32 index, Sint32 delta)
{
	Uint8 *stock = RareItemStock;
	Sint32 value;
	Sint32 count;

	if (index >= RARE_ITEM_STOCK_COUNT)
		return 0;
	value  = stock[index];

	value += delta;

	if (value < 0)
	{
		value = 0;
		count = 0;
	}
	else if (value > RARE_ITEM_STOCK_MAX)
	{
		value = RARE_ITEM_STOCK_MAX;
		count = RARE_ITEM_STOCK_MAX;
	}
	else
	{
		count = value;
	}
	stock[index] = value;
	return count;
}

// Adds delta to how many of an item the shops' Artifacts offer, and returns the new count; 0 for an
// item they keep no stock of. A delta of 0 reads it.
Sint32 gmItemAdjustRareItemStock(Sint32 itemId, Sint32 delta)
{
	Sint32 count   = 0;
	Uint8  counter = ItemRareItemStockIndex[itemId & INVENTORY_ITEM_ID_MASK];

	if (counter != 0)
		count = gmRareItemStockAdjust(counter - 1, delta);
	return count;
}

// Counts matching nonstackable items, or the quantity in the first matching stack.
Sint32 gmInventoryCountItem(Sint32 unitId, Sint32 itemId)
{
	UNIT  *unit  = gmUnitGet(unitId);
	Sint32 count = 0;
	Sint32 slot;

	itemId &= INVENTORY_ITEM_ID_MASK;
	for (slot = 0; slot < INVENTORY_SLOTS; slot++)
	{
		if ((unit->inventory[slot] & INVENTORY_ITEM_ID_MASK) == itemId)
		{
			if (gmItemGetDirect(itemId)->flags & ITEM_STACKABLE)
			{
				count = (unit->inventory[slot] >> INVENTORY_QUANTITY_SHIFT) + 1;
				break;
			}
			count++;
		}
	}
	return count;
}

// Counts `itemId` across the party.
Sint32 gmPartyInventoryCountItem(Sint32 itemId)
{
	Uint16 unitIds[16];
	Sint32 count     = 0;
	Sint32 unitCount = gmPartyListMembers(unitIds);
	Sint32 i;

	for (i = 0; i < unitCount; i++)
		count += gmInventoryCountItem(unitIds[i], itemId);
	return count;
}
