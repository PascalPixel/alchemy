/// @file
/// Adding a palette cycle.

// The main image and overlay 674 both compile this file.

/// Adds a palette cycle: count colours of background palette bank from index,
/// rotated by one every delay + 1 frames (PaletteCycle_Step). Returns 0, or -1
/// when all four cycles are taken.
s32 PaletteCycle_Add(s16 bank, s16 index, s16 delay, s16 count) {
  struct PaletteSnapshotWork *work;
  struct PaletteSnapshot *entry;
  u16 *source;
  u32 slot;
  u16 size;

  work = gPaletteWork;
  slot = work->count;
  if (slot > 3)
    return -1;
  entry = &work->entries[slot];
  source = (u16 *)BG_PLTT + (((u16)bank << 4) + (u16)index);
  size = count;
  entry->position = 0;
  entry->timer = 0;
  entry->count = size;
  entry->source = source;
  entry->delay = delay;
  Dma_Set(source, entry->colors, DMA_COPY_HALFWORDS(size), REG_DMA3);
  work->count++;
  return 0;
}
