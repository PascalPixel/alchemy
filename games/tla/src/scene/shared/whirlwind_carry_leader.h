/* A whirlwind blown out of a wall catches Felix: he spins inside it, lifted a
   little, as it carries him along, and when it dies away he drops to the ground
   beside where it stopped with a thud, and the camera follows him again. */
void fdWhirlwindCarryLeader(void)
{
	FIELD_STATE *state = RamHeapSlots->fieldState;
	ACTOR		*actor = fdActorGet(MainRoutineStateWork.leaderActorId);
	ACTOR		*linked = &actor->watchedActor->actor;
	ACTOR		*shadow;
	Uint8		*p;
	Sint32		 ang;
	Sint32		 rise;
	Sint32		 frm;

	fdScriptStart();
	syFieldScriptEndLastingPsynergy(0);
	rise = 0;
	state->glowHeld = rise;

	actor->motionFlg = ACTOR_FLOATS;
	fdActorSetSprFlags(actor, 0);

	while(*(Uint8 *)&linked->sceneByte != 0) {
		actor->x.fixed = linked->x.fixed;
		actor->y.fixed = linked->y.fixed + rise;
		actor->z.fixed = linked->z.fixed;
		if(rise < toFIXED(4))
			rise += toFIXED(0.1);
		actor->facing = (Uint8)TickCount * FACING_STEP;
		/* As he spins he is drawn now under the wind, now over it. */
		p = &actor->priFlg;
		*p ^= ACTOR_PRIORITY_UNDERFOOT << (TickCount & 1);
		sySynch(1);
	}
	state->glow = 1;
	state->glowHeld = 1;
	ang = linked->facing;
	actor->x.fixed =
		FIELD_HALF_METATILE_ORIGIN((FIELD_HALF_METATILE(linked->x.fixed) & ~1) +
			FACING_EIGHTH_INDEX(ang) - 1);
	actor->z.fixed =
		FIELD_HALF_METATILE_ORIGIN((FIELD_HALF_METATILE(linked->z.fixed) & ~1) -
			(FACING_EIGHTH_INDEX(ang) & 2) + 1);
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
	frm = 0;
	if(actor->y.fixed > actor->groundHeight + toFIXED(16)) {
		do {
			sySynch(1);
			frm++;
			if(frm >= 60)
				break;
		} while(actor->y.fixed > actor->groundHeight + toFIXED(16));
	}
	fdScriptPlaySnd(SE_LANDING_THUD);
	frm = 0;
	if(actor->velY != 0) {
		do {
			sySynch(1);
			frm++;
			if(frm >= 60)
				break;
		} while(actor->velY != 0);
	}
	scFieldActorWaitForMove(shadow);
	fdActorGet(MainRoutineStateWork.leaderActorId)->priFlg |=
		ACTOR_PRIORITY_AUTOMATIC;
	syScriptCameraFollowActor(MainRoutineStateWork.leaderActorId, 1);
	state->glowHeld = 1;
	state->stepCue = 0;
	fdScriptEnd();
}
