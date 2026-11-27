/* The leader slides over ice from the metatile they stand on: from its centre
   in the direction the pad holds, or else from its edge toward the centre, then
   metatile by metatile until a wall, a raised metatile or another actor stops
   them. */
void scLeaderSlideOnIceFromEdge(void)
{
	ACTOR  *actor;
	Sint32	startMetatile;
	FIXED	tgtX;
	FIXED	tgtZ;
	FIXED	x;
	FIXED	z;
	Uint8  *layer;
	Sint32	radius;
	Sint32	pos[3];
	FIXED	dx, dz;
	Sint32	metatile;
	Sint16	ang;
	Sint32	i;
	Sint32	j;
	ACTOR  *entry;
	Uint8  *flg;
	Sint32 *q;

	/* SlideOnIceStopped is volatile: an event of the VBlank pass sets it while
	   the slide waits (scLeaderSetSlideStopped), and each test reads it
	   again. */
	actor = fdActorLookup(MainRoutineStateWork.leaderActorId);
	SlideOnIceStopped = 0;
retry:
	fdLeaderStopIfPending();
	actor->speedLimit = toFIXED(1);
	actor->acceleration = toFIXED(1) / 2;
	actor->tgtX = ACTOR_NO_TARGET;
	actor->tgtZ = ACTOR_NO_TARGET;
	actor->velX = 0;
	actor->velZ = 0;
	pos[0] = x = (actor->x.fixed & FIELD_METATILE_MASK) + toFIXED(8);
	pos[1] = actor->y.fixed;
	pos[2] = z = (actor->z.fixed & FIELD_METATILE_MASK) + toFIXED(8);
	tgtZ = z;
	tgtX = x;
	layer = &actor->layer;
	startMetatile = fdMapGridGetMetatileBehaviorAt(*layer, tgtX, tgtZ);
	dz = tgtZ - actor->z.fixed;
	dx = tgtX - actor->x.fixed;
	if(syMulFX(dx, dx) + syMulFX(dz, dz) < toFIXED(4)) {
		ang = SlideOnIceAnglesByKey[DPAD_INDEX(PerWork.data)];
		/* -1 for no direction, tested as its halfword. */
		if((Uint16)ang == DPAD_ANGLE_NONE)
#if !ICE_SLIDE_ENDS_CLEANLY
			return;
#else
			goto end;
#endif
	} else {
		ang = syAtan(tgtZ - actor->z.fixed, tgtX - actor->x.fixed);
		ang = FACING_NEAREST_CARDINAL(ang);
	}
	syFieldVectorAddPolarOffset(16, ang, pos);
	metatile = fdMapGridGetMetatileBehaviorAt(*layer, pos[0], pos[2]);
	if(metatile == METATILE_BEHAVIOR_WALL ||
		fdMapGetTerrainHeight(*layer, pos[0], pos[2]) - actor->y.fixed >
			toFIXED(8)) {
		actor->facing = ang;
		actor->velX = 0;
		actor->velZ = 0;
		actor->x.fixed = x;
		actor->z.fixed = z;
		goto end;
	}
	pos[0] = x;
	pos[2] = z;
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
		if(SlideOnIceStopped != 0)
			goto end;
		syWait(1);
	}
#endif
	actor->update = (OBJECT_UPDATE)fdActorSteerByPad;
	for(;;) {
		syFieldVectorAddPolarOffset(16, ang, pos);
		q = pos;
		metatile = fdMapGridGetMetatileBehaviorAt(*layer, q[0], q[2]);
		if(metatile == METATILE_BEHAVIOR_WALL)
			break;
		if(fdMapGetTerrainHeight(*layer, q[0], q[2]) - actor->y.fixed >
			toFIXED(8))
			goto blocked;
		if(SlideOnIceStopped != 0)
#if !ICE_SLIDE_ENDS_CLEANLY
			goto blocked;
#else
			goto end;
#endif
		radius = actor->radius - 2;
		entry = RamHeapSlots->actors;
		flg = &entry->collisionFlg;
		for(j = 0; j < ACTOR_COUNT; j++, flg += sizeof(*entry), entry++) {
			if(entry->script != NULL && (ACTOR_SOLID & *flg) &&
				entry != actor) {
				if(scScriptCheckSpheresOverlap(
					   &entry->x.fixed, radius, pos, entry->radius - 2) >= 0)
					goto blocked;
			}
		}
		tgtX = pos[0];
		tgtZ = pos[2];
		actor->speedLimit = toFIXED(2);
		actor->acceleration = toFIXED(1) / 10;
		fdActorSetMoveTarget(actor, pos[0], pos[1], pos[2]);
		for(i = 0;
			i <= 179 && fdActorHasArrived(actor) == 0 && SlideOnIceStopped == 0;
			i++)
			syWait(1);
		if(SlideOnIceStopped != 0)
			goto end;
		if(metatile != startMetatile)
			goto changed;
	}
blocked:
	actor->speedLimit = toFIXED(2);
	actor->acceleration = toFIXED(1);
	fdActorSetMoveTarget(actor, tgtX, actor->y.fixed, tgtZ);
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
