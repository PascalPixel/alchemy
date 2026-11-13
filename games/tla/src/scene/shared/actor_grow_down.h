/// Grows an actor downward over 32 frames.
void Actor_GrowDown(struct Actor *object) {
  struct Actor *parent = object->watchedObject;
  s32 step = ++object->timer;
  s32 scale;
  s32 rise;

  if (step > 31) {
    Actor_Release(object);
    return;
  }
  scale = Math_Sin(step << 10);
  object->scaleX = scale;
  object->scaleY = -scale;

  rise = 0x10000;
  object->x = parent->x;
  object->y += rise;
  object->z = parent->z - (rise - scale) * 5 + 0x100000;
}
