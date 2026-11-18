/// @file
/// The Jupiter Lighthouse rooms' palettes, which most of them keep a copy of.
/// An includer whose cycle steps through PaletteCycle_StepAt defines
/// PALETTE_CYCLE_TASK as that first.

#include "palette_cycle_places.h"

#ifndef PALETTE_CYCLE_TASK
#define PALETTE_CYCLE_TASK PaletteCycle_Step
#endif

/// The BG palette after the cycle's, which holds its colours as they are while
/// the lighthouse is unlit.
#define UNLIT_PALETTE (CYCLE_PALETTE + 1)

/// Keeps a copy of the room's cycling palette and of UNLIT_PALETTE (mode 2
/// keeps the copies it has), then starts the cycle once the lighthouse is lit
/// (FLAG_JUPITER_LIGHTHOUSE_ACTIVATED), or for mode 1 whatever the flag; else
/// the cycle's places show the unlit colours.
void JupiterLighthouse_LoadPalettes(s32 mode) {
  gPaletteCyclePausesInEvents = 1;
  if (mode != 2) {
    Iwram_CopyWordsEntry(gPaletteCycleColors,
                         (void *)&BG_PLTT_COLOR(CYCLE_PALETTE, 0),
                         PALETTE_BYTES);
    Iwram_CopyWordsEntry(gPaletteCycleUnlitPalette,
                         (void *)&BG_PLTT_COLOR(UNLIT_PALETTE, 0),
                         PALETTE_BYTES);
  }
  if (Event_GetFlag(FLAG_JUPITER_LIGHTHOUSE_ACTIVATED) || mode == 1) {
    PaletteCycle_SetFrame(0);
    Task_Add(PALETTE_CYCLE_TASK, TASK_PASS_FRAME);
  } else {
    Dma_Set(gPaletteCycleUnlitColors,
            (void *)&BG_PLTT_COLOR(CYCLE_PALETTE, CYCLE_FIRST_COLOR),
            DMA_COPY_WORDS((CYCLE_PLACES - 1) / 2), REG_DMA3);
  }
}

#undef UNLIT_PALETTE
#undef CYCLE_PALETTE
#undef CYCLE_FIRST_COLOR
#undef CYCLE_PLACES
#undef CYCLE_STEP_FRAMES
