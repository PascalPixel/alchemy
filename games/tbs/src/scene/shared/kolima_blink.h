/// @file
/// Kolima and Kolima Forest (SceneId_Kolima and SceneId_KolimaForestEntrance):
/// the sparkling dust that falls when Tret's curse strikes. "That sparkly stuff
/// on the ground... What is it?"

/// An actor script call, each frame of the fall: the speck flickers between
/// palettes 10 and 7, every second frame.
static s32 Kolima_FlickerCurseDust(struct Actor *speck) {
  if ((gFrameCount >> 1) & 1) {
    Actor_SetSpriteColor(speck, SPRITE_COLOR_RED_TINT);
  } else {
    Actor_SetSpriteColor(speck, SPRITE_COLOR_WHITE);
  }
  return 0;
}
