/* Kolima Forest (SceneIdKolimaForestEntrance and SceneIdKolimaForestCliff):
   the hop east from the bank onto the log floating in the river
   (ACTOR_RIVER_LOG, which each scene names). */

/* The log's animation as the leader lands on it, one of its sheet's. */
#define RIVER_LOG_LANDING_ANIMATION 7

/* Hops the leader to the 16.16 position request, unless something blocks the
   way: the leader crouches and jumps, lands on the log as it plays its landing
   animation, sinks two pixels and rises back. Returns 1 after a hop. */
static Bool scKolimaForestHopTo(FIELD_POS *request)
{
	ACTOR *actor = fdActorGetById(ACTOR_PARTY_LEADER);
	Uint8  saved = actor->motionFlg;
	Sint32 ret = syFieldActorCheckMovementCollision(actor, request);

	if(ret == 0) {
		fdScriptStart();
		fdActorSelectAnimation(actor, ANIM_LEADER_SPRING);
		sySynch(6);
		sySndPlay(SE_JUMP);
		fdActorSelectAnimation(actor, ANIM_LEADER_LEAP);
		actor->speedLimit = toFIXED(3);
		actor->acceleration = toFIXED(2);
		actor->velY = toFIXED(4);
		actor->motionFlg &= ACTOR_JUMP_KEEPS_MOTION;
		fdActorSetSprFlags(actor, 0);
		fdActorMoveToAndWaitById(ACTOR_PARTY_LEADER,
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
		fdScriptEnd();
		return TRUE;
	}
	return FALSE;
}

/* Walking east off the bank, the leader hops onto the log: 30 pixels east, or,
   in every edition but the Japanese, to the middle of the metatile 40 pixels
   east. */
static void scKolimaForestHopEast(void)
{
	FIELD_POS tgt;
#if !JUMP_LANDS_ON_CELL_CENTER
	tgt.x = fdActorGetById(ACTOR_PARTY_LEADER)->x.fixed + toFIXED(30);
	tgt.y = fdActorGetById(ACTOR_PARTY_LEADER)->y.fixed;
	tgt.z = fdActorGetById(ACTOR_PARTY_LEADER)->z.fixed;
#else
	ACTOR *leader = fdActorGetById(MainRoutineStateWork.leaderActorId);
	Uint32 base = leader->x.fixed & FIELD_METATILE_MASK;

	tgt.x = base + toFIXED(8);
	tgt.y = leader->y.fixed;
	tgt.z = (leader->z.fixed & FIELD_METATILE_MASK) + toFIXED(8);
	tgt.x = base + toFIXED(40);
#endif
	scKolimaForestHopTo(&tgt);
}
