/// @file
/// The Teleport circles' shimmer, which every scene with a circle keeps a copy
/// of (Contigo, the Anemos Inner Sanctum, the Islet Cave, the Yampi Desert's
/// cave and three of Mars Lighthouse's rooms): the circle's colours run round
/// it, a palette cycle, as Teleport is cast and as the party arrives on a
/// circle.

/// Teleport's steps (gMainRoutineWork.teleportState), as the field sequence
/// names them: a place was chosen, then the party arrives in its scene.
enum TeleportState {
  TELEPORT_NONE,
  TELEPORT_CHOSEN,
  TELEPORT_ARRIVING,
};

/// Writes each listed background colour from its red, green and blue, keeps
/// the lists for the shimmer, and saves the palettes as the fade's
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
  Dma_Set(
      (const void *)OBJ_PLTT, &fade->palette[PALETTE_FADE_BG_COLORS],
      DMA_COPY_WORDS((ARRAY_COUNT(fade->palette) - PALETTE_FADE_BG_COLORS) / 2),
      REG_DMA3);
  PaletteFade_ApplyTarget(PALETTE_TINT_NONE, PALETTES_ALL);
  if (gMainRoutineWork.teleportState == TELEPORT_ARRIVING)
    Teleport_ArriveWhirling();
}

/// Starts the shimmer at its slow pace, as Teleport is cast: the first
/// step after 16 frames, each after a frame less, down to two.
void Teleport_StartShimmer(void) {
  gPaletteCycleIndex = 0;
  gPaletteCycleLength = List_CountEntries(gPaletteSlots);
  gPaletteCycleDelay = 15;
  gPaletteCycleTimer = 16;
  gPaletteCycleBright = 1;
  Task_AddFrame(PaletteCycle_Fade, TASK_PASS_FRAME);
}

/// Stops the shimmer, leaving the next transition a backdrop fade.
void Teleport_StopShimmer(void) {
  gFieldWork->transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
  FieldTask_Remove(PaletteCycle_Fade);
}

/// Starts the shimmer at a step every two frames, as the party arrives.
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
/// the circle shimmers and the whirl plays, then its colours come back.
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

    party->teleportState = TELEPORT_NONE;
  }
  work->transition = saved;
}
