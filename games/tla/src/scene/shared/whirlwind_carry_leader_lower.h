/* The leader spins in the whirlwind that caught it, then drops beside it and
   the camera follows again: the lighthouse's lower rooms' copy of
   fdWhirlwindCarryLeader. */
void scWhirlwindCarryLeaderLower(void)
{
	FIELD_STATE *state = RamHeapSlots->fieldState;
	ACTOR		*actor = fdActorGet(MainRoutineStateWork.leaderActorId);
	ACTOR		*linked = &actor->watchedActor->actor;
	ACTOR		*shadow;
	Sint32		 facing;
	Sint32		 i;

	fdScriptStart();
	syFieldScriptEndLastingPsynergy(0);
	i = 0;
	state->glowHeld = 0;
	actor->motionFlg = ACTOR_FLOATS;
	fdActorSetSprFlags(actor, 0);
	while(*(Uint8 *)&linked->sceneByte != 0) {
		actor->x.fixed = linked->x.fixed;
		actor->y.fixed = linked->y.fixed + i;
		actor->z.fixed = linked->z.fixed;
		if(i <= toFIXED(4) - 1)
			i += toFIXED(0.1);
		actor->facing = (Uint8)TickCount * FACING_STEP;
		actor->priFlg ^= ACTOR_PRIORITY_UNDERFOOT << (TickCount & 1);
		syWait(1);
	}
	state->glow = 1;
	state->glowHeld = 1;
	facing = linked->facing;
	actor->x.fixed =
		FIELD_HALF_METATILE_ORIGIN((FIELD_HALF_METATILE(linked->x.fixed) & ~1) +
			FACING_EIGHTH_INDEX(facing) - 1);
	actor->z.fixed =
		FIELD_HALF_METATILE_ORIGIN((FIELD_HALF_METATILE(linked->z.fixed) & ~1) -
			(FACING_EIGHTH_INDEX(facing) & 2) + 1);
	actor->velY = toFIXED(6);
	actor->priFlg = ACTOR_PRIORITY_AUTOMATIC;
	actor->motionFlg = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
	fdActorSetSprFlags(actor, 1);
	fdCameraMoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
	shadow = syScriptCameraGetActor();
	shadow->speedLimit = toFIXED(8);
	shadow->acceleration = toFIXED(1);
	shadow->motionFlg = 0;
	scFieldActorSetMoveTargetFX(shadow, actor->x.fixed,
		fdMapGetTerrainHeight(0, actor->x.fixed, actor->z.fixed),
		actor->z.fixed);
	i = 0;
	while(i <= 59 && actor->y.fixed > actor->groundHeight + toFIXED(16)) {
		i++;
		syWait(1);
	}
	fdScriptPlaySnd(SE_LANDING_THUD);
	i = 0;
	while(i <= 59 && actor->velY != 0) {
		i++;
		syWait(1);
	}
	scFieldActorWaitForMove(shadow);
	fdActorGet(MainRoutineStateWork.leaderActorId)->priFlg |=
		ACTOR_PRIORITY_AUTOMATIC;
	syScriptCameraFollowActor(MainRoutineStateWork.leaderActorId, 1);
	state->glowHeld = 1;
	state->stepCue = 0;
	fdScriptEnd();
}
