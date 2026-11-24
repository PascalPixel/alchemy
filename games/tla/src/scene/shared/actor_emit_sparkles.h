/// Sparkles rising about an actor, as about an Adept a tablet has taught
/// Psynergy: every eighth frame a white sparkle starts within 16 pixels to
/// either side of it, 8 to 39 pixels up, and rises half a pixel a frame.
void Actor_EmitSparkles(struct Actor *actor) {
  if ((gFrameCount & 7) == 0) {
    struct EffectOptions sparkle;
    FIXED position[3];

    sparkle.palette = SPRITE_COLOR_WHITE;
    sparkle.type = SPRITE_SPARKLE;
    sparkle.priority = 1;
    position[0] = actor->x.fixed + toFIXED((Random16() & 31) - 16);
    position[1] = actor->y.fixed + toFIXED(Random16() & 31) + toFIXED(8);
    position[2] = actor->z.fixed;
    FieldEffect_Spawn(
        position[0], position[1], position[2] + toFIXED(4), 0, toFIXED(0.5), 0,
        EFFECT_USE_TYPE | EFFECT_USE_PRIORITY | EFFECT_USE_PALETTE, &sparkle);
  }
}
