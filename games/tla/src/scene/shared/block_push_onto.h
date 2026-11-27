// Pushes the block ahead onto another, if nothing blocks the way.
void scMagmaRockInsidePushBlockOnto(void)
{
	POS    dest;
	ACTOR *lead;
	ACTOR *next;
	ACTOR *blocker;
	Sint32 direction;
	Uint32 step;
	Sint32 rate;

	lead      = fdActorGetById(MainRoutineStateWork.leaderActorId);
	direction = lead->facing / FACING_STEP;
	// A push step holds x pixels in its high half and z in its low (PUSH_STEP).
	step      = PushLeaderSteps[direction];
	dest.x    = lead->x.fixed + (step & FIELD_PIXEL_MASK);
	dest.y    = lead->y.fixed;
	step      = toFIXED(step);
	dest.z    = lead->z.fixed + step;
	next      = fdActorFindInMetatile(&dest, lead);
	if (next == 0)
		return;

	step    = PushLeaderSteps[direction];
	dest.x  = next->x.fixed + (step & FIELD_PIXEL_MASK);
	dest.y  = next->y.fixed;
	step    = toFIXED(step);
	dest.z  = next->z.fixed + step;
	blocker = fdActorFindInMetatile(&dest, next);
	if (blocker != 0 && (blocker->collisionFlags & ACTOR_SOLID) != 0)
		return;

	dest.x  = next->x.fixed;
	dest.y  = next->y.fixed + toFIXED(16);
	dest.z  = next->z.fixed;
	blocker = fdActorFindInMetatile(&dest, next);
	if (blocker != 0 && (blocker->collisionFlags & ACTOR_SOLID) != 0)
		return;

	next->layer = MAPGRID_LAYER_OVERHEAD;
	step        = PushLeaderSteps[direction];
	dest.x      = next->x.fixed + (step & FIELD_PIXEL_MASK);
	dest.y      = next->y.fixed;
	step        = toFIXED(step);
	dest.z      = next->z.fixed + step;
	if (fdMapGridCheckCollision(next, &dest) > 0)
		return;

	if (next->sceneCount != 0)
		return;

	fdActorSelectAnimation(lead, ANIM_PUSH);
	rate = toFIXED(0.2);
	syWait(15);
	fdScriptPlaySound(SE_PUSH);
	next->speedLimit   = rate;
	next->acceleration = rate;
	scFieldActorSetMoveTargetFX(next, dest.x, dest.y, dest.z);
	lead->speedLimit   = rate;
	lead->acceleration = rate;
	scFieldActorSetMoveTargetFX(lead, dest.x, dest.y, dest.z);
	scFieldActorWaitForMove(next);
	fdScriptNoOperation();
	next->x.fixed   = dest.x;
	next->z.fixed   = dest.z;
	next->velocityX = 0;
	next->velocityZ = 0;
	lead->targetX   = ACTOR_NO_TARGET;
	lead->targetZ   = ACTOR_NO_TARGET;
	lead->velocityX = 0;
	lead->velocityZ = 0;
	lead->x.fixed   = toFIXED(lead->x.part[0].integer);
	lead->z.fixed   = toFIXED(lead->z.part[0].integer);
	fdActorSelectAnimation(lead, ANIM_STAND);
}
