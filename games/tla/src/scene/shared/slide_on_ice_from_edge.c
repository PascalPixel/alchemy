/* The leader slides over ice from the cell they stand on: from its centre in */
/* the direction the pad holds, or else from its edge toward the centre, then */
/* cell by cell until a wall, a raised cell or another actor stops them. */
void scLeaderSlideOnIceFromEdge(void) {
	ACTOR *actor;
	s32 startCell;
	FIXED targetX;
	FIXED targetZ;
	FIXED x;
	FIXED z;
	u8 *layer;
	s32 radius;
	s32 position[3];
	FIXED dx, dz;
	s32 cell;
	s16 angle;
	s32 i;
	s32 j;
	ACTOR *entry;
	u8 *flags;
	s32 *q;

	/* SlideOnIce_Stopped is volatile: a task of the VBlank pass sets it while
	 */
	/* the slide waits (scLeaderSetSlideStopped), and each test reads it again.
	 */
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
	position[0] = x = (actor->x.fixed & FIELD_CELL_MASK) + toFIXED(8);
	position[1] = actor->y.fixed;
	position[2] = z = (actor->z.fixed & FIELD_CELL_MASK) + toFIXED(8);
	targetZ = z;
	targetX = x;
	layer = &actor->layer;
	startCell = fdMapGridGetCellBehaviorAt(*layer, targetX, targetZ);
	dz = targetZ - actor->z.fixed;
	dx = targetX - actor->x.fixed;
	if(syMulFX(dx, dx) + syMulFX(dz, dz) < toFIXED(4)) {
		angle = SlideOnIce_AnglesByKey[DPAD_INDEX(InputWork.held)];
		/* -1 for no direction, tested as its halfword. */
		if((u16)angle == DPAD_ANGLE_NONE)
#if !TLA_PATCH_1
			return;
#else
			goto end;
#endif
	}
	else {
		angle = syAtan(targetZ - actor->z.fixed, targetX - actor->x.fixed);
		angle = FACING_NEAREST_CARDINAL(angle);
	}
	syFieldVectorAddPolarOffset(16, angle, position);
	cell = fdMapGridGetCellBehaviorAt(*layer, position[0], position[2]);
	if(cell == CELL_BEHAVIOR_WALL ||
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
#if !TLA_PATCH_1
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
		cell = fdMapGridGetCellBehaviorAt(*layer, q[0], q[2]);
		if(cell == CELL_BEHAVIOR_WALL)
			break;
		if(fdMapGetTerrainHeight(*layer, q[0], q[2]) - actor->y.fixed >
		   toFIXED(8))
			goto blocked;
		if(SlideOnIce_Stopped != 0)
#if !TLA_PATCH_1
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
				if(scEventCheckSpheresOverlap(&entry->x.fixed, radius, position,
											  entry->radius - 2) >= 0)
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
		if(cell != startCell)
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
