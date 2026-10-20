/// @file
/// Jumping and climbing on the field.
/// Part of psynergy_effects.c, compiled only through it.

#include "../psynergy_effects_internal.h"

// The map's cells (128 to a row), and the same cells seen one row south and
// one row north: TILE_CELLS_SOUTH[i] is the cell below TILE_CELLS[i].
#define TILE_CELLS ((struct MapCell *)Ram_MapCellBuffer)
#define TILE_CELLS_SOUTH ((struct MapCell *)(Ram_MapCellBuffer + 0x200))
#define TILE_CELLS_NORTH ((struct MapCell *)(Ram_MapCellBuffer - 0x200))

/// Makes the leader jump two tiles forward, over the tile in front, when the
/// map allows it (Map_CheckObjectStep). On a platform object (sprite 207 or
/// 205) the platform plays its animation 7 and the leader bobs down 2 pixels
/// and back. Each jump adds two tiles to work->walkingDistance, as walking
/// them would, and the leader keeps jumping while it lands on another jump
/// spot (collision 0xf9). Returns 0 after a jump, -1 when the leader cannot
/// jump; a chain that is blocked after its first jump also returns -1, with
/// the leader standing on the jump spot it reached.
///
/// Called by Field_RunPushTileEvent when the leader has pushed against a jump
/// spot (0xf9 or 0xfe) for more than 12 frames, and by the gap jumps of Goma
/// Cave's pillar and Kolima Forest's bend, which import it. Moves the leader,
/// and takes the field over between FieldEvent_Begin and FieldEvent_End.
s32 Field_TryJumpForward(void) {
  // A chained jump repeats both the gap and landing probes, including their
  // failure returns, before taking control of the field again.
  struct ObjectRuntime *leader =
      (struct ObjectRuntime *)Object_GetById(gGameState.selectedActor);
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
  // The middle of the tile in front, then of the tile beyond. The tile
  // jumped over may be a wall (2) or a drop (-1), but not ground more than
  // 8 pixels higher (1); the landing must be within a step's height (0).
  SET_POSITION(pos, (leader->x & 0xfff00000) + 0x80000, leader->y,
               (leader->z & 0xfff00000) + 0x80000);
  Vector_AddPolarOffset(0x100000, angle, pos);
  if (Map_CheckObjectStepFar(leader, pos) == 1)
    return -1;
  SET_POSITION(pos, (leader->x & 0xfff00000) + 0x80000, leader->y,
               (leader->z & 0xfff00000) + 0x80000);
  Vector_AddPolarOffset(0x200000, angle, pos);
  if (Map_CheckObjectStepFar(leader, pos) == 0) {
    // Suspend terrain following and the shadow (sprite flag bit 0) while
    // airborne: 4 pixels a frame up, 3 a frame across. Restore the original
    // motion and sprite flags after landing, before returning field control.
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
    ObjectDispatch_SetSingleChildFlagsFar((struct DispatchObject *)leader,
                                          spriteFlags & 0xfe);
    ObjectMotion_SetPositionAndCommit(
        gGameState.selectedActor,
        ((union FieldCoordinate *)&pos->x)->part.pixel,
        ((union FieldCoordinate *)&pos->z)->part.pixel);
    ObjectDispatch_SelectAnimationFar(leader, 6);
    ObjectDispatch_SetSingleChildFlagsFar((struct DispatchObject *)leader,
                                          spriteFlags);

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
    // Landed on another jump spot: stand, then jump again.
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
/// when there is nothing to climb or the way is blocked by a wall cell.
///
/// Called by Field_RunPushTileEvent for a climbing spot (0xfd) with Down held,
/// and imported by the scenes with ladders and climbable walls (Babi's
/// Lighthouse, Crossbone Isle, Tolbi's ship and others). Sets
/// gGameState.movementMode: 1 while the leader is on the wall, 0 off it.
s32 Field_ClimbDown(void) {
  struct ObjectRuntime *object =
      (struct ObjectRuntime *)Object_GetById(gGameState.selectedActor);
  s32 gridX =
      8 + ((((union FieldCoordinate *)&object->x)->part.pixel) & 0xfff0);
  s32 gridZ =
      8 + ((((union FieldCoordinate *)&object->z)->part.pixel) & 0xfff0);
  s32 result;

  FieldEvent_Begin();

  if (gGameState.movementMode == 0) {
    s32 index = gridX / 16 + (gridZ / 16) * 128;

    // A wall goes down only where the cell below is the same climbing code.
    if (TILE_CELLS[index].collisionCode ==
        TILE_CELLS_SOUTH[index].collisionCode) {
      /* FAKEMATCH(dead): a six-word probe, though only its first three words
       * are the position, which keeps the native 24-byte frame; as one
       * struct FieldPosition, 4 of 163 instructions differ in tbs-en. */
      s32 vector[6];
      struct FieldPosition *position = (struct FieldPosition *)vector;

      // The ground under the point 16 pixels lower may not be a wall cell
      // (collision 0xff) of layer 2: the check returns nonzero for a wall.
      SET_POSITION(position, object->x, object->y - 0x100000, object->z);
      result =
          CheckMapPositionCellOccupiedFar((struct WorldPosition *)position);
      if (result != 0)
        goto failure;

      // Step to the middle of the tile, face north and climb down a tile at
      // 1 pixel a frame, in free flight (motion flags 0) with no shadow.
      ObjectMotion_SetPositionAndCommit(gGameState.selectedActor, gridX, gridZ);
      object->speedLimit = 0x10000;
      ObjectMotion_ArmCallback(gGameState.selectedActor, 0xc000, 0);
      Object_RefreshSelectorById(gGameState.selectedActor);
      object->actionFlags = 1;
      object->flags = 0;
      ObjectDispatch_SetSingleChildFlagsFar((struct DispatchObject *)object, 0);
      ObjectDispatch_SelectAnimationFar(object, 13);
      Object_SetMoveTargetFar(object, gridX << 16, object->y - 0x100000,
                              (gridZ << 16) + 0x100000);
      ObjectMotion_CommitCurrentPositionAndActivate(gGameState.selectedActor);
      gGameState.movementMode = 1;
    } else {
      goto failure;
    }
  } else {
    // Let go of the wall and fall: terrain following and gravity again
    // (motion flags 3) after a hop of 4 pixels a frame, the ground taken as
    // where the leader is until the terrain is read, and the shadow back.
    ObjectDispatch_SelectAnimationFar(object, 10);
    object->flags = 3;
    object->velocityY = 0x40000;
    object->terrainHeight = object->y;
    ObjectDispatch_SetSingleChildFlagsFar((struct DispatchObject *)object, 1);
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
/// to climb or the way is blocked by a wall cell.
///
/// Called by Field_RunPushTileEvent for a climbing spot (0xfd) with Up held,
/// and imported by the scenes with ladders and climbable walls (Babi's
/// Lighthouse, Colosso, Crossbone Isle and others). Sets
/// gGameState.movementMode as Field_ClimbDown does.
s32 Field_ClimbUp(void) {
  struct ObjectRuntime *object =
      (struct ObjectRuntime *)Object_GetById(gGameState.selectedActor);
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

      // The ground under the leader may not be a wall cell (collision 0xff)
      // of layer 2.
      SET_POSITION(position, object->x, object->y, object->z);
      result =
          CheckMapPositionCellOccupiedFar((struct WorldPosition *)position);
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
      ObjectDispatch_SetSingleChildFlagsFar((struct DispatchObject *)object,
                                            spriteFlags);
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
    ObjectDispatch_SetSingleChildFlagsFar((struct DispatchObject *)object,
                                          spriteFlags);
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
