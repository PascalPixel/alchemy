/* @file */
/* The sites of the three Colosso finals matches (Colosseum Final 1, 2 and 3):
 */
/* the rocks, weights and pillars on a stage that Isaac pushes, and finding */
/* what stands ahead of him. */

/* The leader pushes the actor standing in the next cell ahead one cell on */
/* (a rock, a weight or a stone pillar on the stages), when nothing solid */
/* stands beyond it or on top of it and the ground lets it move, with the */
/* pushing pose and SE_PUSH_STEP. */
static void scColossoPushActorAhead(void) {
	ACTOR *leader;
	ACTOR *pushed;
	ACTOR *blocker;
	u32 step;
	u32 direction;
	FIELD_POS position;
	s32 leaderId;

	leaderId = MainRoutineStateWork.leaderActorId;
	leader = fdActorGet(leaderId);

	direction = FACING_INDEX(leader->facing);

	step = PushedActor_DirectionSteps[direction];
	position.x = leader->x.fixed + DIRECTION_STEP_X_FIXED(step);
	position.y = leader->y.fixed;
	step = DIRECTION_STEP_Z_FIXED(step);
	position.z = leader->z.fixed + step;

	pushed = scActorFindAtPos(&position, leader);
	if(pushed == NULL) {
		return;
	}

	step = PushedActor_DirectionSteps[direction];
	position.x = pushed->x.fixed + DIRECTION_STEP_X_FIXED(step);
	position.y = pushed->y.fixed;
	step = DIRECTION_STEP_Z_FIXED(step);
	position.z = pushed->z.fixed + step;

	blocker = scActorFindAtPos(&position, pushed);
	if(blocker != NULL && (blocker->collisionFlg & ACTOR_SOLID) != 0) {
		return;
	}

	position.x = pushed->x.fixed;
	position.y = pushed->y.fixed + toFIXED(16);
	position.z = pushed->z.fixed;

	blocker = scActorFindAtPos(&position, pushed);
	if(blocker != NULL && (blocker->collisionFlg & ACTOR_SOLID) != 0) {
		return;
	}

	pushed->layer = 2;

	step = PushedActor_DirectionSteps[direction];
	position.x = pushed->x.fixed + DIRECTION_STEP_X_FIXED(step);
	position.y = pushed->y.fixed;
	step = DIRECTION_STEP_Z_FIXED(step);
	position.z = pushed->z.fixed + step;

	if(syFieldActorCheckMovementCollision(pushed, &position) > 0) {
		return;
	}

	fdActorSelectAnimation(leader, ANIM_PUSH);
	sySynch(15);

	pushed->speedLimit = toFIXED(1) / 5;
	pushed->acceleration = toFIXED(1) / 5;
	syFieldActorSetMoveTargetFX(pushed, position.x, position.y, position.z);

	leader->speedLimit = toFIXED(1) / 5;
	leader->acceleration = toFIXED(1) / 5;
	syFieldActorSetMoveTargetFX(leader, position.x, position.y, position.z);

	sySndPlay(SE_PUSH_STEP);
	fdActorWaitForMove(pushed);
	sySndPlay(SE_STOP_ACTION_SE);

	pushed->x.fixed = position.x;
	pushed->z.fixed = position.z;
	pushed->velX = 0;
	pushed->velZ = 0;

	fdActorSelectAnimation(leader, ANIM_STAND);
}

/* Returns the placed actor standing one cell, or else two cells, ahead */
/* of the actor the player moves (straight ahead in the nearest of the */
/* four directions), or 0 when there is none. */
void *scColossoFindActorAhead(void) {
	ACTOR *leader;
	s32 facing;
	FIELD_POS position;
	ACTOR *occupant;

	leader = fdActorGetById(MainRoutineStateWork.leaderActorId);

	facing = FACING_NEAREST_CARDINAL(leader->facing);

	position.x = (leader->x.fixed & FIELD_CELL_MASK) + toFIXED(8);
	position.y = leader->y.fixed;
	position.z = (leader->z.fixed & FIELD_CELL_MASK) + toFIXED(8);
	syVectorAddPolarOffset(toFIXED(16), facing, (FIXED *)&position);

	occupant = scActorFindAtPos(&position, leader);
	if(occupant == NULL) {
		position.x = (leader->x.fixed & FIELD_CELL_MASK) + toFIXED(8);
		position.y = leader->y.fixed;
		position.z = (leader->z.fixed & FIELD_CELL_MASK) + toFIXED(8);
		syVectorAddPolarOffset(toFIXED(32), facing, (FIXED *)&position);

		occupant = scActorFindAtPos(&position, leader);
	}

	return occupant;
}
