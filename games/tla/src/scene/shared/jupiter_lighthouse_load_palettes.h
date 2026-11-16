/// @file
/// The Jupiter Lighthouse rooms' palettes, which most of them keep a copy of.
/// An includer whose cycle steps through PaletteCycle_StepAt defines
/// PALETTE_CYCLE_TASK as that first.

#ifndef PALETTE_CYCLE_TASK
#define PALETTE_CYCLE_TASK PaletteCycle_Step
#endif

/// Loads the room's two palettes and starts their cycle once the lighthouse is
/// lit (FLAG_JUPITER_LIGHTHOUSE_ACTIVATED).
void JupiterLighthouse_LoadPalettes(s32 mode) {
  gPaletteCyclePausesInEvents = 1;
  if (mode != 2) {
    Iwram_CopyWordsEntry(gPaletteCycleColors, (void *)&BG_PLTT_COLOR(12, 0),
                         32);
    Iwram_CopyWordsEntry(gPaletteCycleUnlitPalette,
                         (void *)&BG_PLTT_COLOR(13, 0), 32);
  }
  if (Event_GetFlag(FLAG_JUPITER_LIGHTHOUSE_ACTIVATED) || mode == 1) {
    PaletteCycle_SetFrame(0);
    Task_Add(PALETTE_CYCLE_TASK, TASK_PASS_VBLANK);
  } else {
    Dma_Set(gPaletteCycleUnlitColors, (void *)&BG_PLTT_COLOR(12, 2),
            DMA_COPY_WORDS(2), REG_DMA3);
  }
}
