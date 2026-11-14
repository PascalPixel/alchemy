/// Opens a door three cells high, with its sound when `mode` is 1.
void JupiterLighthouse_OpenDoor(s32 mode) {
  if (mode == 1) {
    Event_PlaySound(SE_IMPACT);
    MapGrid_CopyCellsLayered(76, 42, 4, 3, 76, 35);
    System_Wait(6);
  }
  MapGrid_CopyCellsLayered(81, 42, 4, 3, 76, 35);
  MapGrid_CopyCellAttributes(12, 41, 4, 1, 12, 35);
}
