#include "palette_cycle_places.h"

/// Steps the palette cycle, unless an event, Reveal, the pause menu or any
/// field Psynergy but Hover holds the field.
void PaletteCycle_Step(void) {
  u16 colors[16];
  s32 phase = (gPaletteCycleFrame / CYCLE_STEP_FRAMES) % CYCLE_PLACES;
  s32 i;

  if (gPaletteCyclePausesInEvents != 0) {
    struct FieldState *fieldState = Ram_HeapSlots->fieldState;
    s16 *state = &fieldState->psynergy;

    if (*state != 0 && ABILITY_ID(*state) != ABILITY_HOVER)
      return;
    if (fieldState->revealActive != 0)
      return;
    if (fieldState->pauseMenu != 0)
      return;
  }
  for (i = 0; i <= CYCLE_PLACES - 1; i++)
    colors[(phase + i) % CYCLE_PLACES] =
        gPaletteCycleColors[i + CYCLE_FIRST_COLOR];
  Dma_Set(colors, (void *)&BG_PLTT_COLOR(CYCLE_PALETTE, CYCLE_FIRST_COLOR),
          DMA_COPY_WORDS((CYCLE_PLACES - 1) / 2), REG_DMA3);
  gPaletteCycleFrame++;
}

#undef CYCLE_PALETTE
#undef CYCLE_FIRST_COLOR
#undef CYCLE_PLACES
#undef CYCLE_STEP_FRAMES
