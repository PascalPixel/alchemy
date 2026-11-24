/// Ripples on the water over an actor, as over Aqua Rock's way inside once its
/// Water Stone is doused: every sixteenth frame a blue ring
/// (SPRITE_SPLASH_RING's animation 2) widens from 7.5 pixels under it, rising
/// a quarter of a pixel a frame for 31 frames (gRippleScript).
void Actor_EmitRipples(struct Actor *actor) {
  s32 phase = gFrameCount & 15;

  if (phase == 0) {
    struct Actor *child = FieldActor_Create(SPRITE_SPLASH_RING, actor->x.fixed,
                                            actor->y.fixed, actor->z.fixed);

    if (child != 0) {
      struct FieldSprite *sprite = child->sprite;

      Actor_StartScript(child, gRippleScript);
      child->motionFlags = 0;
      child->y.fixed = -(toFIXED(15) / 2);
      child->layer = 1;
      if (sprite != 0) {
        Sprite_StartAnimation(sprite, 2);
        sprite->flags = 0;
        sprite->priority = 2;
      }
    }
  }
}
