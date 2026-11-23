/// @file
/// Player control: the configured keys and moving actors by the pad.
// Part of field.c.

#include "field_internal.h"

/// On the world map, the keys whose R and L open the map screen and the
/// zoomed map (newly pressed from patch 1, held before), and how far ahead of
/// the leader each step is probed, in pixels.
#if TBS_PATCH_1
#define FIELD_SHORTCUT_KEYS gKeysPressed
#define WORLD_MAP_PROBE_RADIUS toFIXED(7)
#else
#define FIELD_SHORTCUT_KEYS gKeysHeld
#define WORLD_MAP_PROBE_RADIUS toFIXED(8)
#endif

/// The leader's speeds and accelerations, in pixels a frame as 16.16 fixed
/// point: the top 16 bits are whole pixels, so toFIXED(1) is one pixel.
#define FIELD_WALK_SPEED toFIXED(1)      ///< 1 pixel a frame
#define FIELD_RUN_SPEED (toFIXED(3) / 2) ///< 1.5, with a dash key held
#define FIELD_ACCELERATION                                                     \
  (toFIXED(1) / 4)                       ///< a quarter pixel faster each frame
#define FIELD_FAST_WALK_SPEED toFIXED(4) ///< 4, B held with FLAG_FAST_WALK
#define FIELD_FAST_WALK_ACCELERATION toFIXED(1)
#define WORLD_MAP_WALK_SPEED (toFIXED(1) / 2) ///< half a pixel a frame
#define WORLD_MAP_RUN_SPEED toFIXED(1)        ///< 1, with a dash key held
#define WORLD_MAP_RUN_ACCELERATION (toFIXED(5) / 4)
#define WORLD_MAP_ACCELERATION (toFIXED(1) / 4)
#define WORLD_MAP_FAST_SPEED toFIXED(4) ///< 4, while L repeats

/// 22.5 degrees: a step is probed this far either side of its heading, and a
/// blocked step tries headings one, two and three times this far off.
#define FIELD_PROBE_ANGLE (TRIG_TURN / 16)
/// A blocked leader turns toward the pad at most 22.5 degrees a frame.
#define FIELD_BLOCKED_TURN_MAX (TRIG_TURN / 16)
/// The world map camera turns toward the pad's yaw by at most about 2.8
/// degrees a frame.
#define WORLD_MAP_YAW_STEP_MAX (TRIG_TURN / 128)
/// The frames between footprints, running and walking.
#define FOOTPRINT_RUN_INTERVAL 12
#define FOOTPRINT_WALK_INTERVAL 18

/// What a shortcut key's assignment requests: its top two bits.
enum FieldShortcutKind {
  FIELD_SHORTCUT_PSYNERGY = 0, ///< fieldWork->psynergy, an ability
  FIELD_SHORTCUT_ITEM = 1      ///< fieldWork->item
};

/// A shortcut key's assignment: its kind in the top two bits, from this one,
/// and its request id in the bits below.
#define FIELD_SHORTCUT_KIND_SHIFT 14
#define FIELD_SHORTCUT_ID_MASK ((1 << FIELD_SHORTCUT_KIND_SHIFT) - 1)

/// The debug beep sounds while fewer resource blocks than this are free.
#define DEBUG_RESOURCE_BEEP_FREE_BLOCKS 136

/// The world map camera's turn is finished at once when its step is smaller
/// than this.
#define WORLD_MAP_YAW_STEP_MIN 16

/// What stopped the leader's step this frame (Actor_UpdatePlayerControl,
/// Actor_MoveOnWorldMap).
enum PlayerStepBlock {
  PLAYER_STEP_BLOCKED_BY_MAP = 1, ///< every heading tried runs into the map
  /// A solid actor ahead, or one with no room to be pushed.
  PLAYER_STEP_BLOCKED_BY_ACTOR = 2,
  PLAYER_STEP_BLOCKED = 3, ///< either of the two
  /// No step, and none counted as blocked: the pad is not held, or a climb
  /// meets a wall.
  PLAYER_STEP_STOPPED = 4
};

/// The footprints left in snow such as Imil's (FIELD_SPRITE_FOOTPRINT,
/// footprints.png), and the leader's userValue that asks for the pair next.
enum FootprintAnimation {
  FOOTPRINT_ONE_FOOT = 1, ///< one print, fading
  /// Two prints side by side, fading: laid first after a stop.
  FOOTPRINT_BOTH_FEET = 2
};

/// Unused: stores value as a word in the actor's update routine.
UNUSED void Actor_SetUpdateValue(void *rawActor, s32 value) {
  struct Actor *actor = rawActor;

  if (actor != NULL)
    *(s32 *)&actor->update = value;
}

/// Handles a shortcut key's assignment: its top two bits choose what it
/// requests (enum FieldShortcutKind) and the low 14 bits which one.
u32 Field_RequestShortcut(u32 encodedAssignment) {
  u32 assignmentKind = encodedAssignment >> FIELD_SHORTCUT_KIND_SHIFT;
  u32 requestId = FIELD_SHORTCUT_ID_MASK & encodedAssignment;
  struct FieldWork *work = gFieldWork;

  if (GameFlag_GetFar(FLAG_MENUS_RAISE_TRIGGER) != 0) {
    work->raisedTrigger = FIELD_RAISED_TRIGGER_MENUS;
  } else if (work->mode == ACTOR_SYSTEM_WORLD_MAP) {
    if (FIELD_SHORTCUT_KEYS & KEY_R) {
      work->raisedTrigger = FIELD_RAISED_TRIGGER_MAP_SCREEN;
    } else if (FIELD_SHORTCUT_KEYS & KEY_L) {
      work->raisedTrigger = FIELD_RAISED_TRIGGER_WORLD_MAP_ZOOM;
    }
  } else {
    switch (assignmentKind) {
    case FIELD_SHORTCUT_PSYNERGY:
      work->psynergy = requestId;
      break;
    case FIELD_SHORTCUT_ITEM:
      work->item = requestId;
      break;
    }
  }

  return requestId;
}

/// Checks the keys the player has configured: Select, A and Start set their
/// flag in the field work and return 1; the two shortcut keys pass on their
/// assignment (Field_RequestShortcut) and return its request id.
s32 Field_CheckConfiguredKeys(void) {
  struct FieldWork *work = gFieldWork;
  s32 result = 0;

  if (work == NULL)
    return 0;
  if (gKeysPressed & gMainRoutineWork.selectKeys) {
    work->fieldMenu = 1;
    result = 1;
  } else if (gKeysPressed & gMainRoutineWork.aKeys) {
    work->aButton = 1;
    result = 1;
  } else if (gKeysPressed & gMainRoutineWork.startKeys) {
    work->pauseMenu = 1;
    result = 1;
  } else if (gKeysPressed & gMainRoutineWork.firstShortcutKeys) {
    result = Field_RequestShortcut(gMainRoutineWork.firstShortcut);
  } else if (gKeysPressed & gMainRoutineWork.secondShortcutKeys) {
    result = Field_RequestShortcut(gMainRoutineWork.secondShortcut);
  }
  return result;
}

/// Returns 0 when two spheres overlap: at positions a and b (16.16 pixels) with
/// radii radiusA and radiusB (pixels); -1 otherwise.
s32 Field_CheckSpheresOverlap(FIXED *firstPosition, s32 radiusA,
                              FIXED *secondPosition, s32 radiusB) {
  const struct FieldPosition *firstPoint =
      (const struct FieldPosition *)firstPosition;
  const struct FieldPosition *secondPoint =
      (const struct FieldPosition *)secondPosition;
  const s32 *firstCoordinate = &firstPoint->x;
  const s32 *secondCoordinate = &secondPoint->x;
  s32 dx = FIXEDtoINT(*firstCoordinate++ - *secondCoordinate++);
  s32 dy = FIXEDtoINT(*firstCoordinate++ - *secondCoordinate++);
  s32 dz = FIXEDtoINT(*firstCoordinate - *secondCoordinate);
  s32 radius = radiusA + radiusB;

  // The bound is 64 pixels in 16.16, the units before the shift, but dx and dz
  // are already whole pixels (at most 32768), so these tests always pass and
  // only the distance decides.
  if (!(dx > toFIXED(64)) && !(dz > toFIXED(64)) &&
      dx * dx + dy * dy + dz * dz < radius * radius)
    return 0;
  return -1;
}

/// SC_PLAYER: moves the player on the field by the direction pad: walking or
/// running, sliding around walls, pushing movable actors, turning, playing the
/// matching animation and leaving footprints.
s32 Actor_UpdatePlayerControl(struct Actor *actor) {

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
  struct Actor *pushedActor;

  collision = 0;
  pushed = 0;
  // Debug (FLAG_DEBUG_RESOURCE_BEEP): beep when few resource blocks are free.
  if (gDebugMode != 0 && GameFlag_GetFar(FLAG_DEBUG_RESOURCE_BEEP) != 0) {
    s32 freeBlocks;
    s32 blockIndex;

    freeBlocks = 0;
    for (blockIndex = 0; blockIndex < VRAM_BLOCK_COUNT; blockIndex++) {
      if (gVramCacheBlockOwners[blockIndex] == VRAM_BLOCK_OWNER_FREE)
        freeBlocks++;
    }
    if (freeBlocks - DEBUG_RESOURCE_BEEP_FREE_BLOCKS < 0)
      Sound_PlayFar(SE_CLICK);
  }
  // Debug: holding L stalls each frame in four empty loops, the last three
  // counting in a halfword.
  if (gDebugMode != 0) {
    if (gKeysHeld & KEY_L) {
      s32 i;
      s16 j;

      for (i = 0; i < 512; i++) {
      }
      for (j = 0; j < 96; j++) {
      }
      for (j = 0; j < 64; j++) {
      }
      for (j = 0; j < 64; j++) {
      }
    }
  }
  // Walk, or run with the dash keys; with FLAG_FAST_WALK, B moves faster
  // still.
  if (gKeysHeld & gMainRoutineWork.dashKeys) {
    actor->speedLimit = FIELD_RUN_SPEED;
    actor->acceleration = FIELD_ACCELERATION;
    movementAnimation = ANIM_LEADER_RUN;
  } else {
    actor->speedLimit = FIELD_WALK_SPEED;
    actor->acceleration = FIELD_ACCELERATION;
    movementAnimation = ANIM_WALK;
  }
  if (GameFlag_GetFar(FLAG_FAST_WALK) != 0 && (gKeysHeld & KEY_B)) {
    actor->speedLimit = FIELD_FAST_WALK_SPEED;
    actor->acceleration = FIELD_FAST_WALK_ACCELERATION;
    movementAnimation = ANIM_LEADER_RUN;
  }

  // Step 8 pixels the way the direction pad points, when that step and the
  // ones 22.5 and 45 degrees either side are clear (debug L walks through
  // walls); otherwise try headings up to 67.5 degrees off.
  angle = gWorldMap_KeyHeadings[DPAD_INDEX(gKeysHeld)];
  if ((u16)angle == WORLD_MAP_KEY_HEADING_NONE) {
    collision |= PLAYER_STEP_STOPPED;
  } else {
    collision = 0;
    position.x = actor->x.fixed;
    position.y = actor->y.fixed;
    position.z = actor->z.fixed;
    Vector_AddPolarOffset(toFIXED(8), (u16)angle, &position);
    if (gDebugMode != 0 && (gKeysHeld & KEY_L)) {
      heading = angle;
    } else {
      if (MapGrid_CheckCollision(actor, &position) == COLLISION_NONE) {
        testPosition.x = actor->x.fixed;
        testPosition.y = actor->y.fixed;
        testPosition.z = actor->z.fixed;
        Vector_AddPolarOffset(toFIXED(8), (u16)angle + FIELD_PROBE_ANGLE,
                              &testPosition);
        if (MapGrid_CheckCollision(actor, &testPosition) == COLLISION_NONE) {
          testPosition.x = actor->x.fixed;
          testPosition.y = actor->y.fixed;
          testPosition.z = actor->z.fixed;
          Vector_AddPolarOffset(toFIXED(8), (u16)angle - FIELD_PROBE_ANGLE,
                                &testPosition);
          if (MapGrid_CheckCollision(actor, &testPosition) == COLLISION_NONE) {
            testPosition.x = actor->x.fixed;
            testPosition.y = actor->y.fixed;
            testPosition.z = actor->z.fixed;
            Vector_AddPolarOffset(toFIXED(8), (u16)angle + DEGtoANG(45),
                                  &testPosition);
            if (MapGrid_CheckCollision(actor, &testPosition) ==
                COLLISION_NONE) {
              testPosition.x = actor->x.fixed;
              testPosition.y = actor->y.fixed;
              testPosition.z = actor->z.fixed;
              Vector_AddPolarOffset(toFIXED(8), (u16)angle - DEGtoANG(45),
                                    &testPosition);
              if (MapGrid_CheckCollision(actor, &testPosition) ==
                  COLLISION_NONE) {
                heading = angle;
                goto push_actors;
              }
            }
          }
        }
      }
      // The pad's way is blocked: try 22.5, 45 and 67.5 degrees off, nearest
      // first, each needing the same three clear probes.
      direction = angle;
      headingCount = 0;
      angleOffsets[headingCount++] = direction + FIELD_PROBE_ANGLE;
      angleOffsets[headingCount++] = direction - FIELD_PROBE_ANGLE;
      angleOffsets[headingCount++] = direction + DEGtoANG(45);
      angleOffsets[headingCount++] = direction - DEGtoANG(45);
      angleOffsets[headingCount++] = direction + FIELD_PROBE_ANGLE * 3;
      angleOffsets[headingCount++] = direction - FIELD_PROBE_ANGLE * 3;
      for (angleIndex = 0; angleIndex < headingCount; angleIndex++) {
        heading = angleOffsets[angleIndex];
        position.x = actor->x.fixed;
        position.y = actor->y.fixed;
        position.z = actor->z.fixed;
        Vector_AddPolarOffset(toFIXED(8), (u16)heading, &position);
        if (MapGrid_CheckCollision(actor, &position) != COLLISION_NONE)
          continue;
        testPosition.x = actor->x.fixed;
        testPosition.y = actor->y.fixed;
        testPosition.z = actor->z.fixed;
        Vector_AddPolarOffset(toFIXED(8), (u16)heading + FIELD_PROBE_ANGLE,
                              &testPosition);
        if (MapGrid_CheckCollision(actor, &testPosition) != COLLISION_NONE)
          continue;
        testPosition.x = actor->x.fixed;
        testPosition.y = actor->y.fixed;
        testPosition.z = actor->z.fixed;
        Vector_AddPolarOffset(toFIXED(8), (u16)heading - FIELD_PROBE_ANGLE,
                              &testPosition);
        if (MapGrid_CheckCollision(actor, &testPosition) != COLLISION_NONE)
          continue;
        testPosition.x = actor->x.fixed;
        testPosition.y = actor->y.fixed;
        testPosition.z = actor->z.fixed;
        Vector_AddPolarOffset(toFIXED(8), (u16)heading + DEGtoANG(45),
                              &testPosition);
        if (MapGrid_CheckCollision(actor, &testPosition) != COLLISION_NONE)
          continue;
        testPosition.x = actor->x.fixed;
        testPosition.y = actor->y.fixed;
        testPosition.z = actor->z.fixed;
        Vector_AddPolarOffset(toFIXED(8), (u16)heading - DEGtoANG(45),
                              &testPosition);
        if (MapGrid_CheckCollision(actor, &testPosition) == COLLISION_NONE)
          goto push_actors;
      }
      // Nothing passed: the step is the actor's own position.
      position.x = actor->x.fixed;
      position.y = actor->y.fixed;
      position.z = actor->z.fixed;
      collision |= PLAYER_STEP_BLOCKED_BY_MAP;

    push_actors:
      // Push the movable actors (ACTOR_PUSHABLE) just ahead 4 pixels
      // further, when they have room; any other solid actor there blocks the
      // step (bit 1 of collision). Pushing slows the player down.
      contact.x = actor->x.fixed;
      contact.y = actor->y.fixed;
      contact.z = actor->z.fixed;
      Vector_AddPolarOffset(toFIXED(4), (u16)heading, &contact);
      pushedActor = gActors;
      for (angleIndex = 0; angleIndex < ACTOR_COUNT;
           angleIndex++, pushedActor++) {
        s32 radius;

        radius = actor->radius - 2;
        if (pushedActor->script == 0)
          continue;
        if ((pushedActor->collisionFlags & ACTOR_SOLID) == 0)
          continue;
        if (pushedActor == actor)
          continue;
        if (Field_CheckSpheresOverlap(&pushedActor->x.fixed, radius, &contact.x,
                                      pushedActor->radius - 2) < 0)
          continue;
        // A movable actor that is not paused slides a quarter pixel away from
        // the player, if no solid actor is there and the ground 10 pixels on,
        // straight and 22.5 degrees either side, is clear.
        if ((pushedActor->collisionFlags & ACTOR_PUSHABLE) != 0 &&
            pushedActor->paused == 0) {
          direction = Trig_Atan(pushedActor->z.fixed - actor->z.fixed,
                                pushedActor->x.fixed - actor->x.fixed);
          testPosition.x = pushedActor->x.fixed;
          testPosition.y = pushedActor->y.fixed;
          testPosition.z = pushedActor->z.fixed;
          heading = direction;
          Vector_AddPolarOffset(toFIXED(1) / 4, direction, &testPosition);
          if (!ActorScript_CheckOverlap(pushedActor, (s32 *)&testPosition)) {
            testPosition.x = pushedActor->x.fixed;
            testPosition.y = pushedActor->y.fixed;
            testPosition.z = pushedActor->z.fixed;
            Vector_AddPolarOffset(toFIXED(10), direction, &testPosition);
            if (MapGrid_CheckCollision(pushedActor, &testPosition) ==
                COLLISION_NONE) {
              testPosition.x = pushedActor->x.fixed;
              testPosition.y = pushedActor->y.fixed;
              testPosition.z = pushedActor->z.fixed;
              Vector_AddPolarOffset(toFIXED(10), direction + FIELD_PROBE_ANGLE,
                                    &testPosition);
              // The same probe is tested twice.
              if (MapGrid_CheckCollision(pushedActor, &testPosition) ==
                      COLLISION_NONE &&
                  MapGrid_CheckCollision(pushedActor, &testPosition) ==
                      COLLISION_NONE) {
                testPosition.x = pushedActor->x.fixed;
                testPosition.y = pushedActor->y.fixed;
                testPosition.z = pushedActor->z.fixed;
                Vector_AddPolarOffset(
                    toFIXED(10), direction - FIELD_PROBE_ANGLE, &testPosition);
                if (MapGrid_CheckCollision(pushedActor, &testPosition) ==
                    COLLISION_NONE) {
                  Vector_AddPolarOffset(
                      toFIXED(1) / 4, direction,
                      (struct FieldPosition *)&pushedActor->x.fixed);
                  pushedActor->targetX = ACTOR_NO_TARGET;
                  pushedActor->targetY = ACTOR_NO_TARGET;
                  pushedActor->targetZ = ACTOR_NO_TARGET;
                  pushed |= 1;
                  continue;
                }
              }
            }
          }
        }
        collision |= PLAYER_STEP_BLOCKED_BY_ACTOR;
      }
      // Pushing: a quarter pixel a frame.
      if (collision == 0 && pushed != 0) {
        actor->speedLimit = toFIXED(1) / 4;
        actor->acceleration = toFIXED(1) / 8;
      }
    }
  }
  // Count the blocked steps, and pick the animation: pushing; stopped,
  // standing and in time glancing about (leaning back when the leader has no
  // HP); or walking or running.
  if (gFieldWork != NULL) {
    if (collision & PLAYER_STEP_BLOCKED)
      gFieldWork->blockedSteps++;
    else
      gFieldWork->blockedSteps = 0;
  }
  if (pushed != 0) {
    Actor_SelectAnimation(actor, ANIM_PUSH);
  } else if (collision != 0) {
    s32 action;

    action = ANIM_LEADER_LOOK_AROUND;
    if (Unit_GetFar(gMainRoutineWork.leaderActorId)->hp == 0)
      action = ANIM_LEADER_LEAN_BACK;
    Actor_SelectAnimation(actor, action);
  } else {
    Actor_SelectAnimation(actor, movementAnimation);
  }

  // Blocked: stop and turn towards the pad, at most 22.5 degrees a frame; the
  // next footprint is the pair.
  if (collision != 0) {
    actor->targetX = ACTOR_NO_TARGET;
    actor->targetY = ACTOR_NO_TARGET;
    actor->targetZ = ACTOR_NO_TARGET;
    actor->velocityX = 0;
    actor->velocityZ = 0;
    if (collision & PLAYER_STEP_BLOCKED) {
      s32 difference;

      difference = (u16)angle - actor->facing;
      difference = (s16)difference;
      if (difference > FIELD_BLOCKED_TURN_MAX)
        difference = FIELD_BLOCKED_TURN_MAX;
      if (difference < -FIELD_BLOCKED_TURN_MAX)
        difference = -FIELD_BLOCKED_TURN_MAX;
      actor->facing += difference;
    }
    actor->timer = 0;
    actor->userValue = FOOTPRINT_BOTH_FEET;
  } else {
    FIXED speed;

    Actor_SetMoveTarget(actor, position.x, position.y, position.z);
    speed = Math_SqrtFX(Iwram_MulFX(actor->velocityX, actor->velocityX) +
                        Iwram_MulFX(actor->velocityZ, actor->velocityZ));
    actor->velocityX = 0;
    actor->velocityZ = 0;
    Vector_AddPolarOffset(speed, (u16)heading,
                          (struct FieldPosition *)&actor->velocityX);
    if (actor->timer != 0)
      actor->timer--;
  }

  // Leave a footprint every 18 walking or 12 running
  // frames on maps that show them, alternating left and right; timer
  // counts down the frames to the next one.
  footprintAngle = (u16)angle;
  if (((struct MapGridScrollWork *)gMapGridWork[0])->footprints != 0 &&
      actor->timer == 0 && collision == 0) {
    struct Actor *effect;

    effect = Actor_Create(FIELD_SPRITE_FOOTPRINT, actor->x.fixed,
                          actor->y.fixed, actor->z.fixed);
    if (effect != 0) {
      struct Sprite *animation;

      // collision is 0 here: the footprint takes no motion flags (it stays
      // where it is laid) and its sprite no shadow.
      effect->groundHeight = actor->groundHeight;
      animation = effect->sprite;
      Actor_StartScript(effect, gWorldMap_FootprintActions);
      effect->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
      effect->motionFlags = collision;
      if (animation != 0) {
        Sprite_StartAnimation(animation, FOOTPRINT_ONE_FOOT);
        animation->flags = collision;
        animation->rotation = footprintAngle + DEGtoANG(90);
        animation->image.priority = 3;
      }
      // The first footprint after a stop is the pair and restarts the sides;
      // each second one faces a half turn round.
      if ((s16)actor->userValue == FOOTPRINT_BOTH_FEET) {
        Sprite_StartAnimation(animation, FOOTPRINT_BOTH_FEET);
        actor->userValue = collision;
      }
      if ((s16)actor->userValue != 0)
        effect->facing = DEGtoANG(180);
      if (movementAnimation == ANIM_LEADER_RUN)
        actor->timer = FOOTPRINT_RUN_INTERVAL;
      else
        actor->timer = FOOTPRINT_WALK_INTERVAL;
      actor->userValue ^= 1;
    }
  }
  Field_CheckConfiguredKeys();
  actor->scriptCursor++;
  return 1;
}

/// SC_WORLD: moves the player on the world map by the direction pad, as
/// SC_PLAYER does on the field, and turns the world map camera with the pad.
s32 Actor_MoveOnWorldMap(struct Actor *actor) {

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
  FIXED speed;
  s32 targetCameraYaw;
  struct WorldMapView *perspective;
  s32 yawStep;
  struct Sprite *playerSprite;
  struct Actor *ripple;
  struct Sprite *rippleSprite;

  movementAnimation = ANIM_WALK;
  collision = 0;
#if !TBS_PATCH_1
  count = 0;
#endif
  // Walk, or run with the dash keys.
  if (gKeysHeld & gMainRoutineWork.dashKeys) {
    actor->speedLimit = WORLD_MAP_RUN_SPEED;
    actor->acceleration = WORLD_MAP_RUN_ACCELERATION;
    movementAnimation = ANIM_LEADER_RUN;
  } else {
    actor->speedLimit = WORLD_MAP_WALK_SPEED;
    actor->acceleration = WORLD_MAP_ACCELERATION;
  }
  if ((gKeysRepeat & KEY_L) != 0)
    actor->speedLimit = WORLD_MAP_FAST_SPEED;

  angle = gWorldMap_KeyHeadings[DPAD_INDEX(gKeysHeld)];
  direction = angle;
  if (direction == WORLD_MAP_KEY_HEADING_NONE) {
    collision |= PLAYER_STEP_STOPPED;
  } else {
    collision = 0;
    position.x = actor->x.fixed;
    position.y = actor->y.fixed;
    position.z = actor->z.fixed;
    Vector_AddPolarOffset(WORLD_MAP_PROBE_RADIUS, direction, &position);
    // Debug L walks anywhere.
    if (gDebugMode == 0 || (gKeysHeld & KEY_L) == 0) {
#if TBS_PATCH_1
      if (WorldMap_IsWalkable((s32)actor, (struct WorldPosition *)&position) ==
          0) {
        testPosition.x = actor->x.fixed;
        testPosition.y = actor->y.fixed;
        testPosition.z = actor->z.fixed;
        Vector_AddPolarOffset(WORLD_MAP_PROBE_RADIUS,
                              direction + FIELD_PROBE_ANGLE, &testPosition);
        if (WorldMap_IsWalkable((s32)actor,
                                (struct WorldPosition *)&testPosition) == 0) {
          testPosition.x = actor->x.fixed;
          testPosition.y = actor->y.fixed;
          testPosition.z = actor->z.fixed;
          Vector_AddPolarOffset(WORLD_MAP_PROBE_RADIUS,
                                direction - FIELD_PROBE_ANGLE, &testPosition);
          if (WorldMap_IsWalkable((s32)actor,
                                  (struct WorldPosition *)&testPosition) == 0) {
            testPosition.x = actor->x.fixed;
            testPosition.y = actor->y.fixed;
            testPosition.z = actor->z.fixed;
            Vector_AddPolarOffset(WORLD_MAP_PROBE_RADIUS,
                                  direction + DEGtoANG(45), &testPosition);
            if (WorldMap_IsWalkable(
                    (s32)actor, (struct WorldPosition *)&testPosition) == 0) {
              testPosition.x = actor->x.fixed;
              testPosition.y = actor->y.fixed;
              testPosition.z = actor->z.fixed;
              Vector_AddPolarOffset(WORLD_MAP_PROBE_RADIUS,
                                    direction - DEGtoANG(45), &testPosition);
              if (WorldMap_IsWalkable(
                      (s32)actor, (struct WorldPosition *)&testPosition) == 0)
                goto update_actor;
            }
          }
        }
      }
      count = 0;
      angleOffsets[count++] = direction + FIELD_PROBE_ANGLE;
      angleOffsets[count++] = direction - FIELD_PROBE_ANGLE;
      angleOffsets[count++] = direction + DEGtoANG(45);
      angleOffsets[count++] = direction - DEGtoANG(45);
      angleOffsets[count++] = direction + FIELD_PROBE_ANGLE * 3;
      angleOffsets[count++] = direction - FIELD_PROBE_ANGLE * 3;
      angleOffsets[count++] = direction + DEGtoANG(90);
      angleOffsets[count++] = direction - DEGtoANG(90);
      for (angleIndex = 0; angleIndex < count; angleIndex++) {
        direction = angleOffsets[angleIndex];
        position.x = actor->x.fixed;
        position.y = actor->y.fixed;
        position.z = actor->z.fixed;
        Vector_AddPolarOffset(WORLD_MAP_PROBE_RADIUS, direction, &position);
        if (WorldMap_IsWalkable((s32)actor,
                                (struct WorldPosition *)&position) != 0)
          continue;
        testPosition.x = actor->x.fixed;
        testPosition.y = actor->y.fixed;
        testPosition.z = actor->z.fixed;
        Vector_AddPolarOffset(WORLD_MAP_PROBE_RADIUS,
                              direction + FIELD_PROBE_ANGLE, &testPosition);
        if (WorldMap_IsWalkable((s32)actor,
                                (struct WorldPosition *)&testPosition) != 0)
          continue;
        testPosition.x = actor->x.fixed;
        testPosition.y = actor->y.fixed;
        testPosition.z = actor->z.fixed;
        Vector_AddPolarOffset(WORLD_MAP_PROBE_RADIUS,
                              direction - FIELD_PROBE_ANGLE, &testPosition);
        if (WorldMap_IsWalkable((s32)actor,
                                (struct WorldPosition *)&testPosition) != 0)
          continue;
        testPosition.x = actor->x.fixed;
        testPosition.y = actor->y.fixed;
        testPosition.z = actor->z.fixed;
        Vector_AddPolarOffset(WORLD_MAP_PROBE_RADIUS, direction + DEGtoANG(45),
                              &testPosition);
        if (WorldMap_IsWalkable((s32)actor,
                                (struct WorldPosition *)&testPosition) != 0)
          continue;
        testPosition.x = actor->x.fixed;
        testPosition.y = actor->y.fixed;
        testPosition.z = actor->z.fixed;
        Vector_AddPolarOffset(WORLD_MAP_PROBE_RADIUS, direction - DEGtoANG(45),
                              &testPosition);
        if (WorldMap_IsWalkable((s32)actor,
                                (struct WorldPosition *)&testPosition) != 0)
          continue;
        angle = (s16)direction;
        goto update_actor;
      }
      collision |= PLAYER_STEP_BLOCKED_BY_MAP;

#else
      leftPosition.x = actor->x.fixed;
      leftPosition.y = actor->y.fixed;
      leftPosition.z = actor->z.fixed;
      Vector_AddPolarOffset(WORLD_MAP_PROBE_RADIUS, direction + DEGtoANG(30),
                            &leftPosition);
      rightPosition.x = actor->x.fixed;
      rightPosition.y = actor->y.fixed;
      rightPosition.z = actor->z.fixed;
      Vector_AddPolarOffset(WORLD_MAP_PROBE_RADIUS, direction - DEGtoANG(30),
                            &rightPosition);
      if ((WorldMap_IsWalkable((s32)actor, (struct WorldPosition *)&position) |
           WorldMap_IsWalkable((s32)actor,
                               (struct WorldPosition *)&leftPosition) |
           WorldMap_IsWalkable((s32)actor,
                               (struct WorldPosition *)&rightPosition)) != 0) {
        angleOffsets[count++] = direction + FIELD_PROBE_ANGLE;
        angleOffsets[count++] = direction - FIELD_PROBE_ANGLE;
        angleOffsets[count++] = direction + DEGtoANG(45);
        angleOffsets[count++] = direction - DEGtoANG(45);
        angleOffsets[count++] = direction + FIELD_PROBE_ANGLE * 3;
        angleOffsets[count++] = direction - FIELD_PROBE_ANGLE * 3;
        for (angleIndex = 0; angleIndex < count; angleIndex++) {
          direction = angleOffsets[angleIndex];
          position.x = actor->x.fixed;
          position.y = actor->y.fixed;
          position.z = actor->z.fixed;
          Vector_AddPolarOffset(WORLD_MAP_PROBE_RADIUS, direction, &position);
          leftPosition.x = actor->x.fixed;
          leftPosition.y = actor->y.fixed;
          leftPosition.z = actor->z.fixed;
          Vector_AddPolarOffset(WORLD_MAP_PROBE_RADIUS,
                                direction + DEGtoANG(30), &leftPosition);
          rightPosition.x = actor->x.fixed;
          rightPosition.y = actor->y.fixed;
          rightPosition.z = actor->z.fixed;
          Vector_AddPolarOffset(WORLD_MAP_PROBE_RADIUS,
                                direction - DEGtoANG(30), &rightPosition);
          if ((WorldMap_IsWalkable((s32)actor,
                                   (struct WorldPosition *)&position) |
               WorldMap_IsWalkable((s32)actor,
                                   (struct WorldPosition *)&leftPosition) |
               WorldMap_IsWalkable(
                   (s32)actor, (struct WorldPosition *)&rightPosition)) != 0)
            continue;
          angle = (s16)direction;
          goto update_actor;
        }
        collision |= PLAYER_STEP_BLOCKED_BY_MAP;
      }
#endif
    }
  }

update_actor:
  if (gFieldWork != NULL) {
    if ((collision & PLAYER_STEP_BLOCKED) != 0)
      gFieldWork->blockedSteps++;
    else
      gFieldWork->blockedSteps = 0;
  }
  if (collision != 0)
    Actor_SelectAnimation(actor, ANIM_LEADER_LOOK_AROUND);
  else
    Actor_SelectAnimation(actor, movementAnimation);

  if (collision != 0) {
    actor->targetX = ACTOR_NO_TARGET;
    actor->targetY = ACTOR_NO_TARGET;
    actor->targetZ = ACTOR_NO_TARGET;
    actor->velocityX = 0;
    actor->velocityZ = 0;
    if ((collision & PLAYER_STEP_BLOCKED) != 0) {
      direction = angle;
      difference = (s16)(direction - actor->facing);
      if (difference > FIELD_BLOCKED_TURN_MAX)
        difference = FIELD_BLOCKED_TURN_MAX;
      if (difference < -FIELD_BLOCKED_TURN_MAX)
        difference = -FIELD_BLOCKED_TURN_MAX;
      actor->facing += difference;
    }
    actor->timer = 0;
    actor->userValue = FOOTPRINT_BOTH_FEET;
  } else {
    Actor_SetMoveTarget(actor, position.x, position.y, position.z);
    speed = Math_SqrtFX(Iwram_MulFX(actor->velocityX, actor->velocityX) +
                        Iwram_MulFX(actor->velocityZ, actor->velocityZ));
    // collision is 0 here: keep the speed, along the chosen heading.
    actor->velocityX = collision;
    actor->velocityZ = collision;
    Vector_AddPolarOffset(speed, (u16)angle,
                          (struct FieldPosition *)&actor->velocityX);
    if (actor->timer != 0)
      actor->timer--;
  }

  // Turn the camera towards the pad's yaw by an eighth of the difference a
  // frame, at most WORLD_MAP_YAW_STEP_MAX, finishing the turn once
  // the step falls under WORLD_MAP_YAW_STEP_MIN.
  perspective = gMapGridWork[0];
  targetCameraYaw = gWorldMap_KeyYaws[DPAD_INDEX(gKeysHeld)];
  yawStep = (s16)(targetCameraYaw - perspective->yaw) / 8;
  if (yawStep > WORLD_MAP_YAW_STEP_MAX)
    yawStep = WORLD_MAP_YAW_STEP_MAX;
  if (yawStep < -WORLD_MAP_YAW_STEP_MAX)
    yawStep = -WORLD_MAP_YAW_STEP_MAX;
  if (yawStep > -WORLD_MAP_YAW_STEP_MIN && yawStep < WORLD_MAP_YAW_STEP_MIN)
    yawStep = targetCameraYaw - perspective->yaw;
  perspective->yaw += yawStep;

  if (actor->spriteStorageKind == ACTOR_SPRITE_ONE) {
    playerSprite = actor->sprite;
    terrainCollision =
        WorldMap_GetCollision((struct WorldPosition *)&actor->x.fixed);
    // In a forest the sprite's second sprite animation is drawn (priority 1)
    // and its shadow is not; elsewhere that sprite animation takes priority 9,
    // and a sprite animation above 3 is not drawn.
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
        actor->timer == 0 && collision == 0) {
      ripple = Actor_Create(FIELD_SPRITE_RIPPLE, actor->x.fixed, actor->y.fixed,
                            actor->z.fixed);
      if (ripple != 0) {
        rippleSprite = ripple->sprite;
        Actor_StartScript(ripple, gWorldMap_FootprintActions + 3);
        ripple->motionFlags = collision;
        ripple->layer = 1;
        if (rippleSprite != 0) {
          Sprite_StartAnimation(rippleSprite, 1);
          rippleSprite->flags = collision;
          rippleSprite->image.objectMode = OBJ_MODE_SEMI_TRANSPARENT;
          rippleSprite->image.priority = 2;
        }
        actor->timer = 10;
      }
    }
  }
  Field_CheckConfiguredKeys();
  actor->scriptCursor++;
  return 1;
}

/// SC_KEY_COUNT: checks the configured keys (Field_CheckConfiguredKeys).
/// Returns 1.
s32 Field_CheckConfiguredKeysAndCount(struct Actor *actor) {

  Field_CheckConfiguredKeys();
  actor->scriptCursor++;
  return 1;
}

/// SC_KEYS: moves an actor 8 pixels at a time by the direction pad, with up and
/// down changing its height (y) rather than its depth.
s32 Actor_MoveByKeys(struct Actor *actor) {

  struct FieldPosition position;
  /* LEFTOVER(frame): an unused local the ROM's frame reserves and never
   * touches. */
  UNUSED u8 unused[68];
  struct MapGridCell *sourceCell;
  struct MapGridCell *destinationCell;
  u16 angle;
  s32 animation;
  s32 collisionFlags;
  s32 deltaZ;
  FIXED groundHeight;

  actor->speedLimit = toFIXED(1) / 2;
  actor->acceleration = toFIXED(1) / 4;
  animation = ANIM_LEADER_CLING;
  angle = gWorldMap_KeyHeadings[DPAD_INDEX(gKeysHeld)];
  if (angle == WORLD_MAP_KEY_HEADING_NONE) {
    collisionFlags = PLAYER_STEP_STOPPED;
  } else {
    // The animation by the pad's heading: climbing east for headings below a
    // sixteenth of a turn, west for the sixteenth from a half turn on, up or
    // down for the rest.
    animation = ANIM_LEADER_CLIMB_EAST;
    if (FACING_SIXTEENTH(angle) != 0) {
      animation = ANIM_LEADER_CLIMB_WEST;
      if (FACING_SIXTEENTH(angle) != FACING_WEST)
        animation = ANIM_LEADER_CLIMB;
    }
    collisionFlags = 0;
    position.x = 0;
    position.y = 0;
    position.z = 0;
    Vector_AddPolarOffset(toFIXED(8), angle, &position);
    // The step's depth becomes height: pad up climbs, pad down descends, and
    // the actor's angle becomes FACING_NORTH going up the map and FACING_SOUTH
    // going down.
    position.x += actor->x.fixed;
    deltaZ = position.z;
    if (deltaZ < 0)
      actor->facing = FACING_NORTH;
    if (deltaZ > 0)
      actor->facing = FACING_SOUTH;
    position.y = actor->y.fixed - position.z;
    position.z = actor->z.fixed;
    // The cells (16 pixels) under the actor and the step; a step
    // into a cell with another cell behavior is refused.
    sourceCell =
        &((struct MapGridCell *)Ram_MapGridCellBuffer)
            [actor->x.fixed / toFIXED(MAPGRID_CELL_PIXELS) +
             (position.z / toFIXED(MAPGRID_CELL_PIXELS)) * MAPGRID_SIZE];
    destinationCell = &(
        (struct MapGridCell *)
            Ram_MapGridCellBuffer)[position.x / toFIXED(MAPGRID_CELL_PIXELS) +
                                   (position.z / toFIXED(MAPGRID_CELL_PIXELS)) *
                                       MAPGRID_SIZE];
    if (MapGrid_IsWallAt((struct WorldPosition *)&position) != 0 ||
        sourceCell->behavior != destinationCell->behavior) {
      collisionFlags = PLAYER_STEP_STOPPED;
      animation = ANIM_LEADER_CLING;
    } else if (gKeysHeld & KEY_UP) {
      // Climbing needs the ground a cell further up to stand at least 16
      // pixels above the actor; descending needs the ground at the step
      // at least 8 pixels below it.
      groundHeight = MapGrid_GetTerrainHeight(actor->layer, position.x,
                                              position.z - toFIXED(16));
      if (groundHeight - actor->y.fixed < toFIXED(16))
        goto refused;
    } else if (gKeysHeld & KEY_DOWN) {
      groundHeight =
          MapGrid_GetTerrainHeight(actor->layer, position.x, position.z);
      if (groundHeight - actor->y.fixed > -toFIXED(8)) {
      refused:
        collisionFlags = PLAYER_STEP_BLOCKED_BY_MAP;
        animation = ANIM_LEADER_CLING;
      }
    }
  }
  if (gFieldWork != NULL) {
    if (collisionFlags & PLAYER_STEP_BLOCKED)
      gFieldWork->blockedSteps++;
    else
      gFieldWork->blockedSteps = 0;
  }
  Actor_SelectAnimation(actor, animation);
  if (collisionFlags != 0) {
    actor->targetX = ACTOR_NO_TARGET;
    actor->targetY = ACTOR_NO_TARGET;
    actor->targetZ = ACTOR_NO_TARGET;
    SET_VELOCITY(actor, 0, 0, 0);
  } else {
    Actor_SetMoveTarget(actor, position.x, position.y, position.z);
  }
  actor->scriptCursor++;
  return 1;
}

#undef FIELD_SHORTCUT_KEYS
#undef WORLD_MAP_PROBE_RADIUS
#undef FIELD_PROBE_ANGLE
#undef FIELD_BLOCKED_TURN_MAX
#undef WORLD_MAP_YAW_STEP_MAX
#undef FOOTPRINT_RUN_INTERVAL
#undef FOOTPRINT_WALK_INTERVAL
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
#undef FIELD_SHORTCUT_ID_MASK
