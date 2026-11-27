/* Jumping and climbing on the field. Part of script_commands.c. */

#include "script_commands_internal.h"

/* The map grid's metatiles seen one row south and one row north:
   MAPGRID_METATILES_SOUTH[i] is the metatile below MAPGRID_METATILES[i]. */
#define MAPGRID_METATILES_SOUTH (MAPGRID_METATILES + MAPGRID_SIZE)
#define MAPGRID_METATILES_NORTH (MAPGRID_METATILES - MAPGRID_SIZE)
/* The middle pixel of the map metatile a whole pixel lies in. */
#define METATILE_MIDDLE(pixels) \
	(MAPGRID_METATILE_PIXELS / 2 + ((pixels) & MAPGRID_METATILE_PIXEL_MASK))

/* Makes the leader jump two metatiles forward, over the metatile in front, when
   the map allows it (fdMapGridCheckCollision). */
Sint32 fdTryJumpForward(void)
{
	ACTOR		*leader = fdActorGetById(MainRoutineStateWork.leaderActorId);
	Sint32		 result = -1;
	Sint32		 angle = FACING_NEAREST_CARDINAL(leader->facing);
	Uint8		 savedMotionFlags = leader->motionFlg;
	FIELD_STATE *state = FieldStateWork;
	Sint32		 spriteFlags = SPRITE_FLAG_SHADOW;
	FIELD_COORDINATE position[3];
	ACTOR			*platform;

again:
	/* The middle of the metatile in front, then of the metatile beyond. */
	position[0].fixed = (leader->x.fixed & FIELD_METATILE_MASK) + toFIXED(8);
	position[1].fixed = leader->y.fixed;
	position[2].fixed = (leader->z.fixed & FIELD_METATILE_MASK) + toFIXED(8);
	syVectorAddPolarOffset(toFIXED(16), angle, &position[0].fixed);
	if(fdMapGridCheckCollisionStub(leader, (FIELD_POS *)position) ==
		COLLISION_TOO_HIGH)
		return -1;
	position[0].fixed = (leader->x.fixed & FIELD_METATILE_MASK) + toFIXED(8);
	position[1].fixed = leader->y.fixed;
	position[2].fixed = (leader->z.fixed & FIELD_METATILE_MASK) + toFIXED(8);
	syVectorAddPolarOffset(toFIXED(32), angle, &position[0].fixed);
	if(fdMapGridCheckCollisionStub(leader, (FIELD_POS *)position) ==
		COLLISION_NONE) {
		/* Suspend ground following and the shadow while airborne: 4 pixels a
		   frame up, 3 a frame across. */
		if(leader->sprStorageKind == ACTOR_SPRITE_ONE)
			spriteFlags = leader->spr->flg;
		fdScriptStart();
		fdActorSelectAnimationStub(leader, ANIM_LEADER_SPRING);
		sySynch(6);
		sySndPlayStub(SE_JUMP);
		fdActorSelectAnimationStub(leader, ANIM_LEADER_LEAP);
		leader->speedLimit = toFIXED(3);
		leader->acceleration = toFIXED(2);
		leader->velY = toFIXED(4);
		leader->motionFlg &= ACTOR_JUMP_KEEPS_MOTION;
		fdActorSetSprFlagsStub(
			leader, spriteFlags & (Uint8)~SPRITE_FLAG_SHADOW);
		fdActorMoveToAndWaitById(MainRoutineStateWork.leaderActorId,
			position[0].part[0].integer, position[2].part[0].integer);
		fdActorSelectAnimationStub(leader, ANIM_LEADER_SPRING);
		fdActorSetSprFlagsStub(leader, spriteFlags);

		/* A log underfoot dips 2 pixels with the leader and comes back up. */
		if((platform = fdActorFindNearestFacingTarget(
				leader, SPRITE_LOG_WEST_EAST)) != NULL ||
			(platform = fdActorFindNearestFacingTarget(
				 leader, SPRITE_LOG_NORTH_SOUTH)) != NULL) {
			fdActorSelectAnimationStub(platform, 7);
			leader->y.fixed -= toFIXED(1);
			leader->groundHeight -= toFIXED(1);
			sySynch(2);
			leader->y.fixed -= toFIXED(1);
			leader->groundHeight -= toFIXED(1);
			sySynch(10);
			leader->y.fixed += toFIXED(1);
			leader->groundHeight += toFIXED(1);
			sySynch(4);
			leader->y.fixed += toFIXED(1);
			leader->groundHeight += toFIXED(1);
		} else {
			sySynch(6);
		}
		leader->motionFlg = savedMotionFlags;
		fdScriptEnd();
		if(state != NULL)
			state->walkingDistance +=
				syMulFX(state->walkingDistanceScale, toFIXED(32));
		/* Landed on another jump spot: stand, then jump again, testing the gap
		   and the landing afresh. */
		if(fdMapGridGetMetatileBehaviorAtStub(leader->layer, position[0].fixed,
			   position[2].fixed) == METATILE_BEHAVIOR_JUMP_FORWARD_UNREACHED) {
			fdActorSelectAnimationStub(leader, ANIM_STAND);
			sySynch(6);
			goto again;
		}
		result = 0;
	}
	return result;
}

/* Climbs the leader down onto a climbable wall below it, or lets go of the wall
   it is on (the player presses down at a climbable wall); -1 when there is none
   to climb. */
Sint32 fdClimbDown(void)
{
	ACTOR *leader = fdActorGetById(MainRoutineStateWork.leaderActorId);
	Sint32 gridX = METATILE_MIDDLE(leader->x.part[0].integer);
	Sint32 gridZ = METATILE_MIDDLE(leader->z.part[0].integer);
	Sint32 result;

	fdScriptStart();

	if(MainRoutineStateWork.movementMode == LEADER_MOVEMENT_WALK) {
		Sint32 index = gridX / MAPGRID_METATILE_PIXELS +
			(gridZ / MAPGRID_METATILE_PIXELS) * MAPGRID_SIZE;

		/* A wall goes down only where the metatile below has the same metatile
		   behavior. */
		if(MAPGRID_METATILES[index].behavior ==
			MAPGRID_METATILES_SOUTH[index].behavior) {
			/* LEFTOVER(frame): two positions, of which only the first is
			 * used. */
			FIELD_POS  positions[2];
			FIELD_POS *position = positions;

			/* The ground under the point 16 pixels lower may not be a wall
			   metatile of the overhead layer: the check returns nonzero for a
			   wall. */
			SET_POSITION(position, leader->x.fixed,
				leader->y.fixed - toFIXED(16), leader->z.fixed);
			result = fdMapGridIsWallAtStub((WORLD_POS *)position);
			if(result != 0)
				goto failure;

			/* Step to the middle of the metatile, face north and climb down a
			   metatile at 1 pixel a frame, in free flight (no motion flags)
			   with no shadow. */
			fdActorMoveToAndWaitById(
				MainRoutineStateWork.leaderActorId, gridX, gridZ);
			leader->speedLimit = toFIXED(1);
			fdActorFaceDirectionById(
				MainRoutineStateWork.leaderActorId, FACING_NORTH, 0);
			fdActorWaitForScriptEndById(MainRoutineStateWork.leaderActorId);
			leader->steeringFlg = ACTOR_FACES_MOTION;
			leader->motionFlg = 0;
			fdActorSetSprFlagsStub(leader, 0);
			fdActorSelectAnimationStub(leader, ANIM_LEADER_CLIMB_DOWN_FROM_TOP);
			fdActorSetMoveTargetStub(leader, toFIXED(gridX),
				leader->y.fixed - toFIXED(16), toFIXED(gridZ) + toFIXED(16));
			fdActorWaitForMoveById(MainRoutineStateWork.leaderActorId);
			MainRoutineStateWork.movementMode = LEADER_MOVEMENT_CLIMB;
		} else {
			goto failure;
		}
	} else {
		/* Let go of the wall and fall: ground following and gravity again after
		   a hop of 4 pixels a frame, the ground taken as where the leader is
		   until the ground is read, and the shadow back. */
		fdActorSelectAnimationStub(leader, ANIM_LEADER_CLIMB);
		leader->motionFlg = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
		leader->velY = toFIXED(4);
		leader->groundHeight = leader->y.fixed;
		fdActorSetSprFlagsStub(leader, SPRITE_FLAG_SHADOW);
		fdScriptWait(6);
		MainRoutineStateWork.movementMode = LEADER_MOVEMENT_WALK;
		leader->steeringFlg = ACTOR_FACES_MOTION;
		leader->facing = FACING_NORTH;
	}

	fdScriptEnd();
	return 0;

failure:
	fdScriptEnd();
	return -1;
}

/* Jumps the leader onto a climbable wall ahead, or climbs it over the top of
   the wall it is on (the player presses up at a climbable wall); -1 when there
   is none to climb. */
Sint32 fdClimbUp(void)
{
	ACTOR *leader = fdActorGetById(MainRoutineStateWork.leaderActorId);
	Sint16 spriteFlags = SPRITE_FLAG_SHADOW;
	Sint32 gridX = METATILE_MIDDLE(leader->x.part[0].integer);
	Sint32 gridZ = METATILE_MIDDLE(leader->z.part[0].integer);
	Sint32 result;

	fdScriptStart();

	if(leader->sprStorageKind == ACTOR_SPRITE_ONE)
		spriteFlags = leader->spr->flg;

	if(MainRoutineStateWork.movementMode == LEADER_MOVEMENT_WALK) {
		Sint32 index = gridX / MAPGRID_METATILE_PIXELS +
			(gridZ / MAPGRID_METATILE_PIXELS) * MAPGRID_SIZE;

		if(MAPGRID_METATILES[index].behavior ==
			MAPGRID_METATILES_NORTH[index].behavior) {
			/* LEFTOVER(frame): two positions, of which only the first is
			 * used. */
			FIELD_POS  positions[2];
			FIELD_POS *position = positions;

			/* The ground under the leader may not be a wall metatile of the
			   overhead layer. */
			SET_POSITION(
				position, leader->x.fixed, leader->y.fixed, leader->z.fixed);
			result = fdMapGridIsWallAtStub((WORLD_POS *)position);
			if(result != 0)
				goto failure;

			/* Step to the middle of the metatile, crouch, jump and grab the
			   wall. */
			leader->steeringFlg = 0;
			fdActorMoveToAndWaitById(
				MainRoutineStateWork.leaderActorId, gridX, gridZ);
			fdActorSelectAnimationStub(leader, ANIM_LEADER_SPRING);
			sySynch(4);
			fdActorSelectAnimationStub(leader, ANIM_LEADER_LEAP);
			leader->velY = toFIXED(4);
			sySynch(4);
			leader->motionFlg = 0;
			spriteFlags &= (Uint8)~SPRITE_FLAG_SHADOW;
			fdActorSetSprFlagsStub(leader, spriteFlags);
			leader->speedLimit = toFIXED(1);
			leader->velY = 0;
			fdActorSelectAnimationStub(leader, ANIM_LEADER_CLING);
			sySynch(4);
			MainRoutineStateWork.movementMode = LEADER_MOVEMENT_CLIMB;
			leader->steeringFlg = ACTOR_FACES_MOTION;
			sySynch(8);
		} else {
			goto failure;
		}
	} else {
		/* Climb over the top: up 8 pixels and a metatile north. */
		leader->motionFlg = 0;
		fdActorSelectAnimationStub(leader, ANIM_LEADER_CLIMB_OVER_TOP);
		fdActorSetMoveTargetStub(leader, toFIXED(gridX),
			leader->y.fixed + toFIXED(8), toFIXED(gridZ) - toFIXED(16));
		fdActorWaitForMoveById(MainRoutineStateWork.leaderActorId);
		leader->motionFlg = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
		spriteFlags |= SPRITE_FLAG_SHADOW;
		leader->groundHeight = leader->y.fixed;
		fdActorSetSprFlagsStub(leader, spriteFlags);
		fdScriptWait(4);
		MainRoutineStateWork.movementMode = LEADER_MOVEMENT_WALK;
		leader->steeringFlg = ACTOR_FACES_MOTION;
	}

	fdScriptEnd();
	return 0;

failure:
	fdScriptEnd();
	return -1;
}

#undef MAPGRID_METATILES_SOUTH
#undef MAPGRID_METATILES_NORTH
#undef METATILE_MIDDLE
