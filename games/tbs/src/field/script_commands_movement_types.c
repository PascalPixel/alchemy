/* Actor scripts: moving, turning and the built-in movement types. Part of
   script_commands.c. */

#include "script_commands_internal.h"

/* A following actor (MOVEMENT_TYPE_FOLLOW_WATCHED) stays its timer's distance
   in pixels from the watched actor: from as far as that it walks toward a point
   this many pixels along the way for each timer's distance it is off, and
   nearer it stands. */
#define FOLLOW_STRIDE		 16
/* fdActorFindNearestFacingTarget's reach in pixels, across and in height, and
   the distance beyond which an actor must be within FACING_TARGET_ANGLE of
   where it faces. */
#define FACING_TARGET_REACH	 40
#define FACING_TARGET_HEIGHT 48
#define FACING_TARGET_NEAR	 23
#define FACING_TARGET_ANGLE	 (TRIG_TURN * 3 / 16)

/* The SC_CALL function of MOVEMENT_TYPE_RESET_ACTION: clears an actor's sprite
   flags, so it has no shadow, and its collision flags, so nothing collides with
   it. */
Sint32 fdActorClearSpriteAndCollisionFlags(ACTOR *actor)
{
	fdActorSetSpriteFlagsStub(actor, 0);
	actor->collisionFlags = 0;
	return 0;
}

/* The SC_CALL function of MOVEMENT_TYPE_FOLLOW_WATCHED: follows the watched
   actor. */
Sint32 fdActorMoveTowardTarget(ACTOR *actor)
{
	Sint32 step;
	ACTOR *target;
	FIXED  deltaX;
	FIXED  deltaZ;
	Sint32 pixelsX;
	Sint32 pixelsZ;
	FIXED  newX;
	FIXED  newZ;
	Sint32 distance;

	target = (ACTOR *)actor->watchedActor;
	if(target != NULL) {
		/* Whole pixels, rounded toward zero. */
		deltaX = target->x.fixed - actor->x.fixed;
		pixelsX = deltaX / toFIXED(1);
		deltaZ = target->z.fixed - actor->z.fixed;
		pixelsZ = deltaZ / toFIXED(1);
		distance = sySquartIWRAM(pixelsX * pixelsX + pixelsZ * pixelsZ);
		step = actor->timer;
		if(distance >= step) {
			newX = actor->x.fixed +
				(Sint32)((Uint32)pixelsX * toFIXED(FOLLOW_STRIDE)) / step;
			newZ = actor->z.fixed +
				(Sint32)((Uint32)pixelsZ * toFIXED(FOLLOW_STRIDE)) / step;
			fdActorSetMoveTargetStub(actor, newX, actor->y.fixed, newZ);
			fdActorSelectAnimationStub(actor, ANIM_WALK);
		} else {
			fdActorSelectAnimationStub(actor, ANIM_STAND);
		}
	}
	return 1;
}

/* The SC_CALL function of MOVEMENT_TYPE_FACE_LEADER: turns the actor toward its
   watched actor, at most FACING_STEP a tick. */
Sint32 fdActorTurnTowardWatchedActor(ACTOR *actor)
{
	Sint16 turnStep;
	Uint16 currentAngle;
	Sint32 targetAngle;
	ACTOR *target;

	target = (ACTOR *)actor->watchedActor;
	if(target != NULL) {
		/* Clear ACTOR_FACES_MOTION, so moving does not turn it away again. */
		actor->steeringFlags &= ~ACTOR_FACES_MOTION;
		targetAngle = (Uint16)syAtan(
			target->z.fixed - actor->z.fixed, target->x.fixed - actor->x.fixed);
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

/* Starts an actor's movement type: an MOVEMENT_TYPE, or any other value as the
   actor script to run itself. */
void fdActorSetMovementType(ACTOR *actor, Sint32 movement)
{
	switch(movement) {
	case MOVEMENT_TYPE_STAND:
		movement = (Sint32)MovementTypeStandScript;
		break;
	case MOVEMENT_TYPE_WANDER:
		movement = (Sint32)MovementTypeWanderScript;
		break;
	case MOVEMENT_TYPE_WANDER_NEAR:
		movement = (Sint32)MovementTypeWanderNearScript;
		break;
	case MOVEMENT_TYPE_WANDER_CLOSE:
		movement = (Sint32)MovementTypeWanderCloseScript;
		break;
	case MOVEMENT_TYPE_FOLLOW_WATCHED:
		movement = (Sint32)MovementTypeFollowWatchedScript;
		break;
	case MOVEMENT_TYPE_FACE_LEADER:
		actor->watchedActor = (union ACTOR_OR_EFFECT *)fdActorGetById(
			MainRoutineStateWork.leaderActorId);
		movement = (Sint32)MovementTypeFaceLeaderScript;
		break;
	case MOVEMENT_TYPE_RESET_ACTION:
		movement = (Sint32)MovementTypeResetActionScript;
		break;
	default:
		break;
	}
	fdActorStartScriptStub(actor, (const Sint32 *)movement);
}

/* Finds the nearest animated actor to self within FACING_TARGET_REACH pixels
   and under FACING_TARGET_HEIGHT of height, ignoring actors more than
   FACING_TARGET_ANGLE to either side once they are over FACING_TARGET_NEAR
   pixels away; returns it if its sprite is `sprite` (an SPR_ID), else NULL. */
ACTOR *fdActorFindNearestFacingTarget(ACTOR *self, Sint32 sprite)
{
	ACTOR *entry;
	ACTOR *found;
	Sint32 slotIndex;
	Sint32 nearestDistance;
	FIXED  heightDifference;
	Sint32 pixelX;
	Sint32 pixelZ;
	Sint32 distance;
	Sint32 angle;
	Sint32 angleDifference;

	found = NULL;
	nearestDistance = FACING_TARGET_REACH;
	entry = ActorsWork;
	for(slotIndex = 0; slotIndex < ACTOR_COUNT; slotIndex++, entry++) {
		if(entry->script == NULL)
			continue;
		if(entry == self)
			continue;
		if(entry->spriteStorageKind != ACTOR_SPRITE_ONE)
			continue;
		heightDifference = entry->y.fixed - self->y.fixed;
		if(heightDifference >= 0) {
			if(heightDifference > toFIXED(FACING_TARGET_HEIGHT) - 1)
				continue;
		} else {
			if(self->y.fixed - entry->y.fixed >
				toFIXED(FACING_TARGET_HEIGHT) - 1)
				continue;
		}
		pixelX = (entry->x.fixed - self->x.fixed) / toFIXED(1);
		pixelZ = (entry->z.fixed - self->z.fixed) / toFIXED(1);
		distance = sySquartIWRAM(pixelX * pixelX + pixelZ * pixelZ);
		if(distance >= nearestDistance)
			continue;
		angle = (Uint16)syAtan(
			entry->z.fixed - self->z.fixed, entry->x.fixed - self->x.fixed);
		if(distance > FACING_TARGET_NEAR) {
			angleDifference = (Sint16)(angle - self->facing);
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
	if(found->sprite->entries[0]->animationId != sprite)
		return NULL;
	return found;
}

#undef FOLLOW_STRIDE
#undef FACING_TARGET_REACH
#undef FACING_TARGET_HEIGHT
#undef FACING_TARGET_NEAR
#undef FACING_TARGET_ANGLE
