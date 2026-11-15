/// @file
/// Adding a palette cycle.

// The main image and overlay 674 both compile this file.

/// Adds a palette cycle: count colours of background palette bank from index,
/// rotated by one every delay + 1 frames (PaletteCycle_Step). Returns 0, or -1
/// when all four cycles are taken.
UNUSED s32 PaletteCycle_Add(s16 paletteNumber, s16 firstColor, s16 delay,
                            s16 colorCount) {
  struct PaletteCycleWork *paletteQueue;
  struct PaletteCycle *cycle;
  u16 *paletteColors;
  u32 cycleIndex;
  u16 colorCount16;

  paletteQueue = Ram_HeapSlots->paletteCycles;
  cycleIndex = paletteQueue->count;
  if (cycleIndex > 3)
    return -1;
  // 16 colours to a palette; the cycle saves the colours as they are
  // now, one halfword each.
  cycle = &paletteQueue->entries[cycleIndex];
  paletteColors =
      (u16 *)BG_PLTT + (((u16)paletteNumber << 4) + (u16)firstColor);
  colorCount16 = colorCount;
  cycle->position = 0;
  cycle->timer = 0;
  cycle->count = colorCount16;
  cycle->source = paletteColors;
  cycle->delay = delay;
  Dma_Set(paletteColors, cycle->colors, DMA_COPY_HALFWORDS(colorCount16),
          REG_DMA3);
  paletteQueue->count++;
  return 0;
}
