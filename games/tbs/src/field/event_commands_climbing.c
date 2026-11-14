/// @file
/// Jumping and climbing on the field.
// Part of event_commands.c.

#include "event_commands_internal.h"

// The map's cells (128 to a row), and the same cells seen one row south and
// one row north: TILE_CELLS_SOUTH[i] is the cell below TILE_CELLS[i].
#define TILE_CELLS ((struct MapGridCell *)Ram_MapGridCellBuffer)
#define TILE_CELLS_SOUTH (TILE_CELLS + 128)
#define TILE_CELLS_NORTH (TILE_CELLS - 128)

/// Makes the leader jump two cells forward, over the cell in front, when the
/// map allows it (MapGrid_CheckCollision).
s32 Field_TryJumpForward(void) {
  // A chained jump repeats both the gap and landing probes, including their
  // failure returns, before taking control of the field again.
  struct Actor *leader = Actor_GetById(gMainRoutineWork.leaderActorId);
  s32 result = -1;
  s32 angle =
      (leader->facing + TRIG_TURN / 8) & 0xc000; // to the nearest quarter
  u8 *flags = &leader->motionFlags;
  u8 saved = *flags;
  struct FieldWork *work = gFieldWork;
  s32 spriteFlags = 1;
  struct FieldPosition positionStorage;
  struct FieldPosition *position;
  struct Actor *platform;

  /* FAKEMATCH(temporary): position holds &positionStorage; as
   * &positionStorage and positionStorage.x directly, 57 of 239 instructions
   * differ in tbs-en. */
  position = &positionStorage;
again:
  // The middle of the cell in front, then of the cell beyond.
  SET_POSITION(position, (leader->x.fixed & FIELD_CELL_MASK) + px(8),
               leader->y.fixed, (leader->z.fixed & FIELD_CELL_MASK) + px(8));
  Vector_AddPolarOffset(px(16), angle, position);
  if (MapGrid_CheckCollisionFar(leader, position) == COLLISION_TOO_HIGH)
    return -1;
  SET_POSITION(position, (leader->x.fixed & FIELD_CELL_MASK) + px(8),
               leader->y.fixed, (leader->z.fixed & FIELD_CELL_MASK) + px(8));
  Vector_AddPolarOffset(px(32), angle, position);
  if (MapGrid_CheckCollisionFar(leader, position) == COLLISION_NONE) {
    // Suspend ground following and the shadow (sprite flag bit 0) while
    // airborne: 4 pixels a frame up, 3 a frame across.
    if (leader->spriteStorageKind == ACTOR_SPRITE_ONE)
      spriteFlags = leader->sprite->flags;
    Event_Start();
    Actor_SelectAnimationFar(leader, ANIM_LEADER_SPRING);
    wait(6);
    Sound_PlayFar(SE_JUMP);
    Actor_SelectAnimationFar(leader, ANIM_LEADER_LEAP);
    leader->speedLimit = px(3);
    leader->acceleration = px(2);
    leader->velocityY = px(4);
    *flags &= ACTOR_JUMP_KEEPS_MOTION;
    Actor_SetSpriteFlagsFar(leader, spriteFlags & 0xfe);
    Actor_MoveToAndWaitById(
        gMainRoutineWork.leaderActorId,
        ((union FieldCoordinate *)&position->x)->part.pixel,
        ((union FieldCoordinate *)&position->z)->part.pixel);
    Actor_SelectAnimationFar(leader, ANIM_LEADER_SPRING);
    Actor_SetSpriteFlagsFar(leader, spriteFlags);

    // A log underfoot dips 2 pixels with the leader and comes back up.
    if ((platform = Actor_FindNearestFacingTarget(
             leader, SPRITE_LOG_WEST_EAST)) != NULL ||
        (platform = Actor_FindNearestFacingTarget(
             leader, SPRITE_LOG_NORTH_SOUTH)) != NULL) {
      Actor_SelectAnimationFar(platform, 7);
      leader->y.fixed -= Q16_ONE;
      leader->groundHeight -= Q16_ONE;
      wait(2);
      leader->y.fixed -= Q16_ONE;
      leader->groundHeight -= Q16_ONE;
      wait(10);
      leader->y.fixed += Q16_ONE;
      leader->groundHeight += Q16_ONE;
      wait(4);
      leader->y.fixed += Q16_ONE;
      leader->groundHeight += Q16_ONE;
    } else {
      wait(6);
    }
    *flags = saved;
    Event_End();
    if (work != NULL)
      work->walkingDistance += Iwram_MulQ16(work->walkingDistanceScale, px(32));
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
  s32 gridX =
      8 + ((((union FieldCoordinate *)&actor->x.fixed)->part.pixel) & 0xfff0);
  s32 gridZ =
      8 + ((((union FieldCoordinate *)&actor->z.fixed)->part.pixel) & 0xfff0);
  s32 result;

  Event_Start();

  if (gMainRoutineWork.movementMode == LEADER_MOVEMENT_WALK) {
    s32 index = gridX / 16 + (gridZ / 16) * 128;

    // A wall goes down only where the cell below is the same climbing code.
    if (TILE_CELLS[index].behavior == TILE_CELLS_SOUTH[index].behavior) {
      // Six words, of which the position uses three; the rest give the stack
      // frame its 24 bytes.
      s32 vector[6];
      struct FieldPosition *position = (struct FieldPosition *)vector;

      // The ground under the point 16 pixels lower may not be a wall cell
      // of the overhead layer: the check returns nonzero for a wall.
      SET_POSITION(position, actor->x.fixed, actor->y.fixed - px(16),
                   actor->z.fixed);
      result = MapGrid_IsWallAtFar((struct WorldPosition *)position);
      if (result != 0)
        goto failure;

      // Step to the middle of the cell, face north and climb down a cell at
      // 1 pixel a frame, in free flight (no motion flags) with no shadow.
      Actor_MoveToAndWaitById(gMainRoutineWork.leaderActorId, gridX, gridZ);
      actor->speedLimit = Q16_ONE;
      Actor_FaceDirectionById(gMainRoutineWork.leaderActorId, FACING_NORTH, 0);
      Actor_WaitForScriptEndById(gMainRoutineWork.leaderActorId);
      actor->steeringFlags = ACTOR_FACES_MOTION;
      actor->motionFlags = 0;
      Actor_SetSpriteFlagsFar(actor, 0);
      Actor_SelectAnimationFar(actor, ANIM_LEADER_CLIMB_DOWN_FROM_TOP);
      Actor_SetMoveTargetFar(actor, px(gridX), actor->y.fixed - px(16),
                             px(gridZ) + px(16));
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
    actor->velocityY = px(4);
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
  s16 spriteFlags = 1;
  s32 gridX =
      8 + ((((union FieldCoordinate *)&actor->x.fixed)->part.pixel) & 0xfff0);
  s32 gridZ =
      8 + ((((union FieldCoordinate *)&actor->z.fixed)->part.pixel) & 0xfff0);
  s32 result;

  Event_Start();

  if (actor->spriteStorageKind == ACTOR_SPRITE_ONE)
    spriteFlags = actor->sprite->flags;

  if (gMainRoutineWork.movementMode == LEADER_MOVEMENT_WALK) {
    s32 index = gridX / 16 + (gridZ / 16) * 128;

    if (TILE_CELLS[index].behavior == TILE_CELLS_NORTH[index].behavior) {
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
      actor->velocityY = px(4);
      wait(4);
      actor->motionFlags = 0;
      spriteFlags &= 0xfe;
      Actor_SetSpriteFlagsFar(actor, spriteFlags);
      actor->speedLimit = Q16_ONE;
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
    Actor_SetMoveTargetFar(actor, px(gridX), actor->y.fixed + px(8),
                           px(gridZ) - px(16));
    Actor_WaitForMoveById(gMainRoutineWork.leaderActorId);
    actor->motionFlags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
    spriteFlags |= 1;
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

#undef TILE_CELLS
#undef TILE_CELLS_SOUTH
#undef TILE_CELLS_NORTH
