/// @file
/// Jumping and climbing on the field.
// Part of psynergy_effects.c.

#include "../psynergy_effects_internal.h"

// The map's cells (128 to a row), and the same cells seen one row south and
// one row north: TILE_CELLS_SOUTH[i] is the cell below TILE_CELLS[i].
#define TILE_CELLS ((struct MapGridCell *)Ram_MapGridCellBuffer)
#define TILE_CELLS_SOUTH ((struct MapGridCell *)(Ram_MapGridCellBuffer + 0x200))
#define TILE_CELLS_NORTH ((struct MapGridCell *)(Ram_MapGridCellBuffer - 0x200))

/// Makes the leader jump two tiles forward, over the tile in front, when the
/// map allows it (MapGrid_CheckObjectStep). On a log (SPRITE_LOG_WEST_EAST
/// or SPRITE_LOG_NORTH_SOUTH) the log plays its animation 7 and the leader bobs
/// down 2 pixels and back. Each jump adds two tiles to work->walkingDistance,
/// as walking them would, and the leader keeps jumping while it lands on
/// another jump spot (MB_JUMP_FORWARD_UNREACHED). Returns 0 after a jump,
/// -1 when the leader cannot jump; a chain that is blocked after its first jump
/// also returns -1, with the leader standing on the jump spot it reached.
///
/// Called by Field_RunPushTileEvent when the leader has pushed against a jump
/// spot (0xf9 or 0xfe) for more than 12 frames, and by the gap jumps of Goma
/// Cave's pillar and Kolima Forest's bend, which import it. Moves the leader,
/// and takes the field over between FieldScript_Start and FieldScript_End.
s32 Field_TryJumpForward(void) {
  // A chained jump repeats both the gap and landing probes, including their
  // failure returns, before taking control of the field again.
  struct ObjectRuntime *leader = (struct ObjectRuntime *)ObjectEvent_GetById(
      gGameState.selectedObjectEvent);
  s32 result = -1;
  s32 angle = (leader->angle + 0x2000) & 0xc000; // to the nearest quarter
  u8 *flags = &leader->flags;
  u8 saved = *flags;
  struct FieldWork *work = (struct FieldWork *)gEventWork;
  s32 spriteFlags = 1;
  struct FieldPosition positionStorage;
  struct FieldPosition *position;
  struct ObjectRuntime *platform;

  position = &positionStorage;
again:
  // The middle of the tile in front, then of the tile beyond. The tile
  // jumped over may be a wall or a drop, but not ground too high; the landing
  // must be open.
  SET_POSITION(position, (leader->x & 0xfff00000) + 0x80000, leader->y,
               (leader->z & 0xfff00000) + 0x80000);
  Vector_AddPolarOffset(0x100000, angle, position);
  if (MapGrid_CheckObjectStepFar(leader, position) == MAPGRID_STEP_TOO_HIGH)
    return -1;
  SET_POSITION(position, (leader->x & 0xfff00000) + 0x80000, leader->y,
               (leader->z & 0xfff00000) + 0x80000);
  Vector_AddPolarOffset(0x200000, angle, position);
  if (MapGrid_CheckObjectStepFar(leader, position) == MAPGRID_STEP_OPEN) {
    // Suspend terrain following and the shadow (sprite flag bit 0) while
    // airborne: 4 pixels a frame up, 3 a frame across. Restore the original
    // motion and sprite flags after landing, before returning field control.
    if (leader->animationKind == OBJECT_SPRITE_ONE)
      spriteFlags = ((struct AnimationObject *)leader->animation)->flags;
    FieldScript_Start();
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
        gGameState.selectedObjectEvent,
        ((union FieldCoordinate *)&position->x)->part.pixel,
        ((union FieldCoordinate *)&position->z)->part.pixel);
    ObjectDispatch_SelectAnimationFar(leader, 6);
    ObjectDispatch_SetSingleChildFlagsFar((struct DispatchObject *)leader,
                                          spriteFlags);

    // A log underfoot dips 2 pixels with the leader and comes back up.
    if ((platform = Object_FindNearestFacingTarget(
             leader, SPRITE_LOG_WEST_EAST)) != NULL ||
        (platform = Object_FindNearestFacingTarget(
             leader, SPRITE_LOG_NORTH_SOUTH)) != NULL) {
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
    FieldScript_End();
    if (work != NULL)
      work->walkingDistance +=
          Iwram_MulQ16(work->walkingDistanceScale, 0x200000);
    // Landed on another jump spot: stand, then jump again.
    if (MapGrid_GetMetatileBehaviorAtFar(leader->terrainId, position->x,
                                         position->z) ==
        MB_JUMP_FORWARD_UNREACHED) {
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
  struct ObjectRuntime *object = (struct ObjectRuntime *)ObjectEvent_GetById(
      gGameState.selectedObjectEvent);
  s32 gridX =
      8 + ((((union FieldCoordinate *)&object->x)->part.pixel) & 0xfff0);
  s32 gridZ =
      8 + ((((union FieldCoordinate *)&object->z)->part.pixel) & 0xfff0);
  s32 result;

  FieldScript_Start();

  if (gGameState.movementMode == LEADER_MOVEMENT_WALK) {
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
      // of the overhead layer: the check returns nonzero for a wall.
      SET_POSITION(position, object->x, object->y - 0x100000, object->z);
      result = MapGrid_IsWallAtFar((struct WorldPosition *)position);
      if (result != 0)
        goto failure;

      // Step to the middle of the tile, face north and climb down a tile at
      // 1 pixel a frame, in free flight (no motion flags) with no shadow.
      ObjectMotion_SetPositionAndCommit(gGameState.selectedObjectEvent, gridX,
                                        gridZ);
      object->speedLimit = 0x10000;
      ObjectMotion_ArmCallback(gGameState.selectedObjectEvent, 0xc000, 0);
      ObjectEvent_RefreshSelectorById(gGameState.selectedObjectEvent);
      object->actionFlags = OBJ_EVENT_FACES_MOTION;
      object->flags = 0;
      ObjectDispatch_SetSingleChildFlagsFar((struct DispatchObject *)object, 0);
      ObjectDispatch_SelectAnimationFar(object, 13);
      Object_SetMoveTargetFar(object, gridX << 16, object->y - 0x100000,
                              (gridZ << 16) + 0x100000);
      ObjectMotion_CommitCurrentPositionAndActivate(
          gGameState.selectedObjectEvent);
      gGameState.movementMode = LEADER_MOVEMENT_CLIMB;
    } else {
      goto failure;
    }
  } else {
    // Let go of the wall and fall: terrain following and gravity again
    // after a hop of 4 pixels a frame, the ground taken as
    // where the leader is until the terrain is read, and the shadow back.
    ObjectDispatch_SelectAnimationFar(object, 10);
    object->flags = OBJ_EVENT_FOLLOWS_TERRAIN | OBJ_EVENT_FALLS;
    object->velocityY = 0x40000;
    object->terrainHeight = object->y;
    ObjectDispatch_SetSingleChildFlagsFar((struct DispatchObject *)object,
                                          SPRITE_FLAG_SHADOW);
    FieldScript_WaitFrames(6);
    gGameState.movementMode = LEADER_MOVEMENT_WALK;
    object->actionFlags = OBJ_EVENT_FACES_MOTION;
    object->angle = 0xc000;
  }

  FieldScript_End();
  return 0;

failure:
  FieldScript_End();
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
  struct ObjectRuntime *object = (struct ObjectRuntime *)ObjectEvent_GetById(
      gGameState.selectedObjectEvent);
  s16 spriteFlags = 1;
  s32 gridX =
      8 + ((((union FieldCoordinate *)&object->x)->part.pixel) & 0xfff0);
  s32 gridZ =
      8 + ((((union FieldCoordinate *)&object->z)->part.pixel) & 0xfff0);
  s32 result;

  FieldScript_Start();

  if (object->animationKind == OBJECT_SPRITE_ONE)
    spriteFlags = ((struct AnimationObject *)object->animation)->flags;

  if (gGameState.movementMode == LEADER_MOVEMENT_WALK) {
    s32 index = gridX / 16 + (gridZ / 16) * 128;

    if (TILE_CELLS[index].collisionCode ==
        TILE_CELLS_NORTH[index].collisionCode) {
      /* FAKEMATCH(dead): a six-word probe, though only its first three words
       * are the position, which keeps the native 24-byte frame; as one
       * struct FieldPosition, 4 of 183 instructions differ in tbs-en. */
      s32 vector[6];
      struct FieldPosition *position = (struct FieldPosition *)vector;

      // The ground under the leader may not be a wall cell of the overhead
      // layer.
      SET_POSITION(position, object->x, object->y, object->z);
      result = MapGrid_IsWallAtFar((struct WorldPosition *)position);
      if (result != 0)
        goto failure;

      // Step to the middle of the tile, crouch, jump and grab the wall.
      object->actionFlags = 0;
      ObjectMotion_SetPositionAndCommit(gGameState.selectedObjectEvent, gridX,
                                        gridZ);
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
      gGameState.movementMode = LEADER_MOVEMENT_CLIMB;
      object->actionFlags = OBJ_EVENT_FACES_MOTION;
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
    ObjectMotion_CommitCurrentPositionAndActivate(
        gGameState.selectedObjectEvent);
    object->flags = OBJ_EVENT_FOLLOWS_TERRAIN | OBJ_EVENT_FALLS;
    spriteFlags |= 1;
    object->terrainHeight = object->y;
    ObjectDispatch_SetSingleChildFlagsFar((struct DispatchObject *)object,
                                          spriteFlags);
    FieldScript_WaitFrames(4);
    gGameState.movementMode = LEADER_MOVEMENT_WALK;
    object->actionFlags = OBJ_EVENT_FACES_MOTION;
  }

  FieldScript_End();
  return 0;

failure:
  FieldScript_End();
  return -1;
}

#undef TILE_CELLS
#undef TILE_CELLS_SOUTH
#undef TILE_CELLS_NORTH
