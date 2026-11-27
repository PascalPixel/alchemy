/* gmInventoryGetEquippedItem, the same in both games. Both games'
   game/game_inventory.c include it where the function stands. */

/* Returns the id of the unit's equipped item of `type`, or 0. */
Sint32 gmInventoryGetEquippedItem(UNIT *unit, Sint32 type)
{
	Sint32 slot;

	for(slot = 0; slot < INVENTORY_SLOTS; slot++) {
		if(unit->inventory[slot] & INVENTORY_EQUIPPED) {
			const ITEM_DEFINITION *item =
				gmItemGetDirect(unit->inventory[slot]);

			if(item->type == type) {
				return unit->inventory[slot] & INVENTORY_ITEM_ID_MASK;
			}
		}
	}
	return 0;
}
