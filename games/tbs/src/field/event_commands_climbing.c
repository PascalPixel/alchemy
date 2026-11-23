/// @file
/// Jumping and climbing on the field.
// Part of event_commands.c.

#include "event_commands_internal.h"

/// The map grid's cells seen one row south and one row north:
/// MAPGRID_CELLS_SOUTH[i] is the cell below MAPGRID_CELLS[i].
#define MAPGRID_CELLS_SOUTH (MAPGRID_CELLS + MAPGRID_SIZE)
#define MAPGRID_CELLS_NORTH (MAPGRID_CELLS - MAPGRID_SIZE)
/// The middle pixel of the map cell a whole pixel lies in.
#define CELL_MIDDLE(pixels)                                                    \
  (MAPGRID_CELL_PIXELS / 2 + ((pixels) & MAPGRID_CELL_PIXEL_MASK))

/// Makes the leader jump two cells forward, over the cell in front, when the
/// map allows it (MapGrid_CheckCollision).
s32 Field_TryJumpForward(void) {
  // A chained jump repeats both the gap and landing probes, including their
  // failure returns, before taking control of the field again.
  struct Actor *leader = Actor_GetById(gMainRoutineWork.leaderActorId);
  s32 result = -1;
  s32 angle = FACING_NEAREST_CARDINAL(leader->facing);
  u8 saved = leader->motionFlags;
  struct FieldWork *work = gFieldWork;
  s32 spriteFlags = SPRITE_FLAG_SHADOW;
  struct FieldPosition positionStorage;
  struct FieldPosition *position;
  struct Actor *platform;

  /* LEFTOVER(template): position holds &positionStorage, as Field_ClimbDown and
   * Field_ClimbUp point position at their stack vector. */
  position = &positionStorage;
again:
  // The middle of the cell in front, then of the cell beyond.
  SET_POSITION(position, (leader->x.fixed & FIELD_CELL_MASK) + toFIXED(8),
               leader->y.fixed,
               (leader->z.fixed & FIELD_CELL_MASK) + toFIXED(8));
  Vector_AddPolarOffset(toFIXED(16), angle, position);
  if (MapGrid_CheckCollisionFar(leader, position) == COLLISION_TOO_HIGH)
    return -1;
  SET_POSITION(position, (leader->x.fixed & FIELD_CELL_MASK) + toFIXED(8),
               leader->y.fixed,
               (leader->z.fixed & FIELD_CELL_MASK) + toFIXED(8));
  Vector_AddPolarOffset(toFIXED(32), angle, position);
  if (MapGrid_CheckCollisionFar(leader, position) == COLLISION_NONE) {
    // Suspend ground following and the shadow while airborne: 4 pixels a
    // frame up, 3 a frame across.
    if (leader->spriteStorageKind == ACTOR_SPRITE_ONE)
      spriteFlags = leader->sprite->flags;
    Event_Start();
    Actor_SelectAnimationFar(leader, ANIM_LEADER_SPRING);
    wait(6);
    Sound_PlayFar(SE_JUMP);
    Actor_SelectAnimationFar(leader, ANIM_LEADER_LEAP);
    leader->speedLimit = toFIXED(3);
    leader->acceleration = toFIXED(2);
    leader->velocityY = toFIXED(4);
    leader->motionFlags &= ACTOR_JUMP_KEEPS_MOTION;
    Actor_SetSpriteFlagsFar(leader, spriteFlags & (u8)~SPRITE_FLAG_SHADOW);
    Actor_MoveToAndWaitById(
        gMainRoutineWork.leaderActorId,
        ((struct FieldCoordinate *)&position->x)->part[0].integer,
        ((struct FieldCoordinate *)&position->z)->part[0].integer);
    Actor_SelectAnimationFar(leader, ANIM_LEADER_SPRING);
    Actor_SetSpriteFlagsFar(leader, spriteFlags);

    // A log underfoot dips 2 pixels with the leader and comes back up.
    if ((platform = Actor_FindNearestFacingTarget(
             leader, SPRITE_LOG_WEST_EAST)) != NULL ||
        (platform = Actor_FindNearestFacingTarget(
             leader, SPRITE_LOG_NORTH_SOUTH)) != NULL) {
      Actor_SelectAnimationFar(platform, 7);
      leader->y.fixed -= FIXED_ONE;
      leader->groundHeight -= FIXED_ONE;
      wait(2);
      leader->y.fixed -= FIXED_ONE;
      leader->groundHeight -= FIXED_ONE;
      wait(10);
      leader->y.fixed += FIXED_ONE;
      leader->groundHeight += FIXED_ONE;
      wait(4);
      leader->y.fixed += FIXED_ONE;
      leader->groundHeight += FIXED_ONE;
    } else {
      wait(6);
    }
    leader->motionFlags = saved;
    Event_End();
    if (work != NULL)
      work->walkingDistance +=
          Iwram_MulFX(work->walkingDistanceScale, toFIXED(32));
    // Landed on another jump spot: stand, then jump again.
    if (MapGrid_GetCellBehaviorAtFar(leader->layer, position->x, position->z) ==
        CELL_BEHAVIOR_JUMP_FORWARD_UNREACHED) {
      Actor_SelectAnimationFar(leader, ANIM_STAND);
      wait(6);
      goto again;
    }
    result = 0;
  }
  return result;
}

/// Climbing down (the player presses down at a climbable wall).
s32 Field_ClimbDown(void) {
  struct Actor *actor = Actor_GetById(gMainRoutineWork.leaderActorId);
  s32 gridX = CELL_MIDDLE(actor->x.part[0].integer);
  s32 gridZ = CELL_MIDDLE(actor->z.part[0].integer);
  s32 result;

  Event_Start();

  if (gMainRoutineWork.movementMode == LEADER_MOVEMENT_WALK) {
    s32 index = gridX / MAPGRID_CELL_PIXELS +
                (gridZ / MAPGRID_CELL_PIXELS) * MAPGRID_SIZE;

    // A wall goes down only where the cell below is the same climbing code.
    if (MAPGRID_CELLS[index].behavior == MAPGRID_CELLS_SOUTH[index].behavior) {
      // Six words, of which the position uses three; the rest give the stack
      // frame its 24 bytes.
      s32 vector[6];
      struct FieldPosition *position = (struct FieldPosition *)vector;

      // The ground under the point 16 pixels lower may not be a wall cell
      // of the overhead layer: the check returns nonzero for a wall.
      SET_POSITION(position, actor->x.fixed, actor->y.fixed - toFIXED(16),
                   actor->z.fixed);
      result = MapGrid_IsWallAtFar((struct WorldPosition *)position);
      if (result != 0)
        goto failure;

      // Step to the middle of the cell, face north and climb down a cell at
      // 1 pixel a frame, in free flight (no motion flags) with no shadow.
      Actor_MoveToAndWaitById(gMainRoutineWork.leaderActorId, gridX, gridZ);
      actor->speedLimit = FIXED_ONE;
      Actor_FaceDirectionById(gMainRoutineWork.leaderActorId, FACING_NORTH, 0);
      Actor_WaitForScriptEndById(gMainRoutineWork.leaderActorId);
      actor->steeringFlags = ACTOR_FACES_MOTION;
      actor->motionFlags = 0;
      Actor_SetSpriteFlagsFar(actor, 0);
      Actor_SelectAnimationFar(actor, ANIM_LEADER_CLIMB_DOWN_FROM_TOP);
      Actor_SetMoveTargetFar(actor, toFIXED(gridX),
                             actor->y.fixed - toFIXED(16),
                             toFIXED(gridZ) + toFIXED(16));
      Actor_WaitForMoveById(gMainRoutineWork.leaderActorId);
      gMainRoutineWork.movementMode = LEADER_MOVEMENT_CLIMB;
    } else {
      goto failure;
    }
  } else {
    // Let go of the wall and fall: ground following and gravity again
    // after a hop of 4 pixels a frame, the ground taken as
    // where the leader is until the ground is read, and the shadow back.
    Actor_SelectAnimationFar(actor, ANIM_LEADER_CLIMB);
    actor->motionFlags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
    actor->velocityY = toFIXED(4);
    actor->groundHeight = actor->y.fixed;
    Actor_SetSpriteFlagsFar(actor, SPRITE_FLAG_SHADOW);
    Event_Wait(6);
    gMainRoutineWork.movementMode = LEADER_MOVEMENT_WALK;
    actor->steeringFlags = ACTOR_FACES_MOTION;
    actor->facing = FACING_NORTH;
  }

  Event_End();
  return 0;

failure:
  Event_End();
  return -1;
}

/// Climbing up (the player presses up at a climbable wall).
s32 Field_ClimbUp(void) {
  struct Actor *actor = Actor_GetById(gMainRoutineWork.leaderActorId);
  s16 spriteFlags = SPRITE_FLAG_SHADOW;
  s32 gridX = CELL_MIDDLE(actor->x.part[0].integer);
  s32 gridZ = CELL_MIDDLE(actor->z.part[0].integer);
  s32 result;

  Event_Start();

  if (actor->spriteStorageKind == ACTOR_SPRITE_ONE)
    spriteFlags = actor->sprite->flags;

  if (gMainRoutineWork.movementMode == LEADER_MOVEMENT_WALK) {
    s32 index = gridX / MAPGRID_CELL_PIXELS +
                (gridZ / MAPGRID_CELL_PIXELS) * MAPGRID_SIZE;

    if (MAPGRID_CELLS[index].behavior == MAPGRID_CELLS_NORTH[index].behavior) {
      // Six words, of which the position uses three; the rest give the stack
      // frame its 24 bytes.
      s32 vector[6];
      struct FieldPosition *position = (struct FieldPosition *)vector;

      // The ground under the leader may not be a wall cell of the overhead
      // layer.
      SET_POSITION(position, actor->x.fixed, actor->y.fixed, actor->z.fixed);
      result = MapGrid_IsWallAtFar((struct WorldPosition *)position);
      if (result != 0)
        goto failure;

      // Step to the middle of the cell, crouch, jump and grab the wall.
      actor->steeringFlags = 0;
      Actor_MoveToAndWaitById(gMainRoutineWork.leaderActorId, gridX, gridZ);
      Actor_SelectAnimationFar(actor, ANIM_LEADER_SPRING);
      wait(4);
      Actor_SelectAnimationFar(actor, ANIM_LEADER_LEAP);
      actor->velocityY = toFIXED(4);
      wait(4);
      actor->motionFlags = 0;
      spriteFlags &= (u8)~SPRITE_FLAG_SHADOW;
      Actor_SetSpriteFlagsFar(actor, spriteFlags);
      actor->speedLimit = FIXED_ONE;
      actor->velocityY = 0;
      Actor_SelectAnimationFar(actor, ANIM_LEADER_CLING);
      wait(4);
      gMainRoutineWork.movementMode = LEADER_MOVEMENT_CLIMB;
      actor->steeringFlags = ACTOR_FACES_MOTION;
      wait(8);
    } else {
      goto failure;
    }
  } else {
    // Climb over the top: up 8 pixels and a cell north.
    actor->motionFlags = 0;
    Actor_SelectAnimationFar(actor, ANIM_LEADER_CLIMB_OVER_TOP);
    Actor_SetMoveTargetFar(actor, toFIXED(gridX), actor->y.fixed + toFIXED(8),
                           toFIXED(gridZ) - toFIXED(16));
    Actor_WaitForMoveById(gMainRoutineWork.leaderActorId);
    actor->motionFlags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
    spriteFlags |= SPRITE_FLAG_SHADOW;
    actor->groundHeight = actor->y.fixed;
    Actor_SetSpriteFlagsFar(actor, spriteFlags);
    Event_Wait(4);
    gMainRoutineWork.movementMode = LEADER_MOVEMENT_WALK;
    actor->steeringFlags = ACTOR_FACES_MOTION;
  }

  Event_End();
  return 0;

failure:
  Event_End();
  return -1;
}

#undef MAPGRID_CELLS_SOUTH
#undef MAPGRID_CELLS_NORTH
#undef CELL_MIDDLE
