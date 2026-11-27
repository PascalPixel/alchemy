// Items and inventories: equipping, adding, removing, breaking and counting. Part of game.c.

// Returns an item's definition; an inventory entry's other bits are ignored.
ITEM_DEFINITION *gmItemGetDirect(Sint32 item)
{
    return &ItemDefinitionTable[item & INVENTORY_ITEM_ID_MASK];
}

// Returns whether the character can equip the item: the bit of its equipMask for who the unit is,
// and 0 for anyone past PIERS.
Sint32 gmItemCanUnitEquipDirect(Sint32 unitId, Sint32 itemId)
{
    UNIT            *unit      = syUnitGet(unitId);
    ITEM_DEFINITION *item      = gmItemGetDirect(itemId);
    Sint32           equipMask = item->equipMask;

    if (unit->enemyId > PIERS)
        return 0;
    return (equipMask >> unit->enemyId) & 1;
}

// Returns 1 for an item that is not a weapon or armor, and for those whether the character can
// equip it.
Sint32 gmItemIsCompatibleWithUnit(Sint32 unit, Sint32 item)
{
    if (gmItemGetEquipmentGroup(item) == EQUIPMENT_GROUP_NONE)
        return 1;
    return gmItemCanUnitEquipDirect(unit, item);
}

// Returns an item's equipment group from its type: weapons, tridents and the class-changing items
// count as weapons, armor, shields, helms, boots and shirts as armor, and anything else as neither.
Sint32 gmItemGetEquipmentGroup(Sint32 itemId)
{
    Sint32 type = gmItemGetDirect(itemId)->type;
    Sint32 group;

    if (type == ITEM_TYPE_WEAPON)
        group = EQUIPMENT_GROUP_WEAPON;
    else if (type == ITEM_TYPE_ARMOR)
        group = EQUIPMENT_GROUP_ARMOR;
    else if (type == ITEM_TYPE_SHIELD)
        group = EQUIPMENT_GROUP_ARMOR;
    else if (type == ITEM_TYPE_HELM)
        group = EQUIPMENT_GROUP_ARMOR;
    else if (type == ITEM_TYPE_BOOTS)
        group = EQUIPMENT_GROUP_ARMOR;
    else if (type == ITEM_TYPE_SHIRT)
        group = EQUIPMENT_GROUP_ARMOR;
    else if (type == ITEM_TYPE_TRIDENT)
        group = EQUIPMENT_GROUP_WEAPON;
    else
        // A class-changing item counts as a weapon: EQUIPMENT_GROUP_WEAPON is 1.
        group = type == ITEM_TYPE_CLASS_CHANGE;
    return group;
}

// Returns how many of the item in `slot` the unit carries (0 for an empty slot). The entry stores
// one less than the count.
Sint32 gmInventoryGetQuantity(Sint32 unitId, Sint32 slot)
{
    Uint32 entry = syUnitGet(unitId)->inventory[slot];

    if ((entry & INVENTORY_ITEM_ID_MASK) == 0)
        return 0;
    return (entry >> INVENTORY_QUANTITY_SHIFT) + 1;
}

// Counts filled inventory slots up to the first empty slot; the inventory is kept closed up, so
// this is how many items the unit carries.
Sint32 gmInventoryCount(Sint32 unitId)
{
    UNIT  *unit = syUnitGet(unitId);
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
// first free slot. A full matching stack fails immediately, even if another slot is free.
Sint32 gmInventoryAddItem(Sint32 unitId, Sint32 itemId)
{
    UNIT                  *unit = syUnitGet(unitId);
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

// Returns the first party member with room for `itemId`, found by giving it and taking it back, or
// -1.
Sint32 gmPartyInventoryFindRoom(Sint32 itemId)
{
    Sint16 owners[10];
    Sint32 ownerCount;
    Sint32 ownerIndex;

    ownerCount = gmPartyListMembers(owners);
    for (ownerIndex = 0; ownerIndex < ownerCount; ownerIndex++)
    {
        Sint16 ownerId = owners[ownerIndex];
        Sint32 slot    = gmInventoryAddItem(ownerId, itemId);

        if (slot >= 0)
        {
            gmInventoryRemove(ownerId, slot);
            return ownerId;
        }
    }
    return -1;
}

// Returns the first slot holding `itemId` (any quantity, equipped or not), or -1.
Sint32 gmInventoryFind(Sint32 unitId, Sint32 itemId)
{
    UNIT  *unit = syUnitGet(unitId);
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

// Equips the item in `slot`, clearing the equipped bit of the item of the same type it replaces.
// Psynergy items may coexist. Recomputes the unit's Psynergy and stats. Returns 0, -1 when the unit
// cannot equip it, or -2 when the item it would replace is cursed on.
Sint32 gmInventoryEquip(Sint32 unitId, Sint32 slot)
{
    UNIT            *unit           = syUnitGet(unitId);
    Uint32           inventoryEntry = unit->inventory[slot];
    ITEM_DEFINITION *item;
    Uint8            type;
    Sint32           equippedSlot;

    if (gmItemCanUnitEquipDirect(unitId, inventoryEntry) == 0)
        return -1;
    if (inventoryEntry & INVENTORY_EQUIPPED)
        return 0;

    item = gmItemGetDirect(inventoryEntry);
    type = item->type;
    if (type != ITEM_TYPE_PSYNERGY && type != ITEM_TYPE_ELEMENTAL_STAR)
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
                return -2;
            unit->inventory[equippedSlot] &= (Uint16)~INVENTORY_EQUIPPED;
        }
    }

    unit->inventory[slot] |= INVENTORY_EQUIPPED;
    gmUnitRefreshPsynergy(unitId);
    gmUnitCalculate(unitId);
    return 0;
}

// Returns the slot of the equipped item of `type`, or -1.
Sint32 gmInventoryFindEquipped(Sint32 unitId, Sint32 type)
{
    UNIT                  *base = syUnitGet(unitId);
    Sint32                 index;
    const ITEM_DEFINITION *item;

    for (index = 0; index < INVENTORY_SLOTS; index++)
    {
        if (base->inventory[index] & INVENTORY_EQUIPPED)
        {
            item = gmItemGetDirect(base->inventory[index]);
            if (item->type == type)
                break;
        }
    }
    if (index == INVENTORY_SLOTS)
        index = -1;
    return index;
}

// Returns the definition of the unit's equipped item of `type`, or NULL.
ITEM_DEFINITION *gmInventoryGetEquippedDefinition(UNIT *unit, Sint32 type)
{
    Sint32           slot;
    ITEM_DEFINITION *item;

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

// Empties an inventory slot outright, closes the inventory up and recalculates the unit; returns
// INVENTORY_REMOVE_LAST, as gmInventoryRemove does for a last item.
Sint32 gmInventoryClearSlot(Sint32 unitId, Sint32 slot)
{
    UNIT *unit            = syUnitGet(unitId);

    unit->inventory[slot] = 0;
    gmInventoryCompactSlots((Sint16 *)unit->inventory);
    gmUnitCalculate(unitId);
    return INVENTORY_REMOVE_LAST;
}

// Takes one item out of `slot`, closing up the inventory when it was the last. The Psynergy list is
// not rebuilt, so a removed item's Psynergy stays listed until something rebuilds it.
Sint32 gmInventoryRemove(Sint32 unitId, Sint32 slot)
{
    UNIT  *unit   = syUnitGet(unitId);
    Uint16 item   = unit->inventory[slot];
    Sint32 result = INVENTORY_REMOVE_EMPTY_SLOT;

    if (item != 0)
    {
        if (item & INVENTORY_QUANTITY_MASK)
        {
            unit->inventory[slot] = item - INVENTORY_QUANTITY_STEP;
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

// Removes one item as gmInventoryRemove does, and puts a rare item back into circulation (its
// counter goes up by one) for the shops to sell.
Sint32 gmInventoryDiscard(Sint32 unitId, Sint32 slot)
{
    Sint32 item          = syUnitGet(unitId)->inventory[slot];
    Sint32 removalResult = gmInventoryRemove(unitId, slot);

    if (removalResult != INVENTORY_REMOVE_EMPTY_SLOT)
    {
        gmItemAdjustRareItemStock(item, 1);
        fdPartyClearUnusableShortcutsStub();
    }
    return removalResult;
}

// Returns 0 when the item in `slot` may be taken off, or else why not: it is equipped and cannot be
// (INVENTORY_REFUSE_CANNOT_UNEQUIP), or the slot is empty.
Sint32 gmInventoryCheckUnequip(Sint32 unitId, Sint32 slot)
{
    UNIT            *unit   = syUnitGet(unitId);
    Sint32           itemId = unit->inventory[slot] & INVENTORY_ITEM_ID_MASK;
    ITEM_DEFINITION *item   = gmItemGetDirect(itemId);
    Sint32           result = 0;

    if (itemId == 0)
        return INVENTORY_REFUSE_EMPTY;
    if ((unit->inventory[slot] & INVENTORY_EQUIPPED) != 0
        && (item->flags & ITEM_CANNOT_UNEQUIP) != 0)
        result = INVENTORY_REFUSE_CANNOT_UNEQUIP;
    return result;
}

// Returns 0 when the item in `slot` may be dropped, or else why not, as INVENTORY_REFUSAL bits: the
// item is indispensable, it is equipped and cannot be taken off, or the slot is empty.
Sint32 gmInventoryCheckDiscard(Sint32 unitId, Sint32 slot)
{
    UNIT            *unit   = syUnitGet(unitId);
    Sint32           itemId = unit->inventory[slot] & INVENTORY_ITEM_ID_MASK;
    ITEM_DEFINITION *item   = gmItemGetDirect(itemId);
    Sint32           result = 0;

    if (itemId == 0)
        return INVENTORY_REFUSE_EMPTY;
    if ((item->flags & ITEM_INDISPENSABLE) != 0)
        result = INVENTORY_REFUSE_INDISPENSABLE;
    if ((unit->inventory[slot] & INVENTORY_EQUIPPED) != 0
        && (item->flags & ITEM_CANNOT_UNEQUIP) != 0)
        result |= INVENTORY_REFUSE_CANNOT_UNEQUIP;
    return result;
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
    UNIT *unit = syUnitGet(unitId);

    if (unit->inventory[slot] == 0)
        return -1;
    unit->inventory[slot] |= INVENTORY_BROKEN;
    return 0;
}

// Clears the broken mark of the item in `slot`; returns -1 for an empty slot.
Sint32 gmInventoryRepair(Sint32 unitId, Sint32 slot)
{
    UNIT *unit = syUnitGet(unitId);

    if (unit->inventory[slot] == 0)
        return -1;
    unit->inventory[slot] &= ~INVENTORY_BROKEN;
    return 0;
}

// Returns how the action an item performs when used picks its targets. Nothing calls it or its
// stub.
Uint8 gmItemGetTargetMode(Sint32 itemId)
{
    return gmAbilityGetDirect(gmItemGetDirect(itemId)->abilityId)->targetMode;
}

// Adds `delta` to rare item counter `index`, kept between 0 and ITEM_COUNTER_MAX, and returns the
// new count (0 for an index from ITEM_COUNTER_COUNT on).
Sint32 gmRareItemStockAdjust(Sint32 index, Sint32 delta)
{
    Uint8 *stock = RareItemStock;
    Sint32 value;
    Sint32 count;

    if (index >= ITEM_COUNTER_COUNT)
        return 0;
    value  = stock[index];

    value += delta;

    if (value < 0)
    {
        value = 0;
        count = 0;
    }
    else if (value > ITEM_COUNTER_MAX)
    {
        value = ITEM_COUNTER_MAX;
        count = ITEM_COUNTER_MAX;
    }
    else
    {
        count = value;
    }
    stock[index] = value;
    return count;
}

// Adds `delta` to how many of a rare item the shops may sell, and returns the new count; ordinary
// items have none.
Sint32 gmItemAdjustRareItemStock(Sint32 itemId, Sint32 delta)
{
    Sint32 result;
    Uint16 counter;

    itemId  &= INVENTORY_ITEM_ID_MASK;
    // The table holds a rare item's counter number plus one, and 0 for an ordinary item.
    counter  = ItemRareItemCounters[itemId];
    result   = 0;

    if (counter)
        result = gmRareItemStockAdjust(counter - 1, delta);
    return result;
}

// Counts matching nonstackable items, or the quantity in the first matching stack.
Sint32 gmInventoryCountItem(Sint32 unitId, Sint32 itemId)
{
    UNIT  *unit  = syUnitGet(unitId);
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
