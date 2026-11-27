// The script call at the end of an attached actor's run (scActorAttach): it launches the actor it
// carries (fdActorLaunchAhead), then is put away at the origin, at rest.
Sint32 scActorPark(ACTOR *actor)
{
	fdActorLaunchAhead(actor);
	SET_ACTOR_POSITION(actor, 0, 0, 0);
	SET_VELOCITY(actor, 0, 0, 0);
	return 0;
}
