/// Starts a palette cycle's task.
void Func_020000f4(void) {
  gPaletteCycleIndex = 0;
  gPaletteCycleLength = List_CountEntries(gPaletteSlots);
  gPaletteCycleDelay = 15;
  gPaletteCycleTimer = 16;
  gPaletteCycleBright = 1;
  Task_AddFrame(PaletteCycle_Fade, TASK_PASS_VBLANK);
}
