/// A drop falls from an actor every sixteenth frame.
void Actor_EmitWaterDrop(struct Actor *object) {
  s32 phase = gFrameCount & 15;

  if (phase == 0) {
    struct Actor *child = Import_ActorCreate_(0x10d, object->x.fixed,
                                              object->y.fixed, object->z.fixed);

    if (child != 0) {
      struct FieldSprite *sprite = child->sprite;

      Actor_StartScript(child, gWaterDropScript);
      child->motionFlags = 0;
      child->y.fixed = -0x78000;
      child->layer = 1;
      if (sprite != 0) {
        Animation_ApplyChildArgument(sprite, 2);
        sprite->nextFrame = 0;
        sprite->priority = 2;
      }
    }
  }
}
