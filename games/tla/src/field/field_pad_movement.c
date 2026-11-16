/// @file
/// Script commands that move the leader by the pad, on the map and the world
/// map, and the configured keys checked from a script.
// Part of field.c.

#include "field_internal.h"
#include "flags.h"
#include "fixed_math.h"
#include "field/flag_call.h"
#include "field/field.h"
#include "field/object.h"
#include "macros.h"
#include "field/resource_slot.h"

/// SC_WORLD: moves the player on the world map by the direction pad, as
/// SC_PLAYER does on the field, and turns the world map camera with the pad.
s32 Actor_MoveOnWorldMap(struct Actor *object) {
  struct FieldPosition position;
  struct FieldPosition leftPosition;
  struct FieldPosition rightPosition;
  s16 angleOffsets[22];
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
  struct WorldMapView *camera;
  s32 yawStep;
  struct PlayerSprite *playerSprite;
  struct Actor *ripple;
  struct PlayerSprite *rippleSprite;
  struct FieldWork *work;

  movementAnimation = ANIM_WALK;
  collision = 0;
  count = 0;
  if (gInput.held & gMainRoutineWork.dashKeys) {
    object->speedLimit = WORLD_MAP_RUN_SPEED;
    object->acceleration = WORLD_MAP_RUN_ACCELERATION;
    movementAnimation = ANIM_LEADER_RUN;
  } else {
    object->speedLimit = WORLD_MAP_WALK_SPEED;
    object->acceleration = WORLD_MAP_ACCELERATION;
  }

  angle = gWorldMap_KeyHeadings[DPAD_INDEX(gInput.held)];
  direction = angle;
  if (direction == WORLD_MAP_KEY_HEADING_NONE) {
    collision |= PLAYER_STEP_STOPPED;
  } else {
    collision = 0;
    SET_POSITION(&position, object->x.fixed, object->y.fixed, object->z.fixed);
    Vector_AddPolarOffset(WORLD_MAP_PROBE_RADIUS, direction, &position);
    if (gDebugMode == 0 || ((gInput.held & KEY_L) == 0 &&
                            GameFlag_Get(FLAG_DEBUG_STEPS_OFF) == 0)) {
      SET_POSITION(&leftPosition, object->x.fixed, object->y.fixed,
                   object->z.fixed);
      Vector_AddPolarOffset(WORLD_MAP_PROBE_RADIUS, direction + DEGtoANG(30),
                            &leftPosition);
      SET_POSITION(&rightPosition, object->x.fixed, object->y.fixed,
                   object->z.fixed);
      Vector_AddPolarOffset(WORLD_MAP_PROBE_RADIUS, direction - DEGtoANG(30),
                            &rightPosition);
      if ((WorldMap_IsWalkable(object, &position) |
           WorldMap_IsWalkable(object, &leftPosition) |
           WorldMap_IsWalkable(object, &rightPosition)) != 0) {
        angleOffsets[count++] = direction + FIELD_PROBE_ANGLE;
        angleOffsets[count++] = direction - FIELD_PROBE_ANGLE;
        angleOffsets[count++] = direction + DEGtoANG(45);
        angleOffsets[count++] = direction - DEGtoANG(45);
        angleOffsets[count++] = direction + FIELD_PROBE_ANGLE * 3;
        angleOffsets[count++] = direction - FIELD_PROBE_ANGLE * 3;
        for (angleIndex = 0; angleIndex < count; angleIndex++) {
          direction = angleOffsets[angleIndex];
          SET_POSITION(&position, object->x.fixed, object->y.fixed,
                       object->z.fixed);
          Vector_AddPolarOffset(WORLD_MAP_PROBE_RADIUS, direction, &position);
          SET_POSITION(&leftPosition, object->x.fixed, object->y.fixed,
                       object->z.fixed);
          Vector_AddPolarOffset(WORLD_MAP_PROBE_RADIUS,
                                direction + DEGtoANG(30), &leftPosition);
          SET_POSITION(&rightPosition, object->x.fixed, object->y.fixed,
                       object->z.fixed);
          Vector_AddPolarOffset(WORLD_MAP_PROBE_RADIUS,
                                direction - DEGtoANG(30), &rightPosition);
          if ((WorldMap_IsWalkable(object, &position) |
               WorldMap_IsWalkable(object, &leftPosition) |
               WorldMap_IsWalkable(object, &rightPosition)) != 0)
            continue;
          angle = (s16)direction;
          goto update_object;
        }
        collision |= PLAYER_STEP_BLOCKED_BY_MAP;
      }
    }
  }

update_object:
  work = Ram_HeapSlots->fieldWork;
  if (work != NULL) {
    if ((collision & PLAYER_STEP_BLOCKED) != 0)
      work->blockedSteps++;
    else
      work->blockedSteps = 0;
  }
  if (collision != 0)
    Actor_SelectAnimation(object, ANIM_LEADER_LOOK_AROUND);
  else
    Actor_SelectAnimation(object, movementAnimation);

  if (collision != 0) {
    object->targetX = TARGET_UNSET;
    object->targetY = TARGET_UNSET;
    object->targetZ = TARGET_UNSET;
    object->velocityX = 0;
    object->velocityZ = 0;
    if ((collision & PLAYER_STEP_BLOCKED) != 0) {
      direction = angle;
      difference = (s16)(direction - object->facing);
      if (difference > FIELD_BLOCKED_TURN_MAX)
        difference = FIELD_BLOCKED_TURN_MAX;
      if (difference < -FIELD_BLOCKED_TURN_MAX)
        difference = -FIELD_BLOCKED_TURN_MAX;
      object->facing += difference;
    }
    object->timer = 0;
    object->userValue = FOOTPRINT_BOTH_FEET;
  } else {
    Actor_SetMoveTarget(object, position.x, position.y, position.z);
    speed = Math_SqrtFX(Iwram_MulFX(object->velocityX, object->velocityX) +
                        Iwram_MulFX(object->velocityZ, object->velocityZ));
    object->velocityX = collision;
    object->velocityZ = collision;
    Vector_AddPolarOffset(speed, (u16)angle,
                          (struct FieldPosition *)&object->velocityX);
    if (object->timer != 0)
      object->timer--;
  }

  camera = (struct WorldMapView *)Ram_HeapSlots->mapGrid;
  targetCameraYaw = gWorldMap_KeyYaws[DPAD_INDEX(gInput.held)];
  yawStep = (s16)(targetCameraYaw - camera->yaw) / 8;
  if (yawStep > WORLD_MAP_YAW_STEP_MAX)
    yawStep = WORLD_MAP_YAW_STEP_MAX;
  if (yawStep < -WORLD_MAP_YAW_STEP_MAX)
    yawStep = -WORLD_MAP_YAW_STEP_MAX;
  if (yawStep > -16 && yawStep < 16)
    yawStep = targetCameraYaw - camera->yaw;
  camera->yaw += yawStep;

  if (object->spriteStorageKind == ACTOR_SPRITE_ONE) {
    playerSprite = object->sprite;
    terrainCollision =
        WorldMap_GetCollision((struct WorldPosition *)&object->x.fixed);
    if (terrainCollision == WORLD_MAP_COLLISION_FOREST) {
      playerSprite->secondLayer->priority = 1;
      playerSprite->flags = 0;
    } else {
      playerSprite->secondLayer->priority = 9;
      playerSprite->flags = SPRITE_FLAG_SHADOW;
    }
    if (terrainCollision == WORLD_MAP_COLLISION_SHALLOW_WATER &&
        object->timer == 0 && collision == 0) {
      ripple = Actor_Create(WORLD_MAP_SPRITE_RIPPLE, object->x.fixed,
                            object->y.fixed, object->z.fixed);
      if (ripple != 0) {
        rippleSprite = ripple->sprite;
        Actor_StartScript(ripple, gActor_RippleScript);
        ripple->motionFlags = collision;
        ripple->layer = 1;
        if (rippleSprite != 0) {
          Sprite_StartAnimation(rippleSprite, 1);
          rippleSprite->flags = collision;
          rippleSprite->objectMode = 1;
          rippleSprite->priority = 2;
        }
        object->timer = 10;
      }
    }
  }
  Field_CheckConfiguredKeys();
  if (gInput.pressed & gMainRoutineWork.aKeys)
    gFieldWork->raisedTrigger = WORLD_MAP_RAISED_BOARD_SHIP;
  object->scriptCursor++;
  return 1;
}

/// SC_KEY_COUNT: checks the configured keys (Field_CheckConfiguredKeys) and
/// steps the script on. Returns 1.
s32 Field_CheckConfiguredKeysAndCount(struct Actor *actor) {

  Field_CheckConfiguredKeys();
  actor->scriptCursor++;
  return 1;
}

/// The ship turns toward the pad by this much a frame, once the turn left is
/// wider than FIELD_BLOCKED_TURN_MAX; a narrower one it makes at once.
#define SHIP_TURN_STEP (TRIG_TURN / 64)

// Two 16.16 coordinates fall in the same 16-pixel cell, rounding to nearest.
#define SAME_CELL(a, b) ((((a) - (b) + 0x80000) >> 20) == 0)

/// Script command 0x1f (control mode 6): the ship moved by the direction pad
/// on a field map, as the party's actor (gMainRoutineWork.ownerOverride), as at
/// Lemuria's entrance.
s32 Actor_SailOnMap(struct Actor *interpreter) {
  struct Actor *object = interpreter;

  struct FieldPosition position;
  struct FieldPosition testPosition;
  struct FieldPosition contact;
  s16 angleOffsets[22];
  s32 collision;
  s32 pushed;
  s16 heading;
  u16 angle;
  u16 direction;
  s32 angleIndex;
  s32 headingCount;
  s32 onObject;
  s32 difference;
  struct Actor *other;
  struct FieldWork *work;

  collision = 0;
  pushed = 0;
  // Debug: holding L spins through four empty loops, a short stall each
  // frame.
  if (gDebugMode != 0) {
    s32 delayKeyMask;

    delayKeyMask = KEY_L;
    if (gInput.held & delayKeyMask) {
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
  object->speedLimit = FIELD_WALK_SPEED;
  object->acceleration = FIELD_ACCELERATION;
  if (GameFlag_Get(FLAG_FAST_WALK) != 0 && (gInput.held & KEY_B)) {
    object->speedLimit = FIELD_FAST_WALK_SPEED;
    object->acceleration = FIELD_FAST_WALK_ACCELERATION;
  }

  angle = gWorldMap_KeyHeadings[DPAD_INDEX(gInput.held)];
  if ((u16)angle == WORLD_MAP_KEY_HEADING_NONE) {
    collision |= PLAYER_STEP_STOPPED;
    goto counted;
  }
  collision = 0;
  SET_POSITION(&position, object->x.fixed, object->y.fixed, object->z.fixed);
  Vector_AddPolarOffset(toFIXED(19), (u16)angle, &position);
  if (object->priorityFlags & ACTOR_PRIORITY_WALKS_ON_PLATFORMS) {
    onObject = 0;
    other = (struct Actor *)Ram_HeapSlots->actors;
    for (angleIndex = 0; angleIndex < ACTOR_COUNT; angleIndex++, other++) {
      FIXED dy;

      if (other->script == NULL)
        continue;
      if (!(other->priorityFlags & ACTOR_PRIORITY_PLATFORM))
        continue;
      if (other == object)
        continue;
      dy = object->y.fixed - other->y.fixed;
      if ((dy - toFIXED(16) < 0 ? -(dy - toFIXED(16)) : dy - toFIXED(16)) >
          toFIXED(8) - 1)
        continue;
      if (SAME_CELL(object->x.fixed, other->x.fixed) &&
          SAME_CELL(object->z.fixed, other->z.fixed))
        onObject = 1;
      if (SAME_CELL(position.x, other->x.fixed) &&
          SAME_CELL(position.z, other->z.fixed)) {
        collision = 0;
        heading = angle;
        goto counted;
      }
    }
    if (onObject && !MapGrid_CheckCollision(object, &position)) {
      collision = 0;
      heading = angle;
      goto counted;
    }
  }
  if (gDebugMode != 0 && (gInput.held & KEY_L)) {
    collision = 0;
    heading = angle;
    goto counted;
  }
  if (!MapGrid_CheckCollision(object, &position)) {
    SET_POSITION(&testPosition, object->x.fixed, object->y.fixed,
                 object->z.fixed);
    Vector_AddPolarOffset(toFIXED(19), (u16)angle + FIELD_PROBE_ANGLE,
                          &testPosition);
    if (!MapGrid_CheckCollision(object, &testPosition)) {
      SET_POSITION(&testPosition, object->x.fixed, object->y.fixed,
                   object->z.fixed);
      Vector_AddPolarOffset(toFIXED(19), (u16)angle - FIELD_PROBE_ANGLE,
                            &testPosition);
      if (!MapGrid_CheckCollision(object, &testPosition)) {
        SET_POSITION(&testPosition, object->x.fixed, object->y.fixed,
                     object->z.fixed);
        Vector_AddPolarOffset(toFIXED(19), (u16)angle + DEGtoANG(45),
                              &testPosition);
        if (!MapGrid_CheckCollision(object, &testPosition)) {
          SET_POSITION(&testPosition, object->x.fixed, object->y.fixed,
                       object->z.fixed);
          Vector_AddPolarOffset(toFIXED(19), (u16)angle - DEGtoANG(45),
                                &testPosition);
          if (!MapGrid_CheckCollision(object, &testPosition)) {
            heading = angle;
            goto push_objects;
          }
        }
      }
    }
  }
  direction = angle;
  headingCount = 0;
  angleOffsets[headingCount++] = direction + FIELD_PROBE_ANGLE;
  angleOffsets[headingCount++] = direction - FIELD_PROBE_ANGLE;
  angleOffsets[headingCount++] = direction + DEGtoANG(45);
  angleOffsets[headingCount++] = direction - DEGtoANG(45);
  angleOffsets[headingCount++] = direction + FIELD_PROBE_ANGLE * 3;
  angleOffsets[headingCount++] = direction - FIELD_PROBE_ANGLE * 3;
  /* FAKEMATCH(dead): an empty once-only loop. */
  do {
  } while (0);
  for (angleIndex = 0; angleIndex < headingCount; angleIndex++) {
    heading = angleOffsets[angleIndex];
    SET_POSITION(&position, object->x.fixed, object->y.fixed, object->z.fixed);
    Vector_AddPolarOffset(toFIXED(19), (u16)heading, &position);
    if (MapGrid_CheckCollision(object, &position))
      continue;
    SET_POSITION(&testPosition, object->x.fixed, object->y.fixed,
                 object->z.fixed);
    Vector_AddPolarOffset(toFIXED(19), (u16)heading + FIELD_PROBE_ANGLE,
                          &testPosition);
    if (MapGrid_CheckCollision(object, &testPosition))
      continue;
    SET_POSITION(&testPosition, object->x.fixed, object->y.fixed,
                 object->z.fixed);
    Vector_AddPolarOffset(toFIXED(19), (u16)heading - FIELD_PROBE_ANGLE,
                          &testPosition);
    if (MapGrid_CheckCollision(object, &testPosition))
      continue;
    SET_POSITION(&testPosition, object->x.fixed, object->y.fixed,
                 object->z.fixed);
    Vector_AddPolarOffset(toFIXED(19), (u16)heading + DEGtoANG(45),
                          &testPosition);
    if (MapGrid_CheckCollision(object, &testPosition))
      continue;
    SET_POSITION(&testPosition, object->x.fixed, object->y.fixed,
                 object->z.fixed);
    Vector_AddPolarOffset(toFIXED(19), (u16)heading - DEGtoANG(45),
                          &testPosition);
    if (!MapGrid_CheckCollision(object, &testPosition))
      goto push_objects;
  }
  SET_POSITION(&position, object->x.fixed, object->y.fixed, object->z.fixed);
  collision |= PLAYER_STEP_BLOCKED_BY_MAP;

push_objects:
  SET_POSITION(&contact, object->x.fixed, object->y.fixed, object->z.fixed);
  Vector_AddPolarOffset(toFIXED(15), (u16)heading, &contact);
  other = (struct Actor *)Ram_HeapSlots->actors;
  for (angleIndex = ACTOR_COUNT - 1; angleIndex >= 0; angleIndex--, other++) {
    s32 radius;

    radius = object->radius - 2;
    if (other->script == NULL)
      continue;
    if (!(other->collisionFlags & 1))
      continue;
    if (other == object)
      continue;
    if (Field_CheckSpheresOverlap(&other->x.fixed, radius, &contact.x,
                                  other->radius - 2) < 0)
      continue;
    if ((other->collisionFlags & 2) && other->paused == 0) {
      direction = Trig_Atan(other->z.fixed - object->z.fixed,
                            other->x.fixed - object->x.fixed);
      SET_POSITION(&testPosition, other->x.fixed, other->y.fixed,
                   other->z.fixed);
      heading = direction;
      Vector_AddPolarOffset(toFIXED(1) / 4, direction, &testPosition);
      if (!ActorScript_CheckOverlap(other, &testPosition.x)) {
        SET_POSITION(&testPosition, other->x.fixed, other->y.fixed,
                     other->z.fixed);
        Vector_AddPolarOffset(toFIXED(21), direction, &testPosition);
        if (!MapGrid_CheckCollision(other, &testPosition)) {
          SET_POSITION(&testPosition, other->x.fixed, other->y.fixed,
                       other->z.fixed);
          Vector_AddPolarOffset(toFIXED(21), direction + FIELD_PROBE_ANGLE,
                                &testPosition);
          // The same probe is tested twice.
          if (!MapGrid_CheckCollision(other, &testPosition) &&
              !MapGrid_CheckCollision(other, &testPosition)) {
            SET_POSITION(&testPosition, other->x.fixed, other->y.fixed,
                         other->z.fixed);
            Vector_AddPolarOffset(toFIXED(21), direction - FIELD_PROBE_ANGLE,
                                  &testPosition);
            if (!MapGrid_CheckCollision(other, &testPosition)) {
              Vector_AddPolarOffset(toFIXED(1) / 4, direction,
                                    (struct FieldPosition *)&other->x.fixed);
              other->targetX = TARGET_UNSET;
              other->targetY = TARGET_UNSET;
              other->targetZ = TARGET_UNSET;
              pushed |= 1;
              continue;
            }
          }
        }
      }
    }
    collision |= PLAYER_STEP_BLOCKED_BY_ACTOR;
  }
  if (collision == 0 && pushed != 0) {
    object->speedLimit = toFIXED(1) / 4;
    object->acceleration = toFIXED(1) / 8;
  }

counted:
  work = Ram_HeapSlots->fieldWork;
  if (work != NULL) {
    if (collision & PLAYER_STEP_BLOCKED)
      work->blockedSteps++;
    else
      work->blockedSteps = 0;
    work->heldKeys = gInput.held;
  }
  if (pushed != 0 || collision != 0)
    Actor_SelectAnimation(object, 1);
  else
    Actor_SelectAnimation(object, 2);

  if (gInput.pressed & gMainRoutineWork.aKeys) {
    gFieldWork->raisedTrigger = WORLD_MAP_RAISED_LEAVE_SHIP;
    collision |= PLAYER_STEP_STOPPED;
  }

  if (collision != 0) {
    object->targetX = TARGET_UNSET;
    object->targetY = TARGET_UNSET;
    object->targetZ = TARGET_UNSET;
    object->velocityX = 0;
    object->velocityZ = 0;
    if (collision & PLAYER_STEP_BLOCKED) {
      difference = (s16)((u16)angle - object->facing);
      if (difference > FIELD_BLOCKED_TURN_MAX)
        difference = SHIP_TURN_STEP;
      if (difference < -FIELD_BLOCKED_TURN_MAX)
        difference = -SHIP_TURN_STEP;
      object->facing += difference;
    }
    object->timer = 0;
    object->userValue = FOOTPRINT_BOTH_FEET;
  } else {
    FIXED speed;

    Actor_SetMoveTarget(object, position.x, position.y, position.z);
    speed = Math_SqrtFX(Iwram_MulFX(object->velocityX, object->velocityX) +
                        Iwram_MulFX(object->velocityZ, object->velocityZ));
    object->velocityX = collision;
    object->velocityZ = collision;
    Vector_AddPolarOffset(speed, (u16)heading,
                          (struct FieldPosition *)&object->velocityX);
    if (object->timer != 0)
      object->timer--;
    object->steeringFlags &= ~1;
    difference = (s16)((u16)angle - object->facing);
    if (difference > FIELD_BLOCKED_TURN_MAX)
      difference = SHIP_TURN_STEP;
    if (difference < -FIELD_BLOCKED_TURN_MAX)
      difference = -SHIP_TURN_STEP;
    object->facing += difference;
  }
  if (object->spriteStorageKind == ACTOR_SPRITE_ONE)
    WorldMap_GetCollision((struct WorldPosition *)&object->x.fixed);
  Field_CheckConfiguredKeys();
  object->scriptCursor++;
  return 1;
}
#undef SAME_CELL

/// Script command 0x20 (control mode 7): the ship sailing the world map, laid
/// out as The Broken Seal's later Actor_MoveOnWorldMap (one probe ahead, then
/// four 22.5 and 45 degrees off it; six headings to retry). It keeps the kind
/// of water under the ship in FieldWork.terrainKind, sails at a run, and turns
/// at most 22.5 degrees a step; A goes ashore, and a dash key takes off once
/// the ship has its wings (FieldWork.hoverReady).
s32 Actor_SailOnWorldMap(struct Actor *object) {
  struct FieldPosition position;
  struct FieldPosition testPosition;
  s16 angleOffsets[28];
  s32 collision;
  s32 angle;
  u16 direction;
  s32 angleIndex;
  s32 count;
  s32 difference;
  s32 terrain;
  FIXED speed;
  s32 targetCameraYaw;
  struct WorldMapView *camera;
  s32 yawStep;
  struct Actor *ripple;
  struct PlayerSprite *rippleSprite;
  struct FieldWork *work;
  struct FieldWork *worldWork;
  u32 kind;

  count = 0;
  terrain = WorldMap_GetCollision((struct WorldPosition *)&object->x.fixed);
  worldWork = Ram_HeapSlots->fieldWork;
  if (terrain >= 1 && terrain <= 3)
    worldWork->terrainKind = 0;
  if (terrain >= 5 && terrain <= 6)
    worldWork->terrainKind = 1;
  if (terrain == 4)
    worldWork->terrainKind = 2;
  if (terrain >= 7 && terrain <= 12)
    worldWork->terrainKind = 3;

  collision = 0;
  object->speedLimit = toFIXED(5) / 4;
  object->acceleration = toFIXED(1) / 2;
  angle = gWorldMap_KeyHeadings[DPAD_INDEX(gInput.held)];
  direction = angle;
  if (direction == WORLD_MAP_KEY_HEADING_NONE) {
    collision |= PLAYER_STEP_STOPPED;
  } else {
    SET_POSITION(&position, object->x.fixed, object->y.fixed, object->z.fixed);
    Vector_AddPolarOffset(WORLD_MAP_PROBE_RADIUS, direction, &position);
    if (gDebugMode == 0 || (gInput.held & KEY_L) == 0) {
      if (WorldMap_IsSailable(object, &position) == 0) {
        SET_POSITION(&testPosition, object->x.fixed, object->y.fixed,
                     object->z.fixed);
        Vector_AddPolarOffset(WORLD_MAP_PROBE_RADIUS,
                              direction + FIELD_PROBE_ANGLE, &testPosition);
        if (WorldMap_IsSailable(object, &testPosition) == 0) {
          SET_POSITION(&testPosition, object->x.fixed, object->y.fixed,
                       object->z.fixed);
          Vector_AddPolarOffset(WORLD_MAP_PROBE_RADIUS,
                                direction - FIELD_PROBE_ANGLE, &testPosition);
          if (WorldMap_IsSailable(object, &testPosition) == 0) {
            SET_POSITION(&testPosition, object->x.fixed, object->y.fixed,
                         object->z.fixed);
            Vector_AddPolarOffset(WORLD_MAP_PROBE_RADIUS, direction + 0x2000,
                                  &testPosition);
            if (WorldMap_IsSailable(object, &testPosition) == 0) {
              SET_POSITION(&testPosition, object->x.fixed, object->y.fixed,
                           object->z.fixed);
              Vector_AddPolarOffset(WORLD_MAP_PROBE_RADIUS, direction - 0x2000,
                                    &testPosition);
              if (WorldMap_IsSailable(object, &testPosition) == 0)
                goto update_object;
            }
          }
        }
      }
      angleOffsets[count++] = direction + FIELD_PROBE_ANGLE;
      angleOffsets[count++] = direction - FIELD_PROBE_ANGLE;
      angleOffsets[count++] = direction + DEGtoANG(45);
      angleOffsets[count++] = direction - DEGtoANG(45);
      angleOffsets[count++] = direction + FIELD_PROBE_ANGLE * 3;
      angleOffsets[count++] = direction - FIELD_PROBE_ANGLE * 3;
      for (angleIndex = 0; angleIndex < count; angleIndex++) {
        direction = angleOffsets[angleIndex];
        SET_POSITION(&position, object->x.fixed, object->y.fixed,
                     object->z.fixed);
        Vector_AddPolarOffset(WORLD_MAP_PROBE_RADIUS, direction, &position);
        if (WorldMap_IsSailable(object, &position) != 0)
          continue;
        SET_POSITION(&testPosition, object->x.fixed, object->y.fixed,
                     object->z.fixed);
        Vector_AddPolarOffset(WORLD_MAP_PROBE_RADIUS,
                              direction + FIELD_PROBE_ANGLE, &testPosition);
        if (WorldMap_IsSailable(object, &testPosition) != 0)
          continue;
        SET_POSITION(&testPosition, object->x.fixed, object->y.fixed,
                     object->z.fixed);
        Vector_AddPolarOffset(WORLD_MAP_PROBE_RADIUS,
                              direction - FIELD_PROBE_ANGLE, &testPosition);
        if (WorldMap_IsSailable(object, &testPosition) != 0)
          continue;
        SET_POSITION(&testPosition, object->x.fixed, object->y.fixed,
                     object->z.fixed);
        Vector_AddPolarOffset(WORLD_MAP_PROBE_RADIUS, direction + 0x2000,
                              &testPosition);
        if (WorldMap_IsSailable(object, &testPosition) != 0)
          continue;
        SET_POSITION(&testPosition, object->x.fixed, object->y.fixed,
                     object->z.fixed);
        Vector_AddPolarOffset(WORLD_MAP_PROBE_RADIUS, direction - 0x2000,
                              &testPosition);
        if (WorldMap_IsSailable(object, &testPosition) != 0)
          continue;
        angle = (s16)direction;
        goto update_object;
      }
      collision |= PLAYER_STEP_BLOCKED_BY_MAP;
    }
  }

update_object:
  work = Ram_HeapSlots->fieldWork;
  if (work != NULL) {
    if ((collision & PLAYER_STEP_BLOCKED) != 0)
      work->blockedSteps++;
    else
      work->blockedSteps = 0;
    work->heldKeys = gInput.held;
  }
  if (collision != 0)
    Actor_SelectAnimation(object, 1);
  else
    Actor_SelectAnimation(object, 2);

  if (gInput.pressed & gMainRoutineWork.aKeys) {
    gFieldWork->raisedTrigger = WORLD_MAP_RAISED_LEAVE_SHIP;
    collision |= PLAYER_STEP_STOPPED;
  }
  if (gInput.pressed & gMainRoutineWork.dashKeys) {
    struct FieldWork *keyWork = Ram_HeapSlots->fieldWork;
    if (keyWork->hoverReady != 0) {
      GameFlag_Clear(0x147);
      ((struct FieldWork *)keyWork)->raisedTrigger = WORLD_MAP_RAISED_TAKE_OFF;
    }
  }

  if (collision != 0) {
    object->targetX = TARGET_UNSET;
    object->targetY = TARGET_UNSET;
    object->targetZ = TARGET_UNSET;
    object->velocityX = 0;
    object->velocityZ = 0;
    if ((collision & PLAYER_STEP_BLOCKED) != 0) {
      direction = angle;
      difference = (s16)(direction - object->facing);
      if (difference > FIELD_BLOCKED_TURN_MAX)
        difference = FIELD_BLOCKED_TURN_MAX;
      if (difference < -FIELD_BLOCKED_TURN_MAX)
        difference = -FIELD_BLOCKED_TURN_MAX;
      object->facing += difference;
    }
    object->timer = 0;
    object->userValue = FOOTPRINT_BOTH_FEET;
  } else {
    Actor_SetMoveTarget(object, position.x, position.y, position.z);
    speed = Math_SqrtFX(Iwram_MulFX(object->velocityX, object->velocityX) +
                        Iwram_MulFX(object->velocityZ, object->velocityZ));
    object->velocityX = collision;
    object->velocityZ = collision;
    direction = angle;
    Vector_AddPolarOffset(speed, direction,
                          (struct FieldPosition *)&object->velocityX);
    if (object->timer != 0)
      object->timer--;
    object->steeringFlags &= ~1;
    difference = (s16)(direction - object->facing);
    if (difference > FIELD_BLOCKED_TURN_MAX)
      difference = SHIP_TURN_STEP;
    if (difference < -FIELD_BLOCKED_TURN_MAX)
      difference = -SHIP_TURN_STEP;
    object->facing += difference;
  }

  camera = (struct WorldMapView *)Ram_HeapSlots->mapGrid;
  targetCameraYaw = gWorldMap_KeyYaws[DPAD_INDEX(gInput.held)];
  yawStep = (s16)(targetCameraYaw - camera->yaw) / 8;
  if (yawStep > WORLD_MAP_YAW_STEP_MAX)
    yawStep = WORLD_MAP_YAW_STEP_MAX;
  if (yawStep < -WORLD_MAP_YAW_STEP_MAX)
    yawStep = -WORLD_MAP_YAW_STEP_MAX;
  if (yawStep > -16 && yawStep < 16)
    yawStep = targetCameraYaw - camera->yaw;
  camera->yaw += yawStep;

  kind = object->spriteStorageKind;
  if (kind == 1) {
    WorldMap_GetCollision((struct WorldPosition *)&object->x.fixed);
    if (object->timer == 0 && collision == 0) {
      ripple = Actor_Create(WORLD_MAP_SPRITE_RIPPLE, object->x.fixed,
                            object->y.fixed, object->z.fixed);
      if (ripple != 0) {
        rippleSprite = ripple->sprite;
        Actor_StartScript(ripple, gActor_RippleScript);
        ripple->motionFlags = 0;
        ripple->layer = kind;
        if (rippleSprite != 0) {
          Sprite_StartAnimation(rippleSprite, 2);
          rippleSprite->flags = 0;
          rippleSprite->objectMode = 0;
          rippleSprite->priority = 2;
        }
        object->timer = 10;
      }
    }
  }
  Field_CheckConfiguredKeys();
  object->scriptCursor++;
  return 1;
}

/// Script command 0x21 (control mode 8): the ship with its wings on the world
/// map, flying over the ground the sea alone would stop (WorldMap_IsFlyable).
s32 Actor_FlyOnWorldMap(struct Actor *object) {
  struct FieldPosition position;
  struct FieldPosition testPosition;
  s16 angleOffsets[28];
  s32 collision;
  s32 angle;
  u16 direction;
  s32 angleIndex;
  s32 count;
  s32 difference;
  s32 terrain;
  FIXED speed;
  FIXED targetCameraYaw;
  struct WorldMapView *camera;
  s32 yawStep;
  struct Actor *ripple;
  struct PlayerSprite *rippleSprite;
  struct FieldWork *work;
  struct FieldWork *worldWork;
  struct Actor *other;
  u32 kind;
  struct FieldWork *flightWork;

  count = 0;
  terrain = WorldMap_GetCollision((struct WorldPosition *)&object->x.fixed);
  worldWork = Ram_HeapSlots->fieldWork;
  if (terrain >= 1 && terrain <= 3)
    worldWork->terrainKind = 0;
  if (terrain >= 5 && terrain <= 6)
    worldWork->terrainKind = 1;
  if (terrain == 4)
    worldWork->terrainKind = 2;
  if (terrain >= 7 && terrain <= 12)
    worldWork->terrainKind = 3;

  object->motionFlags = 0;
  collision = 0;
  object->speedLimit = object->y.fixed / 16 + 0x14000;
  object->acceleration = toFIXED(1) / 2;
  angle = gWorldMap_KeyHeadings[DPAD_INDEX(gInput.held)];
  direction = angle;
  if (direction == WORLD_MAP_KEY_HEADING_NONE) {
    collision |= PLAYER_STEP_STOPPED;
    goto update_object;
  }
  SET_POSITION(&position, object->x.fixed, object->y.fixed, object->z.fixed);
  Vector_AddPolarOffset(WORLD_MAP_PROBE_RADIUS, direction, &position);
  if (gDebugMode != 0 && (gInput.held & KEY_L) != 0)
    goto update_object;
  if (WorldMap_IsFlyable(object, &position) == 0) {
    SET_POSITION(&testPosition, object->x.fixed, object->y.fixed,
                 object->z.fixed);
    Vector_AddPolarOffset(WORLD_MAP_PROBE_RADIUS, direction + FIELD_PROBE_ANGLE,
                          &testPosition);
    if (WorldMap_IsFlyable(object, &testPosition) == 0) {
      SET_POSITION(&testPosition, object->x.fixed, object->y.fixed,
                   object->z.fixed);
      Vector_AddPolarOffset(WORLD_MAP_PROBE_RADIUS,
                            direction - FIELD_PROBE_ANGLE, &testPosition);
      if (WorldMap_IsFlyable(object, &testPosition) == 0) {
        SET_POSITION(&testPosition, object->x.fixed, object->y.fixed,
                     object->z.fixed);
        Vector_AddPolarOffset(WORLD_MAP_PROBE_RADIUS, direction + 0x2000,
                              &testPosition);
        if (WorldMap_IsFlyable(object, &testPosition) == 0) {
          SET_POSITION(&testPosition, object->x.fixed, object->y.fixed,
                       object->z.fixed);
          Vector_AddPolarOffset(WORLD_MAP_PROBE_RADIUS, direction - 0x2000,
                                &testPosition);
          if (WorldMap_IsFlyable(object, &testPosition) == 0)
            goto check_objects;
        }
      }
    }
  }
  angleOffsets[count++] = direction + FIELD_PROBE_ANGLE;
  angleOffsets[count++] = direction - FIELD_PROBE_ANGLE;
  angleOffsets[count++] = direction + DEGtoANG(45);
  angleOffsets[count++] = direction - DEGtoANG(45);
  angleOffsets[count++] = direction + FIELD_PROBE_ANGLE * 3;
  angleOffsets[count++] = direction - FIELD_PROBE_ANGLE * 3;
  for (angleIndex = 0; angleIndex < count; angleIndex++) {
    direction = angleOffsets[angleIndex];
    SET_POSITION(&position, object->x.fixed, object->y.fixed, object->z.fixed);
    Vector_AddPolarOffset(WORLD_MAP_PROBE_RADIUS, direction, &position);
    if (WorldMap_IsFlyable(object, &position) != 0)
      continue;
    SET_POSITION(&testPosition, object->x.fixed, object->y.fixed,
                 object->z.fixed);
    Vector_AddPolarOffset(WORLD_MAP_PROBE_RADIUS, direction + FIELD_PROBE_ANGLE,
                          &testPosition);
    if (WorldMap_IsFlyable(object, &testPosition) != 0)
      continue;
    SET_POSITION(&testPosition, object->x.fixed, object->y.fixed,
                 object->z.fixed);
    Vector_AddPolarOffset(WORLD_MAP_PROBE_RADIUS, direction - FIELD_PROBE_ANGLE,
                          &testPosition);
    if (WorldMap_IsFlyable(object, &testPosition) != 0)
      continue;
    SET_POSITION(&testPosition, object->x.fixed, object->y.fixed,
                 object->z.fixed);
    Vector_AddPolarOffset(WORLD_MAP_PROBE_RADIUS, direction + 0x2000,
                          &testPosition);
    if (WorldMap_IsFlyable(object, &testPosition) != 0)
      continue;
    SET_POSITION(&testPosition, object->x.fixed, object->y.fixed,
                 object->z.fixed);
    Vector_AddPolarOffset(WORLD_MAP_PROBE_RADIUS, direction - 0x2000,
                          &testPosition);
    if (WorldMap_IsFlyable(object, &testPosition) != 0)
      continue;
    angle = (s16)direction;
    goto check_objects;
  }
  collision |= PLAYER_STEP_BLOCKED_BY_MAP;

check_objects:
  other = Ram_HeapSlots->actors;
  for (angleIndex = ACTOR_COUNT - 1; angleIndex >= 0;
       angleIndex -= 1, other++) {
    if (other->script != NULL && (other->collisionFlags & 1) &&
        other != object &&
        Field_CheckSpheresOverlap(&other->x.fixed, other->radius, &position.x,
                                  object->radius) >= 0)
      collision |= PLAYER_STEP_BLOCKED_BY_ACTOR;
  }

update_object:
  work = Ram_HeapSlots->fieldWork;
  if (work != NULL) {
    if ((collision & PLAYER_STEP_BLOCKED) != 0)
      work->blockedSteps++;
    else
      work->blockedSteps = 0;
    work->heldKeys = gInput.held;
  }
  if (object->y.fixed >= toFIXED(4))
    Actor_SelectAnimation(object, 6);
  else if (collision != 0)
    Actor_SelectAnimation(object, 1);
  else
    Actor_SelectAnimation(object, 2);

  if (collision != 0) {
    object->targetX = TARGET_UNSET;
    object->targetY = TARGET_UNSET;
    object->targetZ = TARGET_UNSET;
    object->velocityX = 0;
    object->velocityZ = 0;
    if ((collision & PLAYER_STEP_BLOCKED) != 0) {
      direction = angle;
      difference = (s16)(direction - object->facing);
      if (difference > FIELD_BLOCKED_TURN_MAX)
        difference = FIELD_BLOCKED_TURN_MAX;
      if (difference < -FIELD_BLOCKED_TURN_MAX)
        difference = -FIELD_BLOCKED_TURN_MAX;
      object->facing += difference;
    }
    object->timer = 0;
    object->userValue = FOOTPRINT_BOTH_FEET;
  } else {
    Actor_SetMoveTarget(object, position.x, position.y, position.z);
    speed = Math_SqrtFX(Iwram_MulFX(object->velocityX, object->velocityX) +
                        Iwram_MulFX(object->velocityZ, object->velocityZ));
    object->velocityX = collision;
    object->velocityZ = collision;
    direction = angle;
    Vector_AddPolarOffset(speed, direction,
                          (struct FieldPosition *)&object->velocityX);
    if (object->timer != 0)
      object->timer--;
    object->steeringFlags &= ~1;
    difference = (s16)(direction - object->facing);
    if (difference > FIELD_BLOCKED_TURN_MAX)
      difference = SHIP_TURN_STEP;
    if (difference < -FIELD_BLOCKED_TURN_MAX)
      difference = -SHIP_TURN_STEP;
    object->facing += difference;
  }

  camera = (struct WorldMapView *)Ram_HeapSlots->mapGrid;
  targetCameraYaw = Iwram_MulFX(gWorldMap_KeyYaws[DPAD_INDEX(gInput.held)],
                                object->y.fixed / 4 + 0x10000);
  yawStep = (s16)(targetCameraYaw - camera->yaw) / 8;
  if (yawStep > 32)
    yawStep = 32;
  if (yawStep < -32)
    yawStep = -32;
  if (yawStep >= -1 && yawStep <= 1)
    yawStep = targetCameraYaw - camera->yaw;
  camera->yaw += yawStep;

  kind = object->spriteStorageKind;
  if (kind == 1) {
    if (object->y.fixed < toFIXED(4)) {
      if (object->timer == 0 && collision == 0) {
        ripple = Actor_Create(WORLD_MAP_SPRITE_RIPPLE, object->x.fixed, 0,
                              object->z.fixed);
        if (ripple != 0) {
          rippleSprite = ripple->sprite;
          Actor_StartScript(ripple, gActor_RippleScript);
          ripple->motionFlags = 0;
          ripple->layer = kind;
          if (rippleSprite != 0) {
            Sprite_StartAnimation(rippleSprite, 1);
            rippleSprite->flags = 0;
            rippleSprite->objectMode = 1;
            rippleSprite->priority = 2;
          }
          object->timer = 10;
        }
      }
    } else {
      Actor_SetSpriteFlags(object, gFrameCount & kind);
    }
  }

  flightWork = Ram_HeapSlots->fieldWork;
  if (gInput.held & (gMainRoutineWork.aKeys | gMainRoutineWork.bKeys)) {
    flightWork->climbing = 1;
    object->y.fixed += toFIXED(1) / 8;
  } else {
    object->y.fixed -= toFIXED(1) / 4;
  }
  if (Party_GetPpRatioFar() == 0)
    object->y.fixed -= toFIXED(1) / 4;
  if (object->y.fixed <= 0 && flightWork->climbing != 0)
    ((struct FieldWork *)flightWork)->raisedTrigger = WORLD_MAP_RAISED_LAND;
  if (object->y.fixed >= toFIXED(12))
    object->y.fixed = toFIXED(12);
  if (object->y.fixed <= 0)
    object->y.fixed = 0;
  object->scriptCursor++;
  return 1;
}
