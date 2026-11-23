/// @file
/// Allocating the palette cycles' queue.

// The main image and overlay 674 both compile this file.

/// Allocates the palette cycling queue (HEAP_SLOT_PALETTE_CYCLES) with its four
/// entries cleared. Nothing in the main image calls it; the Islets call their
/// own copy.
void PaletteCycle_Initialize(void) {
  struct PaletteCycleWork *paletteCycles;
  struct PaletteCycle *cycle;
  u16 cycleIndex;
  u16 colorIndex;

  paletteCycles = HeapSlot_AllocEwram(HEAP_SLOT_PALETTE_CYCLES,
                                      sizeof(struct PaletteCycleWork));
  cycle = paletteCycles->entries;
  for (cycleIndex = 0; cycleIndex != ARRAY_COUNT(paletteCycles->entries);
       cycleIndex++) {
    cycle->source = 0;
    cycle->position = 0;
    cycle->timer = 0;
    cycle->delay = 0;
    cycle->count = 0;
    for (colorIndex = 0; colorIndex != ARRAY_COUNT(cycle->colors);
         colorIndex++) {
      cycle->colors[colorIndex] = 0;
    }
    cycle++;
  }
  paletteCycles->count = 0;
}
