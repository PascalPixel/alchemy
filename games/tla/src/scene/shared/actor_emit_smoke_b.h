/// Unused: a copy of Actor_EmitSmoke, which three overlays keep and
/// nothing sets as an update: every eighth frame a black puff of smoke at 0.7
/// of its size rises from within 4 pixels of the actor, 13 to 32 pixels up, at
/// half a pixel to 0.9 a frame.
UNUSED s32 Actor_EmitSmokeB(struct Actor *actor) {
  struct EffectOptions puff;

  if ((gFrameCount & 7) == 0) {
    FIXED x;
    FIXED y;

    puff.priority = 3;
    puff.startScaleX = FIXED_ONE * 7 / 10;
    puff.startScaleY = FIXED_ONE * 7 / 10;
    puff.palette = SPRITE_COLOR_BLACK;
    x = actor->x.fixed + toFIXED(Random_Below(9) - 4);
    y = actor->y.fixed + toFIXED(32 - Random_Below(20));
    FieldEffect_Spawn(
        x, y, actor->z.fixed, 0, toFIXED(Random_Below(5) + 5) / 10, 0,
        EFFECT_USE_PALETTE | EFFECT_USE_PRIORITY | EFFECT_USE_START_SCALE,
        &puff);
  }
  return 0;
}
