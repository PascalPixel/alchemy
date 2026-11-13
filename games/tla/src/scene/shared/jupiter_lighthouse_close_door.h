/// Closes a door three cells high.
void JupiterLighthouse_CloseDoor(void) {
  Event_PlaySound(SE_HEAVY_IMPACT);
  MapGrid_CopyCellsLayered(76, 42, 4, 3, 76, 35);
  System_Wait(6);
  MapGrid_CopyCellsLayered(71, 42, 4, 3, 76, 35);
  MapGrid_CopyCellAttributes(12, 36, 4, 1, 12, 35);
}
