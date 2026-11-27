/* Allocating the palette cycles' queue. The main image (field_tile_animation.c)
   and the Islets' overlay (islets.c) each include a copy. */

/* Allocates the palette cycling queue (HEAP_SLOT_PALETTE_CYCLES) with its four
   entries cleared. Nothing in the main image calls it; the Islets call their
   own copy. */
void fdPaletteCycleInit(void)
{
	PALETTE_CYCLE_STATE *paletteCycles;
	PALETTE_CYCLE		*cycle;
	Uint16				 cycleIndex;
	Uint16				 colorIndex;

	paletteCycles = syHeapSlotAllocateEWRAM(
		HEAP_SLOT_PALETTE_CYCLES, sizeof(PALETTE_CYCLE_STATE));
	cycle = paletteCycles->entries;
	for(cycleIndex = 0; cycleIndex != ARRAY_COUNT(paletteCycles->entries);
		cycleIndex++) {
		cycle->src = 0;
		cycle->pos = 0;
		cycle->timer = 0;
		cycle->delay = 0;
		cycle->count = 0;
		for(colorIndex = 0; colorIndex != ARRAY_COUNT(cycle->colors);
			colorIndex++) {
			cycle->colors[colorIndex] = 0;
		}
		cycle++;
	}
	paletteCycles->count = 0;
}
