/// Grows an actor upward over 32 frames.
void Actor_GrowUp(struct Actor *actor) {
  struct Actor *parent = actor->watchedActor;
  s32 step = ++actor->timer;
  FIXED scale;
  FIXED rise;

  if (step > 31) {
    Actor_Release(actor);
    return;
  }
  scale = Math_Sin(step << 10);
  actor->scaleX = scale;
  actor->scaleY = scale;

  rise = toFIXED(1);
  actor->x.fixed = parent->x.fixed;
  actor->y.fixed += rise;
  actor->z.fixed = parent->z.fixed + (rise - scale) * 5 + toFIXED(8);
}
