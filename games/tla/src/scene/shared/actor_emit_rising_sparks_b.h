/// Unused: a copy of Actor_EmitRisingSparks, which three overlays keep and
/// nothing sets as an update: every eighth frame a spark at 0.7 of its size
/// rises from within 4 pixels of the actor, 13 to 32 pixels up, at half a
/// pixel to 0.9 a frame.
UNUSED s32 Actor_EmitRisingSparksB(struct Actor *actor) {
  struct EffectOptions spark;

  if ((gFrameCount & 7) == 0) {
    FIXED x;
    FIXED y;

    spark.priority = 3;
    spark.startScaleX = FIXED_ONE * 7 / 10;
    spark.startScaleY = FIXED_ONE * 7 / 10;
    spark.palette = SPRITE_COLOR_BLACK;
    x = actor->x.fixed + toFIXED(Random_Below(9) - 4);
    y = actor->y.fixed + toFIXED(32 - Random_Below(20));
    FieldEffect_Spawn(
        x, y, actor->z.fixed, 0, toFIXED(Random_Below(5) + 5) / 10, 0,
        EFFECT_USE_PALETTE | EFFECT_USE_PRIORITY | EFFECT_USE_START_SCALE,
        &spark);
  }
  return 0;
}
