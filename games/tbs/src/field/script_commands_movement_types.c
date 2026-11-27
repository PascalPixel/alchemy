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
Sint32 fdActorClearSprAndCollisionFlags(ACTOR *actor)
{
	fdActorSetSprFlagsStub(actor, 0);
	actor->collisionFlg = 0;
	return 0;
}

/* The SC_CALL function of MOVEMENT_TYPE_FOLLOW_WATCHED: follows the watched
   actor. */
Sint32 fdActorMoveTowardTarget(ACTOR *actor)
{
	Sint32 step;
	ACTOR *tgt;
	FIXED  deltaX;
	FIXED  deltaZ;
	Sint32 pixelsX;
	Sint32 pixelsZ;
	FIXED  newX;
	FIXED  newZ;
	Sint32 distance;

	tgt = (ACTOR *)actor->watchedActor;
	if(tgt != NULL) {
		/* Whole pixels, rounded toward zero. */
		deltaX = tgt->x.fixed - actor->x.fixed;
		pixelsX = deltaX / toFIXED(1);
		deltaZ = tgt->z.fixed - actor->z.fixed;
		pixelsZ = deltaZ / toFIXED(1);
		distance = sySquartIWRAM(pixelsX * pixelsX + pixelsZ * pixelsZ);
		step = actor->tmr;
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

#include "field/actor_turn_toward_watched_actor.c"

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
   pixels away; returns it if its sprite is `spr` (an SPR_ID), else NULL. */
ACTOR *fdActorFindNearestFacingTarget(ACTOR *self, Sint32 spr)
{
	ACTOR *entry;
	ACTOR *found;
	Sint32 slotIdx;
	Sint32 nearestDistance;
	FIXED  heightDifference;
	Sint32 pixelX;
	Sint32 pixelZ;
	Sint32 distance;
	Sint32 ang;
	Sint32 angDifference;

	found = NULL;
	nearestDistance = FACING_TARGET_REACH;
	entry = ActorsWork;
	for(slotIdx = 0; slotIdx < ACTOR_COUNT; slotIdx++, entry++) {
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
		ang = (Uint16)syAtan(
			entry->z.fixed - self->z.fixed, entry->x.fixed - self->x.fixed);
		if(distance > FACING_TARGET_NEAR) {
			angDifference = (Sint16)(ang - self->facing);
			if(angDifference < -(FACING_TARGET_ANGLE - 1))
				continue;
			if(angDifference > FACING_TARGET_ANGLE - 1)
				continue;
		}
		found = entry;
		nearestDistance = distance;
	}
	if(found == NULL)
		return NULL;
	if(found->spr->entries[0]->anmId != spr)
		return NULL;
	return found;
}

#undef FOLLOW_STRIDE
#undef FACING_TARGET_REACH
#undef FACING_TARGET_HEIGHT
#undef FACING_TARGET_NEAR
#undef FACING_TARGET_ANGLE
