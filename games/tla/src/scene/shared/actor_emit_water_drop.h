/// Every sixteenth frame a splash ring (SPRITE_SPLASH_RING's animation 2)
/// starts 7.5 pixels down at the actor and rises a quarter of a pixel a frame
/// for 31 frames (gWaterDropScript).
void Actor_EmitWaterDrop(struct Actor *actor) {
  s32 phase = gFrameCount & 15;

  if (phase == 0) {
    struct Actor *child = Import_ActorCreate_(
        SPRITE_SPLASH_RING, actor->x.fixed, actor->y.fixed, actor->z.fixed);

    if (child != 0) {
      struct FieldSprite *sprite = child->sprite;

      Actor_StartScript(child, gWaterDropScript);
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
