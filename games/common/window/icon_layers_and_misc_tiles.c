/* mnItemIconLoadLayersIntoSlot and mnIconBuildMiscIconTiles, the same in
   both games. Both games' window/window_icons.c include it where the
   functions stand. */

/* Draws an item's icon from its inventory code with the layers asked for
   (mnItemIconCompose) and loads it into VRAM cache entry cacheSlot. */
Sint32 mnItemIconLoadLayersIntoSlot(
	Sint32 inventoryCode, Sint32 layers, Sint32 cacheSlot)
{
	ICON_TRANSFER *state;

	state = syHeapSlotAlloc(HEAP_SLOT_ICON_TRANSFER, sizeof(ICON_TRANSFER));
	mnItemIconCompose(inventoryCode, layers);
	syVRAMCacheLoad(cacheSlot, 2 * 2 * TILE_SIZE_4BPP, state->tiles);
	syHeapSlotRelease(HEAP_SLOT_ICON_TRANSFER);
	return 1;
}

/* Draws one of the other icons (ICON_KIND_PATTERN) and loads it into cacheSlot,
   as mnItemIconLoadLayersIntoSlot does an item's. */
Sint32 mnIconBuildMiscIconTiles(Sint32 icon, Sint32 unused, Sint32 cacheSlot)
{
	ICON_TRANSFER *state;

	state = syHeapSlotAlloc(HEAP_SLOT_ICON_TRANSFER, sizeof(ICON_TRANSFER));
	mnIconDrawMisc(icon);
	syVRAMCacheLoad(cacheSlot, 2 * 2 * TILE_SIZE_4BPP, state->tiles);
	syHeapSlotRelease(HEAP_SLOT_ICON_TRANSFER);
	return 1;
}
