// A whirlwind blown out of a wall catches Felix: he spins inside it, lifted a little, as it carries
// him along, and when it dies away he drops to the ground beside where it stopped with a thud, and
// the camera follows him again.
void fdWhirlwindCarryLeader(void)
{
	FIELD_STATE *state  = RamHeapSlots->fieldState;
	ACTOR       *actor  = fdActorGet(MainRoutineStateWork.leaderActorId);
	ACTOR       *linked = &actor->watchedActor->actor;
	ACTOR       *shadow;
	Uint8       *p;
	Sint32       angle;
	Sint32       rise;
	Sint32       frame;

	fdScriptStart();
	syFieldScriptEndLastingPsynergy(0);
	rise               = 0;
	state->glowHeld    = rise;

	actor->motionFlags = ACTOR_FLOATS;
	fdActorSetSpriteFlags(actor, 0);

	while (*(Uint8 *)&linked->sceneByte != 0)
	{
		actor->x.fixed = linked->x.fixed;
		actor->y.fixed = linked->y.fixed + rise;
		actor->z.fixed = linked->z.fixed;
		if (rise < toFIXED(4))
			rise += toFIXED(0.1);
		actor->facing  = (Uint8)TickCount * FACING_STEP;
		// As he spins he is drawn now under the wind, now over it.
		p              = &actor->priorityFlags;
		*p            ^= ACTOR_PRIORITY_UNDERFOOT << (TickCount & 1);
		sySynch(1);
	}
	state->glow          = 1;
	state->glowHeld      = 1;
	angle                = linked->facing;
	actor->x.fixed       = FIELD_HALF_METATILE_ORIGIN((FIELD_HALF_METATILE(linked->x.fixed) & ~1)
                                                + FACING_EIGHTH_INDEX(angle) - 1);
	actor->z.fixed       = FIELD_HALF_METATILE_ORIGIN((FIELD_HALF_METATILE(linked->z.fixed) & ~1)
                                                - (FACING_EIGHTH_INDEX(angle) & 2) + 1);
	actor->velocityY     = toFIXED(6);
	actor->priorityFlags = ACTOR_PRIORITY_AUTOMATIC;
	actor->motionFlags   = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
	fdActorSetSpriteFlags(actor, 1);
	fdCameraMoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
	shadow               = syScriptCameraGetActor();
	shadow->speedLimit   = toFIXED(8);
	shadow->acceleration = toFIXED(1);
	shadow->motionFlags  = 0;
	scFieldActorSetMoveTargetFX(shadow,
	                            actor->x.fixed,
	                            fdMapGetTerrainHeight(0, actor->x.fixed, actor->z.fixed),
	                            actor->z.fixed);
	frame = 0;
	if (actor->y.fixed > actor->groundHeight + toFIXED(16))
	{
		do
		{
			sySynch(1);
			frame++;
			if (frame >= 60)
				break;
		} while (actor->y.fixed > actor->groundHeight + toFIXED(16));
	}
	fdScriptPlaySound(SE_LANDING_THUD);
	frame = 0;
	if (actor->velocityY != 0)
	{
		do
		{
			sySynch(1);
			frame++;
			if (frame >= 60)
				break;
		} while (actor->velocityY != 0);
	}
	scFieldActorWaitForMove(shadow);
	fdActorGet(MainRoutineStateWork.leaderActorId)->priorityFlags |= ACTOR_PRIORITY_AUTOMATIC;
	syScriptCameraFollowActor(MainRoutineStateWork.leaderActorId, 1);
	state->glowHeld = 1;
	state->stepCue  = 0;
	fdScriptEnd();
}
