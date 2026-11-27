/* Puts actor `id` on actor `sourceId` and starts its script; at the script's */
/* end it launches actor `launchedId` ahead of it, facing `launchFacing` */
/* (fdActorPark). */
void scActorAttach(s32 sourceId, s32 id, s32 launchedId, s32 launchFacing,
				   const void *script) {
	ACTOR *source = fdActorGet(sourceId);
	ACTOR *actor = fdActorGet(id);

	scFieldActorSetPosAndResetMotion(actor, source->x.fixed, source->y.fixed,
									 source->z.fixed);
	fdActorStartScript(actor, script);
	fdActorSetSprFlags(actor, 0);
	actor->motionFlg = 0;
	fdActorSelectAnimation(actor, 1);
	actor->watchedActor = (FIELD_OBJECT *)source;
	actor->tmr = 0;
	actor->userValue = launchFacing;
	actor->launchedActorId = launchedId;
	actor->update = scActorHoldDuringEvent;
}
