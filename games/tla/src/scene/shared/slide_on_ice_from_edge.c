/* The leader slides over ice from the metatile they stand on: from its centre
   in the direction the pad holds, or else from its edge toward the centre, then
   metatile by metatile until a wall, a raised metatile or another actor stops
   them. */
void scLeaderSlideOnIceFromEdge(void)
{
	ACTOR  *actor;
	Sint32	startMetatile;
	FIXED	targetX;
	FIXED	targetZ;
	FIXED	x;
	FIXED	z;
	Uint8  *layer;
	Sint32	radius;
	Sint32	position[3];
	FIXED	dx, dz;
	Sint32	metatile;
	Sint16	angle;
	Sint32	i;
	Sint32	j;
	ACTOR  *entry;
	Uint8  *flags;
	Sint32 *q;

	/* SlideOnIce_Stopped is volatile: an event of the VBlank pass sets it while
	   the slide waits (scLeaderSetSlideStopped), and each test reads it
	   again. */
	actor = fdActorLookup(MainRoutineStateWork.leaderActorId);
	SlideOnIce_Stopped = 0;
retry:
	fdLeaderStopIfPending();
	actor->speedLimit = toFIXED(1);
	actor->acceleration = toFIXED(1) / 2;
	actor->tgtX = ACTOR_NO_TARGET;
	actor->tgtZ = ACTOR_NO_TARGET;
	actor->velX = 0;
	actor->velZ = 0;
	position[0] = x = (actor->x.fixed & FIELD_METATILE_MASK) + toFIXED(8);
	position[1] = actor->y.fixed;
	position[2] = z = (actor->z.fixed & FIELD_METATILE_MASK) + toFIXED(8);
	targetZ = z;
	targetX = x;
	layer = &actor->layer;
	startMetatile = fdMapGridGetMetatileBehaviorAt(*layer, targetX, targetZ);
	dz = targetZ - actor->z.fixed;
	dx = targetX - actor->x.fixed;
	if(syMulFX(dx, dx) + syMulFX(dz, dz) < toFIXED(4)) {
		angle = SlideOnIce_AnglesByKey[DPAD_INDEX(PerWork.data)];
		/* -1 for no direction, tested as its halfword. */
		if((Uint16)angle == DPAD_ANGLE_NONE)
#if !ICE_SLIDE_ENDS_CLEANLY
			return;
#else
			goto end;
#endif
	} else {
		angle = syAtan(targetZ - actor->z.fixed, targetX - actor->x.fixed);
		angle = FACING_NEAREST_CARDINAL(angle);
	}
	syFieldVectorAddPolarOffset(16, angle, position);
	metatile = fdMapGridGetMetatileBehaviorAt(*layer, position[0], position[2]);
	if(metatile == METATILE_BEHAVIOR_WALL ||
		fdMapGetTerrainHeight(*layer, position[0], position[2]) -
				actor->y.fixed >
			toFIXED(8)) {
		actor->facing = angle;
		actor->velX = 0;
		actor->velZ = 0;
		actor->x.fixed = x;
		actor->z.fixed = z;
		goto end;
	}
	position[0] = x;
	position[2] = z;
	actor->speedLimit = toFIXED(2);
	actor->acceleration = toFIXED(1) / 10;
	actor->tmr = 0;
	fdActorSetMoveTarget(actor, x, actor->y.fixed, z);
	fdActorSelectAnimation(actor, ANIM_WALK);
	fdActorSetAnimationSpeed(actor, ANIM_STEP_NORMAL * 3);
#if !ICE_SLIDE_ENDS_CLEANLY
	scFieldActorWaitForMove(actor);
#else
	for(i = 0; i <= 179 && fdActorHasArrived(actor) == 0; i++) {
		if(SlideOnIce_Stopped != 0)
			goto end;
		syWait(1);
	}
#endif
	actor->update = (OBJECT_UPDATE)fdActorSteerByPad;
	for(;;) {
		syFieldVectorAddPolarOffset(16, angle, position);
		q = position;
		metatile = fdMapGridGetMetatileBehaviorAt(*layer, q[0], q[2]);
		if(metatile == METATILE_BEHAVIOR_WALL)
			break;
		if(fdMapGetTerrainHeight(*layer, q[0], q[2]) - actor->y.fixed >
			toFIXED(8))
			goto blocked;
		if(SlideOnIce_Stopped != 0)
#if !ICE_SLIDE_ENDS_CLEANLY
			goto blocked;
#else
			goto end;
#endif
		radius = actor->radius - 2;
		entry = Ram_HeapSlots->actors;
		flags = &entry->collisionFlg;
		for(j = 0; j < ACTOR_COUNT; j++, flags += sizeof(*entry), entry++) {
			if(entry->script != NULL && (ACTOR_SOLID & *flags) &&
				entry != actor) {
				if(scScriptCheckSpheresOverlap(&entry->x.fixed, radius,
					   position, entry->radius - 2) >= 0)
					goto blocked;
			}
		}
		targetX = position[0];
		targetZ = position[2];
		actor->speedLimit = toFIXED(2);
		actor->acceleration = toFIXED(1) / 10;
		fdActorSetMoveTarget(actor, position[0], position[1], position[2]);
		for(i = 0; i <= 179 && fdActorHasArrived(actor) == 0 &&
			SlideOnIce_Stopped == 0;
			i++)
			syWait(1);
		if(SlideOnIce_Stopped != 0)
			goto end;
		if(metatile != startMetatile)
			goto changed;
	}
blocked:
	actor->speedLimit = toFIXED(2);
	actor->acceleration = toFIXED(1);
	fdActorSetMoveTarget(actor, targetX, actor->y.fixed, targetZ);
	scFieldActorWaitForMove(actor);
	syWait(2);
	goto retry;
changed:
	syWait(10);
end:
	actor->update = NULL;
	actor->steeringFlg |= ACTOR_FACES_MOTION;
	actor->acceleration = toFIXED(1) / 4;
	fdActorSelectAnimation(actor, ANIM_STAND);
}
