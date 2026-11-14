/// PaletteCycle_Start with a VBlank task.
void YampiDesertCave_StartMarkGlow(void) {
  gYampiDesertCave_MarkCycleIndex = 0;
  gYampiDesertCave_MarkCount = List_CountEntries(gYampiDesertCave_MarkSlots);
  gYampiDesertCave_MarkPeriod = 15;
  gYampiDesertCave_MarkTimer = 16;
  gYampiDesertCave_MarkBright = 1;
  Task_Add(PaletteCycle_Fade, TASK_PASS_VBLANK);
}
