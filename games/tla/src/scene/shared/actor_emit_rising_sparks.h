/// Sparks rising from an actor, every eighth frame.
s32 Actor_EmitRisingSparks(struct Actor *object) {
  struct EffectOptions spark;
  s32 frame = gFrameCount & 7;

  if (frame == 0) {
    s32 x;
    s32 y;
    s32 speed;

    spark.priority = 3;
    spark.startScaleX = Q16_ONE * 7 / 10;
    spark.startScaleY = Q16_ONE * 7 / 10;
    spark.palette = 14;
    x = object->x.fixed + (((Random_Below(9)) - 4) << 16);
    y = object->y.fixed + ((32 - (Random_Below(20))) << 16);
    speed = (s32)(((Random_Below(5)) << 16) + px(5)) / 10;
    FieldEffect_Spawn(x, y, object->z.fixed, 0, speed, frame,
                      EFFECT_USE_PALETTE | EFFECT_USE_PRIORITY |
                          EFFECT_USE_START_SCALE,
                      &spark);
  }
  return 0;
}
