/* @file */
/* The sites of the three Colosso finals matches (Colosseum Final 1, 2 and 3):
 */
/* the end of the race at the arena, where each warrior walks up to a prize */
/* and holds it up. The first to arrive takes the better one */
/* (Colosso_OpponentFinishScript, Colosso_LeaderFinishScript). */

/* UB: calls a void function through an s32 function type, the call the bytes */
/* show; UBFIX calls it as declared. */
#ifdef UBFIX
#define COLOSSO_GIVE_AND_EQUIP_ITEM scGiveAndEquipItem
#else
#define COLOSSO_GIVE_AND_EQUIP_ITEM ((s32 (*)(s32, s32))scGiveAndEquipItem)
#endif

/* Does nothing; the arena's step event calls it first. */
static void scColossoNoOp(void) {}

/* Unused: marks the opponent's recorded run finished, as */
/* scColossoUpdatePathOpponent does when it reaches the end of the path. */
UNUSED static void scColossoMarkOpponentFinished(void) {
	PATH_RECORDER *recorder = (PATH_RECORDER *)SceneLocalState;

	recorder->mode = PATH_RECORDER_FINISHED;
}

/* Waits until the opponent has run his recorded path to its end. */
static void scColossoWaitForOpponentToFinish(void) {
	PATH_RECORDER *recorder = (PATH_RECORDER *)SceneLocalState;

	while(recorder->mode != PATH_RECORDER_FINISHED) {
		sySynch(1);
	}
}

/* The speed, in pixels a frame, below which the raised prize has settled. */
#define PRIZE_SETTLE_SPEED toFIXED(0.0039)

/* The raised prize's hook (Colosso_PrizeScript): it settles once it has */
/* almost stopped rising, and on about one frame in ten a sparkle */
/* (Colosso_PrizeSparkleScript) appears somewhere within 16 pixels of it. */
static void scColossoSparkleAroundPrize(ACTOR *prize) {
	s32 position[3];

	if(prize->velY >= -PRIZE_SETTLE_SPEED &&
	   prize->velY <= PRIZE_SETTLE_SPEED) {
		prize->motionFlg = 0;
	}
	if(syRandomBelow(100) < 10) {
		ACTOR *sparkle;
		s32 distance;
		s32 angle;

		SET_VECTOR(position, prize->x.fixed, prize->y.fixed, prize->z.fixed);
		distance = syRandomNext();
		angle = syRandomNext();
		syVectorAddPolarOffset(distance << 4, angle, position);
		{
			s32 x = position[0];
			s32 y = position[1];
			s32 z = position[2];

			sparkle = syFieldActorCreate(SPRITE_SPARKLE, x, y, z);
		}
		if(sparkle != NULL) {
			sparkle->motionFlg = 0;
			fdActorSetSprFlags(sparkle, 0);
			fdActorStartScript(sparkle, Colosso_PrizeSparkleScript);
			fdActorSelectAnimation(sparkle, 1);
			fdActorSelectAnimation(sparkle, 0);
		}
	}
}

/* Called from a warrior's finish script once he stands at his prize: the */
/* prize (the actor id in his timer) rises to 36 pixels above him with the */
/* treasure jingle, and the cleared timer lets scColossoTakePrize go on. */
static s32 scColossoRaisePrize(ACTOR *warrior) {
	ACTOR *prize = fdActorGet(warrior->tmr);

	syFieldActorSetMoveTargetFX(prize, warrior->x.fixed,
								warrior->y.fixed + toFIXED(36),
								warrior->z.fixed);
	prize->motionFlg = 0;
	fdActorStartScript(prize, Colosso_PrizeScript);
	sySndPlay(MUS_TREASURE_FOUND);
	warrior->tmr = 0;
	return 0;
}

/* Isaac reaches the finish: he walks up to the first prize, or the second */
/* when the opponent is there before him, holds it up, is given and equipped */
/* with it, and MsgGotItem names him and the item he got. Returns */
/* nonzero when the opponent arrived first. */
static s32 scColossoTakePrize(s32 firstItem, s32 secondItem) {
	COLOSSO_GAUGE_STATE *gauge = (COLOSSO_GAUGE_STATE *)ColossoState;
	ACTOR *leader;
	s32 opponentFirst;
	FIXED x;
	FIXED z;
	u16 *prizeActorId;
	s16 *raisedPrizeActorId;

	opponentFirst = gmFlagGet(FLAG_ARENA_REACHED);
	leader = fdActorGet(MainRoutineStateWork.leaderActorId);
	if(gauge->finishX < leader->x.fixed) {
		x = gauge->finishX + toFIXED(12);
	}
	else {
		x = gauge->finishX - toFIXED(12);
	}
	if(opponentFirst != 0) {
		z = gauge->finishZ + toFIXED(16);
		prizeActorId = &gauge->finishItemActorIdB;
	}
	else {
		z = gauge->finishZ - toFIXED(16);
		prizeActorId = &gauge->finishItemActorIdA;
	}
	raisedPrizeActorId = &leader->tmr;
	*raisedPrizeActorId = *prizeActorId;
	leader->acceleration = toFIXED(1) / 4;
	leader->speedLimit = toFIXED(1);
	syFieldActorSetMoveTargetFX(leader, x, 0, z);
	gmFlagSet(FLAG_ARENA_REACHED);
	fdActorStartScript(leader, Colosso_LeaderFinishScript);
	while(*raisedPrizeActorId != 0) {
		sySynch(1);
	}
	if(opponentFirst == 0) {
		COLOSSO_GIVE_AND_EQUIP_ITEM(ISAAC, firstItem);
		mnMessageArgumentPush(firstItem, MESSAGE_ARGUMENT_ITEM);
	}
	else {
		COLOSSO_GIVE_AND_EQUIP_ITEM(ISAAC, secondItem);
		mnMessageArgumentPush(secondItem, MESSAGE_ARGUMENT_ITEM);
	}
	mnMessageArgumentPush(MainRoutineStateWork.leaderActorId,
						  MESSAGE_ARGUMENT_ACTOR);
	mnFieldMessageShow(MsgGotItem, 3);
	fdActorWaitForScriptEnd(leader);
	return opponentFirst;
}
