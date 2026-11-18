/// The cycle: BG palette CYCLE_PALETTE's colours from CYCLE_FIRST_COLOR take
/// their places among CYCLE_PLACES one further every CYCLE_STEP_FRAMES frames.
#define CYCLE_PALETTE 12
#define CYCLE_FIRST_COLOR 2
#define CYCLE_PLACES 5
#define CYCLE_STEP_FRAMES 4

/// The lower floors' copy of PaletteCycle_Step (palette_cycle_step.h), under
/// the name their task list calls it by: steps the palette cycle, unless an
/// event, Reveal, the pause menu or any field Psynergy but Hover holds the
/// field.
void PaletteCycle_StepAt(void) {
  u16 colors[16];
  s32 phase = (gPaletteCycleFrame / CYCLE_STEP_FRAMES) % CYCLE_PLACES;
  s32 i;

  if (gPaletteCyclePausesInEvents != 0) {
    struct FieldWork *work = Ram_HeapSlots->fieldWork;
    s16 *state = &work->psynergy;

    if (*state != 0 && ABILITY_ID(*state) != ABILITY_HOVER)
      return;
    if (work->revealActive != 0)
      return;
    if (work->pauseMenu != 0)
      return;
  }
  for (i = 0; i <= CYCLE_PLACES - 1; i++)
    colors[(phase + i) % CYCLE_PLACES] =
        gPaletteCycleColors[i + CYCLE_FIRST_COLOR];
  Dma_Set(colors, (void *)&BG_PLTT_COLOR(CYCLE_PALETTE, CYCLE_FIRST_COLOR),
          DMA_COPY_WORDS(2), REG_DMA3);
  gPaletteCycleFrame++;
}

#undef CYCLE_PALETTE
#undef CYCLE_FIRST_COLOR
#undef CYCLE_PLACES
#undef CYCLE_STEP_FRAMES
