/// Grows an actor upward over 32 frames.
void Actor_GrowUp(struct Actor *object) {
  struct Actor *parent = object->watchedActor;
  s32 step = ++object->timer;
  FIXED scale;
  FIXED rise;

  if (step > 31) {
    Actor_Release(object);
    return;
  }
  scale = Math_Sin(step << 10);
  object->scaleX = scale;
  object->scaleY = scale;

  rise = 0x10000;
  object->x.fixed = parent->x.fixed;
  object->y.fixed += rise;
  object->z.fixed = parent->z.fixed + (rise - scale) * 5 + toFIXED(8);
}
