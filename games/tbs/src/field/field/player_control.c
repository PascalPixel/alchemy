/// @file
/// Player control: the configured keys and moving objects by the pad.
// Part of field.c.

#include "../field_internal.h"

// What differs by edition. On the world map, L and R raise their
// fixed events while held in the Japanese release, and once for each press
// from the English one on. The world map's probes for a step reach 8 pixels in
// Japan and 7 from the English release on.
#if TBS_PATCH_1
#define FIELD_SHORTCUT_KEYS gKeysPressed
#define WORLD_MAP_PROBE_RADIUS 0x70000
#else
#define FIELD_SHORTCUT_KEYS gKeysHeld
#define WORLD_MAP_PROBE_RADIUS 0x80000
#endif

// The leader's speeds and accelerations, in pixels a frame as 16.16 fixed
// point: the top 16 bits are whole pixels, so 0x10000 is one pixel.
#define FIELD_WALK_SPEED 0x10000      ///< 1 pixel a frame
#define FIELD_RUN_SPEED 0x18000       ///< 1.5, with a dash key held
#define FIELD_ACCELERATION 0x4000     ///< a quarter pixel faster each frame
#define FIELD_FAST_WALK_SPEED 0x40000 ///< 4, B held with FLAG_FAST_WALK
#define FIELD_FAST_WALK_ACCELERATION 0x10000
#define WORLD_MAP_WALK_SPEED 0x8000 ///< half a pixel a frame
#define WORLD_MAP_RUN_SPEED 0x10000 ///< 1, with a dash key held
#define WORLD_MAP_RUN_ACCELERATION 0x14000
#define WORLD_MAP_ACCELERATION 0x4000
#define WORLD_MAP_FAST_SPEED 0x40000 ///< 4, while L repeats

/// What a shortcut key's assignment requests: its top two bits.
enum FieldShortcutKind {
  FIELD_SHORTCUT_PSYNERGY = 0, ///< fieldWork->command, an ability
  FIELD_SHORTCUT_ITEM = 1      ///< fieldWork->item
};

/// Stores value in a halfword of the event work.
static __inline__ void Field_StoreHalfword(s16 *address, s32 value) {
  // XXX: why the stores go through a helper rather than plain assignment;
  // whether the assignment form changes Field_RequestShortcut's bytes would
  // settle it.
  *address = value;
}

/// Unused: stores value as a word at the object's unk6c.
/// Nothing calls it.
UNUSED void ObjectDispatch_SetUpdateValue(void *rawObject, s32 value) {
  struct ObjectRuntime *object = rawObject;

  if (object != NULL)
    *(s32 *)object->unk6c = value;
}

/// Handles a shortcut key's assignment: its top two bits choose what it
/// requests (enum FieldShortcutKind) and the low 14 bits which one. With
/// FLAG_MENUS_RAISE_EVENT set it raises FIELD_EVENT_MENUS_RAISED instead, as
/// the menus do then; on the world map R raises
/// FIELD_EVENT_WORLD_MAP_PICTURE and L FIELD_EVENT_WORLD_MAP_ZOOM. Returns the
/// request id, the assignment's low 14 bits, even when it raised an event
/// instead; a shortcut with nothing assigned returns 0, as no key pressed does.
///
/// Called by Field_CheckConfiguredKeys. Sets fieldWork->command,
/// fieldWork->item or fieldWork->event, which the scene loop (run_scene.c)
/// acts on.
u32 Field_RequestShortcut(u32 encodedAssignment) {
  u32 assignmentKind = encodedAssignment >> 14;
  u32 requestId = 0x3FFF & encodedAssignment;
  struct FieldWork *fieldWork = (struct FieldWork *)gEventWork;

  if (GameFlag_TestFar(FLAG_MENUS_RAISE_EVENT) != 0) {
    Field_StoreHalfword(&fieldWork->event, FIELD_EVENT_MENUS_RAISED);
  } else if (fieldWork->mode == MODE_WORLD_MAP) {
    if (FIELD_SHORTCUT_KEYS & KEY_R) {
      Field_StoreHalfword(&fieldWork->event,
                          (u16)FIELD_EVENT_WORLD_MAP_PICTURE);
    } else if (FIELD_SHORTCUT_KEYS & KEY_L) {
      Field_StoreHalfword(&fieldWork->event, (u16)FIELD_EVENT_WORLD_MAP_ZOOM);
    }
  } else {
    switch (assignmentKind) {
    case FIELD_SHORTCUT_PSYNERGY:
      Field_StoreHalfword(&fieldWork->command, requestId);
      break;
    case FIELD_SHORTCUT_ITEM:
      Field_StoreHalfword(&fieldWork->item, requestId);
      break;
    }
  }

  return requestId;
}

/// Checks the keys the player has configured: Select, A and Start set their
/// flag in the event work and return 1; the two shortcut keys pass on their
/// assignment (Field_RequestShortcut) and return its request id. Returns 0
/// when none is pressed, or with no field work. Every caller drops the result.
/// Only the first configured key pressed this frame counts, in that order.
///
/// Called each frame by the leader's control commands: SC_PLAYER, SC_WORLD
/// and SC_KEY_COUNT. Sets fieldWork->menu, examine or start.
s32 Field_CheckConfiguredKeys(void) {
  struct FieldWork *fieldWork = (struct FieldWork *)gEventWork;
  s32 result = 0;

  if (fieldWork == NULL)
    return 0;
  if (gKeysPressed & gGameState.selectKeys) {
    fieldWork->menu = 1;
    result = 1;
  } else if (gKeysPressed & gGameState.aKeys) {
    fieldWork->examine = 1;
    result = 1;
  } else if (gKeysPressed & gGameState.startKeys) {
    fieldWork->start = 1;
    result = 1;
  } else if (gKeysPressed & gGameState.firstShortcutKeys) {
    result = Field_RequestShortcut(gGameState.firstShortcut);
  } else if (gKeysPressed & gGameState.secondShortcutKeys) {
    result = Field_RequestShortcut(gGameState.secondShortcut);
  }
  return result;
}

/// Returns 0 when two spheres overlap: at positions a and b (16.16 pixels)
/// with radii radiusA and radiusB (pixels); -1 otherwise. Called by
/// SC_PLAYER's push test, ScriptObject_CheckOverlap and
/// ScriptObject_FindOverlappingEntry.
s32 Runtime_CheckRadiusOverlap(s32 *firstPosition, s32 radiusA,
                               s32 *secondPosition, s32 radiusB) {
  const struct FieldPosition *firstPoint =
      (const struct FieldPosition *)firstPosition;
  const struct FieldPosition *secondPoint =
      (const struct FieldPosition *)secondPosition;
  const s32 *firstCoordinate = &firstPoint->x;
  const s32 *secondCoordinate = &secondPoint->x;
  s32 dx = (*firstCoordinate++ - *secondCoordinate++) >> 16;
  s32 dy = (*firstCoordinate++ - *secondCoordinate++) >> 16;
  s32 dz = (*firstCoordinate - *secondCoordinate) >> 16;
  s32 radius = radiusA + radiusB;

  // dx and dz are already whole pixels (at most 32768), so these tests
  // always pass and only the distance decides.
  // XXX: likely meant as a quick reject of points over 64 pixels apart,
  // because 0x400000 is 64 pixels in 16.16, the units before the shift.
  if (!(dx > 0x400000) && !(dz > 0x400000) &&
      dx * dx + dy * dy + dz * dz < radius * radius)
    return 0;
  return -1;
}

/// SC_PLAYER: moves the player on the field by the direction pad: walking or
/// running, sliding around walls, pushing movable objects, turning, playing
/// the matching animation and leaving footprints. Also checks the configured
/// keys. Returns 1.
///
/// Run each frame by gObjectDispatch_PlayerScript, which the scene loop
/// (run_scene.c) starts on the leader through
/// ObjectDispatch_StartPlayerControl when it hands control back on the field.
/// Sets the leader's speed, target, velocity, angle and action, moves the
/// objects it pushes, counts gEventWork->blockedSteps and creates the
/// footprint objects.
s32 FieldObject_UpdatePlayerControl(struct ScriptInterpreter *interpreter) {
  struct ObjectRuntime *object = (struct ObjectRuntime *)interpreter;

  struct FieldPosition position;
  struct FieldPosition testPosition;
  struct FieldPosition contact;
  s16 angleOffsets[22];
  s32 collision;
  s32 pushed;
  s16 heading;
  s32 angle;
  s32 movementAnimation;
  u16 direction;
  s32 footprintAngle;
  s32 angleIndex;
  s32 headingCount;
  struct ObjectRuntime *pushedObject;

  collision = 0;
  pushed = 0;
  // Debug (flag 350): beep when fewer than 136 of the 512 resource blocks
  // are free.
  if (gDebugMode != 0 && GameFlag_TestFar(350) != 0) {
    s32 freeBlocks;
    s32 blockIndex;

    freeBlocks = 0;
    for (blockIndex = 0; blockIndex < 512; blockIndex++) {
      if (gResourceBlockOwners[blockIndex] == 0xff)
        freeBlocks++;
    }
    if (freeBlocks - 136 < 0)
      AudioCommand_PlayFar(SE_135);
  }
  /* FAKEMATCH(volatile): the volatile read puts gDebugMode's address in r5 here
   * and at the test before it, as native; as gDebugMode != 0, 9 of 808
   * instructions differ in tbs-en. */
  // Debug: holding L spins through four empty loops (511, 96, 64 and 64
  // passes), a short stall each frame.
  if (*(volatile u8 *)&gDebugMode != 0) {
    s32 delayKeyMask;

    delayKeyMask = KEY_L;
    if (gKeysHeld & delayKeyMask) {
      s32 delayIterations;

      delayIterations = delayKeyMask;
    wait_a:
      --delayIterations;
      if (delayIterations != 0)
        goto wait_a;
      delayIterations = 95;
    wait_b:
      delayIterations--;
      if (delayIterations >= 0)
        goto wait_b;
      delayIterations = 63;
    wait_c:
      delayIterations--;
      if (delayIterations >= 0)
        goto wait_c;
      delayIterations = 63;
    wait_d:
      delayIterations--;
      if (delayIterations >= 0)
        goto wait_d;
    }
  }
  // Walk, or run with the dash keys (animation 5 instead of 2); with
  // FLAG_FAST_WALK, B moves faster still.
  if (gKeysHeld & gGameState.dashKeys) {
    object->speedLimit = FIELD_RUN_SPEED;
    object->acceleration = FIELD_ACCELERATION;
    movementAnimation = 5;
  } else {
    object->speedLimit = FIELD_WALK_SPEED;
    object->acceleration = FIELD_ACCELERATION;
    movementAnimation = 2;
  }
  if (GameFlag_TestFar(FLAG_FAST_WALK) != 0 && (gKeysHeld & KEY_B)) {
    object->speedLimit = FIELD_FAST_WALK_SPEED;
    object->acceleration = FIELD_FAST_WALK_ACCELERATION;
    movementAnimation = 5;
  }

  // Step 8 pixels the way the direction pad points, when that step and the
  // ones 22.5 and 45 degrees either side are clear (debug L walks through
  // walls); otherwise try headings up to 67.5 degrees off. Bit 2 of
  // collision means no direction is held, bit 0 that every heading is
  // blocked.
  angle = gWorldMap_KeyHeadings[(gKeysHeld >> 4) & 15];
  if ((u16)angle == 0xffff) {
    collision |= 4;
  } else {
    collision = 0;
    position.x = object->x;
    position.y = object->y;
    position.z = object->z;
    Vector_AddPolarOffset(0x80000, (u16)angle, &position);
    if (gDebugMode != 0 && (gKeysHeld & KEY_L)) {
      heading = angle;
    } else {
      if (Map_CheckObjectStep(object, &position) == MAP_STEP_OPEN) {
        testPosition.x = object->x;
        testPosition.y = object->y;
        testPosition.z = object->z;
        Vector_AddPolarOffset(0x80000, (u16)angle + 0x1000, &testPosition);
        if (Map_CheckObjectStep(object, &testPosition) == MAP_STEP_OPEN) {
          testPosition.x = object->x;
          testPosition.y = object->y;
          testPosition.z = object->z;
          Vector_AddPolarOffset(0x80000, (u16)angle - 0x1000, &testPosition);
          if (Map_CheckObjectStep(object, &testPosition) == MAP_STEP_OPEN) {
            testPosition.x = object->x;
            testPosition.y = object->y;
            testPosition.z = object->z;
            Vector_AddPolarOffset(0x80000, (u16)angle + 0x2000, &testPosition);
            if (Map_CheckObjectStep(object, &testPosition) == MAP_STEP_OPEN) {
              testPosition.x = object->x;
              testPosition.y = object->y;
              testPosition.z = object->z;
              Vector_AddPolarOffset(0x80000, (u16)angle - 0x2000,
                                    &testPosition);
              if (Map_CheckObjectStep(object, &testPosition) == MAP_STEP_OPEN) {
                heading = angle;
                goto push_objects;
              }
            }
          }
        }
      }
      // The pad's way is blocked: try 22.5, 45 and 67.5 degrees off, nearest
      // first, each needing the same three clear probes. The first that
      // passes slides the player along the wall.
      direction = angle;
      headingCount = 0;
      angleOffsets[headingCount++] = direction + 0x1000;
      angleOffsets[headingCount++] = direction - 0x1000;
      angleOffsets[headingCount++] = direction + 0x2000;
      angleOffsets[headingCount++] = direction - 0x2000;
      angleOffsets[headingCount++] = direction + 0x3000;
      angleOffsets[headingCount++] = direction - 0x3000;
      for (angleIndex = 0; angleIndex < headingCount; angleIndex++) {
        heading = angleOffsets[angleIndex];
        position.x = object->x;
        position.y = object->y;
        position.z = object->z;
        Vector_AddPolarOffset(0x80000, (u16)heading, &position);
        if (Map_CheckObjectStep(object, &position) != MAP_STEP_OPEN)
          continue;
        testPosition.x = object->x;
        testPosition.y = object->y;
        testPosition.z = object->z;
        Vector_AddPolarOffset(0x80000, (u16)heading + 0x1000, &testPosition);
        if (Map_CheckObjectStep(object, &testPosition) != MAP_STEP_OPEN)
          continue;
        testPosition.x = object->x;
        testPosition.y = object->y;
        testPosition.z = object->z;
        Vector_AddPolarOffset(0x80000, (u16)heading - 0x1000, &testPosition);
        if (Map_CheckObjectStep(object, &testPosition) != MAP_STEP_OPEN)
          continue;
        testPosition.x = object->x;
        testPosition.y = object->y;
        testPosition.z = object->z;
        Vector_AddPolarOffset(0x80000, (u16)heading + 0x2000, &testPosition);
        if (Map_CheckObjectStep(object, &testPosition) != MAP_STEP_OPEN)
          continue;
        testPosition.x = object->x;
        testPosition.y = object->y;
        testPosition.z = object->z;
        Vector_AddPolarOffset(0x80000, (u16)heading - 0x2000, &testPosition);
        if (Map_CheckObjectStep(object, &testPosition) == MAP_STEP_OPEN)
          goto push_objects;
      }
      // Nothing passed: the step is the object's own position.
      position.x = object->x;
      position.y = object->y;
      position.z = object->z;
      collision |= 1;

    push_objects:
      // Push the movable objects (ACTOR_PUSHABLE) just ahead 4 pixels
      // further, when they have room; any other solid object there blocks the
      // step (bit 1 of collision). Pushing slows the player down.
      contact.x = object->x;
      contact.y = object->y;
      contact.z = object->z;
      Vector_AddPolarOffset(0x40000, (u16)heading, &contact);
      pushedObject = gObjectSlots;
      for (angleIndex = 63; angleIndex >= 0; angleIndex -= 1, pushedObject++) {
        s32 radius;

        radius = object->radius - 2;
        if (pushedObject->script == 0)
          continue;
        if ((pushedObject->collisionFlags & ACTOR_SOLID) == 0)
          continue;
        if (pushedObject == object)
          continue;
        if (Runtime_CheckRadiusOverlap(&pushedObject->x, radius, &contact.x,
                                       pushedObject->radius - 2) < 0)
          continue;
        // A movable object that is not paused slides a quarter pixel away from
        // the player, if no solid object is there and the ground 10 pixels on,
        // straight and 22.5 degrees either side, is clear.
        if ((pushedObject->collisionFlags & ACTOR_PUSHABLE) != 0 &&
            pushedObject->movementState == 0) {
          direction =
              ArcTan2(pushedObject->z - object->z, pushedObject->x - object->x);
          testPosition.x = pushedObject->x;
          testPosition.y = pushedObject->y;
          testPosition.z = pushedObject->z;
          heading = direction;
          Vector_AddPolarOffset(0x4000, direction, &testPosition);
          if (!ScriptObject_CheckOverlap(
                  (struct ScriptObjectEntry *)pushedObject,
                  (s32 *)&testPosition)) {
            testPosition.x = pushedObject->x;
            testPosition.y = pushedObject->y;
            testPosition.z = pushedObject->z;
            Vector_AddPolarOffset(0xa0000, direction, &testPosition);
            if (Map_CheckObjectStep(pushedObject, &testPosition) ==
                MAP_STEP_OPEN) {
              testPosition.x = pushedObject->x;
              testPosition.y = pushedObject->y;
              testPosition.z = pushedObject->z;
              Vector_AddPolarOffset(0xa0000, direction + 0x1000, &testPosition);
              // The same probe is tested twice.
              if (Map_CheckObjectStep(pushedObject, &testPosition) ==
                      MAP_STEP_OPEN &&
                  Map_CheckObjectStep(pushedObject, &testPosition) ==
                      MAP_STEP_OPEN) {
                testPosition.x = pushedObject->x;
                testPosition.y = pushedObject->y;
                testPosition.z = pushedObject->z;
                Vector_AddPolarOffset(0xa0000, direction - 0x1000,
                                      &testPosition);
                if (Map_CheckObjectStep(pushedObject, &testPosition) ==
                    MAP_STEP_OPEN) {
                  Vector_AddPolarOffset(
                      0x4000, direction,
                      (struct FieldPosition *)&pushedObject->x);
                  pushedObject->targetX = TARGET_UNSET;
                  pushedObject->targetY = TARGET_UNSET;
                  pushedObject->targetZ = TARGET_UNSET;
                  pushed |= 1;
                  continue;
                }
              }
            }
          }
        }
        collision |= 2;
      }
      // Pushing: a quarter pixel a frame.
      if (collision == 0 && pushed != 0) {
        object->speedLimit = 0x4000;
        object->acceleration = 0x2000;
      }
    }
  }
  // Count the blocked steps, and pick the animation: push (8), bump (9, or
  // 22 when the leader is down) or walk / run.
  if (gEventWork != NULL) {
    if (collision & 3)
      gEventWork->blockedSteps++;
    else
      gEventWork->blockedSteps = 0;
  }
  if (pushed != 0) {
    ObjectDispatch_SelectAnimation(object, 8);
  } else if (collision != 0) {
    s32 action;

    action = 9;
    if (UnitGetFar(gGameState.selectedActor)->hp == 0)
      action = 22;
    ObjectDispatch_SelectAnimation(object, action);
  } else {
    ObjectDispatch_SelectAnimation(object, movementAnimation);
  }

  // Blocked: stop and turn towards the pad, at most 22.5 degrees a frame.
  // Otherwise move to the step, keeping the speed but along the heading.
  // footprintSide = 2 marks a stop; the next footprint then takes animation 2.
  if (collision != 0) {
    object->targetX = TARGET_UNSET;
    object->targetY = TARGET_UNSET;
    object->targetZ = TARGET_UNSET;
    object->velocityX = 0;
    object->velocityZ = 0;
    if (collision & 3) {
      s32 difference;

      difference = (u16)angle - object->angle;
      difference = (s16)difference;
      if (difference > 0x1000)
        difference = 0x1000;
      if (difference < -0x1000)
        difference = -0x1000;
      object->angle += difference;
    }
    object->action = 0;
    object->footprintSide = 2;
  } else {
    s32 speed;

    Object_SetMoveTarget(object, position.x, position.y, position.z);
    speed = FixedSqrt(Iwram_MulQ16(object->velocityX, object->velocityX) +
                      Iwram_MulQ16(object->velocityZ, object->velocityZ));
    object->velocityX = 0;
    object->velocityZ = 0;
    Vector_AddPolarOffset(speed, (u16)heading,
                          (struct FieldPosition *)&object->velocityX);
    if (object->action != 0)
      object->action--;
  }

  // Leave a footprint every 18 walking or 12 running
  // frames on maps that show them, alternating left and right; action
  // counts down the frames to the next one.
  footprintAngle = (u16)angle;
  if (((struct MapScrollWork *)gMapWork[0])->footprints != 0 &&
      object->action == 0 && collision == 0) {
    struct ObjectRuntime *effect;

    effect = FieldObject_Create(FIELD_SPRITE_FOOTPRINT, object->x, object->y,
                                object->z);
    if (effect != 0) {
      struct AnimationObject *animation;

      // collision is 0 here: the footprint takes no motion flags (it stays
      // where it is laid) and its sprite no shadow. It lasts 120 frames,
      // sorts underfoot, draws at OAM priority 3, under
      // any map layer of priority 0 to 2, and lies rotated a quarter turn
      // from the heading.
      effect->terrainHeight = object->terrainHeight;
      animation = effect->animation;
      ObjectDispatch_Initialize((struct DispatchObject *)effect,
                                gWorldMap_FootprintActions);
      effect->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
      effect->flags = collision;
      if (animation != 0) {
        AnimationObjects_SelectAnimation(animation, 1);
        animation->flags = collision;
        animation->rotation = footprintAngle + 0x4000;
        animation->image.priority = 3;
      }
      // The first footprint after a stop takes animation 2 and restarts the
      // sides; each second one faces a half turn round.
      if (object->footprintSide == 2) {
        AnimationObjects_SelectAnimation(animation, 2);
        object->footprintSide = collision;
      }
      if (object->footprintSide != 0)
        effect->angle = 0x8000;
      if (movementAnimation == 5)
        object->action = 12;
      else
        object->action = 18;
      object->footprintSide ^= 1;
    }
  }
  Field_CheckConfiguredKeys();
  object->step++;
  return 1;
}

/// SC_WORLD: moves the player on the world map by the direction pad, as
/// SC_PLAYER does on the field, and turns the world map camera with the pad.
/// Shallow water (terrain 6) leaves a ripple and terrain 9 hides the shadow.
///
/// Run each frame by gObjectDispatch_WorldMapScript, which the scene loop
/// (run_scene.c) starts on the leader through
/// ObjectDispatch_StartWorldMapControl on the world map. Sets the leader's
/// speed, target, velocity, angle and action, turns the perspective work's
/// yaw, counts gEventWork->blockedSteps and creates the ripple objects.
s32 Object_MoveOnWorldMap(struct ScriptInterpreter *interpreter) {
  struct ObjectRuntime *object = (struct ObjectRuntime *)interpreter;

  struct FieldPosition position;
#if TBS_PATCH_1
  struct FieldPosition testPosition;
  s16 angleOffsets[28];
#else
  struct FieldPosition leftPosition;
  struct FieldPosition rightPosition;
  s16 angleOffsets[22];
#endif
  s32 collision;
  s32 angle;
  u16 direction;
  s32 angleIndex;
  s32 movementAnimation;
  s32 count;
  s32 difference;
  s32 terrainCollision;
  s32 speed;
  s32 targetCameraYaw;
  struct PerspectiveWork *perspective;
  s32 yawStep;
  struct AnimationObject *playerSprite;
  struct ObjectRuntime *ripple;
  struct AnimationObject *rippleSprite;

  movementAnimation = 2;
  collision = 0;
#if !TBS_PATCH_1
  count = 0;
#endif
  // Walk, or run with the dash keys. L, as it repeats, raises the limit
  // further; unlike the field's fast walk, no debug test or flag guards it.
  if (gKeysHeld & gGameState.dashKeys) {
    object->speedLimit = WORLD_MAP_RUN_SPEED;
    object->acceleration = WORLD_MAP_RUN_ACCELERATION;
    movementAnimation = 5;
  } else {
    object->speedLimit = WORLD_MAP_WALK_SPEED;
    object->acceleration = WORLD_MAP_ACCELERATION;
  }
  if ((gKeysRepeat & KEY_L) != 0)
    object->speedLimit = WORLD_MAP_FAST_SPEED;

  angle = gWorldMap_KeyHeadings[(gKeysHeld >> 4) & 15];
  direction = angle;
  if (direction == 0xffff) {
    collision |= 4;
  } else {
    collision = 0;
    position.x = object->x;
    position.y = object->y;
    position.z = object->z;
    Vector_AddPolarOffset(WORLD_MAP_PROBE_RADIUS, direction, &position);
    // Debug L walks anywhere. Otherwise the English release on tests the
    // step and 22.5 and 45 degrees either side, then headings up to 90
    // degrees off; the Japanese one tests the step and 30 degrees (0x1555)
    // either side, then headings up to 67.5 degrees off.
    if (gDebugMode == 0 || (gKeysHeld & KEY_L) == 0) {
#if TBS_PATCH_1
      if (CheckWorldMapWalkable((s32)object,
                                (struct WorldPosition *)&position) == 0) {
        testPosition.x = object->x;
        testPosition.y = object->y;
        testPosition.z = object->z;
        Vector_AddPolarOffset(WORLD_MAP_PROBE_RADIUS, direction + 0x1000,
                              &testPosition);
        if (CheckWorldMapWalkable((s32)object,
                                  (struct WorldPosition *)&testPosition) == 0) {
          testPosition.x = object->x;
          testPosition.y = object->y;
          testPosition.z = object->z;
          Vector_AddPolarOffset(WORLD_MAP_PROBE_RADIUS, direction - 0x1000,
                                &testPosition);
          if (CheckWorldMapWalkable(
                  (s32)object, (struct WorldPosition *)&testPosition) == 0) {
            testPosition.x = object->x;
            testPosition.y = object->y;
            testPosition.z = object->z;
            Vector_AddPolarOffset(WORLD_MAP_PROBE_RADIUS, direction + 0x2000,
                                  &testPosition);
            if (CheckWorldMapWalkable(
                    (s32)object, (struct WorldPosition *)&testPosition) == 0) {
              testPosition.x = object->x;
              testPosition.y = object->y;
              testPosition.z = object->z;
              Vector_AddPolarOffset(WORLD_MAP_PROBE_RADIUS, direction - 0x2000,
                                    &testPosition);
              if (CheckWorldMapWalkable(
                      (s32)object, (struct WorldPosition *)&testPosition) == 0)
                goto update_object;
            }
          }
        }
      }
      count = 0;
      angleOffsets[count++] = direction + 0x1000;
      angleOffsets[count++] = direction - 0x1000;
      angleOffsets[count++] = direction + 0x2000;
      angleOffsets[count++] = direction - 0x2000;
      angleOffsets[count++] = direction + 0x3000;
      angleOffsets[count++] = direction - 0x3000;
      angleOffsets[count++] = direction + 0x4000;
      angleOffsets[count++] = direction - 0x4000;
      for (angleIndex = 0; angleIndex < count; angleIndex++) {
        direction = angleOffsets[angleIndex];
        position.x = object->x;
        position.y = object->y;
        position.z = object->z;
        Vector_AddPolarOffset(WORLD_MAP_PROBE_RADIUS, direction, &position);
        if (CheckWorldMapWalkable((s32)object,
                                  (struct WorldPosition *)&position) != 0)
          continue;
        testPosition.x = object->x;
        testPosition.y = object->y;
        testPosition.z = object->z;
        Vector_AddPolarOffset(WORLD_MAP_PROBE_RADIUS, direction + 0x1000,
                              &testPosition);
        if (CheckWorldMapWalkable((s32)object,
                                  (struct WorldPosition *)&testPosition) != 0)
          continue;
        testPosition.x = object->x;
        testPosition.y = object->y;
        testPosition.z = object->z;
        Vector_AddPolarOffset(WORLD_MAP_PROBE_RADIUS, direction - 0x1000,
                              &testPosition);
        if (CheckWorldMapWalkable((s32)object,
                                  (struct WorldPosition *)&testPosition) != 0)
          continue;
        testPosition.x = object->x;
        testPosition.y = object->y;
        testPosition.z = object->z;
        Vector_AddPolarOffset(WORLD_MAP_PROBE_RADIUS, direction + 0x2000,
                              &testPosition);
        if (CheckWorldMapWalkable((s32)object,
                                  (struct WorldPosition *)&testPosition) != 0)
          continue;
        testPosition.x = object->x;
        testPosition.y = object->y;
        testPosition.z = object->z;
        Vector_AddPolarOffset(WORLD_MAP_PROBE_RADIUS, direction - 0x2000,
                              &testPosition);
        if (CheckWorldMapWalkable((s32)object,
                                  (struct WorldPosition *)&testPosition) != 0)
          continue;
        angle = (s16)direction;
        goto update_object;
      }
      collision |= 1;

#else
      leftPosition.x = object->x;
      leftPosition.y = object->y;
      leftPosition.z = object->z;
      Vector_AddPolarOffset(WORLD_MAP_PROBE_RADIUS, direction + 0x1555,
                            &leftPosition);
      rightPosition.x = object->x;
      rightPosition.y = object->y;
      rightPosition.z = object->z;
      Vector_AddPolarOffset(WORLD_MAP_PROBE_RADIUS, direction - 0x1555,
                            &rightPosition);
      if ((CheckWorldMapWalkable((s32)object,
                                 (struct WorldPosition *)&position) |
           CheckWorldMapWalkable((s32)object,
                                 (struct WorldPosition *)&leftPosition) |
           CheckWorldMapWalkable(
               (s32)object, (struct WorldPosition *)&rightPosition)) != 0) {
        angleOffsets[count++] = direction + 0x1000;
        angleOffsets[count++] = direction - 0x1000;
        angleOffsets[count++] = direction + 0x2000;
        angleOffsets[count++] = direction - 0x2000;
        angleOffsets[count++] = direction + 0x3000;
        angleOffsets[count++] = direction - 0x3000;
        for (angleIndex = 0; angleIndex < count; angleIndex++) {
          direction = angleOffsets[angleIndex];
          position.x = object->x;
          position.y = object->y;
          position.z = object->z;
          Vector_AddPolarOffset(WORLD_MAP_PROBE_RADIUS, direction, &position);
          leftPosition.x = object->x;
          leftPosition.y = object->y;
          leftPosition.z = object->z;
          Vector_AddPolarOffset(WORLD_MAP_PROBE_RADIUS, direction + 0x1555,
                                &leftPosition);
          rightPosition.x = object->x;
          rightPosition.y = object->y;
          rightPosition.z = object->z;
          Vector_AddPolarOffset(WORLD_MAP_PROBE_RADIUS, direction - 0x1555,
                                &rightPosition);
          if ((CheckWorldMapWalkable((s32)object,
                                     (struct WorldPosition *)&position) |
               CheckWorldMapWalkable((s32)object,
                                     (struct WorldPosition *)&leftPosition) |
               CheckWorldMapWalkable(
                   (s32)object, (struct WorldPosition *)&rightPosition)) != 0)
            continue;
          angle = (s16)direction;
          goto update_object;
        }
        collision |= 1;
      }
#endif
    }
  }

update_object:
  if (gEventWork != NULL) {
    if ((collision & 3) != 0)
      gEventWork->blockedSteps++;
    else
      gEventWork->blockedSteps = 0;
  }
  if (collision != 0)
    ObjectDispatch_SelectAnimation(object, 9);
  else
    ObjectDispatch_SelectAnimation(object, movementAnimation);

  if (collision != 0) {
    object->targetX = TARGET_UNSET;
    object->targetY = TARGET_UNSET;
    object->targetZ = TARGET_UNSET;
    object->velocityX = 0;
    object->velocityZ = 0;
    if ((collision & 3) != 0) {
      direction = angle;
      difference = (s16)(direction - object->angle);
      if (difference > 0x1000)
        difference = 0x1000;
      if (difference < -0x1000)
        difference = -0x1000;
      object->angle += difference;
    }
    object->action = 0;
    object->footprintSide = 2;
  } else {
    Object_SetMoveTarget(object, position.x, position.y, position.z);
    speed = FixedSqrt(Iwram_MulQ16(object->velocityX, object->velocityX) +
                      Iwram_MulQ16(object->velocityZ, object->velocityZ));
    // collision is 0 here: keep the speed, along the chosen heading.
    object->velocityX = collision;
    object->velocityZ = collision;
    Vector_AddPolarOffset(speed, (u16)angle,
                          (struct FieldPosition *)&object->velocityX);
    if (object->action != 0)
      object->action--;
  }

  // Turn the camera towards the pad's yaw by an eighth of the difference a
  // frame, at most 0x200 (about 2.8 degrees), finishing the turn once the
  // step falls under 16.
  perspective = gMapWork[0];
  targetCameraYaw = gWorldMap_KeyYaws[(gKeysHeld >> 4) & 15];
  yawStep = (s16)(targetCameraYaw - perspective->yaw) / 8;
  if (yawStep > 0x200)
    yawStep = 0x200;
  if (yawStep < -0x200)
    yawStep = -0x200;
  if (yawStep > -16 && yawStep < 16)
    yawStep = targetCameraYaw - perspective->yaw;
  perspective->yaw += yawStep;

  if (object->animationKind == OBJECT_SPRITE_ONE) {
    playerSprite = object->animation;
    terrainCollision = GetWorldMapCollision((struct WorldPosition *)&object->x);
    // In a forest the sprite's second layer is drawn (priority 1) and its
    // shadow is not; elsewhere that layer takes priority 9, and a layer above
    // 3 is not drawn.
    if (terrainCollision == WORLD_MAP_COLLISION_FOREST) {
      playerSprite->entries[1]->priority = 1;
      playerSprite->flags = 0;
    } else {
      playerSprite->entries[1]->priority = 9;
      playerSprite->flags = SPRITE_FLAG_SHADOW;
    }
    // In shallow water a ripple follows every 10 frames of
    // walking: it lasts 30 frames, takes no motion flags, and its sprite,
    // with no shadow, is drawn semi-transparent at priority 2.
    if (terrainCollision == WORLD_MAP_COLLISION_SHALLOW_WATER &&
        object->action == 0 && collision == 0) {
      ripple = FieldObject_Create(FIELD_SPRITE_RIPPLE, object->x, object->y,
                                  object->z);
      if (ripple != 0) {
        rippleSprite = ripple->animation;
        ObjectDispatch_Initialize((struct DispatchObject *)ripple,
                                  gWorldMap_FootprintActions + 3);
        ripple->flags = collision;
        ripple->terrainId = 1;
        if (rippleSprite != 0) {
          AnimationObjects_SelectAnimation(rippleSprite, 1);
          rippleSprite->flags = collision;
          rippleSprite->image.objectMode = OBJ_MODE_SEMI_TRANSPARENT;
          rippleSprite->image.priority = 2;
        }
        object->action = 10;
      }
    }
  }
  Field_CheckConfiguredKeys();
  object->step++;
  return 1;
}

/// SC_KEY_COUNT: checks the configured keys (Field_CheckConfiguredKeys).
/// Returns 1. Run each frame by gObjectDispatch_KeyCheckScript, which the
/// scene loop starts on the leader in LEADER_MOVEMENT_AIRBORNE, where the pad
/// does not move it.
s32 Field_CheckConfiguredKeysAndCount(struct ScriptInterpreter *interpreter) {
  struct ObjectRuntime *object = (struct ObjectRuntime *)interpreter;

  Field_CheckConfiguredKeys();
  object->step++;
  return 1;
}

/// SC_KEYS: moves an object 8 pixels at a time by the direction pad, with up
/// and down changing its height (y) rather than its depth. Refuses steps
/// into an occupied cell, across a change of collision code, or onto ground
/// too high (up) or too low (down). Returns 1.
///
/// Run each frame by gObjectDispatch_KeysScript, which the scene loop starts
/// on the leader in LEADER_MOVEMENT_CLIMB. Sets the leader's speed,
/// angle, target or velocity, and counts gEventWork->blockedSteps.
s32 Object_MoveByKeys(struct ScriptInterpreter *interpreter) {
  struct ObjectRuntime *object = (struct ObjectRuntime *)interpreter;

  struct FieldPosition position;
  /* FAKEMATCH(dead): an unused local for the 68 bytes of native frame no
   * instruction touches, the size of Object_MoveOnWorldMap's probe locals;
   * without it, 4 of 213 instructions differ in tbs-en. */
  UNUSED u8 unusedProbeFrame[68];
  struct MapCell *sourceCell;
  struct MapCell *destinationCell;
  u16 angle;
  s32 animation;
  s32 collisionFlags;
  s32 deltaZ;
  s32 terrainHeight;

  object->speedLimit = 0x8000;
  object->acceleration = 0x4000;
  animation = 12;
  angle = gWorldMap_KeyHeadings[(gKeysHeld >> 4) & 15];
  if (angle == 0xffff) {
    collisionFlags = 4;
  } else {
    // The animation by the pad's heading: 14 for headings below 0x1000, 15
    // for 0x8000 to 0x8fff, 10 for the rest.
    animation = 14;
    if ((angle & 0xf000) != 0) {
      animation = 15;
      if ((angle & 0xf000) != 0x8000)
        animation = 10;
    }
    collisionFlags = 0;
    position.x = 0;
    position.y = 0;
    position.z = 0;
    Vector_AddPolarOffset(0x80000, angle, &position);
    // The step's depth becomes height: pad up climbs, pad down descends, and
    // the object's angle becomes 0xc000 going up the map and 0x4000 going down.
    position.x += object->x;
    deltaZ = position.z;
    if (deltaZ < 0)
      object->angle = 0xc000;
    if (deltaZ > 0)
      object->angle = 0x4000;
    position.y = object->y - position.z;
    position.z = object->z;
    // The cells (16 pixels, 0x100000) under the object and the step; a step
    // into a cell with another collision code is refused.
    sourceCell =
        &((struct MapCell *)Ram_MapCellBuffer)[object->x / 0x100000 +
                                               (position.z / 0x100000) * 128];
    destinationCell =
        &((struct MapCell *)Ram_MapCellBuffer)[position.x / 0x100000 +
                                               (position.z / 0x100000) * 128];
    if (CheckMapPositionCellOccupied((struct WorldPosition *)&position) != 0 ||
        sourceCell->collisionCode != destinationCell->collisionCode) {
      collisionFlags = 4;
      animation = 12;
    } else if (gKeysHeld & KEY_UP) {
      // Climbing needs the ground a cell further up to stand at least 16
      // pixels above the object; descending needs the ground at the step
      // at least 8 pixels below it.
      terrainHeight = Map_GetTerrainHeight(object->terrainId, position.x,
                                           position.z - 0x100000);
      if (terrainHeight - object->y < 0x100000)
        goto refused;
    } else if (gKeysHeld & KEY_DOWN) {
      terrainHeight =
          Map_GetTerrainHeight(object->terrainId, position.x, position.z);
      if (terrainHeight - object->y > -0x80000) {
      refused:
        collisionFlags = 1;
        animation = 12;
      }
    }
  }
  if (gEventWork != NULL) {
    if (collisionFlags & 3)
      gEventWork->blockedSteps++;
    else
      gEventWork->blockedSteps = 0;
  }
  ObjectDispatch_SelectAnimation(object, animation);
  if (collisionFlags != 0) {
    object->targetX = TARGET_UNSET;
    object->targetY = TARGET_UNSET;
    object->targetZ = TARGET_UNSET;
    SET_VELOCITY(object, 0, 0, 0);
  } else {
    Object_SetMoveTarget(object, position.x, position.y, position.z);
  }
  object->step++;
  return 1;
}

#undef FIELD_SHORTCUT_KEYS
#undef WORLD_MAP_PROBE_RADIUS
#undef FIELD_WALK_SPEED
#undef FIELD_RUN_SPEED
#undef FIELD_ACCELERATION
#undef FIELD_FAST_WALK_SPEED
#undef FIELD_FAST_WALK_ACCELERATION
#undef WORLD_MAP_WALK_SPEED
#undef WORLD_MAP_RUN_SPEED
#undef WORLD_MAP_RUN_ACCELERATION
#undef WORLD_MAP_ACCELERATION
#undef WORLD_MAP_FAST_SPEED
