/* mnCommandIconLoadIntoSlot, the same in both games. Both games'
   window/screens_icon_menu.c include it where the function stands. */

/* Decodes command icon `idx` into the VRAM cache entry `slot`, through a
   block it allocates and frees. */
void mnCommandIconLoadIntoSlot(Sint32 slot, Sint32 idx)
{
	Sint32	size = COMMAND_ICON_BYTES;
	void   *buf = syHeapAlloc(size);
	Uint16 *ofsTbl = syResourceGetTableEntry(RESOURCE_COMMAND_ICONS);

	syDecompressByteLZ((void *)((Uint32)ofsTbl + ofsTbl[idx]), buf);
	syVRAMCacheLoad(slot, size, buf);
	syHeapRelease(buf);
}
