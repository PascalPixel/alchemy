/* Adding a palette cycle. field_tile_animation.c and the islets' scene
   (scene/islets.c) each include a copy. */

/* Unused: starts rotating colorCount colours of background palette
   paletteNumber, from colour firstColor, by one place every delay + 1 ticks
   (fdPaletteCycleStep). Returns 0, or -1 when all four cycles are taken. */
UNUSED Sint32 fdPaletteCycleAdd(
	Sint16 paletteNumber, Sint16 firstColor, Sint16 delay, Sint16 colorCount)
{
	PALETTE_CYCLE_STATE *queue;
	PALETTE_CYCLE		*cycle;
	Uint16				*colors;
	Uint32				 cycleIndex;
	Uint16				 count;

	queue = RamHeapSlots->paletteCycles;
	cycleIndex = queue->count;
	if(cycleIndex >= ARRAY_COUNT(queue->entries))
		return -1;
	/* 16 colours to a palette. The cycle keeps the colours as they are now, to
	   rotate from. */
	cycle = &queue->entries[cycleIndex];
	colors =
		(Uint16 *)BG_PLTT + (((Uint16)paletteNumber << 4) + (Uint16)firstColor);
	count = colorCount;
	cycle->pos = 0;
	cycle->timer = 0;
	cycle->count = count;
	cycle->src = colors;
	cycle->delay = delay;
	syDMAXCopy(colors, cycle->colors, DMA_COPY_HALFWORDS(count), REG_DMA3);
	queue->count++;
	return 0;
}
