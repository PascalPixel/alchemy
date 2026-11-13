/// Aims an actor at the actor it watches, to reach it in ten steps.
s32 Actor_AimAtWatched(struct Actor *object) {
  struct Actor *target = object->watchedObject;
  s32 steps;
  s32 from;

  object->bounce = (target->x - object->x) / (steps = 10);
  from = object->y;
  object->unk4c = (target->y - from + 0x100000) / steps;
  object->gravity = (target->z - object->z) / steps;
  BYTE_AT(object, 98) = steps;
  return 0;
}
