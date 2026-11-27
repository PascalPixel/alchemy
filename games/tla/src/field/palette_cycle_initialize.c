/* Allocating the palette cycles' queue. The main image (field_tile_animation.c)
   and the Islets' overlay (islets.c) each include a copy. */

/* Allocates the palette cycling queue (HEAP_SLOT_PALETTE_CYCLES) with its four
   entries cleared. Nothing in the main image calls it; the Islets call their
   own copy. */
void fdPaletteCycleInit(void)
{
	PALETTE_CYCLE_STATE *paletteCycles;
	PALETTE_CYCLE		*cycle;
	Uint16				 cycleIdx;
	Uint16				 colIdx;

	paletteCycles = syHeapSlotAllocEWRAM(
		HEAP_SLOT_PALETTE_CYCLES, sizeof(PALETTE_CYCLE_STATE));
	cycle = paletteCycles->entries;
	for(cycleIdx = 0; cycleIdx != ARRAY_COUNT(paletteCycles->entries);
		cycleIdx++) {
		cycle->src = 0;
		cycle->pos = 0;
		cycle->tmr = 0;
		cycle->delay = 0;
		cycle->cnt = 0;
		for(colIdx = 0; colIdx != ARRAY_COUNT(cycle->cols); colIdx++) {
			cycle->cols[colIdx] = 0;
		}
		cycle++;
	}
	paletteCycles->cnt = 0;
}
