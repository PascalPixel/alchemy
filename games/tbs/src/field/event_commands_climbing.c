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
  struct Actor *leader = Actor_GetById(gMainRoutineState.leaderActorId);
  s32 result = -1;
  s32 angle = FACING_NEAREST_CARDINAL(leader->facing);
  u8 savedMotionFlags = leader->motionFlags;
  struct FieldState *state = gFieldState;
  s32 spriteFlags = SPRITE_FLAG_SHADOW;
  struct FieldPosition positionStorage;
  struct FieldPosition *position;
  struct Actor *platform;

  /* LEFTOVER(template): position holds &positionStorage, as Field_ClimbDown and
   * Field_ClimbUp point position at their first of two positions. */
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
        gMainRoutineState.leaderActorId,
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
    leader->motionFlags = savedMotionFlags;
    Event_End();
    if (state != NULL)
      state->walkingDistance +=
          Iwram_MulFX(state->walkingDistanceScale, toFIXED(32));
    // Landed on another jump spot: stand, then jump again, testing the gap
    // and the landing afresh.
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

/// Climbs the leader down onto a climbable wall below it, or lets go of the
/// wall it is on (the player presses down at a climbable wall); -1 when there
/// is none to climb.
s32 Field_ClimbDown(void) {
  struct Actor *leader = Actor_GetById(gMainRoutineState.leaderActorId);
  s32 gridX = CELL_MIDDLE(leader->x.part[0].integer);
  s32 gridZ = CELL_MIDDLE(leader->z.part[0].integer);
  s32 result;

  Event_Start();

  if (gMainRoutineState.movementMode == LEADER_MOVEMENT_WALK) {
    s32 index = gridX / MAPGRID_CELL_PIXELS +
                (gridZ / MAPGRID_CELL_PIXELS) * MAPGRID_SIZE;

    // A wall goes down only where the cell below has the same cell behavior.
    if (MAPGRID_CELLS[index].behavior == MAPGRID_CELLS_SOUTH[index].behavior) {
      /* LEFTOVER(frame): two positions, of which only the first is used. */
      struct FieldPosition positions[2];
      struct FieldPosition *position = positions;

      // The ground under the point 16 pixels lower may not be a wall cell
      // of the overhead layer: the check returns nonzero for a wall.
      SET_POSITION(position, leader->x.fixed, leader->y.fixed - toFIXED(16),
                   leader->z.fixed);
      result = MapGrid_IsWallAtFar((struct WorldPosition *)position);
      if (result != 0)
        goto failure;

      // Step to the middle of the cell, face north and climb down a cell at
      // 1 pixel a frame, in free flight (no motion flags) with no shadow.
      Actor_MoveToAndWaitById(gMainRoutineState.leaderActorId, gridX, gridZ);
      leader->speedLimit = FIXED_ONE;
      Actor_FaceDirectionById(gMainRoutineState.leaderActorId, FACING_NORTH, 0);
      Actor_WaitForScriptEndById(gMainRoutineState.leaderActorId);
      leader->steeringFlags = ACTOR_FACES_MOTION;
      leader->motionFlags = 0;
      Actor_SetSpriteFlagsFar(leader, 0);
      Actor_SelectAnimationFar(leader, ANIM_LEADER_CLIMB_DOWN_FROM_TOP);
      Actor_SetMoveTargetFar(leader, toFIXED(gridX),
                             leader->y.fixed - toFIXED(16),
                             toFIXED(gridZ) + toFIXED(16));
      Actor_WaitForMoveById(gMainRoutineState.leaderActorId);
      gMainRoutineState.movementMode = LEADER_MOVEMENT_CLIMB;
    } else {
      goto failure;
    }
  } else {
    // Let go of the wall and fall: ground following and gravity again
    // after a hop of 4 pixels a frame, the ground taken as
    // where the leader is until the ground is read, and the shadow back.
    Actor_SelectAnimationFar(leader, ANIM_LEADER_CLIMB);
    leader->motionFlags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
    leader->velocityY = toFIXED(4);
    leader->groundHeight = leader->y.fixed;
    Actor_SetSpriteFlagsFar(leader, SPRITE_FLAG_SHADOW);
    Event_Wait(6);
    gMainRoutineState.movementMode = LEADER_MOVEMENT_WALK;
    leader->steeringFlags = ACTOR_FACES_MOTION;
    leader->facing = FACING_NORTH;
  }

  Event_End();
  return 0;

failure:
  Event_End();
  return -1;
}

/// Jumps the leader onto a climbable wall ahead, or climbs it over the top of
/// the wall it is on (the player presses up at a climbable wall); -1 when there
/// is none to climb.
s32 Field_ClimbUp(void) {
  struct Actor *leader = Actor_GetById(gMainRoutineState.leaderActorId);
  s16 spriteFlags = SPRITE_FLAG_SHADOW;
  s32 gridX = CELL_MIDDLE(leader->x.part[0].integer);
  s32 gridZ = CELL_MIDDLE(leader->z.part[0].integer);
  s32 result;

  Event_Start();

  if (leader->spriteStorageKind == ACTOR_SPRITE_ONE)
    spriteFlags = leader->sprite->flags;

  if (gMainRoutineState.movementMode == LEADER_MOVEMENT_WALK) {
    s32 index = gridX / MAPGRID_CELL_PIXELS +
                (gridZ / MAPGRID_CELL_PIXELS) * MAPGRID_SIZE;

    if (MAPGRID_CELLS[index].behavior == MAPGRID_CELLS_NORTH[index].behavior) {
      /* LEFTOVER(frame): two positions, of which only the first is used. */
      struct FieldPosition positions[2];
      struct FieldPosition *position = positions;

      // The ground under the leader may not be a wall cell of the overhead
      // layer.
      SET_POSITION(position, leader->x.fixed, leader->y.fixed, leader->z.fixed);
      result = MapGrid_IsWallAtFar((struct WorldPosition *)position);
      if (result != 0)
        goto failure;

      // Step to the middle of the cell, crouch, jump and grab the wall.
      leader->steeringFlags = 0;
      Actor_MoveToAndWaitById(gMainRoutineState.leaderActorId, gridX, gridZ);
      Actor_SelectAnimationFar(leader, ANIM_LEADER_SPRING);
      wait(4);
      Actor_SelectAnimationFar(leader, ANIM_LEADER_LEAP);
      leader->velocityY = toFIXED(4);
      wait(4);
      leader->motionFlags = 0;
      spriteFlags &= (u8)~SPRITE_FLAG_SHADOW;
      Actor_SetSpriteFlagsFar(leader, spriteFlags);
      leader->speedLimit = FIXED_ONE;
      leader->velocityY = 0;
      Actor_SelectAnimationFar(leader, ANIM_LEADER_CLING);
      wait(4);
      gMainRoutineState.movementMode = LEADER_MOVEMENT_CLIMB;
      leader->steeringFlags = ACTOR_FACES_MOTION;
      wait(8);
    } else {
      goto failure;
    }
  } else {
    // Climb over the top: up 8 pixels and a cell north.
    leader->motionFlags = 0;
    Actor_SelectAnimationFar(leader, ANIM_LEADER_CLIMB_OVER_TOP);
    Actor_SetMoveTargetFar(leader, toFIXED(gridX), leader->y.fixed + toFIXED(8),
                           toFIXED(gridZ) - toFIXED(16));
    Actor_WaitForMoveById(gMainRoutineState.leaderActorId);
    leader->motionFlags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
    spriteFlags |= SPRITE_FLAG_SHADOW;
    leader->groundHeight = leader->y.fixed;
    Actor_SetSpriteFlagsFar(leader, spriteFlags);
    Event_Wait(4);
    gMainRoutineState.movementMode = LEADER_MOVEMENT_WALK;
    leader->steeringFlags = ACTOR_FACES_MOTION;
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
