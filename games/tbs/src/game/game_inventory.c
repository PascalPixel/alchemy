/* Items and inventories: equipping, adding, removing, breaking and counting.
   Part of game.c. */

/* Returns an item's definition; an inventory entry's other bits are ignored. */
const ITEM_DEFINITION *gmItemGetDirect(Sint32 item)
{
	return &ItemDefinitionTable[item & INVENTORY_ITEM_ID_MASK];
}

/* Returns whether the character can equip the item. */
Sint32 gmItemCanUnitEquipDirect(Sint32 unitId, Sint32 itemId)
{
	UNIT				  *unit = gmUnitGet(unitId);
	const ITEM_DEFINITION *item = gmItemGetDirect(itemId);
	Sint32				   equipMask = item->equipMask;

	if(unit->id >= PARTY_UNIT_COUNT)
		return 0;
	return (equipMask >> unit->id) & 1;
}

/* Returns 1 for an item that is not a weapon or armor, and for those whether
   the character can equip it. */
Sint32 gmItemIsCompatibleWithUnit(Sint32 unitId, Sint32 itemId)
{
	if(gmItemGetEquipmentGroup(itemId) == EQUIPMENT_GROUP_NONE)
		return 1;
	return gmItemCanUnitEquipDirect(unitId, itemId);
}

/* Returns what kind of thing an item is, an EQUIPMENT_GROUP. */
Sint32 gmItemGetEquipmentGroup(Sint32 itemId)
{
	const ITEM_DEFINITION *item = gmItemGetDirect(itemId);

	Sint32 group = EQUIPMENT_GROUP_NONE;

	if(item->type == ITEM_TYPE_WEAPON)
		group = EQUIPMENT_GROUP_WEAPON;
	else if(item->type == ITEM_TYPE_ARMOR)
		group = EQUIPMENT_GROUP_ARMOR;
	else if(item->type == ITEM_TYPE_SHIELD)
		group = EQUIPMENT_GROUP_ARMOR;
	else if(item->type == ITEM_TYPE_HELM)
		group = EQUIPMENT_GROUP_ARMOR;
	else if(item->type == ITEM_TYPE_BOOTS)
		group = EQUIPMENT_GROUP_ARMOR;
	else if(item->type == ITEM_TYPE_SHIRT)
		group = EQUIPMENT_GROUP_ARMOR;
	return group;
}

/* Returns how many of the item in `slot` the unit carries (0 for an empty
   slot). The entry stores one less than the count. */
Sint32 gmInventoryGetQuantity(Sint32 unitId, Sint32 slot)
{
	Uint32 entry = gmUnitGet(unitId)->inventory[slot];

	if((entry & INVENTORY_ITEM_ID_MASK) == 0)
		return 0;
	return (entry >> INVENTORY_QUANTITY_SHIFT) + 1;
}

/* Counts filled inventory slots up to the first empty slot; the inventory is
   kept closed up, so this is how many items the unit carries. */
Sint32 gmInventoryCount(Sint32 unitId)
{
	UNIT  *unit = gmUnitGet(unitId);
	Sint32 cnt;

	for(cnt = 0; cnt < INVENTORY_SLOTS; cnt++) {
		if(unit->inventory[cnt] == 0)
			break;
	}
	return cnt;
}

#include "game/party_inventory_space.c"

/* Gives a unit one item, joining its first matching stack (up to
   INVENTORY_STACK_MAX) or taking the first free slot, and returns the slot, or
   -1. A full matching stack fails at once, even if another slot is free. */
Sint32 gmInventoryAddItem(Sint32 unitId, Sint32 itemId)
{
	UNIT				  *unit = gmUnitGet(unitId);
	const ITEM_DEFINITION *item = gmItemGetDirect(itemId);
	Sint32				   slot;

	if(item->flg & ITEM_STACKABLE) {
		for(slot = 0; slot < INVENTORY_SLOTS; slot++) {
			if(((unit->inventory[slot] ^ itemId) & INVENTORY_ITEM_ID_MASK) == 0)
				break;
		}
		if(slot != INVENTORY_SLOTS) {
			Sint32 entry = unit->inventory[slot];
			Uint32 cnt = ((Uint32)entry >> INVENTORY_QUANTITY_SHIFT) + 1;

			if(cnt >= INVENTORY_STACK_MAX)
				return -1;
			{
				Sint32 value = INVENTORY_ENTRY_MASK;

				value &= entry;
				value |= cnt << INVENTORY_QUANTITY_SHIFT;
				unit->inventory[slot] = value;
			}
			return slot;
		}
	}

	for(slot = 0; slot < INVENTORY_SLOTS; slot++) {
		if(unit->inventory[slot] == 0) {
			unit->inventory[slot] = itemId;
			return slot;
		}
	}
	return -1;
}

#include "game/party_inventory_add.c"

/* Returns the first slot holding `itemId` (any quantity, equipped or not), or
   -1. */
Sint32 gmInventoryFind(Sint32 unitId, Sint32 itemId)
{
	UNIT  *unit = gmUnitGet(unitId);
	Sint32 slot;

	for(slot = 0; slot < INVENTORY_SLOTS; slot++) {
		if((unit->inventory[slot] & INVENTORY_ITEM_ID_MASK) == itemId)
			return slot;
	}
	return -1;
}

#include "game/party_inventory_find_member.c"

/* Equips the item in `slot`, clearing the equipped bit of the item of the same
   type it replaces; any number of Psynergy items may be equipped. Recomputes
   the unit's Psynergy and stats. Returns an INVENTORY_EQUIP_RESULT. */
Sint32 gmInventoryEquip(Sint32 unitId, Sint32 slot)
{
	UNIT				  *unit = gmUnitGet(unitId);
	Uint32				   inventoryEntry = unit->inventory[slot];
	const ITEM_DEFINITION *item;
	Uint8				   type;
	Sint32				   equippedSlot;

	if(gmItemCanUnitEquipDirect(unitId, inventoryEntry) == 0)
		return INVENTORY_EQUIP_REFUSED;
	if(inventoryEntry & INVENTORY_EQUIPPED)
		return INVENTORY_EQUIP_DONE;

	item = gmItemGetDirect(inventoryEntry);
	type = item->type;
	if(type != ITEM_TYPE_PSYNERGY) {
		for(equippedSlot = 0; equippedSlot < INVENTORY_SLOTS; equippedSlot++) {
			if((unit->inventory[equippedSlot] & INVENTORY_EQUIPPED) == 0)
				continue;
			if(gmItemGetDirect(unit->inventory[equippedSlot])->type == type)
				break;
		}

		if(equippedSlot != INVENTORY_SLOTS) {
			item = gmItemGetDirect(unit->inventory[equippedSlot]);
			if(item->flg & ITEM_CANNOT_UNEQUIP)
				return INVENTORY_EQUIP_STUCK;
			unit->inventory[equippedSlot] &= (Uint16)~INVENTORY_EQUIPPED;
		}
	}

	unit->inventory[slot] |= INVENTORY_EQUIPPED;
	gmUnitRefreshPsynergy(unitId);
	gmUnitCalc(unitId);
	return INVENTORY_EQUIP_DONE;
}

/* Returns the slot of the equipped item of `type`, or -1. */
Sint32 gmInventoryFindEquipped(Sint32 unitId, Sint32 type)
{
	UNIT				  *unit = gmUnitGet(unitId);
	Sint32				   slot;
	const ITEM_DEFINITION *item;

	for(slot = 0; slot < INVENTORY_SLOTS; slot++) {
		if(unit->inventory[slot] & INVENTORY_EQUIPPED) {
			item = gmItemGetDirect(unit->inventory[slot]);
			if(item->type == type)
				break;
		}
	}
	if(slot == INVENTORY_SLOTS)
		slot = -1;
	return slot;
}

/* Returns the definition of the unit's equipped item of `type`, or NULL.
   Nothing calls it by name. */
const ITEM_DEFINITION *gmInventoryGetEquippedDefinition(UNIT *unit, Sint32 type)
{
	Sint32				   slot;
	const ITEM_DEFINITION *item;

	for(slot = 0; slot < INVENTORY_SLOTS; slot++) {
		if(unit->inventory[slot] & INVENTORY_EQUIPPED) {
			item = gmItemGetDirect(unit->inventory[slot]);
			if(item->type == type) {
				return item;
			}
		}
	}
	return 0;
}

#include "game/inventory_get_equipped_item.c"

/* Takes one item out of slot, closing up the inventory when it was the last.
   The Psynergy list is not rebuilt, so a removed item's Psynergy stays listed
   until something rebuilds it. */
Sint32 gmInventoryRemove(Sint32 unitId, Sint32 slot)
{
	UNIT  *unit = gmUnitGet(unitId);
	Uint16 entry = unit->inventory[slot];
	Sint32 result = INVENTORY_REMOVE_EMPTY_SLOT;

	if(entry != 0) {
		if(entry & INVENTORY_QUANTITY_MASK) {
			unit->inventory[slot] = entry - INVENTORY_QUANTITY_STEP;
			result = INVENTORY_REMOVE_ONE;
		} else {
			unit->inventory[slot] = 0;
			gmInventoryCompactSlots(unit->inventory);
			result = INVENTORY_REMOVE_LAST;
		}
	}
	gmUnitCalc(unitId);
	return result;
}

/* Removes one item as gmInventoryRemove does, and gives the shops' Artifacts
   one more of it to sell, if they keep a stock of it. */
Sint32 gmInventoryDiscard(Sint32 unitId, Sint32 slot)
{
	Sint32 item = gmUnitGet(unitId)->inventory[slot];
	Sint32 removalResult = gmInventoryRemove(unitId, slot);

	if(removalResult != INVENTORY_REMOVE_EMPTY_SLOT) {
		gmItemAdjustRareItemStock(item, 1);
		fdPartyClearUnusableShortcutsStub();
	}
	return removalResult;
}

/* Returns whether the item in `slot` may be dropped or sold, an enum
   INVENTORY_DISCARD_CHECK. */
Sint32 gmInventoryCheckDiscard(Sint32 unitId, Sint32 slot)
{
	UNIT  *unit = gmUnitGet(unitId);
	Sint32 itemId = unit->inventory[slot] & INVENTORY_ITEM_ID_MASK;
	const ITEM_DEFINITION *item = gmItemGetDirect(itemId);

	if(itemId == 0) {
		return INVENTORY_DISCARD_EMPTY_SLOT;
	}
	if((item->flg & ITEM_INDISPENSABLE) != 0) {
		return INVENTORY_DISCARD_INDISPENSABLE;
	}
	if((unit->inventory[slot] & INVENTORY_EQUIPPED) != 0 &&
		(item->flg & ITEM_CANNOT_UNEQUIP) != 0) {
		return INVENTORY_DISCARD_STUCK;
	}
	return INVENTORY_DISCARD_ALLOWED;
}

#include "game/party_inventory_remove.c"

/* Marks the item in `slot` broken; returns -1 for an empty slot. */
Sint32 gmInventoryBreak(Sint32 unitId, Sint32 slot)
{
	UNIT *unit = gmUnitGet(unitId);

	if(unit->inventory[slot] == 0)
		return -1;
	unit->inventory[slot] |= INVENTORY_BROKEN;
	return 0;
}

/* Clears the broken mark of the item in `slot`; returns -1 for an empty
   slot. */
Sint32 gmInventoryRepair(Sint32 unitId, Sint32 slot)
{
	UNIT *unit = gmUnitGet(unitId);

	if(unit->inventory[slot] == 0)
		return -1;
	unit->inventory[slot] &= ~INVENTORY_BROKEN;
	return 0;
}

#include "game/item_get_target_mode.c"

/* Adds `delta` to the shops' stock of rare item `idx`, kept between 0 and
   RARE_ITEM_STOCK_MAX, and returns the new count (0 for an index from
   RARE_ITEM_STOCK_COUNT on). */
Sint32 gmRareItemStockAdjust(Sint32 idx, Sint32 delta)
{
	Uint8 *stock = RareItemStock;
	Sint32 value;
	Sint32 cnt;

	if(idx >= RARE_ITEM_STOCK_COUNT)
		return 0;
	value = stock[idx];

	value += delta;

	if(value < 0) {
		value = 0;
		cnt = 0;
	} else if(value > RARE_ITEM_STOCK_MAX) {
		value = RARE_ITEM_STOCK_MAX;
		cnt = RARE_ITEM_STOCK_MAX;
	} else {
		cnt = value;
	}
	stock[idx] = value;
	return cnt;
}

/* Adds delta to how many of an item the shops' Artifacts offer, and returns the
   new count; 0 for an item they keep no stock of. A delta of 0 reads it. */
Sint32 gmItemAdjustRareItemStock(Sint32 itemId, Sint32 delta)
{
	Sint32 cnt = 0;
	Uint8  counter = ItemRareItemStockIndex[itemId & INVENTORY_ITEM_ID_MASK];

	if(counter != 0)
		cnt = gmRareItemStockAdjust(counter - 1, delta);
	return cnt;
}

/* Counts matching nonstackable items, or the quantity in the first matching
   stack. */
Sint32 gmInventoryCountItem(Sint32 unitId, Sint32 itemId)
{
	UNIT  *unit = gmUnitGet(unitId);
	Sint32 cnt = 0;
	Sint32 slot;

	itemId &= INVENTORY_ITEM_ID_MASK;
	for(slot = 0; slot < INVENTORY_SLOTS; slot++) {
		if((unit->inventory[slot] & INVENTORY_ITEM_ID_MASK) == itemId) {
			if(gmItemGetDirect(itemId)->flg & ITEM_STACKABLE) {
				cnt = (unit->inventory[slot] >> INVENTORY_QUANTITY_SHIFT) + 1;
				break;
			}
			cnt++;
		}
	}
	return cnt;
}

#include "game/party_inventory_count_item.c"
