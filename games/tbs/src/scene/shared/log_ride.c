/* @file */
/* Fuchin Falls and Vale's cave: the logs that float the leader across a pool.
 */

/* Floats the log actor id and the leader standing on it by (dx, dz) pixels, */
/* to the centres of their cells, at a pixel a frame. The leader is drawn in */
/* ANIM_LEADER_WALK_BACKWARDS and the log in its sheet's animation 4 going */
/* north or west, 3 going south or east. */
static void scActorFloatLogWithLeader(s32 id, s32 dx, s32 dz) {
	ACTOR *leader;
	ACTOR *raft;
	FIXED x;
	FIXED z;

	leader = fdActorGetById(MainRoutineStateWork.leaderActorId);
	raft = fdActorGetById(id);
	fdEventStart();
	x = ((leader->x.fixed + INTtoFIXED(dx)) & FIELD_CELL_MASK) + toFIXED(8);
	z = ((leader->z.fixed + INTtoFIXED(dz)) & FIELD_CELL_MASK) + toFIXED(8);

	leader->speedLimit = toFIXED(1);
	leader->acceleration = toFIXED(1) / 2;
	syFieldActorSetMoveTargetFX(leader, x, leader->y.fixed, z);
	fdActorSelectAnimation(leader, ANIM_LEADER_WALK_BACKWARDS);
	x = ((raft->x.fixed + INTtoFIXED(dx)) & FIELD_CELL_MASK) + toFIXED(8);
	z = ((raft->z.fixed + INTtoFIXED(dz)) & FIELD_CELL_MASK) + toFIXED(8);

	raft->speedLimit = toFIXED(1);
	raft->acceleration = toFIXED(1) / 2;
	syFieldActorSetMoveTargetFX(raft, x, raft->y.fixed, z);
	if(dx < 0 || dz < 0) {
		fdActorSelectAnimation(raft, 4);
	}
	else {
		fdActorSelectAnimation(raft, 3);
	}
	fdActorWaitForMove(leader);
	fdEventEnd();
}
