/* @file */
/* Actor script commands: waits, jumps, flags, moves and state values. */
/* */
/* Each `SC_` handler here is reached through ActorScript_CommandTable */
/* (actor_script.c), which fdActorUpdateAll and fdActorUpdateAllOnField call */
/* for each live actor; each returns 1 to run on and 0 to end the actor's */
/* frame (the protocol is in this folder's README). */

#include "field_internal.h"

/* The interpreter's limits, distances and turns. */
enum {
	SCRIPT_LABEL_SEARCH_WORDS = 1024, /* script words a jump searches */
	SCRIPT_BLOCKED_MOVE_FRAMES = 60,  /* refused frames SC_SYNC waits through */
	SCRIPT_WANDER_TRIES = 7,		  /* random points a wander tries a frame */
	/* The wander probes' distance, 8 pixels in 16.16: past the point for */
	/* ground, and along the heading for solid actors (SC_WANDER_HOME). */
	SCRIPT_WANDER_PROBE_DISTANCE = toFIXED(8),
	SCRIPT_WANDER_DIAGONAL_ANGLE = DEGtoANG(45), /* one eighth of a turn */
	SCRIPT_WANDER_SIDE_ANGLE = DEGtoANG(90),	 /* one quarter of a turn */
	SCRIPT_HALF_TURN = DEGtoANG(180),
	/* Whole pixels SC_APPROACH_WATCHED stays short of its watched actor. */
	SCRIPT_APPROACH_DISTANCE = 16,
	/* Under 64 pixels (16.16) SC_CAMERA measures its distance in 16.16. */
	CAMERA_FOLLOW_FINE_DISTANCE = toFIXED(64),
	/* Within a quarter pixel (16.16) SC_CAMERA lands on its point. */
	CAMERA_FOLLOW_SNAP_DISTANCE = toFIXED(1) / 4,
	/* Within half a pixel (16.16) SC_CAMERA takes the leader's height at once.
	 */
	CAMERA_FOLLOW_HEIGHT_SNAP = toFIXED(1) / 2
};

/* SC_LABEL and SC_TO differ only in bit 30; a jump searches for the label. */
#define SCRIPT_LABEL_KEY_MASK 0xbfffffff

/* SC_WAIT(n): waits n frames; this frame counts as the first. */
s32 fdActorScriptWait(ACTOR *interpreter) {
	s16 commandCursor = interpreter->scriptCursor;

	interpreter->wait = interpreter->script[commandCursor + 1] - 1;
	interpreter->scriptCursor += 2;
	return 0;
}

/* SC_SYNC: waits until the actor arrives at its target, or until a solid actor
 */
/* has blocked it for 60 frames (refused is the blocked count, which */
/* fdActorUpdateAllOnField raises). */
s32 fdActorScriptWaitForArrival(ACTOR *interpreter) {
	if(interpreter->refused > SCRIPT_BLOCKED_MOVE_FRAMES - 1) {
		interpreter->refused = 0;
		interpreter->scriptCursor++;
		return 1;
	}
	if(fdActorIsTargetUnset(interpreter) != 0) {
		interpreter->scriptCursor++;
		return 1;
	}
	return 0;
}

/* SC_CALL(fn): calls fn with the actor. */
s32 fdActorScriptInvokeCallback(ACTOR *interpreter) {
	s16 initialCursor = interpreter->scriptCursor;
	SCRIPT_COMMAND callback =
		(SCRIPT_COMMAND)interpreter->script[initialCursor + 1];

	if(callback(interpreter) != 0)
		return 0;
	if(interpreter->scriptCursor == initialCursor)
		interpreter->scriptCursor = interpreter->scriptCursor + 2;
	return 1;
}

/* Returns the cursor just past the script's label key (SC_LABEL or SC_TO, */
/* matched without the bit that tells them apart), searching its first 1024 */
/* words, or 0 (the start) when key is 0 or not found, so a missing label */
/* restarts the script. Clears the wait. */
s32 fdActorScriptFindLabel(ACTOR *interpreter, u32 labelKey) {
	u32 *scriptWords;
	s32 wordIndex;

	interpreter->wait = 0;
	if(labelKey == 0) {
		return 0;
	}

	labelKey &= SCRIPT_LABEL_KEY_MASK;
	scriptWords = (u32 *)interpreter->script;
	for(wordIndex = 0; wordIndex < SCRIPT_LABEL_SEARCH_WORDS; wordIndex++) {
		if(*scriptWords++ == labelKey) {
			return wordIndex + 1;
		}
	}
	return 0;
}

/* SC_LOOP(n, to): jumps to label to until it has done so n - 1 times, then */
/* moves on; with n = SC_FOREVER it always jumps. */
s32 fdActorScriptRepeatOrJump(ACTOR *interpreter) {
	const s32 *arguments;
	s32 repeatLimit;
	s32 jumpKey;
	s32 nextCount;

	arguments = &interpreter->script[interpreter->scriptCursor + 1];
	repeatLimit = *arguments++;
	jumpKey = *arguments;
	if(repeatLimit == SC_FOREVER) {
		interpreter->scriptCursor =
			fdActorScriptFindLabel(interpreter, jumpKey);
	}
	else {
		nextCount = interpreter->repeatCnt + 1;
		interpreter->repeatCnt = nextCount;
		if((u8)nextCount < (s16)repeatLimit) {
			interpreter->scriptCursor =
				fdActorScriptFindLabel(interpreter, jumpKey);
		}
		else {
			interpreter->repeatCnt = 0;
			interpreter->scriptCursor += 3;
		}
	}
	return 1;
}

/* SC_JUMP(to): jumps to label to. */
s32 fdActorScriptJumpToLabel(ACTOR *interpreter) {
	interpreter->scriptCursor = fdActorScriptFindLabel(
		interpreter, interpreter->script[interpreter->scriptCursor + 1]);
	return 1;
}

/* SC_IF(to): jumps to label to when the last test was true. */
s32 fdActorScriptJumpIfTrue(ACTOR *interpreter) {
	s32 commandCursor = interpreter->scriptCursor;
	const s32 *script = interpreter->script;
	u32 labelKey = script[commandCursor + 1];

	if(interpreter->conditionResult != 0) {
		interpreter->scriptCursor =
			fdActorScriptFindLabel(interpreter, labelKey);
	}
	else {
		interpreter->scriptCursor += 2;
	}
	return 1;
}

/* SC_IFNOT(to): jumps to label to when the last test was false. */
s32 fdActorScriptJumpIfFalse(ACTOR *interpreter) {
	s32 commandCursor = interpreter->scriptCursor;
	const s32 *script = interpreter->script;
	u32 labelKey = script[commandCursor + 1];

	if(interpreter->conditionResult == 0) {
		interpreter->scriptCursor =
			fdActorScriptFindLabel(interpreter, labelKey);
	}
	else {
		interpreter->scriptCursor += 2;
	}
	return 1;
}

/* SC_END: hands the actor back to the idle script, ActorScript_EndScript, */
/* whose lone SC_END runs again each frame. Returns 0. */
s32 fdActorScriptEnd(ACTOR *interpreter) {
	interpreter->script = ActorScript_EndScript;
	interpreter->scriptCursor = 0;
	return 0;
}

/* SC_FLAG_CHECK(f): tests game flag f. */
s32 fdActorScriptTestFlag(ACTOR *interpreter) {
	interpreter->conditionResult =
		gmFlagGetStub(interpreter->script[interpreter->scriptCursor + 1]);
	interpreter->scriptCursor += 2;
	return 1;
}

/* Defines a flag command: stores the flag's current value in conditionResult,
 */
/* then changes the flag with change. */
#define SCRIPT_DEFINE_FLAG_COMMAND(name, change)                               \
	s32 name(ACTOR *interpreter) {                                             \
		s32 flagId;                                                            \
                                                                               \
		flagId = interpreter->script[interpreter->scriptCursor + 1];           \
		interpreter->conditionResult = gmFlagGetStub(flagId);                  \
		change(flagId);                                                        \
		interpreter->scriptCursor += 2;                                        \
		return 1;                                                              \
	}

/* SC_FLAG_ON(f): tests game flag f, then sets it. */
SCRIPT_DEFINE_FLAG_COMMAND(fdActorScriptSetFlagAndTest, gmFlagSetStub)
/* SC_FLAG_OFF(f): tests game flag f, then clears it. */
SCRIPT_DEFINE_FLAG_COMMAND(fdActorScriptClearFlagAndTest, gmFlagClearStub)

/* SC_FLAG_REV(f): tests game flag f, then flips it. */
s32 fdActorScriptToggleFlagAndTest(ACTOR *interpreter) {
	s32 flagId;
	s32 previousFlagValue;

	flagId = interpreter->script[interpreter->scriptCursor + 1];
	previousFlagValue = gmFlagGetStub(flagId);
	interpreter->conditionResult = previousFlagValue;
	if((u8)previousFlagValue == 1) {
		gmFlagClearStub(flagId);
	}
	else {
		gmFlagSetStub(flagId);
	}
	interpreter->scriptCursor += 2;
	return 1;
}

/* SC_ANIMATION(v): switches the actor's sprites to animation v. */
s32 fdActorScriptSetAnimation(ACTOR *interpreter) {
	fdActorSelectAnimation(interpreter,
						   interpreter->script[interpreter->scriptCursor + 1]);
	interpreter->scriptCursor += 2;
	return 1;
}

/* SC_RELEASE: releases the actor through fdActorRelease, which releases its */
/* sprites and zeroes its slot. */
s32 fdActorScriptRelease(ACTOR *interpreter) {
	fdActorRelease(interpreter);
	return 0;
}

/* SC_NOP(v): does nothing with its operand. */
s32 fdActorScriptNop(ACTOR *interpreter) {
	interpreter->scriptCursor += 2;
	return 1;
}

/* SC_SE(n): sends sound command n. */
s32 fdActorScriptPlaySnd(ACTOR *interpreter) {
	sySndPlayStub(interpreter->script[interpreter->scriptCursor + 1]);
	interpreter->scriptCursor += 2;
	return 1;
}

/* Returns -1 when an actor at position values would overlap a solid actor */
/* (ACTOR_SOLID), each radius less 2 pixels; otherwise 0. */
s32 fdActorScriptCheckOverlap(ACTOR *actor, s32 *values) {
	s32 actorIndex;
	ACTOR *candidate = ActorsWork;

	for(actorIndex = 0; actorIndex < ACTOR_COUNT; actorIndex++, candidate++) {
		if(candidate->script != NULL &&
		   (candidate->collisionFlg & ACTOR_SOLID) && candidate != actor) {
			if(fdCheckSpheresOverlap(&candidate->x.fixed, candidate->radius - 2,
									 values, actor->radius - 2) >= 0)
				return -1;
		}
	}
	return 0;
}

/* Returns the first solid actor that an actor at position values would */
/* overlap, as fdActorScriptCheckOverlap tests, or NULL. */
ACTOR *fdActorScriptFindOverlappingActor(ACTOR *actor, s32 *values) {
	s32 actorIndex;
	ACTOR *candidate = ActorsWork;

	for(actorIndex = 0; actorIndex < ACTOR_COUNT; actorIndex++, candidate++) {
		if(candidate->script != NULL &&
		   (candidate->collisionFlg & ACTOR_SOLID) && candidate != actor) {
			if(fdCheckSpheresOverlap(&candidate->x.fixed, candidate->radius - 2,
									 values, actor->radius - 2) >= 0)
				return candidate;
		}
	}
	return NULL;
}

/* Reads three coordinate words and applies the selected positioning operation.
 */
static __inline__ s32 fdActorScriptApplyPosCommand(
	ACTOR *interpreter,
	void (*applyPosition)(ACTOR *actor, s32 x, s32 y, s32 z)) {
	s32 x;
	s32 y;
	s32 z;
	const s32 *coordinates;

	coordinates = interpreter->script + interpreter->scriptCursor;
	coordinates++;
	x = *coordinates;
	coordinates++;
	y = *coordinates;
	coordinates++;
	z = *coordinates;
	applyPosition(interpreter, x, y, z);
	interpreter->scriptCursor += 4;
	return 1;
}

/* SC_POSITION(x, y, z): places the actor at (x, y, z) and stops it. */
s32 fdActorScriptSetPosAndResetMotion(ACTOR *interpreter) {
	return fdActorScriptApplyPosCommand(interpreter,
										fdActorSetPosAndResetMotion);
}

/* SC_MOVE(x, y, z): sends the actor towards (x, y, z). */
s32 fdActorScriptApplyAbsolutePos(ACTOR *interpreter) {
	return fdActorScriptApplyPosCommand(interpreter, fdActorSetMoveTarget);
}

/* SC_MOVE_BY(x, y, z): sends the actor towards its position plus (x, y, z). */
s32 fdActorScriptApplyRelativePos(ACTOR *actor) {
	u8 *commandBytes = (u8 *)(actor->script + actor->scriptCursor);
	s32 *arguments = (s32 *)(commandBytes + 4);
	s32 deltaX = *arguments++;
	s32 deltaY = *arguments++;
	s32 deltaZ = *arguments;

	fdActorSetMoveTarget(actor, actor->x.fixed + deltaX,
						 actor->y.fixed + deltaY, actor->z.fixed + deltaZ);
	actor->scriptCursor += 4;
	return 1;
}

/* SC_FACE_WATCHED: turns the actor to face its watched actor across the */
/* ground. */
s32 fdActorScriptStoreAngToWatchedActor(ACTOR *actor) {
	ACTOR *target;

	target = (ACTOR *)actor->watchedActor;
	actor->facing = syAtan((target->z.fixed - actor->z.fixed),
						   (target->x.fixed - actor->x.fixed));
	actor->scriptCursor++;
	return 1;
}

/* SC_MOVE_TO_WATCHED: sends the actor towards its watched actor. */
s32 fdActorScriptApplyWatchedActorPos(ACTOR *actor) {
	ACTOR *target;

	target = (ACTOR *)actor->watchedActor;
	fdActorSetMoveTarget(actor, target->x.fixed, target->y.fixed,
						 target->z.fixed);
	actor->scriptCursor++;
	return 1;
}

/* SC_MOVE_LOCAL(a, b): sends the actor towards its position plus an offset */
/* that is never filled in; the operands are skipped unread. */
s32 fdActorScriptApplyLocalOffsetPos(ACTOR *actor) {
	s32 localOffset[3];

	/* BUG: offset is never set, so the target is the position plus whatever */
	/* the stack held. */
#ifdef UBFIX
	localOffset[0] = localOffset[1] = localOffset[2] = 0;
#endif
	fdActorSetMoveTarget(actor, actor->x.fixed + localOffset[0],
						 actor->y.fixed + localOffset[1],
						 actor->z.fixed + localOffset[2]);
	actor->scriptCursor += 3;
	return 1;
}

/* SC_CAMERA: keeps the camera actor on its leader (watchedActor), clamped so */
/* the view stays on the map. */
s32 fdCameraFollowLeaderInBounds(ACTOR *cameraActor) {
	ACTOR *leader;
	MAP_GRID_SCROLL_STATE *bounds;
	s32 minX;
	FIXED minZ;
	s32 maxX;
	FIXED maxZ;
	FIXED x;
	FIXED y;
	FIXED z;
	s32 dz;
	s32 dy;
	s32 dx;
	FIXED distance;
	FIXED moveStep;

	/* Use fdMapGridUpdateLayerScroll's viewport origin and the leader's
	 * elevation */
	/* when clamping its ground position to the viewport bounds. */
	bounds = MapGridState[0];
	leader = (ACTOR *)cameraActor->watchedActor;
	/* UB: the bounds read the leader's height before the null check below. */
#ifdef UBFIX
	if(leader == NULL) {
		cameraActor->motionFlg = 0;
		cameraActor->scriptCursor++;
		return 1;
	}
#endif
	/* A layered map draws a point at screen row z - y, so the z bounds move */
	/* with the leader's height and the clamp keeps that row inside the map. */
	minX = bounds->minX + FIELD_VIEW_FOCUS_X;
	minZ = bounds->minY + leader->y.fixed + FIELD_VIEW_FOCUS_Y;
	maxX = bounds->maxX - FIELD_VIEW_FOCUS_X;
	maxZ = bounds->maxY + leader->y.fixed -
		   (FIELD_VIEW_HEIGHT - FIELD_VIEW_FOCUS_Y);
	cameraActor->motionFlg = 0;
	if(leader != NULL && leader->script != NULL) {
		x = leader->x.fixed;
		y = leader->y.fixed;
		z = leader->z.fixed;
		cameraActor->tgtX = ACTOR_NO_TARGET;
		cameraActor->tgtY = ACTOR_NO_TARGET;
		cameraActor->tgtZ = ACTOR_NO_TARGET;
		if(x < minX)
			x = minX;
		if(z < minZ)
			z = minZ;
		if(x > maxX)
			x = maxX;
		if(z > maxZ)
			z = maxZ;
		if(cameraActor->tmr != 0) {
			SET_ACTOR_POSITION(cameraActor, x, y, z);
		}
		else {
			/* The distance in the XZ plane: dy is computed as in */
			/* fdActorSetMoveTarget but not used for the distance. */
			dx = (x - cameraActor->x.fixed) / FIXED_ONE;
			/* LEFTOVER(template): this whole-pixel dy, overwritten below and
			 * not used for the distance, as fdActorSetMoveTarget
			 * (field_actor_motion.c) computes the same three whole-pixel deltas
			 * and uses y's in its distance; as the line removed. */
			dy = (y - cameraActor->y.fixed) / FIXED_ONE;
			dz = (z - cameraActor->z.fixed) / FIXED_ONE;
			distance = toFIXED(sySquart(dx * dx + dz * dz));
			dx = x - cameraActor->x.fixed;
			dy = y - cameraActor->y.fixed;
			dz = z - cameraActor->z.fixed;
			/* The whole-pixel root is coarse; under 64 pixels the distance is
			 */
			/* measured again in 16.16 for a finer result. */
			if(distance < CAMERA_FOLLOW_FINE_DISTANCE)
				distance = sySquartFX(syMulFX(dx, dx) + syMulFX(dz, dz));
			if((moveStep = distance / 8) > cameraActor->speedLimit)
				moveStep = cameraActor->speedLimit;
			/* Within a quarter pixel the camera snaps to the point. */
			if(distance < CAMERA_FOLLOW_SNAP_DISTANCE) {
				cameraActor->x.fixed = x;
				cameraActor->z.fixed = z;
			}
			else {
				/* Shorten the offset to moveStep along the same direction: dx
				 * over */
				/* the distance is a 16.16 fraction, times the step. */
				if(distance > moveStep) {
					dx = syMulFX(syDivFX(distance, dx), moveStep);
					dz = syMulFX(syDivFX(distance, dz), moveStep);
				}
				cameraActor->x.fixed += dx;
				cameraActor->z.fixed += dz;
			}
			/* Height follows at once within half a pixel, else a quarter a
			 * frame. */
			if((dy < 0 ? -dy : dy) > CAMERA_FOLLOW_HEIGHT_SNAP)
				dy /= 4;
			cameraActor->y.fixed += dy;
		}
	}
	cameraActor->scriptCursor++;
	return 1;
}

/* SC_APPROACH_WATCHED: follows the watched actor at its speed. */
s32 fdActorApproachWatchedActor(ACTOR *actor) {
	s32 distancePixels;
	FIXED moveZ;
	FIXED dx;
	FIXED dz;
	s32 deltaZPixels;
	s32 deltaXPixels;
	s32 remainingDistance;
	FIXED moveX;
	ACTOR *watchedActor;

	watchedActor = (ACTOR *)actor->watchedActor;
	actor->speedLimit = watchedActor->speedLimit;
	actor->acceleration = watchedActor->acceleration;
	dx = watchedActor->x.fixed - actor->x.fixed;
	dz = watchedActor->z.fixed - actor->z.fixed;
	deltaXPixels = FIXEDtoINT(dx);
	deltaZPixels = FIXEDtoINT(dz);
	distancePixels =
		sySquart((deltaXPixels * deltaXPixels) + (deltaZPixels * deltaZPixels));
	/* Further than 16 pixels: aim at the point 16 pixels short of the watched
	 */
	/* actor along the line to it (the 16.16 offset scaled by the whole-pixel */
	/* ratio) and run on to SC_WAIT(1); otherwise stand and stay on this
	 * command. */
	if(distancePixels > SCRIPT_APPROACH_DISTANCE) {
		remainingDistance = distancePixels - SCRIPT_APPROACH_DISTANCE;
		moveX = dx * remainingDistance / distancePixels;
		moveZ = dz * remainingDistance / distancePixels;
		fdActorSetMoveTarget(actor, actor->x.fixed + moveX, actor->y.fixed,
							 actor->z.fixed + moveZ);
		fdActorSelectAnimation(actor, ANIM_WALK);
		actor->scriptCursor++;
		return 1;
	}
	fdActorSelectAnimation(actor, ANIM_STAND);
	return 0;
}

/* SC_WANDER(base, range, limit): walks to a random point base to base + range
 */
/* away, within 90 degrees either side of the way it faces, that no */
/* solid actor or wall blocks, with clear ground 8 pixels further at 45 degrees
 */
/* either side, and within limit pixels of home. */
s32 fdActorWander(ACTOR *actor) {
	FIELD_POS candidatePosition;
	FIELD_POS probePosition;
	s32 minimumDistance;
	const s32 *operands;
	s32 distanceRange;
	s32 homeRadiusSquared;
	FIXED travelDistance;
	s32 heading;
	s32 attempts;
	s32 dx;
	s32 dz;
#ifdef BUGFIX
	s32 aheadBlocked;
#endif

	/* base and range are 16.16 pixels; limit is compared in whole pixels, */
	/* against homeX and homeZ. */
	operands = &actor->script[actor->scriptCursor + 1];
	minimumDistance = *operands++;
	distanceRange = *operands++;
	homeRadiusSquared = *operands / FIXED_ONE;
	attempts = 0;
	homeRadiusSquared = homeRadiusSquared * homeRadiusSquared;
retry:
	attempts++;
	if(attempts <= SCRIPT_WANDER_TRIES) {
		SET_POSITION(&candidatePosition, actor->x.fixed, actor->y.fixed,
					 actor->z.fixed);
		/* syRandom is a 16.16 fraction below 1, so the distance is base plus up
		 * to */
		/* range. */
		travelDistance = minimumDistance + syMulFX(syRandom(), distanceRange);
		heading = actor->facing + (syRandom() >> 2) - (syRandom() >> 2);
		syVectorAddPolarOffset(travelDistance, heading,
							   (FIXED *)&candidatePosition);
		if(fdActorScriptCheckOverlap(actor, (s32 *)&candidatePosition) != 0 ||
		   fdMapGridCheckCollision(actor, &candidatePosition) != COLLISION_NONE)
			goto retry;
		/* Probes 8 pixels past the point, ahead and at 45 degrees
		 * (DEGtoANG(45)) */
		/* either side. */
		travelDistance += SCRIPT_WANDER_PROBE_DISTANCE;
		SET_POSITION(&probePosition, actor->x.fixed, actor->y.fixed,
					 actor->z.fixed);
		syVectorAddPolarOffset(travelDistance, heading,
							   (FIXED *)&probePosition);
		/* BUG: the probe straight ahead is placed but never tested, so only the
		 */
		/* two at 45 degrees need clear ground; SC_WANDER_HOME tests all three.
		 */
#ifdef BUGFIX
		aheadBlocked = fdMapGridCheckCollision(actor, &probePosition);
#endif
		SET_POSITION(&probePosition, actor->x.fixed, actor->y.fixed,
					 actor->z.fixed);
		syVectorAddPolarOffset(travelDistance,
							   heading + SCRIPT_WANDER_DIAGONAL_ANGLE,
							   (FIXED *)&probePosition);
#ifdef BUGFIX
		if(aheadBlocked != 0 ||
		   fdMapGridCheckCollision(actor, &probePosition) != COLLISION_NONE)
#else
		if(fdMapGridCheckCollision(actor, &probePosition) != COLLISION_NONE)
#endif
			goto retry;
		SET_POSITION(&probePosition, actor->x.fixed, actor->y.fixed,
					 actor->z.fixed);
		syVectorAddPolarOffset(travelDistance,
							   heading - SCRIPT_WANDER_DIAGONAL_ANGLE,
							   (FIXED *)&probePosition);
		if(fdMapGridCheckCollision(actor, &probePosition) != COLLISION_NONE)
			goto retry;
		/* FAKEMATCH(dead): the offsets in both arms of a test of actor. */
		if(actor != 0) {
			dx = candidatePosition.x / FIXED_ONE - actor->homeX;
			dz = candidatePosition.z / FIXED_ONE - actor->homeZ;
		}
		else {
			dx = candidatePosition.x / FIXED_ONE - actor->homeX;
			dz = candidatePosition.z / FIXED_ONE - actor->homeZ;
		}
		if(dx * dx + dz * dz > homeRadiusSquared)
			goto retry;
	}
	else {
		/* Out of tries: face the other way (half a turn) and stay on this */
		/* command, to try again next frame. */
		actor->facing += SCRIPT_HALF_TURN;
		actor->wait = 1;
		return 0;
	}
	fdActorSetMoveTarget(actor, candidatePosition.x, candidatePosition.y,
						 candidatePosition.z);
	actor->scriptCursor += 4;
	return 1;
}

/* SC_WANDER_HOME(base, range, limit): wanders as SC_WANDER does, testing more
 */
/* ground around the point, while it is within limit pixels of home; outside */
/* that, or when no point works, it heads back towards home. */
s32 fdActorScriptWanderNearHome(ACTOR *actor) {
	FIELD_POS candidatePosition;
	FIELD_POS probePosition;
	s32 minimumDistance;
	const s32 *operands;
	s32 distanceRange;
	s32 homeRadiusSquared;
	FIXED travelDistance;
	u16 heading;
	u16 angle;
	s32 attempts;
	s32 dx;
	s32 dz;

	operands = &actor->script[actor->scriptCursor + 1];
	minimumDistance = *operands++;
	distanceRange = *operands++;
	homeRadiusSquared = *operands / FIXED_ONE;
	angle = actor->facing;
	homeRadiusSquared *= homeRadiusSquared;
	attempts = 0;
	dx = actor->x.fixed / FIXED_ONE - actor->homeX;
	dz = actor->z.fixed / FIXED_ONE - actor->homeZ;
	if(dx * dx + dz * dz <= homeRadiusSquared) {
	roam:
		attempts++;
		if(attempts <= SCRIPT_WANDER_TRIES) {
			travelDistance =
				minimumDistance + syMulFX(syRandom(), distanceRange);
			heading = angle + (syRandom() >> 2) - (syRandom() >> 2);
			SET_POSITION(&candidatePosition, actor->x.fixed, actor->y.fixed,
						 actor->z.fixed);
			/* First no solid actor 8 pixels along the heading, then the point
			 */
			/* itself, then clear ground 8 pixels past it ahead, at 45 degrees
			 * and */
			/* at 90 degrees either side. */
			syVectorAddPolarOffset(SCRIPT_WANDER_PROBE_DISTANCE, heading,
								   (FIXED *)&candidatePosition);
			if(fdActorScriptCheckOverlap(actor, (s32 *)&candidatePosition) != 0)
				goto roam;
			SET_POSITION(&candidatePosition, actor->x.fixed, actor->y.fixed,
						 actor->z.fixed);
			syVectorAddPolarOffset(travelDistance, heading,
								   (FIXED *)&candidatePosition);
			if(fdMapGridCheckCollision(actor, &candidatePosition) !=
			   COLLISION_NONE)
				goto roam;
			probePosition.x = actor->x.fixed;
			probePosition.y = actor->y.fixed;
			travelDistance += SCRIPT_WANDER_PROBE_DISTANCE;
			probePosition.z = actor->z.fixed;
			syVectorAddPolarOffset(travelDistance, heading,
								   (FIXED *)&probePosition);
			if(fdMapGridCheckCollision(actor, &probePosition) != COLLISION_NONE)
				goto roam;
			SET_POSITION(&probePosition, actor->x.fixed, actor->y.fixed,
						 actor->z.fixed);
			syVectorAddPolarOffset(travelDistance,
								   heading + SCRIPT_WANDER_DIAGONAL_ANGLE,
								   (FIXED *)&probePosition);
			if(fdMapGridCheckCollision(actor, &probePosition) != COLLISION_NONE)
				goto roam;
			SET_POSITION(&probePosition, actor->x.fixed, actor->y.fixed,
						 actor->z.fixed);
			syVectorAddPolarOffset(travelDistance,
								   heading - SCRIPT_WANDER_DIAGONAL_ANGLE,
								   (FIXED *)&probePosition);
			if(fdMapGridCheckCollision(actor, &probePosition) != COLLISION_NONE)
				goto roam;
			SET_POSITION(&probePosition, actor->x.fixed, actor->y.fixed,
						 actor->z.fixed);
			syVectorAddPolarOffset(travelDistance,
								   heading + SCRIPT_WANDER_SIDE_ANGLE,
								   (FIXED *)&probePosition);
			if(fdMapGridCheckCollision(actor, &probePosition) != COLLISION_NONE)
				goto roam;
			SET_POSITION(&probePosition, actor->x.fixed, actor->y.fixed,
						 actor->z.fixed);
			syVectorAddPolarOffset(travelDistance,
								   heading - SCRIPT_WANDER_SIDE_ANGLE,
								   (FIXED *)&probePosition);
			if(fdMapGridCheckCollision(actor, &probePosition) != COLLISION_NONE)
				goto roam;
			dx = candidatePosition.x / FIXED_ONE - actor->homeX;
			dz = candidatePosition.z / FIXED_ONE - actor->homeZ;
			if(dx * dx + dz * dz > homeRadiusSquared)
				goto roam;
			actor->collisionFlg |= ACTOR_PUSHABLE;
			fdActorSetMoveTarget(actor, candidatePosition.x,
								 candidatePosition.y, candidatePosition.z);
			goto done;
		}
	}
	/* Head back: aim opposite the angle from home to the actor (dx and dz as */
	/* measured at the start, or at the last point found too far from home),
	 * with */
	/* the same spread, testing only the near and far points. */
	attempts = 0;
	angle = (u16)syAtan(dz, dx) + SCRIPT_HALF_TURN;
back:
	attempts++;
	if(attempts <= SCRIPT_WANDER_TRIES) {
		travelDistance = minimumDistance + syMulFX(syRandom(), distanceRange);
		heading = angle + (syRandom() >> 2) - (syRandom() >> 2);
		SET_POSITION(&candidatePosition, actor->x.fixed, actor->y.fixed,
					 actor->z.fixed);
		syVectorAddPolarOffset(SCRIPT_WANDER_PROBE_DISTANCE, heading,
							   (FIXED *)&candidatePosition);
		if(fdActorScriptCheckOverlap(actor, (s32 *)&candidatePosition) != 0)
			goto back;
		SET_POSITION(&candidatePosition, actor->x.fixed, actor->y.fixed,
					 actor->z.fixed);
		syVectorAddPolarOffset(travelDistance, heading,
							   (FIXED *)&candidatePosition);
		if(fdMapGridCheckCollision(actor, &candidatePosition) != COLLISION_NONE)
			goto back;
		actor->collisionFlg &= ~ACTOR_PUSHABLE;
		fdActorSetMoveTarget(actor, candidatePosition.x, candidatePosition.y,
							 candidatePosition.z);
	}
done:
	actor->scriptCursor += 4;
	return 1;
}

/* What a state value handler does with its value, for SC_SET, SC_ADD and */
/* SC_CMP in turn. */
typedef enum SCRIPT_OPERAND_OPERATION {
	SCRIPT_OPERAND_SET,
	SCRIPT_OPERAND_ADD,
	SCRIPT_OPERAND_COMPARE /* sets conditionResult to 1 when equal, else 0 */
} SCRIPT_OPERAND_OPERATION;

/* Defines the handler for a 32-bit state value: set it, add to it, or compare
 */
/* it (into conditionResult). */
#define SCRIPT_DEFINE_OPERAND_WORD(name, field)                                \
	void name(SCRIPT_OPERANDS *operands, s32 operation, s32 value) {           \
		if(operation == SCRIPT_OPERAND_SET) {                                  \
			operands->field = value;                                           \
			return;                                                            \
		}                                                                      \
		if(operation == SCRIPT_OPERAND_ADD) {                                  \
			operands->field += value;                                          \
			return;                                                            \
		}                                                                      \
		operands->conditionResult = operands->field == value;                  \
	}

/* The same for an address: an add steps by value words (value * 4 bytes). */
#define SCRIPT_DEFINE_OPERAND_ADDRESS(name, field)                             \
	void name(SCRIPT_OPERANDS *operands, s32 operation, s32 value) {           \
		if(operation == SCRIPT_OPERAND_SET) {                                  \
			operands->field = value;                                           \
			return;                                                            \
		}                                                                      \
		if(operation == SCRIPT_OPERAND_ADD) {                                  \
			operands->field += value * 4;                                      \
			return;                                                            \
		}                                                                      \
		operands->conditionResult = operands->field == value;                  \
	}

/* The same for an 8- or 16-bit value: a compare narrows value to its type. */
#define SCRIPT_DEFINE_OPERAND_NARROW(name, field, type)                        \
	void name(SCRIPT_OPERANDS *operands, s32 operation, s32 value) {           \
		if(operation == SCRIPT_OPERAND_SET) {                                  \
			operands->field = value;                                           \
		}                                                                      \
		else if(operation == SCRIPT_OPERAND_ADD) {                             \
			operands->field += value;                                          \
		}                                                                      \
		else {                                                                 \
			operands->conditionResult = operands->field == (type)value;        \
		}                                                                      \
	}

/* The state value handlers, one for each entry of ACTOR_SCRIPT_VALUE (see */
/* ActorScript_OperandHandlerTable in actor_script_operands.c); each names the
 */
/* actor field it sets, adds to or compares. */
SCRIPT_DEFINE_OPERAND_ADDRESS(fdActorScriptSetOrCompareScriptAddress, scriptAdr)

/* State value SW_CURSOR: sets, adds to or compares the script cursor. */
void fdActorScriptSetOrCompareCursor(SCRIPT_OPERANDS *operands, s32 operation,
									 s32 value) {
	if(operation == SCRIPT_OPERAND_SET) {
		operands->cursor = value;
	}
	else if(operation == SCRIPT_OPERAND_ADD) {
		operands->cursor = ((s16)operands->cursor + value);
	}
	else {
		operands->conditionResult = (s16)operands->cursor == (s16)value;
	}
}

/* State value SW_FACING, and the values 2 and 4 either side of it, which */
/* actor_script_operands.c points here too: sets, adds to or compares the */
/* actor's facing. */
SCRIPT_DEFINE_OPERAND_NARROW(fdActorScriptSetOrCompareFacing, facing, u16)
/* State value SW_X: sets, adds to or compares the actor's x. */
SCRIPT_DEFINE_OPERAND_WORD(fdActorScriptSetOrCompareX, x)
/* State value SW_Y: sets, adds to or compares the actor's y. */
SCRIPT_DEFINE_OPERAND_WORD(fdActorScriptSetOrCompareY, y)
/* State value SW_Z: sets, adds to or compares the actor's z. */
SCRIPT_DEFINE_OPERAND_WORD(fdActorScriptSetOrCompareZ, z)

/* State value SW_RADIUS: sets, adds to or compares the actor's radius. */
void fdActorScriptSetOrCompareRadius(SCRIPT_OPERANDS *state, s32 operation,
									 s32 value) {
	if(operation == SCRIPT_OPERAND_SET) {
		state->radius = value;
		return;
	}
	if(operation == SCRIPT_OPERAND_ADD) {
		state->radius += value;
		return;
	}
	state->conditionResult = state->radius == (s16)value;
}

/* State value SW_SCALE_X: sets, adds to or compares the actor's horizontal */
/* scale. */
SCRIPT_DEFINE_OPERAND_WORD(fdActorScriptSetOrCompareScaleX, scaleX)
/* State value SW_SCALE_Y, and the value 11 after it, which */
/* actor_script_operands.c points here too: sets, adds to or compares the */
/* actor's vertical scale. */
SCRIPT_DEFINE_OPERAND_WORD(fdActorScriptSetOrCompareScaleY, scaleY)
/* State value SW_VELOCITY_X: sets, adds to or compares the actor's x velocity.
 */
SCRIPT_DEFINE_OPERAND_WORD(fdActorScriptSetOrCompareVelocityX, velX)
/* State value SW_VELOCITY_Y: sets, adds to or compares the actor's y velocity.
 */
SCRIPT_DEFINE_OPERAND_WORD(fdActorScriptSetOrCompareVelocityY, velY)
/* State value SW_VELOCITY_Z: sets, adds to or compares the actor's z velocity.
 */
SCRIPT_DEFINE_OPERAND_WORD(fdActorScriptSetOrCompareVelocityZ, velZ)
/* State value SW_SPEED: sets, adds to or compares the actor's top speed. */
SCRIPT_DEFINE_OPERAND_WORD(fdActorScriptSetOrCompareSpeedLimit, speedLimit)
/* State value SW_ACCELERATION: sets, adds to or compares the actor's */
/* acceleration. */
SCRIPT_DEFINE_OPERAND_WORD(fdActorScriptSetOrCompareAcceleration, acceleration)
/* State value SW_TARGET_X: sets, adds to or compares the x the actor heads */
/* for. */
SCRIPT_DEFINE_OPERAND_WORD(fdActorScriptSetOrCompareTargetX, tgtX)
/* State value SW_TARGET_Y: sets, adds to or compares the y the actor heads */
/* for. */
SCRIPT_DEFINE_OPERAND_WORD(fdActorScriptSetOrCompareTargetY, tgtY)
/* State value SW_TARGET_Z: sets, adds to or compares the z the actor heads */
/* for. */
SCRIPT_DEFINE_OPERAND_WORD(fdActorScriptSetOrCompareTargetZ, tgtZ)
/* State value SW_BOUNCE: sets, adds to or compares the actor's bounce. */
SCRIPT_DEFINE_OPERAND_WORD(fdActorScriptSetOrCompareBounce, bounce)
/* State value SW_GRAVITY: sets, adds to or compares the actor's gravity. */
SCRIPT_DEFINE_OPERAND_WORD(fdActorScriptSetOrCompareGravity, gravity)
/* State value SW_GROUND: sets, adds to or compares the ground height under the
 */
/* actor. */
SCRIPT_DEFINE_OPERAND_WORD(fdActorScriptSetOrCompareTerrainHeight, groundHeight)
/* State value SW_SCENE_WORD: sets, adds to or compares the actor's sceneWord,
 */
/* scene code's own word. */
SCRIPT_DEFINE_OPERAND_WORD(fdActorScriptSetOrCompareSceneWord, sceneWord)
/* State value SW_SPRITE: sets, adds to or compares the actor's sprite. */
SCRIPT_DEFINE_OPERAND_ADDRESS(fdActorScriptSetOrCompareSpr, spr)
/* State value SW_SPRITE_KIND: sets, adds to or compares whether the actor */
/* draws no sprite, one or a group. */
SCRIPT_DEFINE_OPERAND_NARROW(fdActorScriptSetOrCompareSprStorageKind,
							 sprStorageKind, u8)
/* State value SW_MOTION: sets, adds to or compares the actor's motion flags. */
SCRIPT_DEFINE_OPERAND_NARROW(fdActorScriptSetOrCompareMotionFlags, motionFlg,
							 u8)
/* State value SW_ARRIVAL: sets, adds to or compares the axis the actor's */
/* arrival is judged on. */
SCRIPT_DEFINE_OPERAND_NARROW(fdActorScriptSetOrCompareArrivalAxis, arrivalAxis,
							 u8)
/* State value SW_RESULT: sets, adds to or compares the last comparison's */
/* result. */
SCRIPT_DEFINE_OPERAND_NARROW(fdActorScriptSetOrCompareComparisonResult,
							 conditionResult, u8)
/* State value SW_SNAP: sets, adds to or compares whether the actor snaps to */
/* its target. */
SCRIPT_DEFINE_OPERAND_NARROW(fdActorScriptSetOrCompareSnapToTarget, snapToTgt,
							 u8)
/* State value SW_COLLISION: sets, adds to or compares the actor's collision */
/* flags. */
SCRIPT_DEFINE_OPERAND_NARROW(fdActorScriptSetOrCompareCollisionFlags,
							 collisionFlg, u8)
/* State value SW_STEERING: sets, adds to or compares the actor's steering */
/* flags. */
SCRIPT_DEFINE_OPERAND_NARROW(fdActorScriptSetOrCompareSteeringFlags,
							 steeringFlg, u8)
/* State value SW_PAUSED: sets, adds to or compares whether the actor is */
/* paused. */
SCRIPT_DEFINE_OPERAND_NARROW(fdActorScriptSetOrComparePaused, paused, u8)
/* State value SW_REPEAT: sets, adds to or compares the script's repeat count.
 */
SCRIPT_DEFINE_OPERAND_NARROW(fdActorScriptSetOrCompareRepeatCount, repeatCnt,
							 u8)

/* State value SW_WAIT: sets, adds to or compares the wait counter. */
SCRIPT_DEFINE_OPERAND_NARROW(fdActorScriptSetOrCompareWait, wait, s16)

/* State value SW_TIMER: sets, adds to or compares the actor's timer. */
SCRIPT_DEFINE_OPERAND_NARROW(fdActorScriptSetOrCompareTimer, tmr, s16)
/* State value SW_USER_VALUE: sets, adds to or compares the actor's user value.
 */
SCRIPT_DEFINE_OPERAND_NARROW(fdActorScriptSetOrCompareUserValue, userValue, s16)
/* State value SW_WATCHED_ACTOR: sets, adds to or compares the actor this one */
/* watches. */
SCRIPT_DEFINE_OPERAND_WORD(fdActorScriptSetOrCompareWatchedActor, watchedActor)
/* State value SW_HOOK: sets, adds to or compares the actor's update function.
 */
SCRIPT_DEFINE_OPERAND_WORD(fdActorScriptSetOrCompareUpdateAddress, update)
/* State value SW_SCENE_COUNTER: sets, adds to or compares the actor's */
/* sceneCnt, scene code's own counter. */
SCRIPT_DEFINE_OPERAND_NARROW(fdActorScriptSetOrCompareSceneCounter, sceneCnt,
							 u8)
/* State value SW_SCENE_BYTE: sets, adds to or compares the actor's sceneByte,
 */
/* scene code's own byte. */
SCRIPT_DEFINE_OPERAND_NARROW(fdActorScriptSetOrCompareSceneByte, sceneByte, u8)

/* Reads the two words after the command (a state value number and a value) and
 */
/* passes operation to that value's handler. */
static __inline__ s32
fdActorScriptApplyOperandCommand(SCRIPT_OPERANDS *operands, s32 operation) {
	s16 commandCursor = operands->cursor;
	const s32 *arguments = (const s32 *)operands->scriptAdr + commandCursor + 1;
	OPERAND_FUNC handler = ActorScript_OperandHandlerTable[arguments[0]];

	if(handler != 0)
		handler(operands, operation, arguments[1]);
	operands->cursor += 3;
	return 1;
}

/* SC_SET(w, v): sets state value w to v. */
s32 fdActorScriptApplyOperandSet(ACTOR *interpreter) {
	SCRIPT_OPERANDS *operands = (SCRIPT_OPERANDS *)interpreter;

	return fdActorScriptApplyOperandCommand(operands, SCRIPT_OPERAND_SET);
}

/* SC_ADD(w, v): adds v to state value w. */
s32 fdActorScriptApplyOperandAdd(ACTOR *interpreter) {
	SCRIPT_OPERANDS *operands = (SCRIPT_OPERANDS *)interpreter;

	return fdActorScriptApplyOperandCommand(operands, SCRIPT_OPERAND_ADD);
}

/* SC_CMP(w, v): tests whether state value w equals v. */
s32 fdActorScriptApplyOperandCompare(ACTOR *interpreter) {
	SCRIPT_OPERANDS *operands = (SCRIPT_OPERANDS *)interpreter;

	return fdActorScriptApplyOperandCommand(operands, SCRIPT_OPERAND_COMPARE);
}

#undef SCRIPT_DEFINE_FLAG_COMMAND
#undef SCRIPT_DEFINE_OPERAND_WORD
#undef SCRIPT_DEFINE_OPERAND_ADDRESS
#undef SCRIPT_DEFINE_OPERAND_NARROW
#undef SCRIPT_LABEL_KEY_MASK
