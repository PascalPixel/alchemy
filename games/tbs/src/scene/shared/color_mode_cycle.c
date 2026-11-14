/// @file
/// Cottage Rooms B and the world map, where nothing reaches it.

/// Unused: an actor's update routine that steps it through the six colour
/// modes from SPRITE_COLOR_NORMAL to SPRITE_COLOR_BRG, one every two frames.
UNUSED static void Scene_CycleColorModesOnOddFrames(struct Actor *actor) {
  if ((gFrameCount & 1) != 0) {
    s32 slot = (gFrameCount >> 1) % 6;

    Actor_SetColorMode(actor, slot);
  }
}
