/* mnWindowSprCreateFromResource and mnWindowSprCreateLoaded, the same in
   both games. Both games' window/screens_window_sprites.c include it where
   the functions stand. */

/* Loads resource `resource` in loading mode `mode` into a VRAM slot and shows
   it as a 16x16 sprite at (x, y) in `win`. */
WINDOW_SPR *mnWindowSprCreateFromResource(
	Sint32 mode, Sint32 resource, WINDOW *win, Sint32 x, Sint32 y)
{
	Sint32 slot;

	slot = mnSelectionIconLoad(mode, resource);
	if(slot < 0) {
		return NULL;
	}
	return mnWindowSprCreate(slot, WINDOW_SPRITE_SIZE_16X16, win, x, y);
}

/* Copies icon `icon` (with `layers` passed on to the copier) into a free VRAM
   slot and shows it as a 16x16 sprite at (x, y) in `win`. */
WINDOW_SPR *mnWindowSprCreateLoaded(
	Sint32 icon, Sint32 layers, WINDOW *win, Sint32 x, Sint32 y)
{
	Sint32		slot;
	WINDOW_SPR *spr;

	slot = syVRAMCacheFindFreeEntry();
	spr = NULL;
	if(slot != VRAM_CACHE_ENTRY_COUNT) {
		mnItemIconLoadLayersIntoSlot(icon, layers, slot);
		spr = mnWindowSprCreate(slot, WINDOW_SPRITE_SIZE_16X16, win, x, y);
	}
	return spr;
}
