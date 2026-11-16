/// Aims an actor at the actor it watches, to reach it in ten steps.
s32 Actor_AimAtWatched(struct Actor *actor) {
  struct Actor *target = actor->watchedActor;
  s32 steps;
  FIXED from;

  actor->bounce = (target->x.fixed - actor->x.fixed) / (steps = 10);
  from = actor->y.fixed;
  actor->riseBaseY = (target->y.fixed - from + toFIXED(16)) / steps;
  actor->gravity = (target->z.fixed - actor->z.fixed) / steps;
  actor->riseCounter = steps;
  return 0;
}
