// The equipment put aside for the Trial Road, as The Broken Seal's for the Colosso: Felix's,
// Jenna's, Sheba's and Piers's inventories, and giving them back. Part of game.c.

// Added in every edition but the Japanese; the Japanese release has no such function.
#if TRIAL_ROAD_TAKES_STAT_ITEMS
// Returns 1 for the six items that raise a stat for good: Power Bread, Cookie, Apple, Hard Nut,
// Mint and Lucky Pepper (an inventory entry may be passed).
Bool gmItemRaisesStat(Sint32 id)
{
	switch (id & INVENTORY_ITEM_ID_MASK)
	{
		case ITEM_POWER_BREAD:
		case ITEM_COOKIE:
		case ITEM_APPLE:
		case ITEM_HARD_NUT:
		case ITEM_MINT:
		case ITEM_LUCKY_PEPPER: return TRUE;
	}
	return FALSE;
}
#endif

// The first halfword of InventorySnapshot while it holds the party's equipment, put aside while the
// Trial Road takes it away.
#define INVENTORY_SNAPSHOT_SENTINEL 26484

// Saves the inventories of Felix's party (FELIX to PIERS), the shortcuts and the party order as the
// leader and Moapa set off on the Trial Road (scShamanVillageTakeLeftPath and its twin), then drops
// every weapon, armor, shield and helm not equipped (and, in every edition but the Japanese, every
// item that raises a stat), sets FLAG_TRIAL_ROAD_HOLDS_EQUIPMENT and restores everyone's HP and PP.
// As The Broken Seal's Colosso snapshot; nothing is saved again while a snapshot is held.
void gmInventorySnapshotSave(void)
{
	Uint16 *save;
	UNIT   *unit;
	Sint16 *inventory;
	Sint32  unitId;
	Sint32  slot;
	Sint16  firstShortcut;
	Sint16  secondShortcut;

	save = (Uint16 *)InventorySnapshot;
	if (*save != INVENTORY_SNAPSHOT_SENTINEL)
	{
		*save++        = INVENTORY_SNAPSHOT_SENTINEL;
		firstShortcut  = MainRoutineStateWork.firstShortcut;
		secondShortcut = MainRoutineStateWork.secondShortcut;
		// Save each inventory before dropping what the Trial Road takes away.
		for (unitId = FELIX; unitId <= PIERS; unitId++)
		{
			unit = syUnitGet(unitId);
			for (slot = 0; slot < INVENTORY_SLOTS; slot++)
				*save++ = unit->inventory[slot];
			for (slot = 0; slot < INVENTORY_SLOTS; slot++)
			{
				ITEM_DEFINITION *item = gmItemGetDirect(unit->inventory[slot]);

				// ITEM_TYPE_WEAPON to ITEM_TYPE_HELM, as one unsigned test: boots, shirts, rings
				// and the rest are kept.
				if ((Uint8)(item->type - ITEM_TYPE_WEAPON) <= ITEM_TYPE_HELM - ITEM_TYPE_WEAPON
				    && (unit->inventory[slot] & INVENTORY_EQUIPPED) == 0)
					unit->inventory[slot] = 0;
#if TRIAL_ROAD_TAKES_STAT_ITEMS
				// Added in every edition but the Japanese; the Japanese release has no such check.
				if (gmItemRaisesStat(unit->inventory[slot]))
					unit->inventory[slot] = 0;
#endif
			}
			inventory = (Sint16 *)unit->inventory;
			gmInventoryCompactSlots(inventory);
			gmUnitUpdateClass(unitId);
			gmUnitCalculate(unitId);
		}
		// Keep shortcuts and party order with the inventory snapshot.
		*save++   = firstShortcut;
		*save++   = secondShortcut;
		inventory = (Sint16 *)MainRoutineStateWork.party;
		save[0]   = inventory[0];
		save[1]   = inventory[1];
	}
	syGameFlagSet(FLAG_TRIAL_ROAD_HOLDS_EQUIPMENT);
	gmPartyRestoreHpPp(1);
	// `slot` counts the Trial Road's stage item flags here.
	for (slot = FLAG_TRIAL_ROAD_STAGE_ITEM_FIRST; slot <= FLAG_TRIAL_ROAD_STAGE_ITEM_LAST; slot++)
		syGameFlagClear(slot);
}

// Gives back the inventories, shortcuts and party order gmInventorySnapshotSave put aside, empties
// the snapshot and clears FLAG_TRIAL_ROAD_HOLDS_EQUIPMENT: at the end of the Trial Road, and from
// the clear scene.
void gmInventorySnapshotRestore(void)
{
	Uint16 *src = (Uint16 *)InventorySnapshot;
	Sint32  unitId;

	if (*src++ == INVENTORY_SNAPSHOT_SENTINEL)
	{
		for (unitId = FELIX; unitId <= PIERS; unitId++)
		{
			Uint16 *dest = syUnitGet(unitId)->inventory;
			Sint32  slot;

			for (slot = 0; slot < INVENTORY_SLOTS; slot++)
				*dest++ = *src++;
			gmUnitUpdateClass(unitId);
			gmUnitCalculate(unitId);
		}
		MainRoutineStateWork.firstShortcut  = *src++;
		MainRoutineStateWork.secondShortcut = *src++;
#if FIX_TYPE_PUNNING
		MainRoutineStateWork.party[0] = src[0];
		MainRoutineStateWork.party[1] = src[0] >> 8;
		MainRoutineStateWork.party[2] = src[1];
		MainRoutineStateWork.party[3] = src[1] >> 8;
#else
		{
			// UB: the party order's four bytes are stored as two halfwords.
			Uint16 *owners = (Uint16 *)MainRoutineStateWork.party;

			*owners++      = *src++;
			*owners        = *src;
		}
#endif
		src  = (Uint16 *)InventorySnapshot;
		*src = 0;
	}
	syGameFlagClear(FLAG_TRIAL_ROAD_HOLDS_EQUIPMENT);
}
