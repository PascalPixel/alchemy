/// Bubbles rise from an actor every eighth frame.
void Actor_EmitBubbles(struct Actor *object) {
  s32 phase = gFrameCount & 7;

  if (phase == 0) {
    struct EffectOptions options;
    s32 position[3];

    options.palette = 7;
    options.type = 0x2a1;
    options.priority = 1;
    position[0] = object->x + (((Random16() & 31) - 16) << 16);
    position[1] = object->y + ((Random16() & 31) << 16) + 0x80000;
    position[2] = object->z;
    FieldEffect_Spawn(
        position[0], position[1], position[2] + 0x40000, 0, 0x8000, phase,
        EFFECT_USE_TYPE | EFFECT_USE_PRIORITY | EFFECT_USE_PALETTE, &options);
  }
}
