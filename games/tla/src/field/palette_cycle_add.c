/* Adding a palette cycle. field_tile_animation.c and the islets' scene
   (scene/islets.c) each include a copy. */

/* Unused: starts rotating colCnt colours of background palette
   paletteNum, from colour firstCol, by one place every delay + 1 ticks
   (fdPaletteCycleStep). Returns 0, or -1 when all four cycles are taken. */
UNUSED Sint32 fdPaletteCycleAdd(
	Sint16 paletteNum, Sint16 firstCol, Sint16 delay, Sint16 colCnt)
{
	PALETTE_CYCLE_STATE *queue;
	PALETTE_CYCLE		*cycle;
	Uint16				*cols;
	Uint32				 cycleIdx;
	Uint16				 cnt;

	queue = RamHeapSlots->palCycles;
	cycleIdx = queue->cnt;
	if(cycleIdx >= ARRAY_COUNT(queue->entries))
		return -1;
	/* 16 colours to a palette. The cycle keeps the colours as they are now, to
	   rotate from. */
	cycle = &queue->entries[cycleIdx];
	cols = (Uint16 *)BG_PLTT + (((Uint16)paletteNum << 4) + (Uint16)firstCol);
	cnt = colCnt;
	cycle->pos = 0;
	cycle->tmr = 0;
	cycle->cnt = cnt;
	cycle->src = cols;
	cycle->delay = delay;
	syDMAXCopy(cols, cycle->cols, DMA_COPY_HALFWORDS(cnt), REG_DMA3);
	queue->cnt++;
	return 0;
}
