/// JupiterLighthouse_LoadPalettes as the aerie and room 3 compile it.
void JupiterLighthouse_LoadPalettesZero(s32 mode) {
  gPaletteCyclePausesInEvents = 1;
  if (mode != 2) {
    Iwram_CopyWordsEntry(gPaletteCycleColors, (void *)&BG_PLTT_COLOR(12, 0),
                         32);
    Iwram_CopyWordsEntry(gPaletteCycleUnlitPalette,
                         (void *)&BG_PLTT_COLOR(13, 0), 32);
  }
  if (Event_GetFlag(FLAG_JUPITER_LIGHTHOUSE_ACTIVATED) || mode == 1) {
    /* FAKEMATCH(temporary): a local holding 0. */
    s32 zero = 0;

    gPaletteCycleFrame = zero;
    Task_Add(PaletteCycle_StepAt, 1152);
  } else {
    Dma_Set(gPaletteCycleUnlitColors, (void *)&BG_PLTT_COLOR(12, 2),
            DMA_COPY_WORDS(2), REG_DMA3);
  }
}
