/* Equipping a given item, and the equipment put aside for the Colosso. Part of
   game.c. */

/* Gives a unit `itemId` and equips every exactly matching inventory entry. */
void gmInventoryAddAndEquip(Sint32 unitId, Sint32 itemId)
{
	UNIT  *unit = gmUnitGet(unitId);
	Sint32 slot;

	gmInventoryAddItem(unitId, itemId);
	for(slot = 0; slot < INVENTORY_SLOTS; slot++) {
		if(unit->inventory[slot] == itemId)
			gmInventoryEquip(unitId, slot);
	}
}

/* The first halfword of InventorySnapshotWork while it holds the party's
   equipment, put aside while the Colosso takes it away. */
#define INVENTORY_SNAPSHOT_SENTINEL 26484

/* What InventorySnapshotWork holds while the Colosso has the party's equipment:
   65 halfwords of the 70 it reserves. */
typedef struct INVENTORY_SNAPSHOT {
	Uint16 sentinel; /* INVENTORY_SNAPSHOT_SENTINEL while it holds them */
	Uint16 inventories[4][INVENTORY_SLOTS]; /* Isaac's to Mia's */
	Uint16 firstShortcut;
	Uint16 secondShortcut;
	Uint8  party[4]; /* the party order */
} INVENTORY_SNAPSHOT;

STATIC_ASSERT(offsetof(INVENTORY_SNAPSHOT, firstShortcut) == 122);
STATIC_ASSERT(offsetof(INVENTORY_SNAPSHOT, party) == 126);

/* Saves Isaac's to Mia's inventories, the shortcuts and the party order, then
   keeps only the Psynergy-granting items, gives Isaac the Short Sword, sets
   FLAG_COLOSSO_HOLDS_EQUIPMENT and restores everyone's HP and PP. Nothing
   happens when a snapshot is already saved, apart from the restore. */
void gmInventorySnapshotSave(void)
{
	Uint16 *save;
	Sint16 *inventory;
	Sint32	unitId;
	Sint32	slot;
	Sint16	firstShortcut;
	Sint16	secondShortcut;

	save = InventorySnapshotWork;
	if(*save != INVENTORY_SNAPSHOT_SENTINEL) {
		*save++ = INVENTORY_SNAPSHOT_SENTINEL;
		firstShortcut = MainRoutineStateWork.firstShortcut;
		secondShortcut = MainRoutineStateWork.secondShortcut;
		/* Save each inventory before retaining its Psynergy-granting items. */
		for(unitId = ISAAC; unitId <= MIA; unitId++) {
			UNIT *unit = gmUnitGet(unitId);

			for(slot = 0; slot < INVENTORY_SLOTS; slot++)
				*save++ = unit->inventory[slot];
			for(slot = 0; slot < INVENTORY_SLOTS; slot++) {
				if(gmItemGetDirect(unit->inventory[slot])->type !=
					ITEM_TYPE_PSYNERGY)
					unit->inventory[slot] = 0;
			}
			inventory = unit->inventory;
			gmInventoryCompactSlots(inventory);
			gmUnitUpdateClass(unitId);
			gmUnitCalculate(unitId);
		}
		*save++ = firstShortcut;
		*save++ = secondShortcut;
		/* UB: the party order's four bytes are read as two halfwords, through a
		   pointer of another type. */
#if FIX_TYPE_PUNNING
		save[0] = MainRoutineStateWork.party[0] |
			(MainRoutineStateWork.party[1] << 8);
		save[1] = MainRoutineStateWork.party[2] |
			(MainRoutineStateWork.party[3] << 8);
#else
		inventory = (Sint16 *)MainRoutineStateWork.party;
		save[0] = inventory[0];
		save[1] = inventory[1];
#endif
		gmInventoryAddAndEquip(ISAAC, ITEM_SHORT_SWORD);
		gmFlagSet(FLAG_COLOSSO_HOLDS_EQUIPMENT);
	}
	gmPartyRestoreHpPp(1);
}

/* Gives back the inventories, shortcuts and party order gmInventorySnapshotSave
   put aside, then clears the snapshot and FLAG_COLOSSO_HOLDS_EQUIPMENT. */
void gmInventorySnapshotRestore(void)
{
	Sint32	unitId;
	Uint16 *src = InventorySnapshotWork;

	if(*src++ == INVENTORY_SNAPSHOT_SENTINEL) {
		for(unitId = ISAAC; unitId <= MIA; unitId++) {
			Uint16 *inventory = gmUnitGet(unitId)->inventory;
			Sint32	slot;

			for(slot = 0; slot < INVENTORY_SLOTS; slot++)
				*inventory++ = *src++;
			gmUnitUpdateClass(unitId);
			gmUnitCalculate(unitId);
		}

		MainRoutineStateWork.firstShortcut = *src++;
		MainRoutineStateWork.secondShortcut = *src++;
		/* UB: the party order's four bytes are written as two halfwords,
		   through a pointer of another type. */
#if FIX_TYPE_PUNNING
		MainRoutineStateWork.party[0] = src[0];
		MainRoutineStateWork.party[1] = src[0] >> 8;
		MainRoutineStateWork.party[2] = src[1];
		MainRoutineStateWork.party[3] = src[1] >> 8;
		src++;
#else
		*(Uint16 *)MainRoutineStateWork.party = *src++;
		*(Uint16 *)&MainRoutineStateWork.party[2] = *src;
#endif
		src = InventorySnapshotWork;
		*src = 0;
		gmFlagClear(FLAG_COLOSSO_HOLDS_EQUIPMENT);
	}
}

#undef INVENTORY_SNAPSHOT_SENTINEL
