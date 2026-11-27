/* mnCommandIconLoad, the same in both games. Both games'
   window/screens_menu_titles.c include it where the function stands. */

/* Decompresses command icon `icon` (a 32x32 picture of 256 colours, 1 KB) into
   VRAM slot `slot`, remapping the colours of its top three rows of tiles by
   table when `remap` is nonzero. */
void mnCommandIconLoad(Sint32 icon, Sint32 slot, Sint32 remap)
{
	Sint32	size = COMMAND_ICON_BYTES;
	void   *buf = syHeapSlotAllocEWRAM(HEAP_SLOT_TILE_SCRATCH, size);
	Uint16 *ofsTbl = syResourceGetTableEntry(RESOURCE_COMMAND_ICONS);

	if(slot < VRAM_CACHE_ENTRY_COUNT) {
		syDecompressByteLZ((void *)((Uint32)ofsTbl + ofsTbl[icon]), buf);
		if(remap != 0)
			fdSprColRemapPixelsStub(buf, 3 * COMMAND_ICON_TILE_ROW_BYTES);
		syVRAMCacheLoad(slot, size, buf);
		syHeapSlotRelease(HEAP_SLOT_TILE_SCRATCH);
	}
}
