/// @file
/// Jumping and climbing on the field.
/// Part of psynergy_effects.c, compiled only through it.

#include "../psynergy_effects_internal.h"

/// The map's cells (128 to a row), and the same cells seen one row south and
/// one row north: TILE_CELLS_SOUTH[i] is the cell below TILE_CELLS[i].
#define TILE_CELLS ((struct MapCell *)Ram_MapCellBuffer)
#define TILE_CELLS_SOUTH ((struct MapCell *)(Ram_MapCellBuffer + 0x200))
#define TILE_CELLS_NORTH ((struct MapCell *)(Ram_MapCellBuffer - 0x200))

/// The event command "leader check ahead": makes the leader jump two tiles
/// forward, over the tile in front, when the map allows it
/// (Map_CheckObjectStep). On a platform object (sprite 207 or 205) the platform
/// plays its animation 7 and the leader bobs down 2 pixels and back. Each jump
/// advances walking distance for PP/status progress, and the leader keeps
/// jumping while it lands on another jump spot (collision 0xf9). Returns 0
/// after a jump, -1 when the leader cannot jump.
s32 Field_TryJumpForward(void) {
  // An ordinary chained-attempt loop grows native English 0x228 -> 0x230.
  // Keep the landing retry, including its existing failure return behavior.
  struct ObjectRuntime *leader = Object_GetById(gGameState.selectedActor);
  s32 result = -1;
  s32 angle = (leader->angle + 0x2000) & 0xc000; // to the nearest quarter
  u8 *flags = &leader->flags;
  u8 saved = *flags;
  struct FieldWork *work = (struct FieldWork *)gEventWork;
  s32 spriteFlags = 1;
  struct FieldPosition position;
  struct FieldPosition *pos;
  struct ObjectRuntime *platform;

  pos = &position;
again:
  // The middle of the tile in front, then of the tile beyond.
  SET_POSITION(pos, (leader->x & 0xfff00000) + 0x80000, leader->y,
               (leader->z & 0xfff00000) + 0x80000);
  Vector_AddPolarOffset(0x100000, angle, pos);
  if (Map_CheckObjectStepFar(leader, pos) == 1)
    return -1;
  SET_POSITION(pos, (leader->x & 0xfff00000) + 0x80000, leader->y,
               (leader->z & 0xfff00000) + 0x80000);
  Vector_AddPolarOffset(0x200000, angle, pos);
  if (Map_CheckObjectStepFar(leader, pos) == 0) {
    // Crouch, then jump there.
    if (leader->animationKind == 1)
      spriteFlags = ((struct AnimationObject *)leader->animation)->flags;
    FieldEvent_Begin();
    ObjectDispatch_SelectAnimationFar(leader, 6);
    WaitFrames(6);
    AudioCommand_PlayFar(SE_JUMP);
    ObjectDispatch_SelectAnimationFar(leader, 7);
    leader->speedLimit = 0x30000;
    leader->acceleration = 0x20000;
    leader->velocityY = 0x40000;
    *flags &= 0x7e;
    ObjectDispatch_SetSingleChildFlagsFar(leader, spriteFlags & 0xfe);
    ObjectMotion_SetPositionAndCommit(
        gGameState.selectedActor,
        ((union FieldCoordinate *)&pos->x)->part.pixel,
        ((union FieldCoordinate *)&pos->z)->part.pixel);
    ObjectDispatch_SelectAnimationFar(leader, 6);
    ObjectDispatch_SetSingleChildFlagsFar(leader, spriteFlags);

    // A platform underfoot dips 2 pixels with the leader and comes back up.
    if ((platform = Object_FindNearestFacingTarget(leader, 207)) != NULL ||
        (platform = Object_FindNearestFacingTarget(leader, 205)) != NULL) {
      ObjectDispatch_SelectAnimationFar(platform, 7);
      leader->y -= 0x10000;
      leader->terrainHeight -= 0x10000;
      WaitFrames(2);
      leader->y -= 0x10000;
      leader->terrainHeight -= 0x10000;
      WaitFrames(10);
      leader->y += 0x10000;
      leader->terrainHeight += 0x10000;
      WaitFrames(4);
      leader->y += 0x10000;
      leader->terrainHeight += 0x10000;
    } else {
      WaitFrames(6);
    }
    *flags = saved;
    FieldEvent_End();
    if (work != NULL)
      work->walkingDistance +=
          Iwram_MulQ16(work->walkingDistanceScale, 0x200000);
    if (GetMapCellCollisionFar(leader->terrainId, pos->x, pos->z) == 0xf9) {
      ObjectDispatch_SelectAnimationFar(leader, 1);
      WaitFrames(6);
      goto again;
    }
    result = 0;
  }
  return result;
}

/// Climbing down (the player presses down at a climbable wall). On the ground,
/// when the cell south of the leader's is the same climbable cell, the leader
/// turns round, grabs the wall and climbs down onto it (climbing pose 13).
/// While climbing, the leader lets go and drops to the ground. Returns 0, or -1
/// when there is nothing to climb or the way is blocked.
s32 Field_ClimbDown(void) {
  struct ObjectRuntime *object = Object_GetById(gGameState.selectedActor);
  s32 gridX =
      8 + ((((union FieldCoordinate *)&object->x)->part.pixel) & 0xfff0);
  s32 gridZ =
      8 + ((((union FieldCoordinate *)&object->z)->part.pixel) & 0xfff0);
  s32 result;

  FieldEvent_Begin();

  if (gGameState.movementMode == 0) {
    s32 index = gridX / 16 + (gridZ / 16) * 128;

    if (TILE_CELLS[index].collisionCode ==
        TILE_CELLS_SOUTH[index].collisionCode) {
      /* FAKEMATCH(dead): a six-word probe, though only its first three words
       * are the position, which keeps the native 24-byte frame; as one
       * struct FieldPosition, 4 of 163 instructions differ in tbs-en. */
      s32 vector[6];
      struct FieldPosition *position = (struct FieldPosition *)vector;

      SET_POSITION(position, object->x, object->y - 0x100000, object->z);
      result = CheckMapPositionCellOccupiedFar((const s32 *)position);
      if (result != 0)
        goto failure;

      // Step to the middle of the tile, face north and climb down a tile.
      ObjectMotion_SetPositionAndCommit(gGameState.selectedActor, gridX, gridZ);
      object->speedLimit = 0x10000;
      ObjectMotion_ArmCallback(gGameState.selectedActor, 0xc000, 0);
      Object_RefreshSelectorById(gGameState.selectedActor);
      object->actionFlags = 1;
      object->flags = 0;
      ObjectDispatch_SetSingleChildFlagsFar(object, 0);
      ObjectDispatch_SelectAnimationFar(object, 13);
      Object_SetMoveTargetFar(object, gridX << 16, object->y - 0x100000,
                              (gridZ << 16) + 0x100000);
      ObjectMotion_CommitCurrentPositionAndActivate(gGameState.selectedActor);
      gGameState.movementMode = 1;
    } else {
      goto failure;
    }
  } else {
    // Let go of the wall and fall.
    ObjectDispatch_SelectAnimationFar(object, 10);
    object->flags = 3;
    object->velocityY = 0x40000;
    object->terrainHeight = object->y;
    ObjectDispatch_SetSingleChildFlagsFar(object, 1);
    FieldEvent_WaitFrames(6);
    gGameState.movementMode = 0;
    object->actionFlags = 1;
    object->angle = 0xc000;
  }

  FieldEvent_End();
  return 0;

failure:
  FieldEvent_End();
  return -1;
}

/// Climbing up (the player presses up at a climbable wall). On the ground, when
/// the cell north of the leader's is the same climbable cell, the leader jumps
/// onto the wall and holds on (climbing pose 12). While climbing, the leader
/// climbs over the top onto the ground. Returns 0, or -1 when there is nothing
/// to climb or the way is blocked.
s32 Field_ClimbUp(void) {
  struct ObjectRuntime *object = Object_GetById(gGameState.selectedActor);
  s16 spriteFlags = 1;
  s32 gridX =
      8 + ((((union FieldCoordinate *)&object->x)->part.pixel) & 0xfff0);
  s32 gridZ =
      8 + ((((union FieldCoordinate *)&object->z)->part.pixel) & 0xfff0);
  s32 result;

  FieldEvent_Begin();

  if (object->animationKind == 1)
    spriteFlags = ((struct AnimationObject *)object->animation)->flags;

  if (gGameState.movementMode == 0) {
    s32 index = gridX / 16 + (gridZ / 16) * 128;

    if (TILE_CELLS[index].collisionCode ==
        TILE_CELLS_NORTH[index].collisionCode) {
      /* FAKEMATCH(dead): a six-word probe, though only its first three words
       * are the position, which keeps the native 24-byte frame; as one
       * struct FieldPosition, 4 of 183 instructions differ in tbs-en. */
      s32 vector[6];
      struct FieldPosition *position = (struct FieldPosition *)vector;

      SET_POSITION(position, object->x, object->y, object->z);
      result = CheckMapPositionCellOccupiedFar((const s32 *)position);
      if (result != 0)
        goto failure;

      // Step to the middle of the tile, crouch, jump and grab the wall.
      object->actionFlags = 0;
      ObjectMotion_SetPositionAndCommit(gGameState.selectedActor, gridX, gridZ);
      ObjectDispatch_SelectAnimationFar(object, 6);
      WaitFrames(4);
      ObjectDispatch_SelectAnimationFar(object, 7);
      object->velocityY = 0x40000;
      WaitFrames(4);
      object->flags = 0;
      spriteFlags &= 0xfe;
      ObjectDispatch_SetSingleChildFlagsFar(object, spriteFlags);
      object->speedLimit = 0x10000;
      object->velocityY = 0;
      ObjectDispatch_SelectAnimationFar(object, 12);
      WaitFrames(4);
      gGameState.movementMode = 1;
      object->actionFlags = 1;
      WaitFrames(8);
    } else {
      goto failure;
    }
  } else {
    // Climb over the top: up 8 pixels and a tile north.
    object->flags = 0;
    ObjectDispatch_SelectAnimationFar(object, 11);
    Object_SetMoveTargetFar(object, gridX << 16, object->y + 0x80000,
                            (gridZ << 16) - 0x100000);
    ObjectMotion_CommitCurrentPositionAndActivate(gGameState.selectedActor);
    object->flags = 3;
    spriteFlags |= 1;
    object->terrainHeight = object->y;
    ObjectDispatch_SetSingleChildFlagsFar(object, spriteFlags);
    FieldEvent_WaitFrames(4);
    gGameState.movementMode = 0;
    object->actionFlags = 1;
  }

  FieldEvent_End();
  return 0;

failure:
  FieldEvent_End();
  return -1;
}

#undef TILE_CELLS
#undef TILE_CELLS_SOUTH
#undef TILE_CELLS_NORTH
