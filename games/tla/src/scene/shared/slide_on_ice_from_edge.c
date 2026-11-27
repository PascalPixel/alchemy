// The leader slides over ice from the metatile they stand on: from its centre in the direction the
// pad holds, or else from its edge toward the centre, then metatile by metatile until a wall, a
// raised metatile or another actor stops them.
void scLeaderSlideOnIceFromEdge(void)
{
	ACTOR  *actor;
	Sint32  startMetatile;
	FIXED   targetX;
	FIXED   targetZ;
	FIXED   x;
	FIXED   z;
	Uint8  *layer;
	Sint32  radius;
	Sint32  pos[3];
	FIXED   dx, dz;
	Sint32  metatile;
	Sint16  angle;
	Sint32  i;
	Sint32  j;
	ACTOR  *entry;
	Uint8  *flags;
	Sint32 *q;

	// SlideOnIceStopped is volatile: an event of the VBlank pass sets it while the slide waits
	// (scLeaderSetSlideStopped), and each test reads it again.
	actor             = fdActorLookup(MainRoutineStateWork.leaderActorId);
	SlideOnIceStopped = 0;
retry:
	fdLeaderStopIfPending();
	actor->speedLimit   = toFIXED(1);
	actor->acceleration = toFIXED(1) / 2;
	actor->targetX      = ACTOR_NO_TARGET;
	actor->targetZ      = ACTOR_NO_TARGET;
	actor->velocityX    = 0;
	actor->velocityZ    = 0;
	pos[0] = x = (actor->x.fixed & FIELD_METATILE_MASK) + toFIXED(8);
	pos[1]     = actor->y.fixed;
	pos[2] = z    = (actor->z.fixed & FIELD_METATILE_MASK) + toFIXED(8);
	targetZ       = z;
	targetX       = x;
	layer         = &actor->layer;
	startMetatile = fdMapGridGetMetatileBehaviorAt(*layer, targetX, targetZ);
	dz            = targetZ - actor->z.fixed;
	dx            = targetX - actor->x.fixed;
	if (syMulFX(dx, dx) + syMulFX(dz, dz) < toFIXED(4))
	{
		angle = SlideOnIceAnglesByKey[DPAD_INDEX(PerWork.data)];
		// -1 for no direction, tested as its halfword.
		if ((Uint16)angle == DPAD_ANGLE_NONE)
#if !ICE_SLIDE_ENDS_CLEANLY
			return;
#else
			goto end;
#endif
	}
	else
	{
		angle = syAtan(targetZ - actor->z.fixed, targetX - actor->x.fixed);
		angle = FACING_NEAREST_CARDINAL(angle);
	}
	syFieldVectorAddPolarOffset(16, angle, pos);
	metatile = fdMapGridGetMetatileBehaviorAt(*layer, pos[0], pos[2]);
	if (metatile == METATILE_BEHAVIOR_WALL
	    || fdMapGetTerrainHeight(*layer, pos[0], pos[2]) - actor->y.fixed > toFIXED(8))
	{
		actor->facing    = angle;
		actor->velocityX = 0;
		actor->velocityZ = 0;
		actor->x.fixed   = x;
		actor->z.fixed   = z;
		goto end;
	}
	pos[0]              = x;
	pos[2]              = z;
	actor->speedLimit   = toFIXED(2);
	actor->acceleration = toFIXED(1) / 10;
	actor->timer        = 0;
	fdActorSetMoveTarget(actor, x, actor->y.fixed, z);
	fdActorSelectAnimation(actor, ANIM_WALK);
	fdActorSetAnimationSpeed(actor, ANIM_STEP_NORMAL * 3);
#if !ICE_SLIDE_ENDS_CLEANLY
	scFieldActorWaitForMove(actor);
#else
	for (i = 0; i <= 179 && fdActorHasArrived(actor) == 0; i++)
	{
		if (SlideOnIceStopped != 0)
			goto end;
		syWait(1);
	}
#endif
	actor->update = (OBJECT_UPDATE)fdActorSteerByPad;
	for (;;)
	{
		syFieldVectorAddPolarOffset(16, angle, pos);
		q        = pos;
		metatile = fdMapGridGetMetatileBehaviorAt(*layer, q[0], q[2]);
		if (metatile == METATILE_BEHAVIOR_WALL)
			break;
		if (fdMapGetTerrainHeight(*layer, q[0], q[2]) - actor->y.fixed > toFIXED(8))
			goto blocked;
		if (SlideOnIceStopped != 0)
#if !ICE_SLIDE_ENDS_CLEANLY
			goto blocked;
#else
			goto end;
#endif
		radius = actor->radius - 2;
		entry  = RamHeapSlots->actors;
		flags  = &entry->collisionFlags;
		for (j = 0; j < ACTOR_COUNT; j++, flags += sizeof(*entry), entry++)
		{
			if (entry->script != NULL && (ACTOR_SOLID & *flags) && entry != actor)
			{
				if (scScriptCheckSpheresOverlap(&entry->x.fixed, radius, pos, entry->radius - 2)
				    >= 0)
					goto blocked;
			}
		}
		targetX             = pos[0];
		targetZ             = pos[2];
		actor->speedLimit   = toFIXED(2);
		actor->acceleration = toFIXED(1) / 10;
		fdActorSetMoveTarget(actor, pos[0], pos[1], pos[2]);
		for (i = 0; i <= 179 && fdActorHasArrived(actor) == 0 && SlideOnIceStopped == 0; i++)
			syWait(1);
		if (SlideOnIceStopped != 0)
			goto end;
		if (metatile != startMetatile)
			goto changed;
	}
blocked:
	actor->speedLimit   = toFIXED(2);
	actor->acceleration = toFIXED(1);
	fdActorSetMoveTarget(actor, targetX, actor->y.fixed, targetZ);
	scFieldActorWaitForMove(actor);
	syWait(2);
	goto retry;
changed:
	syWait(10);
end:
	actor->update         = NULL;
	actor->steeringFlags |= ACTOR_FACES_MOTION;
	actor->acceleration   = toFIXED(1) / 4;
	fdActorSelectAnimation(actor, ANIM_STAND);
}
