/// Smoke rising from an actor: every eighth frame a black puff at 0.7 of its
/// size rises from within 4 pixels of it, 13 to 32 pixels up, at half a pixel
/// to 0.9 a frame. The energy block smokes so once lightning has struck it
/// (JupiterLighthouse_FlashSky).
s32 Actor_EmitSmoke(struct Actor *actor) {
  struct EffectOptions puff;

  if ((gFrameCount & 7) == 0) {
    FIXED x;
    FIXED y;
    FIXED speed;

    puff.priority = 3;
    puff.startScaleX = FIXED_ONE * 7 / 10;
    puff.startScaleY = FIXED_ONE * 7 / 10;
    puff.palette = SPRITE_COLOR_BLACK;
    x = actor->x.fixed + toFIXED(Random_Below(9) - 4);
    y = actor->y.fixed + toFIXED(32 - Random_Below(20));
    speed = toFIXED(Random_Below(5) + 5) / 10;
    FieldEffect_Spawn(x, y, actor->z.fixed, 0, speed, 0,
                      EFFECT_USE_PALETTE | EFFECT_USE_PRIORITY |
                          EFFECT_USE_START_SCALE,
                      &puff);
  }
  return 0;
}
