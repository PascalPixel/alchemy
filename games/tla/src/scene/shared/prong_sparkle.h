/// A Prong's update while it waits to be taken, the Left Prong in Ankohl Ruins
/// and the Right Prong in the Shrine of the Sea God: every eighth frame a
/// sparkle rises somewhere about it.
void Prong_Sparkle(struct Actor *actor) {
  struct EffectOptions options;
  u32 phase = gFrameCount & 7;
  s32 dx;
  s32 dy;

  if (phase == 0) {
    dx = (Random16() & 15) - 8;
    dy = Random16() & 7;
    options.type = SPRITE_SPARKLE;
    FieldEffect_Spawn(actor->x.fixed + INTtoFIXED(dx),
                      actor->y.fixed + INTtoFIXED(dy - 8), actor->z.fixed, 0,
                      toFIXED(1) / 2, phase, EFFECT_USE_TYPE, &options);
  }
}
