/// Sparks rising from an actor, every eighth frame.
s32 Actor_EmitRisingSparks(struct Actor *object) {
  struct EffectOptions spark;
  s32 frame = gFrameCount & 7;

  if (frame == 0) {
    s32 x;
    s32 y;
    s32 speed;

    spark.priority = 3;
    spark.startScaleX = 0xb333;
    spark.startScaleY = 0xb333;
    spark.palette = 14;
    x = object->x + (((((u32)Random16() * 9) >> 16) - 4) << 16);
    y = object->y + ((32 - (((u32)Random16() * 20) >> 16)) << 16);
    speed = (s32)(((((u32)Random16() * 5) >> 16) << 16) + 0x50000) / 10;
    FieldEffect_Spawn(x, y, object->z, 0, speed, frame,
                      EFFECT_USE_PALETTE | EFFECT_USE_PRIORITY |
                          EFFECT_USE_START_SCALE,
                      &spark);
  }
  return 0;
}
