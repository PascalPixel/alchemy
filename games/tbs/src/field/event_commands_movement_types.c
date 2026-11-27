/* @file */
/* Actor scripts: moving, turning and the built-in movement types. */
/* Part of event_commands.c. */

#include "event_commands_internal.h"

/* A following actor (MOVEMENT_TYPE_FOLLOW_WATCHED) stays its timer's */
/* distance in pixels from the watched actor: from as far as that it walks */
/* toward a point this many pixels along the way for each timer's distance it */
/* is off, and nearer it stands. */
#define FOLLOW_STRIDE 16
/* fdActorFindNearestFacingTarget's reach in pixels, across and in height, and
 */
/* the distance beyond which an actor must be within FACING_TARGET_ANGLE of */
/* where it faces. */
#define FACING_TARGET_REACH	 40
#define FACING_TARGET_HEIGHT 48
#define FACING_TARGET_NEAR	 23
#define FACING_TARGET_ANGLE	 (TRIG_TURN * 3 / 16)

/* The SC_CALL function of MOVEMENT_TYPE_RESET_ACTION: clears an actor's sprite
 */
/* flags, so it has no shadow, and its collision flags, so nothing collides */
/* with it. */
s32 fdActorClearSprAndCollisionFlags(ACTOR *actor) {
	fdActorSetSprFlagsStub(actor, 0);
	actor->collisionFlg = 0;
	return 0;
}

/* The SC_CALL function of MOVEMENT_TYPE_FOLLOW_WATCHED: follows the watched */
/* actor. */
s32 fdActorMoveTowardTarget(ACTOR *actor) {
	s32 step;
	ACTOR *target;
	FIXED deltaX;
	FIXED deltaZ;
	s32 pixelsX;
	s32 pixelsZ;
	FIXED newX;
	FIXED newZ;
	s32 distance;

	target = (ACTOR *)actor->watchedActor;
	if(target != NULL) {
		/* Whole pixels, rounded toward zero. */
		deltaX = target->x.fixed - actor->x.fixed;
		pixelsX = deltaX / FIXED_ONE;
		deltaZ = target->z.fixed - actor->z.fixed;
		pixelsZ = deltaZ / FIXED_ONE;
		distance = sySquart(pixelsX * pixelsX + pixelsZ * pixelsZ);
		step = actor->tmr;
		if(distance >= step) {
			newX = actor->x.fixed +
				   (s32)((u32)pixelsX * toFIXED(FOLLOW_STRIDE)) / step;
			newZ = actor->z.fixed +
				   (s32)((u32)pixelsZ * toFIXED(FOLLOW_STRIDE)) / step;
			fdActorSetMoveTargetStub(actor, newX, actor->y.fixed, newZ);
			fdActorSelectAnimationStub(actor, ANIM_WALK);
		}
		else {
			fdActorSelectAnimationStub(actor, ANIM_STAND);
		}
	}
	return 1;
}

/* The SC_CALL function of MOVEMENT_TYPE_FACE_LEADER: turns the actor toward */
/* its watched actor, at most FACING_STEP a frame. */
s32 fdActorTurnTowardWatchedActor(ACTOR *actor) {
	s16 turnStep;
	u16 currentAngle;
	s32 targetAngle;
	ACTOR *target;

	target = (ACTOR *)actor->watchedActor;
	if(target != NULL) {
		/* Clear ACTOR_FACES_MOTION, so moving does not turn it away again. */
		actor->steeringFlg &= ~ACTOR_FACES_MOTION;
		targetAngle = (u16)syAtan(target->z.fixed - actor->z.fixed,
								  target->x.fixed - actor->x.fixed);
		currentAngle = actor->facing;
		turnStep = targetAngle - currentAngle;
		if(turnStep != 0) {
			if(turnStep > FACING_STEP)
				turnStep = FACING_STEP;
			if(turnStep < -FACING_STEP)
				turnStep = -FACING_STEP;
			actor->facing = (currentAngle + turnStep);
		}
	}
	return 1;
}

/* Starts an actor's movement type: an MOVEMENT_TYPE, or any other value as
 */
/* the actor script to run itself. */
void fdActorSetMovementType(ACTOR *actor, s32 movement) {
	switch(movement) {
	case MOVEMENT_TYPE_STAND:
		movement = (s32)MovementType_StandScript;
		break;
	case MOVEMENT_TYPE_WANDER:
		movement = (s32)MovementType_WanderScript;
		break;
	case MOVEMENT_TYPE_WANDER_NEAR:
		movement = (s32)MovementType_WanderNearScript;
		break;
	case MOVEMENT_TYPE_WANDER_CLOSE:
		movement = (s32)MovementType_WanderCloseScript;
		break;
	case MOVEMENT_TYPE_FOLLOW_WATCHED:
		movement = (s32)MovementType_FollowWatchedScript;
		break;
	case MOVEMENT_TYPE_FACE_LEADER:
		actor->watchedActor = (union ACTOR_OR_EFFECT *)fdActorGetById(
			MainRoutineStateWork.leaderActorId);
		movement = (s32)MovementType_FaceLeaderScript;
		break;
	case MOVEMENT_TYPE_RESET_ACTION:
		movement = (s32)MovementType_ResetActionScript;
		break;
	default:
		break;
	}
	fdActorStartScriptStub(actor, (const s32 *)movement);
}

/* Finds the nearest animated actor to self within FACING_TARGET_REACH pixels */
/* and under FACING_TARGET_HEIGHT of height, ignoring actors more than */
/* FACING_TARGET_ANGLE to either side once they are over FACING_TARGET_NEAR */
/* pixels away; returns it if its sprite is `sprite` (an SPR_ID), else */
/* NULL. */
ACTOR *fdActorFindNearestFacingTarget(ACTOR *self, s32 sprite) {
	ACTOR *entry;
	ACTOR *found;
	s32 slotIndex;
	s32 nearestDistance;
	FIXED heightDifference;
	s32 pixelX;
	s32 pixelZ;
	s32 distance;
	s32 angle;
	s32 angleDifference;

	found = NULL;
	nearestDistance = FACING_TARGET_REACH;
	entry = ActorsWork;
	for(slotIndex = 0; slotIndex < ACTOR_COUNT; slotIndex++, entry++) {
		if(entry->script == NULL)
			continue;
		if(entry == self)
			continue;
		if(entry->sprStorageKind != ACTOR_SPRITE_ONE)
			continue;
		heightDifference = entry->y.fixed - self->y.fixed;
		if(heightDifference >= 0) {
			if(heightDifference > toFIXED(FACING_TARGET_HEIGHT) - 1)
				continue;
		}
		else {
			if(self->y.fixed - entry->y.fixed >
			   toFIXED(FACING_TARGET_HEIGHT) - 1)
				continue;
		}
		pixelX = (entry->x.fixed - self->x.fixed) / FIXED_ONE;
		pixelZ = (entry->z.fixed - self->z.fixed) / FIXED_ONE;
		distance = sySquart(pixelX * pixelX + pixelZ * pixelZ);
		if(distance >= nearestDistance)
			continue;
		angle = (u16)syAtan(entry->z.fixed - self->z.fixed,
							entry->x.fixed - self->x.fixed);
		if(distance > FACING_TARGET_NEAR) {
			angleDifference = (s16)(angle - self->facing);
			if(angleDifference < -(FACING_TARGET_ANGLE - 1))
				continue;
			if(angleDifference > FACING_TARGET_ANGLE - 1)
				continue;
		}
		found = entry;
		nearestDistance = distance;
	}
	if(found == NULL)
		return NULL;
	if(found->spr->entries[0]->anmId != sprite)
		return NULL;
	return found;
}

#undef FOLLOW_STRIDE
#undef FACING_TARGET_REACH
#undef FACING_TARGET_HEIGHT
#undef FACING_TARGET_NEAR
#undef FACING_TARGET_ANGLE
