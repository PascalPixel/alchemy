/// Aims an actor at the actor it watches, to reach it in ten steps.
s32 Actor_AimAtWatched(struct Actor *object) {
  struct Actor *target = object->watchedActor;
  s32 steps;
  s32 from;

  object->bounce = (target->x.fixed - object->x.fixed) / (steps = 10);
  from = object->y.fixed;
  object->riseBaseY = (target->y.fixed - from + 0x100000) / steps;
  object->gravity = (target->z.fixed - object->z.fixed) / steps;
  object->riseCounter = steps;
  return 0;
}
