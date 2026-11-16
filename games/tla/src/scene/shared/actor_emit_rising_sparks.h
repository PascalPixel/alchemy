/// Sparks rising from an actor, every eighth frame.
s32 Actor_EmitRisingSparks(struct Actor *actor) {
  struct EffectOptions spark;
  s32 frame = gFrameCount & 7;

  if (frame == 0) {
    FIXED x;
    FIXED y;
    FIXED speed;

    spark.priority = 3;
    spark.startScaleX = FIXED_ONE * 7 / 10;
    spark.startScaleY = FIXED_ONE * 7 / 10;
    spark.palette = 14;
    x = actor->x.fixed + (((Random_Below(9)) - 4) << 16);
    y = actor->y.fixed + ((32 - (Random_Below(20))) << 16);
    speed = (s32)(((Random_Below(5)) << 16) + toFIXED(5)) / 10;
    FieldEffect_Spawn(x, y, actor->z.fixed, 0, speed, frame,
                      EFFECT_USE_PALETTE | EFFECT_USE_PRIORITY |
                          EFFECT_USE_START_SCALE,
                      &spark);
  }
  return 0;
}
