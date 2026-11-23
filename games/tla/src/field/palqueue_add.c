/// @file
/// Adding a palette cycle. field_tile_animation.c and the islets' scene
/// (scene/islets.c) each include a copy.

/// Unused: starts rotating colorCount colours of background palette
/// paletteNumber, from colour firstColor, by one place every delay + 1 frames
/// (PaletteCycle_Step). Returns 0, or -1 when all four cycles are taken.
UNUSED s32 PaletteCycle_Add(s16 paletteNumber, s16 firstColor, s16 delay,
                            s16 colorCount) {
  struct PaletteCycleWork *queue;
  struct PaletteCycle *cycle;
  u16 *colors;
  u32 cycleIndex;
  u16 count;

  queue = Ram_HeapSlots->paletteCycles;
  cycleIndex = queue->count;
  if (cycleIndex >= ARRAY_COUNT(queue->entries))
    return -1;
  // 16 colours to a palette. The cycle keeps the colours as they are now, to
  // rotate from.
  cycle = &queue->entries[cycleIndex];
  colors = (u16 *)BG_PLTT + (((u16)paletteNumber << 4) + (u16)firstColor);
  count = colorCount;
  cycle->position = 0;
  cycle->timer = 0;
  cycle->count = count;
  cycle->source = colors;
  cycle->delay = delay;
  Dma_Set(colors, cycle->colors, DMA_COPY_HALFWORDS(count), REG_DMA3);
  queue->count++;
  return 0;
}
