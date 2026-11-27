/* btShiftVRAMTiles, the same in both games. Both games'
   battle/battle_setup.c include it where the function stands. */

/* Unused: moves 16 bytes of VRAM down by 16 and clears the 20 from 12 bytes
   past where they land. */
UNUSED Sint32 btShiftVRAMTiles(void)
{
	/* XXX (no evidence): what the bytes moved are, tiles of character block 0
	   or entries of screen block 0; nothing calls this in either game. */
	syDMAXCopy((const void *)0x06000290, (void *)0x06000280,
		DMA_COPY_HALFWORDS(8), REG_DMA3);
	return syWordsClear((void *)0x0600028c, 20);
}
