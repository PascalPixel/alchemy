/// Steps a five-colour palette cycle, unless an event holds the field.
void PaletteCycle_Step(void) {
  u16 colors[16];
  s32 phase = (gPaletteCycleFrame / 4) % 5;
  s32 i;

  if (gPaletteCyclePausesInEvents != 0) {
    struct FieldWork *work = Ram_HeapSlots->fieldWork;
    s16 *state = &work->psynergy;

    if (*state != 0 && (*state & 0x3ff) != 153)
      return;
    if (work->revealActive != 0)
      return;
    if (work->pauseMenu != 0)
      return;
  }
  for (i = 0; i <= 4; i++)
    colors[(phase + i) % 5] = gPaletteCycleColors[i + 2];
  Dma_Set(colors, (void *)&BG_PLTT_COLOR(12, 2), DMA_COPY_WORDS(2), REG_DMA3);
  gPaletteCycleFrame++;
}
