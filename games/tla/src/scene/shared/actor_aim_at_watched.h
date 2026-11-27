/* The steps an aimed actor takes to reach the actor it watches. */
#define AIM_STEPS 10

/* Aims an actor at the actor it watches, 16 pixels above its feet, to reach it
   in AIM_STEPS steps of scActorStepToWatched. */
Sint32 scActorAimAtWatched(ACTOR *actor)
{
	ACTOR *tgt = &actor->watchedActor->actor;

	actor->bounce = (tgt->x.fixed - actor->x.fixed) / AIM_STEPS;
	actor->sceneWord =
		(tgt->y.fixed - actor->y.fixed + toFIXED(16)) / AIM_STEPS;
	actor->gravity = (tgt->z.fixed - actor->z.fixed) / AIM_STEPS;
	actor->sceneCnt = AIM_STEPS;
	return 0;
}

#undef AIM_STEPS
