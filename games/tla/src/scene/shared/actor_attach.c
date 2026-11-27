/* Puts actor `id` on actor `srcId` and starts its script; at the script's
   end it launches actor `launchedId` ahead of it, facing `launchFacing`
   (scActorPark). */
void scActorAttach(Sint32 srcId, Sint32 id, Sint32 launchedId,
	Sint32 launchFacing, const void *script)
{
	ACTOR *src = fdActorGet(srcId);
	ACTOR *actor = fdActorGet(id);

	scFieldActorSetPosAndResetMotion(
		actor, src->x.fixed, src->y.fixed, src->z.fixed);
	fdActorStartScript(actor, script);
	fdActorSetSpriteFlags(actor, 0);
	actor->motionFlags = 0;
	fdActorSelectAnimation(actor, 1);
	actor->watchedActor = (FIELD_OBJECT *)src;
	actor->timer = 0;
	actor->userValue = launchFacing;
	actor->launchedActorId = launchedId;
	actor->update = scActorHoldDuringScript;
}
