/// The steps an aimed actor takes to reach the actor it watches.
#define AIM_STEPS 10

/// Aims an actor at the actor it watches, 16 pixels above its feet, to reach
/// it in AIM_STEPS steps of Actor_StepToWatched.
s32 Actor_AimAtWatched(struct Actor *actor) {
  struct Actor *target = &actor->watchedActor->actor;

  actor->bounce = (target->x.fixed - actor->x.fixed) / AIM_STEPS;
  actor->sceneWord =
      (target->y.fixed - actor->y.fixed + toFIXED(16)) / AIM_STEPS;
  actor->gravity = (target->z.fixed - actor->z.fixed) / AIM_STEPS;
  actor->sceneCounter = AIM_STEPS;
  return 0;
}

#undef AIM_STEPS
