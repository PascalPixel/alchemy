/* @file */
/* Kolima Forest (SceneId_KolimaForestEntrance and SceneId_KolimaForestCliff):
 */
/* the hop east from the bank onto the log floating in the river */
/* (ACTOR_RIVER_LOG, which each scene names). */

/* The log's animation as the leader lands on it, one of its sheet's. */
#define RIVER_LOG_LANDING_ANIMATION 7

/* Hops the leader to the 16.16 position request, unless something blocks the */
/* way: the leader crouches and jumps, lands on the log as it plays its */
/* landing animation, sinks two pixels and rises back. Returns 1 after a hop. */
static s32 scKolimaForestHopTo(FIELD_POS *request) {
	ACTOR *actor = fdActorGetById(ACTOR_PARTY_LEADER);
	u8 saved = actor->motionFlg;
	s32 ret = syFieldActorCheckMovementCollision(actor, request);

	if(ret == 0) {
		fdEventStart();
		fdActorSelectAnimation(actor, ANIM_LEADER_SPRING);
		sySynch(6);
		sySndPlay(SE_JUMP);
		fdActorSelectAnimation(actor, ANIM_LEADER_LEAP);
		actor->speedLimit = toFIXED(3);
		actor->acceleration = toFIXED(2);
		actor->velY = toFIXED(4);
		actor->motionFlg &= ACTOR_JUMP_KEEPS_MOTION;
		fdActorSetSprFlags(actor, 0);
		fdActorMoveToAndWaitById(
			ACTOR_PARTY_LEADER,
			((FIELD_COORDINATE *)&request->x)->part[0].integer,
			((FIELD_COORDINATE *)&request->z)->part[0].integer);
		fdActorSelectAnimation(actor, ANIM_LEADER_SPRING);
		fdActorSetSprFlags(actor, SPRITE_FLAG_SHADOW);
		actor->motionFlg = ret;
		fdActorSetAnimation(ACTOR_RIVER_LOG, RIVER_LOG_LANDING_ANIMATION);
		actor->y.fixed -= toFIXED(1);
		actor->groundHeight -= toFIXED(1);
		sySynch(2);
		actor->y.fixed -= toFIXED(1);
		actor->groundHeight -= toFIXED(1);
		sySynch(10);
		actor->y.fixed += toFIXED(1);
		actor->groundHeight += toFIXED(1);
		sySynch(4);
		actor->y.fixed += toFIXED(1);
		actor->groundHeight += toFIXED(1);
		actor->motionFlg = saved;
		fdEventEnd();
		return 1;
	}
	return 0;
}

/* Walking east off the bank, the leader hops onto the log: 30 pixels east, */
/* or, in every edition but the Japanese, to the middle of the cell 40 pixels */
/* east. */
static void scKolimaForestHopEast(void) {
	FIELD_POS target;
#if !TBS_PATCH_1
	target.x = fdActorGetById(ACTOR_PARTY_LEADER)->x.fixed + toFIXED(30);
	target.y = fdActorGetById(ACTOR_PARTY_LEADER)->y.fixed;
	target.z = fdActorGetById(ACTOR_PARTY_LEADER)->z.fixed;
#else
	ACTOR *leader = fdActorGetById(MainRoutineStateWork.leaderActorId);
	u32 base = leader->x.fixed & FIELD_CELL_MASK;

	target.x = base + toFIXED(8);
	target.y = leader->y.fixed;
	target.z = (leader->z.fixed & FIELD_CELL_MASK) + toFIXED(8);
	target.x = base + toFIXED(40);
#endif
	scKolimaForestHopTo(&target);
}
