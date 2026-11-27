/* mnItemIconLoadIntoSlot and mnAbilityIconLoadIntoSlot, the same in both
   games. Both games' window/screens_menu_titles.c include it where the
   functions stand. */

/* Builds the icon of inventory item `resource` (the count above one and both
   marks) in a transfer block taken from HEAP_SLOT_ICON_TRANSFER, reloads VRAM
   cache entry `idx` with it and returns the entry's first tile. */
Sint32 mnItemIconLoadIntoSlot(Sint32 resource, Sint32 idx)
{
	Sint32		   result;
	ICON_TRANSFER *state;

	state = syHeapSlotAllocEWRAM(HEAP_SLOT_ICON_TRANSFER, sizeof(*state));
	mnItemIconCompose(
		resource, ICON_COUNT_ABOVE_ONE | ICON_EQUIPPED | ICON_BROKEN);
	result = syVRAMCacheReload(idx, (Sint32)state->tiles);
	syHeapSlotRelease(HEAP_SLOT_ICON_TRANSFER);
	return result;
}

/* Builds the icon of ability `resource` in a transfer block taken from
   HEAP_SLOT_ICON_TRANSFER, reloads VRAM cache entry `idx` with it and returns
   the entry's first tile. */
Sint32 mnAbilityIconLoadIntoSlot(Sint32 resource, Sint32 idx)
{
	Sint32		   tile;
	Sint32		   slot;
	Sint32		   result;
	ICON_TRANSFER *state;

	state = syHeapSlotAllocEWRAM(HEAP_SLOT_ICON_TRANSFER, sizeof(*state));
	slot = idx;
	mnAbilityBuildIconTiles(resource, 0, &slot, &tile, 1);
	result = syVRAMCacheReload(idx, (Sint32)state->tiles);
	syHeapSlotRelease(HEAP_SLOT_ICON_TRANSFER);
	return result;
}
