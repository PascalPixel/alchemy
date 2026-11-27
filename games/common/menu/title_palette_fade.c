/* mnTitlePaletteFadeInit and mnTitlePaletteFadeRelease, the same in both
   games. Both games' menu/title_intro.c include it where the functions
   stand. */

/* Allocates the palette fade, saves both hardware palettes in it as the
   starting colours and adds the event mnTitlePaletteFadeUpdate. */
void mnTitlePaletteFadeInit(void)
{
	TITLE_PALETTE_FADE *buf;

	buf = syHeapSlotAllocEWRAM(HEAP_SLOT_PALETTE_FADE, sizeof(*buf));
	DMA_FILL32(0, buf, sizeof(*buf));
	syDMAXCopy(
		(void *)BG_PLTT, buf, DMA_COPY_WORDS(BG_PLTT_SIZE / 4), REG_DMA3);
	syDMAXCopy((void *)OBJ_PLTT, &buf->pal[BG_PLTT_COLORS],
		DMA_COPY_WORDS(BG_PLTT_SIZE / 4), REG_DMA3);
	mnTitlePaletteFadeBuildCols(
		PALETTE_TINT_NONE, buf->pal, buf->tgt, PALETTES_ALL);
	sySetEvent(mnTitlePaletteFadeUpdate, EVENT_PASS_TICK);
}

/* Unused: removes the palette fade's event and frees its buffers. */
UNUSED void mnTitlePaletteFadeRelease(void)
{
	syCloseEvent(&mnTitlePaletteFadeUpdate);
	syHeapSlotRelease(HEAP_SLOT_PALETTE_FADE);
}
