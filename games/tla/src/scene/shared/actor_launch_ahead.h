/* Launches the actor this one carries (scActorAttach) 32 pixels ahead of it, */
/* the way `userValue` faces, rolling at up to 8 pixels a frame */
/* (scActorRollAndCrumble), with the launch sound. */
void fdActorLaunchAhead(ACTOR *self) {
	ACTOR *actor = fdActorGet(self->launchedActorId);
	s32 angle = self->userValue;
	FIELD_SPR *sprite = actor->spr;
	FIXED position[3];

	position[0] = self->x.fixed;
	position[1] = self->y.fixed - toFIXED(12);
	position[2] = self->z.fixed;
	syFieldVectorAddPolarOffset(32, angle, position);
	actor->facing = angle;
	actor->motionFlg = 0;
	scFieldActorSetPosAndResetMotion(actor, position[0], position[1],
									 position[2]);
	actor->update = (OBJECT_UPDATE)scActorRollAndCrumble;
	actor->steeringFlg = 0;
	actor->speedLimit = toFIXED(8);
	actor->acceleration = toFIXED(1) / 8;
	sprite->rotation = angle - DEGtoANG(90);
	actor->scaleX = 0;
	actor->scaleY = 0;
	actor->tmr = 0;
	actor->watchedActor = self->watchedActor;
	fdEventPlaySnd(SE_LAUNCH);
}
