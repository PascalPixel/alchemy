/// @file
/// The Teleport circles' palette cycle, which three Mars Lighthouse overlays
/// keep a copy of: the circle's colours run round it as Teleport is cast, and
/// the party arrives on a circle.

/// The BG colours a palette fade keeps, palettes 0 to 13, before its OBJ
/// colours.
#define PALETTE_FADE_BG_COLORS (14 * PALETTE_COLORS)

/// Writes each listed background colour from its red, green and blue, keeps
/// the lists for the palette cycle, and saves the palettes as the fade's
/// source; arriving by Teleport, the party then arrives on the circle.
void Palette_SetColorsFade(u16 *slots, u16 *colours, u16 *cycleColors,
                           u16 *cycleSlots) {
  struct PaletteFade *fade = Ram_HeapSlots->paletteFade;

  gPaletteSlots = slots;
  gPaletteColors = colours;
  gPaletteCycleColors = cycleColors;
  gPaletteCycleSlots = cycleSlots;
  while (*slots != PALETTE_LIST_END && *colours != PALETTE_LIST_END) {
    int index = *slots++;
    int red = *colours++;
    int green = *colours++;
    int blue = *colours++;

    SET_RGB(BG_PLTT[index], red, green, blue);
  }
  Dma_Set((const void *)BG_PLTT, fade->palette,
          DMA_COPY_WORDS(PALETTE_FADE_BG_COLORS / 2), REG_DMA3);
  Dma_Set((const void *)OBJ_PLTT, &fade->palette[PALETTE_FADE_BG_COLORS],
          DMA_COPY_WORDS(PALETTE_FADE_BG_COLORS / 2), REG_DMA3);
  PaletteFade_ApplyTarget(PALETTE_TINT_NONE, PALETTES_ALL);
  if (gMainRoutineWork.teleportState == 2)
    Teleport_ArriveWhirling();
}

/// Starts the palette cycle at its slow pace, as Teleport is cast: the first
/// step after 16 frames, each after a frame less, down to two.
void Teleport_StartShimmer(void) {
  gPaletteCycleIndex = 0;
  gPaletteCycleLength = List_CountEntries(gPaletteSlots);
  gPaletteCycleDelay = 15;
  gPaletteCycleTimer = 16;
  gPaletteCycleBright = 1;
  Task_AddFrame(PaletteCycle_Fade, TASK_PASS_FRAME);
}

/// Stops the palette cycle, leaving the next transition a backdrop fade.
void Teleport_StopShimmer(void) {
  gFieldWork->transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
  FieldTask_Remove(PaletteCycle_Fade);
}

/// Starts the palette cycle at a step every two frames, as the party arrives.
void Teleport_StartFastShimmer(void) {
  gPaletteCycleIndex = 0;
  gPaletteCycleLength = List_CountEntries(gPaletteSlots);
  gPaletteCycleDelay = 2;
  gPaletteCycleTimer = 2;
  gPaletteCycleBright = 1;
  Task_AddFrame(PaletteCycle_Fade, TASK_PASS_FRAME);
}

#include "list_count_entries.h"

#include "palette_cycle_fade.h"

/// The party arrives by Teleport on the circle: the room fades in grey while
/// the circle's colours run and the whirl plays, then its colours come back.
void Teleport_ArriveWhirling(void) {
  struct FieldWork *work = Ram_HeapSlots->fieldWork;
  s32 saved;

  saved = work->transition;
  PaletteFade_ApplyTarget(PALETTE_TINT_CUSTOM | RGB(8, 8, 8), PALETTES_BG);
  PaletteFade_ApplyTarget(PALETTE_TINT_NONE, PALETTES_OBJECTS);
  gFieldWork->transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Teleport_StartFastShimmer();
  Teleport_PlayWhirl();
  System_Wait(40);
  Teleport_StopShimmer();
  PaletteFade_ApplyTarget(PALETTE_TINT_NONE, PALETTES_BG);
  PaletteFade_Interpolate(16);
  System_Wait(16);
  {
    struct MainRoutineWork *party = &gMainRoutineWork;

    party->teleportState = 0;
  }
  work->transition = saved;
}
