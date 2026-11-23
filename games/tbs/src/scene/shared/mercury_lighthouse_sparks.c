/// @file
/// Mercury Lighthouse and its entrance: the trail of sparks the leader leaves
/// while passing between floors.

/// The leader's update routine while passing: each frame a small spark rises
/// from a point that sweeps across the leader's chest, white and tinted by
/// turns.
static UNREAD_RESULT MercuryLighthouse_TrailSparks(struct Actor *actor) {
  struct EffectOptions options;
  FIXED velocityY;

  options.palette = SPRITE_COLOR_WHITE;
  if ((gFrameCount & 1) == 0) {
    options.palette = SPRITE_COLOR_BRG;
  }
  options.startScaleX = 4 * FIXED_ONE / 5;
  options.startScaleY = 4 * FIXED_ONE / 5;
  options.priority = 0;
  velocityY = -((Random_Below(8)) * (toFIXED(1) / 5));
  FieldEffect_Spawn(
      actor->x.fixed + INTtoFIXED(8 - (gFrameCount & 15)),
      actor->y.fixed + toFIXED(26), actor->z.fixed, 0, velocityY, 0,
      EFFECT_USE_START_SCALE | EFFECT_USE_PRIORITY | EFFECT_USE_PALETTE,
      &options);
  RETURN_UNREAD(0);
}
