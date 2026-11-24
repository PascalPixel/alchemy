/// @file
/// Script commands that move the leader by the pad, on the map and the world
/// map, and the configured keys checked from a script.
// Part of field.c.

#include "field_internal.h"
#include "flags.h"
#include "fixed_math.h"
#include "field/field.h"
#include "field/object.h"
#include "macros.h"
#include "field/resource_slot.h"
#include "gamepak/far_call/game.h"

/// The flying ship's animation once it is 4 pixels up: its picture moved 10
/// pixels (ANIMCMD_OFFSET_Y) and its wings beating, the seventh of
/// winged_ship_flying_small.tsv.
#define ANIM_SHIP_FLY_HIGH 6

/// A world map camera turn of less than this either way is made at once.
#define WORLD_MAP_YAW_SNAP 16
/// The flying ship's camera turns toward the pad's yaw by at most this a frame.
#define FLYING_YAW_STEP_MAX (TRIG_TURN / 2048)

/// SC_WORLD: moves the player on the world map by the direction pad, as
/// SC_PLAYER does on the field, and turns the world map camera with the pad.
s32 Actor_MoveOnWorldMap(struct Actor *actor) {
  struct Point position;
  struct Point leftPosition;
  struct Point rightPosition;
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
  struct FieldState *state;

  movementAnimation = ANIM_WALK;
  collision = 0;
  count = 0;
  if (gInput.held & gMainRoutineState.dashKeys) {
    actor->speedLimit = WORLD_MAP_RUN_SPEED;
    actor->acceleration = WORLD_MAP_RUN_ACCELERATION;
    movementAnimation = ANIM_LEADER_RUN;
  } else {
    actor->speedLimit = WORLD_MAP_WALK_SPEED;
    actor->acceleration = WORLD_MAP_ACCELERATION;
  }

  angle = gWorldMap_KeyHeadings[DPAD_INDEX(gInput.held)];
  direction = angle;
  if (direction == WORLD_MAP_KEY_HEADING_NONE) {
    collision |= PLAYER_STEP_STOPPED;
  } else {
    collision = 0;
    SET_POSITION(&position, actor->x.fixed, actor->y.fixed, actor->z.fixed);
    Vector_AddPolarOffset(WORLD_MAP_PROBE_RADIUS, direction, &position);
    if (gDebugMode == 0 || ((gInput.held & KEY_L) == 0 &&
                            GameFlag_Get(FLAG_DEBUG_STEPS_OFF) == 0)) {
      SET_POSITION(&leftPosition, actor->x.fixed, actor->y.fixed,
                   actor->z.fixed);
      Vector_AddPolarOffset(WORLD_MAP_PROBE_RADIUS, direction + DEGtoANG(30),
                            &leftPosition);
      SET_POSITION(&rightPosition, actor->x.fixed, actor->y.fixed,
                   actor->z.fixed);
      Vector_AddPolarOffset(WORLD_MAP_PROBE_RADIUS, direction - DEGtoANG(30),
                            &rightPosition);
      if ((WorldMap_IsWalkable(actor, &position) |
           WorldMap_IsWalkable(actor, &leftPosition) |
           WorldMap_IsWalkable(actor, &rightPosition)) != 0) {
        angleOffsets[count++] = direction + FIELD_PROBE_ANGLE;
        angleOffsets[count++] = direction - FIELD_PROBE_ANGLE;
        angleOffsets[count++] = direction + DEGtoANG(45);
        angleOffsets[count++] = direction - DEGtoANG(45);
        angleOffsets[count++] = direction + FIELD_PROBE_ANGLE * 3;
        angleOffsets[count++] = direction - FIELD_PROBE_ANGLE * 3;
        for (angleIndex = 0; angleIndex < count; angleIndex++) {
          direction = angleOffsets[angleIndex];
          SET_POSITION(&position, actor->x.fixed, actor->y.fixed,
                       actor->z.fixed);
          Vector_AddPolarOffset(WORLD_MAP_PROBE_RADIUS, direction, &position);
          SET_POSITION(&leftPosition, actor->x.fixed, actor->y.fixed,
                       actor->z.fixed);
          Vector_AddPolarOffset(WORLD_MAP_PROBE_RADIUS,
                                direction + DEGtoANG(30), &leftPosition);
          SET_POSITION(&rightPosition, actor->x.fixed, actor->y.fixed,
                       actor->z.fixed);
          Vector_AddPolarOffset(WORLD_MAP_PROBE_RADIUS,
                                direction - DEGtoANG(30), &rightPosition);
          if ((WorldMap_IsWalkable(actor, &position) |
               WorldMap_IsWalkable(actor, &leftPosition) |
               WorldMap_IsWalkable(actor, &rightPosition)) != 0)
            continue;
          angle = (s16)direction;
          goto update_object;
        }
        collision |= PLAYER_STEP_BLOCKED_BY_MAP;
      }
    }
  }

update_object:
  state = Ram_HeapSlots->fieldState;
  if (state != NULL) {
    if ((collision & PLAYER_STEP_BLOCKED) != 0)
      state->blockedSteps++;
    else
      state->blockedSteps = 0;
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
    actor->velocityX = collision;
    actor->velocityZ = collision;
    Vector_AddPolarOffset(speed, (u16)angle, (struct Point *)&actor->velocityX);
    if (actor->timer != 0)
      actor->timer--;
  }

  camera = (struct WorldMapView *)Ram_HeapSlots->mapGrid;
  targetCameraYaw = gWorldMap_KeyYaws[DPAD_INDEX(gInput.held)];
  yawStep = (s16)(targetCameraYaw - camera->yaw) / 8;
  if (yawStep > WORLD_MAP_YAW_STEP_MAX)
    yawStep = WORLD_MAP_YAW_STEP_MAX;
  if (yawStep < -WORLD_MAP_YAW_STEP_MAX)
    yawStep = -WORLD_MAP_YAW_STEP_MAX;
  if (yawStep > -WORLD_MAP_YAW_SNAP && yawStep < WORLD_MAP_YAW_SNAP)
    yawStep = targetCameraYaw - camera->yaw;
  camera->yaw += yawStep;

  if (actor->spriteStorageKind == ACTOR_SPRITE_ONE) {
    playerSprite = actor->sprite;
    terrainCollision =
        WorldMap_GetCollision((struct WorldPosition *)&actor->x.fixed);
    if (terrainCollision == WORLD_MAP_COLLISION_FOREST) {
      playerSprite->secondLayer->priority = 1;
      playerSprite->flags = 0;
    } else {
      playerSprite->secondLayer->priority = SPRITE_LAYER_NOT_DRAWN;
      playerSprite->flags = SPRITE_FLAG_SHADOW;
    }
    if (terrainCollision == WORLD_MAP_COLLISION_SHALLOW_WATER &&
        actor->timer == 0 && collision == 0) {
      ripple = Actor_Create(SPRITE_SPLASH_RING, actor->x.fixed, actor->y.fixed,
                            actor->z.fixed);
      if (ripple != 0) {
        rippleSprite = ripple->sprite;
        Actor_StartScript(ripple, gActor_RippleScript);
        ripple->motionFlags = collision;
        ripple->layer = 1;
        if (rippleSprite != 0) {
          Sprite_StartAnimation(rippleSprite, 1);
          rippleSprite->flags = collision;
          rippleSprite->objectMode = OBJ_MODE_SEMI_TRANSPARENT;
          rippleSprite->priority = 2;
        }
        actor->timer = 10;
      }
    }
  }
  Field_CheckConfiguredKeys();
  if (gInput.pressed & gMainRoutineState.aKeys)
    gFieldState->raisedTrigger = WORLD_MAP_RAISED_BOARD_SHIP;
  actor->scriptCursor++;
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

/// SC_SAIL (LEADER_MOVEMENT_SHIP_ON_MAP): the ship moved by the direction pad
/// on a field map, as the party's actor (gMainRoutineState.ownerOverride), as
/// at Lemuria's entrance; A leaves the ship.
s32 Actor_SailOnMap(struct Actor *interpreter) {
  struct Actor *actor = interpreter;

  struct Point position;
  struct Point testPosition;
  struct Point contact;
  s16 angleOffsets[22];
  s32 collision;
  s32 pushed;
  s16 heading;
  u16 angle;
  u16 direction;
  s32 angleIndex;
  s32 headingCount;
  s32 onPlatform;
  s32 difference;
  struct Actor *other;
  struct FieldState *state;

  collision = 0;
  pushed = 0;
  headingCount = 0;
  // Debug: holding L counts the free sprite VRAM blocks, cache entries, actors
  // and sprites.
  // DEAD: nothing reads the four counts, so the tests are gone; the ROM keeps
  // each loop's count and its branch (512, 96, 64 and 64 passes).
  if (gDebugMode != 0) {
    if (gInput.held & KEY_L) {
      s32 freeBlocks = 0;
      s32 freeEntries = 0;
      s32 freeActors = 0;
      s32 freeSprites = 0;
      s32 i;

      for (i = 0; i < VRAM_BLOCK_COUNT; i++)
        if (gVramCacheBlockOwners[i] == VRAM_BLOCK_OWNER_FREE)
          freeBlocks++;
      for (i = 0; i < VRAM_CACHE_ENTRY_COUNT; i++)
        if (gVramCacheEntries[i].offset == VRAM_CACHE_OFFSET_FREE)
          freeEntries++;
      for (i = 0; i < ACTOR_COUNT; i++)
        if (((struct Actor *)Ram_HeapSlots->actors)[i].script == NULL)
          freeActors++;
      for (i = 0; i < ANIM_OBJECT_COUNT; i++)
        if (((struct Sprite *)Ram_HeapSlots->sprites)[i].width == 0)
          freeSprites++;
    }
  }
  actor->speedLimit = FIELD_WALK_SPEED;
  actor->acceleration = FIELD_ACCELERATION;
  if (GameFlag_Get(FLAG_FAST_WALK) != 0 && (gInput.held & KEY_B)) {
    actor->speedLimit = FIELD_FAST_WALK_SPEED;
    actor->acceleration = FIELD_FAST_WALK_ACCELERATION;
  }

  angle = gWorldMap_KeyHeadings[DPAD_INDEX(gInput.held)];
  if ((u16)angle == WORLD_MAP_KEY_HEADING_NONE) {
    collision |= PLAYER_STEP_STOPPED;
    goto counted;
  }
  collision = 0;
  SET_POSITION(&position, actor->x.fixed, actor->y.fixed, actor->z.fixed);
  Vector_AddPolarOffset(toFIXED(19), (u16)angle, &position);
  if (actor->priorityFlags & ACTOR_PRIORITY_WALKS_ON_PLATFORMS) {
    onPlatform = 0;
    other = (struct Actor *)Ram_HeapSlots->actors;
    for (angleIndex = 0; angleIndex < ACTOR_COUNT; angleIndex++, other++) {
      FIXED dy;

      if (other->script == NULL)
        continue;
      if (!(other->priorityFlags & ACTOR_PRIORITY_PLATFORM))
        continue;
      if (other == actor)
        continue;
      dy = actor->y.fixed - other->y.fixed;
      if ((dy - toFIXED(16) < 0 ? -(dy - toFIXED(16)) : dy - toFIXED(16)) >
          toFIXED(8) - 1)
        continue;
      if (SAME_CELL(actor->x.fixed, other->x.fixed) &&
          SAME_CELL(actor->z.fixed, other->z.fixed))
        onPlatform = 1;
      if (SAME_CELL(position.x, other->x.fixed) &&
          SAME_CELL(position.z, other->z.fixed)) {
        collision = 0;
        heading = angle;
        goto counted;
      }
    }
    if (onPlatform && !MapGrid_CheckCollision(actor, &position)) {
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
  if (!MapGrid_CheckCollision(actor, &position) &&
      SIDE_PROBES_CLEAR(MapGrid_CheckCollision, actor, toFIXED(19), (u16)angle,
                        &testPosition)) {
    heading = angle;
    goto push_objects;
  }
  direction = angle;
  angleOffsets[headingCount++] = direction + FIELD_PROBE_ANGLE;
  angleOffsets[headingCount++] = direction - FIELD_PROBE_ANGLE;
  angleOffsets[headingCount++] = direction + DEGtoANG(45);
  angleOffsets[headingCount++] = direction - DEGtoANG(45);
  angleOffsets[headingCount++] = direction + FIELD_PROBE_ANGLE * 3;
  angleOffsets[headingCount++] = direction - FIELD_PROBE_ANGLE * 3;
  for (angleIndex = 0; angleIndex < headingCount; angleIndex++) {
    heading = angleOffsets[angleIndex];
    if (PROBE_CLEAR(MapGrid_CheckCollision, actor, toFIXED(19), (u16)heading,
                    &position) &&
        SIDE_PROBES_CLEAR(MapGrid_CheckCollision, actor, toFIXED(19),
                          (u16)heading, &testPosition))
      goto push_objects;
  }
  SET_POSITION(&position, actor->x.fixed, actor->y.fixed, actor->z.fixed);
  collision |= PLAYER_STEP_BLOCKED_BY_MAP;

push_objects:
  SET_POSITION(&contact, actor->x.fixed, actor->y.fixed, actor->z.fixed);
  Vector_AddPolarOffset(toFIXED(15), (u16)heading, &contact);
  other = (struct Actor *)Ram_HeapSlots->actors;
  for (angleIndex = 0; angleIndex < ACTOR_COUNT; angleIndex++, other++) {
    s32 radius;

    radius = actor->radius - 2;
    if (other->script == NULL)
      continue;
    if (!(other->collisionFlags & ACTOR_SOLID))
      continue;
    if (other == actor)
      continue;
    if (Field_CheckSpheresOverlap(&other->x.fixed, radius, &contact.x,
                                  other->radius - 2) < 0)
      continue;
    if ((other->collisionFlags & ACTOR_PUSHABLE) && other->paused == 0) {
      direction = Trig_Atan(other->z.fixed - actor->z.fixed,
                            other->x.fixed - actor->x.fixed);
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
                                    (struct Point *)&other->x.fixed);
              other->targetX = ACTOR_NO_TARGET;
              other->targetY = ACTOR_NO_TARGET;
              other->targetZ = ACTOR_NO_TARGET;
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
    actor->speedLimit = toFIXED(1) / 4;
    actor->acceleration = toFIXED(1) / 8;
  }

counted:
  state = Ram_HeapSlots->fieldState;
  if (state != NULL) {
    if (collision & PLAYER_STEP_BLOCKED)
      state->blockedSteps++;
    else
      state->blockedSteps = 0;
    state->heldKeys = gInput.held;
  }
  if (pushed != 0 || collision != 0)
    Actor_SelectAnimation(actor, ANIM_STAND);
  else
    Actor_SelectAnimation(actor, ANIM_WALK);

  if (gInput.pressed & gMainRoutineState.aKeys) {
    gFieldState->raisedTrigger = WORLD_MAP_RAISED_LEAVE_SHIP;
    collision |= PLAYER_STEP_STOPPED;
  }

  if (collision != 0) {
    actor->targetX = ACTOR_NO_TARGET;
    actor->targetY = ACTOR_NO_TARGET;
    actor->targetZ = ACTOR_NO_TARGET;
    actor->velocityX = 0;
    actor->velocityZ = 0;
    if (collision & PLAYER_STEP_BLOCKED) {
      difference = (s16)((u16)angle - actor->facing);
      if (difference > FIELD_BLOCKED_TURN_MAX)
        difference = SHIP_TURN_STEP;
      if (difference < -FIELD_BLOCKED_TURN_MAX)
        difference = -SHIP_TURN_STEP;
      actor->facing += difference;
    }
    actor->timer = 0;
    actor->userValue = FOOTPRINT_BOTH_FEET;
  } else {
    FIXED speed;

    Actor_SetMoveTarget(actor, position.x, position.y, position.z);
    speed = Math_SqrtFX(Iwram_MulFX(actor->velocityX, actor->velocityX) +
                        Iwram_MulFX(actor->velocityZ, actor->velocityZ));
    actor->velocityX = collision;
    actor->velocityZ = collision;
    Vector_AddPolarOffset(speed, (u16)heading,
                          (struct Point *)&actor->velocityX);
    if (actor->timer != 0)
      actor->timer--;
    actor->steeringFlags &= ~ACTOR_FACES_MOTION;
    difference = (s16)((u16)angle - actor->facing);
    if (difference > FIELD_BLOCKED_TURN_MAX)
      difference = SHIP_TURN_STEP;
    if (difference < -FIELD_BLOCKED_TURN_MAX)
      difference = -SHIP_TURN_STEP;
    actor->facing += difference;
  }
  if (actor->spriteStorageKind == ACTOR_SPRITE_ONE)
    WorldMap_GetCollision((struct WorldPosition *)&actor->x.fixed);
  Field_CheckConfiguredKeys();
  actor->scriptCursor++;
  return 1;
}

/// SC_WORLD_SAIL (LEADER_MOVEMENT_SHIP): the ship sailing the world map. It
/// keeps the kind of water under the ship in FieldState.terrainKind, sails at a
/// run and turns a little at a time, leaving ripples; A goes ashore, and a dash
/// key takes off once the ship has its wings (FieldState.hoverReady).
s32 Actor_SailOnWorldMap(struct Actor *actor) {
  struct Point position;
  struct Point testPosition;
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
  struct FieldState *state;
  struct FieldState *worldState;
  u32 kind;

  count = 0;
  terrain = WorldMap_GetCollision((struct WorldPosition *)&actor->x.fixed);
  worldState = Ram_HeapSlots->fieldState;
  if (terrain >= WORLD_MAP_COLLISION_SEA_FIRST &&
      terrain < WORLD_MAP_COLLISION_SEA_LAST)
    worldState->terrainKind = SHIP_TERRAIN_SEA;
  if (terrain >= WORLD_MAP_COLLISION_WALKABLE_FIRST &&
      terrain <= WORLD_MAP_COLLISION_SHALLOW_WATER)
    worldState->terrainKind = SHIP_TERRAIN_SHALLOWS;
  if (terrain == WORLD_MAP_COLLISION_SEA_LAST)
    worldState->terrainKind = SHIP_TERRAIN_SEA_LAST;
  if (terrain >= WORLD_MAP_COLLISION_DEFAULT &&
      terrain <= WORLD_MAP_COLLISION_WALKABLE_LAST)
    worldState->terrainKind = SHIP_TERRAIN_GROUND;

  collision = 0;
  actor->speedLimit = toFIXED(5) / 4;
  actor->acceleration = toFIXED(1) / 2;
  angle = gWorldMap_KeyHeadings[DPAD_INDEX(gInput.held)];
  direction = angle;
  if (direction == WORLD_MAP_KEY_HEADING_NONE) {
    collision |= PLAYER_STEP_STOPPED;
  } else {
    SET_POSITION(&position, actor->x.fixed, actor->y.fixed, actor->z.fixed);
    Vector_AddPolarOffset(WORLD_MAP_PROBE_RADIUS, direction, &position);
    if (gDebugMode == 0 || (gInput.held & KEY_L) == 0) {
      if (WorldMap_IsSailable(actor, &position) == 0 &&
          SIDE_PROBES_CLEAR(WorldMap_IsSailable, actor, WORLD_MAP_PROBE_RADIUS,
                            direction, &testPosition))
        goto update_object;
      angleOffsets[count++] = direction + FIELD_PROBE_ANGLE;
      angleOffsets[count++] = direction - FIELD_PROBE_ANGLE;
      angleOffsets[count++] = direction + DEGtoANG(45);
      angleOffsets[count++] = direction - DEGtoANG(45);
      angleOffsets[count++] = direction + FIELD_PROBE_ANGLE * 3;
      angleOffsets[count++] = direction - FIELD_PROBE_ANGLE * 3;
      for (angleIndex = 0; angleIndex < count; angleIndex++) {
        direction = angleOffsets[angleIndex];
        if (PROBE_CLEAR(WorldMap_IsSailable, actor, WORLD_MAP_PROBE_RADIUS,
                        direction, &position) &&
            SIDE_PROBES_CLEAR(WorldMap_IsSailable, actor,
                              WORLD_MAP_PROBE_RADIUS, direction,
                              &testPosition)) {
          angle = (s16)direction;
          goto update_object;
        }
      }
      collision |= PLAYER_STEP_BLOCKED_BY_MAP;
    }
  }

update_object:
  state = Ram_HeapSlots->fieldState;
  if (state != NULL) {
    if ((collision & PLAYER_STEP_BLOCKED) != 0)
      state->blockedSteps++;
    else
      state->blockedSteps = 0;
    state->heldKeys = gInput.held;
  }
  if (collision != 0)
    Actor_SelectAnimation(actor, ANIM_STAND);
  else
    Actor_SelectAnimation(actor, ANIM_WALK);

  if (gInput.pressed & gMainRoutineState.aKeys) {
    gFieldState->raisedTrigger = WORLD_MAP_RAISED_LEAVE_SHIP;
    collision |= PLAYER_STEP_STOPPED;
  }
  if (gInput.pressed & gMainRoutineState.dashKeys) {
    struct FieldState *fieldState = Ram_HeapSlots->fieldState;
    if (fieldState->hoverReady != 0) {
      GameFlag_Clear(FLAG_SHIP_HOVERING);
      ((struct FieldState *)fieldState)->raisedTrigger =
          WORLD_MAP_RAISED_TAKE_OFF;
    }
  }

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
    actor->velocityX = collision;
    actor->velocityZ = collision;
    direction = angle;
    Vector_AddPolarOffset(speed, direction, (struct Point *)&actor->velocityX);
    if (actor->timer != 0)
      actor->timer--;
    actor->steeringFlags &= ~ACTOR_FACES_MOTION;
    difference = (s16)(direction - actor->facing);
    if (difference > FIELD_BLOCKED_TURN_MAX)
      difference = SHIP_TURN_STEP;
    if (difference < -FIELD_BLOCKED_TURN_MAX)
      difference = -SHIP_TURN_STEP;
    actor->facing += difference;
  }

  camera = (struct WorldMapView *)Ram_HeapSlots->mapGrid;
  targetCameraYaw = gWorldMap_KeyYaws[DPAD_INDEX(gInput.held)];
  yawStep = (s16)(targetCameraYaw - camera->yaw) / 8;
  if (yawStep > WORLD_MAP_YAW_STEP_MAX)
    yawStep = WORLD_MAP_YAW_STEP_MAX;
  if (yawStep < -WORLD_MAP_YAW_STEP_MAX)
    yawStep = -WORLD_MAP_YAW_STEP_MAX;
  if (yawStep > -WORLD_MAP_YAW_SNAP && yawStep < WORLD_MAP_YAW_SNAP)
    yawStep = targetCameraYaw - camera->yaw;
  camera->yaw += yawStep;

  kind = actor->spriteStorageKind;
  if (kind == ACTOR_SPRITE_ONE) {
    WorldMap_GetCollision((struct WorldPosition *)&actor->x.fixed);
    if (actor->timer == 0 && collision == 0) {
      ripple = Actor_Create(SPRITE_SPLASH_RING, actor->x.fixed, actor->y.fixed,
                            actor->z.fixed);
      if (ripple != 0) {
        rippleSprite = ripple->sprite;
        Actor_StartScript(ripple, gActor_RippleScript);
        ripple->motionFlags = 0;
        ripple->layer = kind;
        if (rippleSprite != 0) {
          Sprite_StartAnimation(rippleSprite, 2);
          rippleSprite->flags = 0;
          rippleSprite->objectMode = OBJ_MODE_NORMAL;
          rippleSprite->priority = 2;
        }
        actor->timer = 10;
      }
    }
  }
  Field_CheckConfiguredKeys();
  actor->scriptCursor++;
  return 1;
}

/// SC_WORLD_FLY (LEADER_MOVEMENT_FLYING_SHIP): the ship with its wings on the
/// world map, flying over the ground the sea alone would stop
/// (WorldMap_IsFlyable). A or B climbs, up to 12 pixels, and the ship sinks
/// otherwise, faster once the party's PP is spent; coming down after a climb
/// lands it.
s32 Actor_FlyOnWorldMap(struct Actor *actor) {
  struct Point position;
  struct Point testPosition;
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
  struct FieldState *state;
  struct FieldState *worldState;
  struct Actor *other;
  u32 kind;
  struct FieldState *flightState;

  count = 0;
  terrain = WorldMap_GetCollision((struct WorldPosition *)&actor->x.fixed);
  worldState = Ram_HeapSlots->fieldState;
  if (terrain >= WORLD_MAP_COLLISION_SEA_FIRST &&
      terrain < WORLD_MAP_COLLISION_SEA_LAST)
    worldState->terrainKind = SHIP_TERRAIN_SEA;
  if (terrain >= WORLD_MAP_COLLISION_WALKABLE_FIRST &&
      terrain <= WORLD_MAP_COLLISION_SHALLOW_WATER)
    worldState->terrainKind = SHIP_TERRAIN_SHALLOWS;
  if (terrain == WORLD_MAP_COLLISION_SEA_LAST)
    worldState->terrainKind = SHIP_TERRAIN_SEA_LAST;
  if (terrain >= WORLD_MAP_COLLISION_DEFAULT &&
      terrain <= WORLD_MAP_COLLISION_WALKABLE_LAST)
    worldState->terrainKind = SHIP_TERRAIN_GROUND;

  actor->motionFlags = 0;
  collision = 0;
  actor->speedLimit = actor->y.fixed / 16 + toFIXED(1.25);
  actor->acceleration = toFIXED(1) / 2;
  angle = gWorldMap_KeyHeadings[DPAD_INDEX(gInput.held)];
  direction = angle;
  if (direction == WORLD_MAP_KEY_HEADING_NONE) {
    collision |= PLAYER_STEP_STOPPED;
  } else {
    SET_POSITION(&position, actor->x.fixed, actor->y.fixed, actor->z.fixed);
    Vector_AddPolarOffset(WORLD_MAP_PROBE_RADIUS, direction, &position);
    if (gDebugMode == 0 || (gInput.held & KEY_L) == 0) {
      if (WorldMap_IsFlyable(actor, &position) == 0 &&
          SIDE_PROBES_CLEAR(WorldMap_IsFlyable, actor, WORLD_MAP_PROBE_RADIUS,
                            direction, &testPosition))
        goto check_actors;
      angleOffsets[count++] = direction + FIELD_PROBE_ANGLE;
      angleOffsets[count++] = direction - FIELD_PROBE_ANGLE;
      angleOffsets[count++] = direction + DEGtoANG(45);
      angleOffsets[count++] = direction - DEGtoANG(45);
      angleOffsets[count++] = direction + FIELD_PROBE_ANGLE * 3;
      angleOffsets[count++] = direction - FIELD_PROBE_ANGLE * 3;
      for (angleIndex = 0; angleIndex < count; angleIndex++) {
        direction = angleOffsets[angleIndex];
        if (PROBE_CLEAR(WorldMap_IsFlyable, actor, WORLD_MAP_PROBE_RADIUS,
                        direction, &position) &&
            SIDE_PROBES_CLEAR(WorldMap_IsFlyable, actor, WORLD_MAP_PROBE_RADIUS,
                              direction, &testPosition)) {
          angle = (s16)direction;
          goto check_actors;
        }
      }
      collision |= PLAYER_STEP_BLOCKED_BY_MAP;

    check_actors:
      other = Ram_HeapSlots->actors;
      for (angleIndex = 0; angleIndex < ACTOR_COUNT; angleIndex++, other++) {
        if (other->script != NULL && (other->collisionFlags & ACTOR_SOLID) &&
            other != actor &&
            Field_CheckSpheresOverlap(&other->x.fixed, other->radius,
                                      &position.x, actor->radius) >= 0)
          collision |= PLAYER_STEP_BLOCKED_BY_ACTOR;
      }
    }
  }
  state = Ram_HeapSlots->fieldState;
  if (state != NULL) {
    if ((collision & PLAYER_STEP_BLOCKED) != 0)
      state->blockedSteps++;
    else
      state->blockedSteps = 0;
    state->heldKeys = gInput.held;
  }
  if (actor->y.fixed >= toFIXED(4))
    Actor_SelectAnimation(actor, ANIM_SHIP_FLY_HIGH);
  else if (collision != 0)
    Actor_SelectAnimation(actor, ANIM_STAND);
  else
    Actor_SelectAnimation(actor, ANIM_WALK);

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
    actor->velocityX = collision;
    actor->velocityZ = collision;
    direction = angle;
    Vector_AddPolarOffset(speed, direction, (struct Point *)&actor->velocityX);
    if (actor->timer != 0)
      actor->timer--;
    actor->steeringFlags &= ~ACTOR_FACES_MOTION;
    difference = (s16)(direction - actor->facing);
    if (difference > FIELD_BLOCKED_TURN_MAX)
      difference = SHIP_TURN_STEP;
    if (difference < -FIELD_BLOCKED_TURN_MAX)
      difference = -SHIP_TURN_STEP;
    actor->facing += difference;
  }

  camera = (struct WorldMapView *)Ram_HeapSlots->mapGrid;
  targetCameraYaw = Iwram_MulFX(gWorldMap_KeyYaws[DPAD_INDEX(gInput.held)],
                                actor->y.fixed / 4 + FIXED_ONE);
  yawStep = (s16)(targetCameraYaw - camera->yaw) / 8;
  if (yawStep > FLYING_YAW_STEP_MAX)
    yawStep = FLYING_YAW_STEP_MAX;
  if (yawStep < -FLYING_YAW_STEP_MAX)
    yawStep = -FLYING_YAW_STEP_MAX;
  if (yawStep >= -1 && yawStep <= 1)
    yawStep = targetCameraYaw - camera->yaw;
  camera->yaw += yawStep;

  kind = actor->spriteStorageKind;
  if (kind == ACTOR_SPRITE_ONE) {
    if (actor->y.fixed < toFIXED(4)) {
      if (actor->timer == 0 && collision == 0) {
        ripple =
            Actor_Create(SPRITE_SPLASH_RING, actor->x.fixed, 0, actor->z.fixed);
        if (ripple != 0) {
          rippleSprite = ripple->sprite;
          Actor_StartScript(ripple, gActor_RippleScript);
          ripple->motionFlags = 0;
          ripple->layer = kind;
          if (rippleSprite != 0) {
            Sprite_StartAnimation(rippleSprite, 1);
            rippleSprite->flags = 0;
            rippleSprite->objectMode = OBJ_MODE_SEMI_TRANSPARENT;
            rippleSprite->priority = 2;
          }
          actor->timer = 10;
        }
      }
    } else {
      Actor_SetSpriteFlags(actor, gFrameCount & kind);
    }
  }

  flightState = Ram_HeapSlots->fieldState;
  if (gInput.held & (gMainRoutineState.aKeys | gMainRoutineState.bKeys)) {
    flightState->climbing = 1;
    actor->y.fixed += toFIXED(1) / 8;
  } else {
    actor->y.fixed -= toFIXED(1) / 4;
  }
  if (Party_GetPpRatioFar() == 0)
    actor->y.fixed -= toFIXED(1) / 4;
  if (actor->y.fixed <= 0 && flightState->climbing != 0)
    ((struct FieldState *)flightState)->raisedTrigger = WORLD_MAP_RAISED_LAND;
  if (actor->y.fixed >= toFIXED(12))
    actor->y.fixed = toFIXED(12);
  if (actor->y.fixed <= 0)
    actor->y.fixed = 0;
  actor->scriptCursor++;
  return 1;
}
