/* Pushes the block ahead onto another, if nothing blocks the way. */
void scMagmaRockInsidePushBlockOnto(void) {
	POS dst;
	ACTOR *lead;
	ACTOR *next;
	ACTOR *blocker;
	s32 dir;
	u32 step;
	s32 rate;

	lead = fdActorGetById(MainRoutineStateWork.leaderActorId);
	dir = lead->facing / FACING_STEP;
	/* A push step holds x pixels in its high half and z in its low (PUSH_STEP).
	 */
	step = PushLeaderSteps[dir];
	dst.x = lead->x.fixed + (step & FIELD_PIXEL_MASK);
	dst.y = lead->y.fixed;
	step = INTtoFIXED(step);
	dst.z = lead->z.fixed + step;
	next = fdActorFindInCell(&dst, lead);
	if(next == 0)
		return;

	step = PushLeaderSteps[dir];
	dst.x = next->x.fixed + (step & FIELD_PIXEL_MASK);
	dst.y = next->y.fixed;
	step = INTtoFIXED(step);
	dst.z = next->z.fixed + step;
	blocker = fdActorFindInCell(&dst, next);
	if(blocker != 0 && (blocker->collisionFlg & ACTOR_SOLID) != 0)
		return;

	dst.x = next->x.fixed;
	dst.y = next->y.fixed + toFIXED(16);
	dst.z = next->z.fixed;
	blocker = fdActorFindInCell(&dst, next);
	if(blocker != 0 && (blocker->collisionFlg & ACTOR_SOLID) != 0)
		return;

	next->layer = MAPGRID_LAYER_OVERHEAD;
	step = PushLeaderSteps[dir];
	dst.x = next->x.fixed + (step & FIELD_PIXEL_MASK);
	dst.y = next->y.fixed;
	step = INTtoFIXED(step);
	dst.z = next->z.fixed + step;
	if(fdMapGridCheckCollision(next, &dst) > 0)
		return;

	if(next->sceneCnt != 0)
		return;

	fdActorSelectAnimation(lead, ANIM_PUSH);
	rate = toFIXED(0.2);
	syWait(15);
	fdEventPlaySnd(SE_PUSH);
	next->speedLimit = rate;
	next->acceleration = rate;
	scFieldActorSetMoveTargetFX(next, dst.x, dst.y, dst.z);
	lead->speedLimit = rate;
	lead->acceleration = rate;
	scFieldActorSetMoveTargetFX(lead, dst.x, dst.y, dst.z);
	scFieldActorWaitForMove(next);
	fdEventNoOp();
	next->x.fixed = dst.x;
	next->z.fixed = dst.z;
	next->velX = 0;
	next->velZ = 0;
	lead->tgtX = ACTOR_NO_TARGET;
	lead->tgtZ = ACTOR_NO_TARGET;
	lead->velX = 0;
	lead->velZ = 0;
	lead->x.fixed = INTtoFIXED(lead->x.part[0].integer);
	lead->z.fixed = INTtoFIXED(lead->z.part[0].integer);
	fdActorSelectAnimation(lead, ANIM_STAND);
}
