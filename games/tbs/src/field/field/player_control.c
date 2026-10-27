/// @file
/// Player control: the configured keys and moving actors by the pad.
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
  FIELD_SHORTCUT_PSYNERGY = 0, ///< fieldWork->psynergy, an ability
  FIELD_SHORTCUT_ITEM = 1      ///< fieldWork->item
};

/// Unused: stores value as a word in the actor's update routine.
UNUSED void Actor_SetUpdateValue(void *rawActor, s32 value) {
  struct Actor *actor = rawActor;

  if (actor != NULL)
    *(s32 *)&actor->update = value;
}

/// Handles a shortcut key's assignment: its top two bits choose what it
/// requests (enum FieldShortcutKind) and the low 14 bits which one. With
/// FLAG_MENUS_RAISE_TRIGGER set it raises FIELD_RAISED_TRIGGER_MENUS instead,
/// as the menus do then; on the world map R raises
/// FIELD_RAISED_TRIGGER_WORLD_PICTURE and L
/// FIELD_RAISED_TRIGGER_WORLD_MAP_ZOOM. Returns the request id, the
/// assignment's low 14 bits, even when it raised an event instead; a shortcut
/// with nothing assigned returns 0, as no key pressed does.
///
/// Called by Field_CheckConfiguredKeys. Sets work->psynergy,
/// work->item or work->event, which the scene manager (scene_manager.c)
/// acts on.
u32 Field_RequestShortcut(u32 encodedAssignment) {
  u32 assignmentKind = encodedAssignment >> 14;
  u32 requestId = 0x3FFF & encodedAssignment;
  struct FieldWork *work = gFieldWork;

  if (GameFlag_GetFar(FLAG_MENUS_RAISE_TRIGGER) != 0) {
    work->raisedTrigger = FIELD_RAISED_TRIGGER_MENUS;
  } else if (work->mode == ACTOR_SYSTEM_WORLD_MAP) {
    if (FIELD_SHORTCUT_KEYS & KEY_R) {
      work->raisedTrigger = FIELD_RAISED_TRIGGER_WORLD_PICTURE;
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
/// assignment (Field_RequestShortcut) and return its request id. Returns 0
/// when none is pressed, or with no field work. Every caller drops the result.
/// Only the first configured key pressed this frame counts, in that order.
///
/// Called each frame by the leader's control commands: SC_PLAYER, SC_WORLD
/// and SC_KEY_COUNT. Sets work->fieldMenu, aButton or pauseMenu.
s32 Field_CheckConfiguredKeys(void) {
  struct FieldWork *work = gFieldWork;
  s32 result = 0;

  if (work == NULL)
    return 0;
  if (gKeysPressed & gSaveBlock.selectKeys) {
    work->fieldMenu = 1;
    result = 1;
  } else if (gKeysPressed & gSaveBlock.aKeys) {
    work->aButton = 1;
    result = 1;
  } else if (gKeysPressed & gSaveBlock.startKeys) {
    work->pauseMenu = 1;
    result = 1;
  } else if (gKeysPressed & gSaveBlock.firstShortcutKeys) {
    result = Field_RequestShortcut(gSaveBlock.firstShortcut);
  } else if (gKeysPressed & gSaveBlock.secondShortcutKeys) {
    result = Field_RequestShortcut(gSaveBlock.secondShortcut);
  }
  return result;
}

/// Returns 0 when two spheres overlap: at positions a and b (16.16 pixels)
/// with radii radiusA and radiusB (pixels); -1 otherwise. Called by
/// SC_PLAYER's push test, ActorScript_CheckOverlap and
/// ActorScript_FindOverlappingEntry.
s32 Field_CheckSpheresOverlap(s32 *firstPosition, s32 radiusA,
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

  // The bound is 64 pixels in 16.16, the units before the shift, but dx and
  // dz are already whole pixels (at most 32768), so these tests always pass
  // and only the distance decides. No two actors on a 2,048-pixel map are
  // far enough apart for the squares to overflow, so nothing the player sees
  // changes.
  if (!(dx > 0x400000) && !(dz > 0x400000) &&
      dx * dx + dy * dy + dz * dz < radius * radius)
    return 0;
  return -1;
}

/// SC_PLAYER: moves the player on the field by the direction pad: walking or
/// running, sliding around walls, pushing movable actors, turning, playing
/// the matching animation and leaving footprints. Also checks the configured
/// keys. Returns 1.
///
/// Run each frame by gActor_PlayerScript, which the scene manager
/// (scene_manager.c) starts on the leader through
/// Actor_StartPlayerControl when it hands control back on the field.
/// Sets the leader's speed, target, velocity, angle and timer, moves the
/// actors it pushes, counts gFieldWork->blockedSteps and creates the
/// footprint actors.
s32 Actor_UpdatePlayerControl(struct Actor *interpreter) {
  struct Actor *actor = interpreter;

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
  // Debug (FLAG_DEBUG_RESOURCE_BEEP): beep when fewer than 136 of the 512
  // resource blocks are free.
  if (gDebugMode != 0 && GameFlag_GetFar(FLAG_DEBUG_RESOURCE_BEEP) != 0) {
    s32 freeBlocks;
    s32 blockIndex;

    freeBlocks = 0;
    for (blockIndex = 0; blockIndex < 512; blockIndex++) {
      if (gVramCacheBlockOwners[blockIndex] == 0xff)
        freeBlocks++;
    }
    if (freeBlocks - 136 < 0)
      Sound_PlaySongFar(SE_CLICK);
  }
  /* FAKEMATCH(volatile): the volatile read puts gDebugMode's address in r5 here
   * and at the test before it, as native; as gDebugMode != 0, 9 of 808
   * instructions differ in tbs-en. */
  // Debug: holding L spins through four empty loops (511, 96, 64 and 64
  // passes), a short stall each frame. They are written with goto
  // (MATCHING.md): as while loops, the loop optimizer deletes them.
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
  if (gKeysHeld & gSaveBlock.dashKeys) {
    actor->speedLimit = FIELD_RUN_SPEED;
    actor->acceleration = FIELD_ACCELERATION;
    movementAnimation = 5;
  } else {
    actor->speedLimit = FIELD_WALK_SPEED;
    actor->acceleration = FIELD_ACCELERATION;
    movementAnimation = 2;
  }
  if (GameFlag_GetFar(FLAG_FAST_WALK) != 0 && (gKeysHeld & KEY_B)) {
    actor->speedLimit = FIELD_FAST_WALK_SPEED;
    actor->acceleration = FIELD_FAST_WALK_ACCELERATION;
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
    position.x = actor->x.fixed;
    position.y = actor->y.fixed;
    position.z = actor->z.fixed;
    Vector_AddPolarOffset(0x80000, (u16)angle, &position);
    if (gDebugMode != 0 && (gKeysHeld & KEY_L)) {
      heading = angle;
    } else {
      if (MapGrid_CheckCollision(actor, &position) == COLLISION_NONE) {
        testPosition.x = actor->x.fixed;
        testPosition.y = actor->y.fixed;
        testPosition.z = actor->z.fixed;
        Vector_AddPolarOffset(0x80000, (u16)angle + 0x1000, &testPosition);
        if (MapGrid_CheckCollision(actor, &testPosition) == COLLISION_NONE) {
          testPosition.x = actor->x.fixed;
          testPosition.y = actor->y.fixed;
          testPosition.z = actor->z.fixed;
          Vector_AddPolarOffset(0x80000, (u16)angle - 0x1000, &testPosition);
          if (MapGrid_CheckCollision(actor, &testPosition) == COLLISION_NONE) {
            testPosition.x = actor->x.fixed;
            testPosition.y = actor->y.fixed;
            testPosition.z = actor->z.fixed;
            Vector_AddPolarOffset(0x80000, (u16)angle + 0x2000, &testPosition);
            if (MapGrid_CheckCollision(actor, &testPosition) ==
                COLLISION_NONE) {
              testPosition.x = actor->x.fixed;
              testPosition.y = actor->y.fixed;
              testPosition.z = actor->z.fixed;
              Vector_AddPolarOffset(0x80000, (u16)angle - 0x2000,
                                    &testPosition);
              if (MapGrid_CheckCollision(actor, &testPosition) ==
                  COLLISION_NONE) {
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
        position.x = actor->x.fixed;
        position.y = actor->y.fixed;
        position.z = actor->z.fixed;
        Vector_AddPolarOffset(0x80000, (u16)heading, &position);
        if (MapGrid_CheckCollision(actor, &position) != COLLISION_NONE)
          continue;
        testPosition.x = actor->x.fixed;
        testPosition.y = actor->y.fixed;
        testPosition.z = actor->z.fixed;
        Vector_AddPolarOffset(0x80000, (u16)heading + 0x1000, &testPosition);
        if (MapGrid_CheckCollision(actor, &testPosition) != COLLISION_NONE)
          continue;
        testPosition.x = actor->x.fixed;
        testPosition.y = actor->y.fixed;
        testPosition.z = actor->z.fixed;
        Vector_AddPolarOffset(0x80000, (u16)heading - 0x1000, &testPosition);
        if (MapGrid_CheckCollision(actor, &testPosition) != COLLISION_NONE)
          continue;
        testPosition.x = actor->x.fixed;
        testPosition.y = actor->y.fixed;
        testPosition.z = actor->z.fixed;
        Vector_AddPolarOffset(0x80000, (u16)heading + 0x2000, &testPosition);
        if (MapGrid_CheckCollision(actor, &testPosition) != COLLISION_NONE)
          continue;
        testPosition.x = actor->x.fixed;
        testPosition.y = actor->y.fixed;
        testPosition.z = actor->z.fixed;
        Vector_AddPolarOffset(0x80000, (u16)heading - 0x2000, &testPosition);
        if (MapGrid_CheckCollision(actor, &testPosition) == COLLISION_NONE)
          goto push_objects;
      }
      // Nothing passed: the step is the actor's own position.
      position.x = actor->x.fixed;
      position.y = actor->y.fixed;
      position.z = actor->z.fixed;
      collision |= 1;

    push_objects:
      // Push the movable actors (ACTOR_PUSHABLE) just ahead 4 pixels
      // further, when they have room; any other solid actor there blocks the
      // step (bit 1 of collision). Pushing slows the player down.
      contact.x = actor->x.fixed;
      contact.y = actor->y.fixed;
      contact.z = actor->z.fixed;
      Vector_AddPolarOffset(0x40000, (u16)heading, &contact);
      pushedActor = gActors;
      for (angleIndex = 63; angleIndex >= 0; angleIndex -= 1, pushedActor++) {
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
          direction = ArcTan2(pushedActor->z.fixed - actor->z.fixed,
                              pushedActor->x.fixed - actor->x.fixed);
          testPosition.x = pushedActor->x.fixed;
          testPosition.y = pushedActor->y.fixed;
          testPosition.z = pushedActor->z.fixed;
          heading = direction;
          Vector_AddPolarOffset(0x4000, direction, &testPosition);
          if (!ActorScript_CheckOverlap(pushedActor, (s32 *)&testPosition)) {
            testPosition.x = pushedActor->x.fixed;
            testPosition.y = pushedActor->y.fixed;
            testPosition.z = pushedActor->z.fixed;
            Vector_AddPolarOffset(0xa0000, direction, &testPosition);
            if (MapGrid_CheckCollision(pushedActor, &testPosition) ==
                COLLISION_NONE) {
              testPosition.x = pushedActor->x.fixed;
              testPosition.y = pushedActor->y.fixed;
              testPosition.z = pushedActor->z.fixed;
              Vector_AddPolarOffset(0xa0000, direction + 0x1000, &testPosition);
              // The same probe is tested twice.
              if (MapGrid_CheckCollision(pushedActor, &testPosition) ==
                      COLLISION_NONE &&
                  MapGrid_CheckCollision(pushedActor, &testPosition) ==
                      COLLISION_NONE) {
                testPosition.x = pushedActor->x.fixed;
                testPosition.y = pushedActor->y.fixed;
                testPosition.z = pushedActor->z.fixed;
                Vector_AddPolarOffset(0xa0000, direction - 0x1000,
                                      &testPosition);
                if (MapGrid_CheckCollision(pushedActor, &testPosition) ==
                    COLLISION_NONE) {
                  Vector_AddPolarOffset(
                      0x4000, direction,
                      (struct FieldPosition *)&pushedActor->x.fixed);
                  pushedActor->targetX = TARGET_UNSET;
                  pushedActor->targetY = TARGET_UNSET;
                  pushedActor->targetZ = TARGET_UNSET;
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
        actor->speedLimit = 0x4000;
        actor->acceleration = 0x2000;
      }
    }
  }
  // Count the blocked steps, and pick the animation: push (8), bump (9, or
  // 22 when the leader is down) or walk / run.
  if (gFieldWork != NULL) {
    if (collision & 3)
      gFieldWork->blockedSteps++;
    else
      gFieldWork->blockedSteps = 0;
  }
  if (pushed != 0) {
    Actor_SelectAnimation(actor, 8);
  } else if (collision != 0) {
    s32 action;

    action = 9;
    if (Unit_GetFar(gSaveBlock.leaderActorId)->hp == 0)
      action = 22;
    Actor_SelectAnimation(actor, action);
  } else {
    Actor_SelectAnimation(actor, movementAnimation);
  }

  // Blocked: stop and turn towards the pad, at most 22.5 degrees a frame.
  // Otherwise move to the step, keeping the speed but along the heading.
  // footprintSide = 2 marks a stop; the next footprint then takes animation 2.
  if (collision != 0) {
    actor->targetX = TARGET_UNSET;
    actor->targetY = TARGET_UNSET;
    actor->targetZ = TARGET_UNSET;
    actor->velocityX = 0;
    actor->velocityZ = 0;
    if (collision & 3) {
      s32 difference;

      difference = (u16)angle - actor->facing;
      difference = (s16)difference;
      if (difference > 0x1000)
        difference = 0x1000;
      if (difference < -0x1000)
        difference = -0x1000;
      actor->facing += difference;
    }
    actor->timer = 0;
    actor->userValue = 2;
  } else {
    s32 speed;

    Actor_SetMoveTarget(actor, position.x, position.y, position.z);
    speed = FixedSqrt(Iwram_MulQ16(actor->velocityX, actor->velocityX) +
                      Iwram_MulQ16(actor->velocityZ, actor->velocityZ));
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
      // where it is laid) and its sprite no shadow. It lasts 120 frames,
      // sorts underfoot, draws at OAM priority 3, under
      // any map layer of priority 0 to 2, and lies rotated a quarter turn
      // from the heading.
      effect->groundHeight = actor->groundHeight;
      animation = effect->sprite;
      Actor_StartScript(effect, gWorldMap_FootprintActions);
      effect->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
      effect->motionFlags = collision;
      if (animation != 0) {
        Sprite_StartAnim(animation, 1);
        animation->flags = collision;
        animation->rotation = footprintAngle + 0x4000;
        animation->image.priority = 3;
      }
      // The first footprint after a stop takes animation 2 and restarts the
      // sides; each second one faces a half turn round.
      // Read signed: the footprint side was a signed halfword here.
      if (*(s16 *)&actor->userValue == 2) {
        Sprite_StartAnim(animation, 2);
        actor->userValue = collision;
      }
      if (*(s16 *)&actor->userValue != 0)
        effect->facing = 0x8000;
      if (movementAnimation == 5)
        actor->timer = 12;
      else
        actor->timer = 18;
      actor->userValue ^= 1;
    }
  }
  Field_CheckConfiguredKeys();
  actor->scriptCursor++;
  return 1;
}

/// SC_WORLD: moves the player on the world map by the direction pad, as
/// SC_PLAYER does on the field, and turns the world map camera with the pad.
/// Shallow water (WORLD_MAP_COLLISION_SHALLOW_WATER) leaves a ripple and a
/// forest (WORLD_MAP_COLLISION_FOREST) hides the shadow.
///
/// Run each frame by gActor_WorldMapScript, which the scene manager
/// (scene_manager.c) starts on the leader through
/// Actor_StartWorldMapControl on the world map. Sets the leader's
/// speed, target, velocity, angle and timer, turns the perspective work's
/// yaw, counts gFieldWork->blockedSteps and creates the ripple actors.
s32 Actor_MoveOnWorldMap(struct Actor *interpreter) {
  struct Actor *actor = interpreter;

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
  struct WorldMapView *perspective;
  s32 yawStep;
  struct Sprite *playerSprite;
  struct Actor *ripple;
  struct Sprite *rippleSprite;

  movementAnimation = 2;
  collision = 0;
#if !TBS_PATCH_1
  count = 0;
#endif
  // Walk, or run with the dash keys. L, as it repeats, raises the limit
  // further; unlike the field's fast walk, no debug test or flag guards it.
  if (gKeysHeld & gSaveBlock.dashKeys) {
    actor->speedLimit = WORLD_MAP_RUN_SPEED;
    actor->acceleration = WORLD_MAP_RUN_ACCELERATION;
    movementAnimation = 5;
  } else {
    actor->speedLimit = WORLD_MAP_WALK_SPEED;
    actor->acceleration = WORLD_MAP_ACCELERATION;
  }
  if ((gKeysRepeat & KEY_L) != 0)
    actor->speedLimit = WORLD_MAP_FAST_SPEED;

  angle = gWorldMap_KeyHeadings[(gKeysHeld >> 4) & 15];
  direction = angle;
  if (direction == 0xffff) {
    collision |= 4;
  } else {
    collision = 0;
    position.x = actor->x.fixed;
    position.y = actor->y.fixed;
    position.z = actor->z.fixed;
    Vector_AddPolarOffset(WORLD_MAP_PROBE_RADIUS, direction, &position);
    // Debug L walks anywhere. Otherwise the English release on tests the
    // step and 22.5 and 45 degrees either side, then headings up to 90
    // degrees off; the Japanese one tests the step and 30 degrees (0x1555)
    // either side, then headings up to 67.5 degrees off.
    if (gDebugMode == 0 || (gKeysHeld & KEY_L) == 0) {
#if TBS_PATCH_1
      if (WorldMap_IsWalkable((s32)actor, (struct WorldPosition *)&position) ==
          0) {
        testPosition.x = actor->x.fixed;
        testPosition.y = actor->y.fixed;
        testPosition.z = actor->z.fixed;
        Vector_AddPolarOffset(WORLD_MAP_PROBE_RADIUS, direction + 0x1000,
                              &testPosition);
        if (WorldMap_IsWalkable((s32)actor,
                                (struct WorldPosition *)&testPosition) == 0) {
          testPosition.x = actor->x.fixed;
          testPosition.y = actor->y.fixed;
          testPosition.z = actor->z.fixed;
          Vector_AddPolarOffset(WORLD_MAP_PROBE_RADIUS, direction - 0x1000,
                                &testPosition);
          if (WorldMap_IsWalkable((s32)actor,
                                  (struct WorldPosition *)&testPosition) == 0) {
            testPosition.x = actor->x.fixed;
            testPosition.y = actor->y.fixed;
            testPosition.z = actor->z.fixed;
            Vector_AddPolarOffset(WORLD_MAP_PROBE_RADIUS, direction + 0x2000,
                                  &testPosition);
            if (WorldMap_IsWalkable(
                    (s32)actor, (struct WorldPosition *)&testPosition) == 0) {
              testPosition.x = actor->x.fixed;
              testPosition.y = actor->y.fixed;
              testPosition.z = actor->z.fixed;
              Vector_AddPolarOffset(WORLD_MAP_PROBE_RADIUS, direction - 0x2000,
                                    &testPosition);
              if (WorldMap_IsWalkable(
                      (s32)actor, (struct WorldPosition *)&testPosition) == 0)
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
        Vector_AddPolarOffset(WORLD_MAP_PROBE_RADIUS, direction + 0x1000,
                              &testPosition);
        if (WorldMap_IsWalkable((s32)actor,
                                (struct WorldPosition *)&testPosition) != 0)
          continue;
        testPosition.x = actor->x.fixed;
        testPosition.y = actor->y.fixed;
        testPosition.z = actor->z.fixed;
        Vector_AddPolarOffset(WORLD_MAP_PROBE_RADIUS, direction - 0x1000,
                              &testPosition);
        if (WorldMap_IsWalkable((s32)actor,
                                (struct WorldPosition *)&testPosition) != 0)
          continue;
        testPosition.x = actor->x.fixed;
        testPosition.y = actor->y.fixed;
        testPosition.z = actor->z.fixed;
        Vector_AddPolarOffset(WORLD_MAP_PROBE_RADIUS, direction + 0x2000,
                              &testPosition);
        if (WorldMap_IsWalkable((s32)actor,
                                (struct WorldPosition *)&testPosition) != 0)
          continue;
        testPosition.x = actor->x.fixed;
        testPosition.y = actor->y.fixed;
        testPosition.z = actor->z.fixed;
        Vector_AddPolarOffset(WORLD_MAP_PROBE_RADIUS, direction - 0x2000,
                              &testPosition);
        if (WorldMap_IsWalkable((s32)actor,
                                (struct WorldPosition *)&testPosition) != 0)
          continue;
        angle = (s16)direction;
        goto update_object;
      }
      collision |= 1;

#else
      leftPosition.x = actor->x.fixed;
      leftPosition.y = actor->y.fixed;
      leftPosition.z = actor->z.fixed;
      Vector_AddPolarOffset(WORLD_MAP_PROBE_RADIUS, direction + 0x1555,
                            &leftPosition);
      rightPosition.x = actor->x.fixed;
      rightPosition.y = actor->y.fixed;
      rightPosition.z = actor->z.fixed;
      Vector_AddPolarOffset(WORLD_MAP_PROBE_RADIUS, direction - 0x1555,
                            &rightPosition);
      if ((WorldMap_IsWalkable((s32)actor, (struct WorldPosition *)&position) |
           WorldMap_IsWalkable((s32)actor,
                               (struct WorldPosition *)&leftPosition) |
           WorldMap_IsWalkable((s32)actor,
                               (struct WorldPosition *)&rightPosition)) != 0) {
        angleOffsets[count++] = direction + 0x1000;
        angleOffsets[count++] = direction - 0x1000;
        angleOffsets[count++] = direction + 0x2000;
        angleOffsets[count++] = direction - 0x2000;
        angleOffsets[count++] = direction + 0x3000;
        angleOffsets[count++] = direction - 0x3000;
        for (angleIndex = 0; angleIndex < count; angleIndex++) {
          direction = angleOffsets[angleIndex];
          position.x = actor->x.fixed;
          position.y = actor->y.fixed;
          position.z = actor->z.fixed;
          Vector_AddPolarOffset(WORLD_MAP_PROBE_RADIUS, direction, &position);
          leftPosition.x = actor->x.fixed;
          leftPosition.y = actor->y.fixed;
          leftPosition.z = actor->z.fixed;
          Vector_AddPolarOffset(WORLD_MAP_PROBE_RADIUS, direction + 0x1555,
                                &leftPosition);
          rightPosition.x = actor->x.fixed;
          rightPosition.y = actor->y.fixed;
          rightPosition.z = actor->z.fixed;
          Vector_AddPolarOffset(WORLD_MAP_PROBE_RADIUS, direction - 0x1555,
                                &rightPosition);
          if ((WorldMap_IsWalkable((s32)actor,
                                   (struct WorldPosition *)&position) |
               WorldMap_IsWalkable((s32)actor,
                                   (struct WorldPosition *)&leftPosition) |
               WorldMap_IsWalkable(
                   (s32)actor, (struct WorldPosition *)&rightPosition)) != 0)
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
  if (gFieldWork != NULL) {
    if ((collision & 3) != 0)
      gFieldWork->blockedSteps++;
    else
      gFieldWork->blockedSteps = 0;
  }
  if (collision != 0)
    Actor_SelectAnimation(actor, 9);
  else
    Actor_SelectAnimation(actor, movementAnimation);

  if (collision != 0) {
    actor->targetX = TARGET_UNSET;
    actor->targetY = TARGET_UNSET;
    actor->targetZ = TARGET_UNSET;
    actor->velocityX = 0;
    actor->velocityZ = 0;
    if ((collision & 3) != 0) {
      direction = angle;
      difference = (s16)(direction - actor->facing);
      if (difference > 0x1000)
        difference = 0x1000;
      if (difference < -0x1000)
        difference = -0x1000;
      actor->facing += difference;
    }
    actor->timer = 0;
    actor->userValue = 2;
  } else {
    Actor_SetMoveTarget(actor, position.x, position.y, position.z);
    speed = FixedSqrt(Iwram_MulQ16(actor->velocityX, actor->velocityX) +
                      Iwram_MulQ16(actor->velocityZ, actor->velocityZ));
    // collision is 0 here: keep the speed, along the chosen heading.
    actor->velocityX = collision;
    actor->velocityZ = collision;
    Vector_AddPolarOffset(speed, (u16)angle,
                          (struct FieldPosition *)&actor->velocityX);
    if (actor->timer != 0)
      actor->timer--;
  }

  // Turn the camera towards the pad's yaw by an eighth of the difference a
  // frame, at most 0x200 (about 2.8 degrees), finishing the turn once the
  // step falls under 16.
  perspective = gMapGridWork[0];
  targetCameraYaw = gWorldMap_KeyYaws[(gKeysHeld >> 4) & 15];
  yawStep = (s16)(targetCameraYaw - perspective->yaw) / 8;
  if (yawStep > 0x200)
    yawStep = 0x200;
  if (yawStep < -0x200)
    yawStep = -0x200;
  if (yawStep > -16 && yawStep < 16)
    yawStep = targetCameraYaw - perspective->yaw;
  perspective->yaw += yawStep;

  if (actor->spriteStorageKind == ACTOR_SPRITE_ONE) {
    playerSprite = actor->sprite;
    terrainCollision =
        WorldMap_GetCollision((struct WorldPosition *)&actor->x.fixed);
    // In a forest the sprite's second anim is drawn (priority 1) and its
    // shadow is not; elsewhere that anim takes priority 9, and an anim above
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
        actor->timer == 0 && collision == 0) {
      ripple = Actor_Create(FIELD_SPRITE_RIPPLE, actor->x.fixed, actor->y.fixed,
                            actor->z.fixed);
      if (ripple != 0) {
        rippleSprite = ripple->sprite;
        Actor_StartScript(ripple, gWorldMap_FootprintActions + 3);
        ripple->motionFlags = collision;
        ripple->layer = 1;
        if (rippleSprite != 0) {
          Sprite_StartAnim(rippleSprite, 1);
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
/// Returns 1. Run each frame by gActor_KeyCheckScript, which the
/// scene manager starts on the leader in LEADER_MOVEMENT_AIRBORNE, where the
/// pad does not move it.
s32 Field_CheckConfiguredKeysAndCount(struct Actor *interpreter) {
  struct Actor *actor = interpreter;

  Field_CheckConfiguredKeys();
  actor->scriptCursor++;
  return 1;
}

/// SC_KEYS: moves an actor 8 pixels at a time by the direction pad, with up
/// and down changing its height (y) rather than its depth. Refuses steps
/// into an occupied cell, across a change of metatile behavior, or onto ground
/// too high (up) or too low (down). Returns 1.
///
/// Run each frame by gActor_KeysScript, which the scene manager starts
/// on the leader in LEADER_MOVEMENT_CLIMB. Sets the leader's speed,
/// angle, target or velocity, and counts gFieldWork->blockedSteps.
s32 Actor_MoveByKeys(struct Actor *interpreter) {
  struct Actor *actor = interpreter;

  struct FieldPosition position;
  /* FAKEMATCH(dead): an unused local for the 68 bytes of native frame no
   * instruction touches, the size of Actor_MoveOnWorldMap's probe locals;
   * without it, 4 of 213 instructions differ in tbs-en. */
  UNUSED u8 unusedProbeFrame[68];
  struct MapGridCell *sourceCell;
  struct MapGridCell *destinationCell;
  u16 angle;
  s32 animation;
  s32 collisionFlags;
  s32 deltaZ;
  s32 groundHeight;

  actor->speedLimit = 0x8000;
  actor->acceleration = 0x4000;
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
    // The cells (16 pixels, 0x100000) under the actor and the step; a step
    // into a cell with another metatile behavior is refused.
    sourceCell = &((struct MapGridCell *)
                       Ram_MapGridCellBuffer)[actor->x.fixed / 0x100000 +
                                              (position.z / 0x100000) * 128];
    destinationCell =
        &((struct MapGridCell *)
              Ram_MapGridCellBuffer)[position.x / 0x100000 +
                                     (position.z / 0x100000) * 128];
    if (MapGrid_IsWallAt((struct WorldPosition *)&position) != 0 ||
        sourceCell->behavior != destinationCell->behavior) {
      collisionFlags = 4;
      animation = 12;
    } else if (gKeysHeld & KEY_UP) {
      // Climbing needs the ground a cell further up to stand at least 16
      // pixels above the actor; descending needs the ground at the step
      // at least 8 pixels below it.
      groundHeight = MapGrid_GetTerrainHeight(actor->layer, position.x,
                                              position.z - 0x100000);
      if (groundHeight - actor->y.fixed < 0x100000)
        goto refused;
    } else if (gKeysHeld & KEY_DOWN) {
      groundHeight =
          MapGrid_GetTerrainHeight(actor->layer, position.x, position.z);
      if (groundHeight - actor->y.fixed > -0x80000) {
      refused:
        collisionFlags = 1;
        animation = 12;
      }
    }
  }
  if (gFieldWork != NULL) {
    if (collisionFlags & 3)
      gFieldWork->blockedSteps++;
    else
      gFieldWork->blockedSteps = 0;
  }
  Actor_SelectAnimation(actor, animation);
  if (collisionFlags != 0) {
    actor->targetX = TARGET_UNSET;
    actor->targetY = TARGET_UNSET;
    actor->targetZ = TARGET_UNSET;
    SET_VELOCITY(actor, 0, 0, 0);
  } else {
    Actor_SetMoveTarget(actor, position.x, position.y, position.z);
  }
  actor->scriptCursor++;
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
