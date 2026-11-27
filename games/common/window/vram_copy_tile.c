/* mnVRAMCopyTile, the same in both games. Both games'
   window/screens_battle_item_menu.c include it where the function stands. */

/* Copies one 4bpp tile of BG character block 0 to another, each given as a
   tilemap entry whose palette and flip bits are ignored. */
void mnVRAMCopyTile(Uint32 srcTile, Uint32 dstTile)
{
	syDMAXCopy(BG_CHAR_BLOCK(0) + (srcTile & TILE_NUMBER_MASK) * TILE_SIZE_4BPP,
		BG_CHAR_BLOCK(0) + (dstTile & TILE_NUMBER_MASK) * TILE_SIZE_4BPP,
		DMA_COPY_WORDS(8), REG_DMA3);
}
