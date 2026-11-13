/// @file
/// Script commands that move the leader by the pad, on the map and the world
/// map, and the configured keys checked from a script.
// Part of field.c.

#include "../field_internal.h"
#include "fixed_math.h"
#include "flag_call.h"
#include "module/field/field.h"
#include "module/field/object.h"
#include "macros.h"

/* VIEW(f0279b0): tag struct PlayerSprite as struct PlayerSprite__f0279b0;
 * resource_slot.c has another definition; tried with the definition the rest of
 * the object has, it does not compile in tla-ja:
 * games/tla/src/field/field.c:6062: structure has no member named `entry'. */
struct PlayerSprite__f0279b0 {
  u8 unk00[5];
  u8 low05 : 2;
  u8 objectMode : 2;
  u8 high05 : 4;
  u8 unk06[3];
  u8 low09 : 2;
  u8 priority : 2;
  u8 high09 : 4;
  u8 unk0a[0x10];
  u8 flags;
  u8 unk1b[0x11];
  u8 *entry;
};

// The heading of each direction-pad combination, -1 for none.
extern const s16 gWorldMap_KeyHeadings[16];

s32 WorldMap_IsWalkable__resource_slot(
    struct Actor *object,
    struct FieldPosition *position) __asm__("WorldMap_IsWalkable");
void Actor_SetMoveTarget(struct Actor *actor, s32 x, s32 y, s32 z);

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
  s32 speed;
  s32 targetCameraYaw;
  struct WorldCamera *camera;
  s32 yawStep;
  struct PlayerSprite__f0279b0 *playerSprite;
  struct Actor *ripple;
  struct PlayerSprite__f0279b0 *rippleSprite;
  struct EventWork *work;

  movementAnimation = 2;
  collision = 0;
  count = 0;
  if (gInput.held & ((struct PartyKeys *)&gMainRoutineWork)->dashKeys) {
    object->speedLimit = px(1);
    object->acceleration = px(1) / 2;
    movementAnimation = 5;
  } else {
    object->speedLimit = px(1) / 2;
    object->acceleration = px(1) / 4;
  }

  angle = gWorldMap_KeyHeadings[(gInput.held >> 4) & 15];
  direction = angle;
  if (direction == 0xffff) {
    collision |= 4;
  } else {
    collision = 0;
    SET_POSITION(&position, object->x, object->y, object->z);
    Vector_AddPolarOffset(px(8), direction, &position);
    if (gDebugMode == 0 ||
        ((gInput.held & KEY_L) == 0 && GameFlag_Get(0x163) == 0)) {
      SET_POSITION(&leftPosition, object->x, object->y, object->z);
      Vector_AddPolarOffset(px(8), direction + 0x1555, &leftPosition);
      SET_POSITION(&rightPosition, object->x, object->y, object->z);
      Vector_AddPolarOffset(px(8), direction - 0x1555, &rightPosition);
      if ((WorldMap_IsWalkable(object, &position) |
           WorldMap_IsWalkable(object, &leftPosition) |
           WorldMap_IsWalkable(object, &rightPosition)) != 0) {
        angleOffsets[count++] = direction + TRIG_TURN / 16;
        angleOffsets[count++] = direction - TRIG_TURN / 16;
        angleOffsets[count++] = direction + TRIG_TURN / 8;
        angleOffsets[count++] = direction - TRIG_TURN / 8;
        angleOffsets[count++] = direction + TRIG_TURN * 3 / 16;
        angleOffsets[count++] = direction - TRIG_TURN * 3 / 16;
        for (angleIndex = 0; angleIndex < count; angleIndex++) {
          direction = angleOffsets[angleIndex];
          SET_POSITION(&position, object->x, object->y, object->z);
          Vector_AddPolarOffset(px(8), direction, &position);
          SET_POSITION(&leftPosition, object->x, object->y, object->z);
          Vector_AddPolarOffset(px(8), direction + 0x1555, &leftPosition);
          SET_POSITION(&rightPosition, object->x, object->y, object->z);
          Vector_AddPolarOffset(px(8), direction - 0x1555, &rightPosition);
          if ((WorldMap_IsWalkable(object, &position) |
               WorldMap_IsWalkable(object, &leftPosition) |
               WorldMap_IsWalkable(object, &rightPosition)) != 0)
            continue;
          angle = (s16)direction;
          goto update_object;
        }
        collision |= 1;
      }
    }
  }

update_object:
  work = Ram_HeapSlots->eventWork;
  if (work != NULL) {
    if ((collision & 3) != 0)
      work->blockedSteps++;
    else
      work->blockedSteps = 0;
  }
  if (collision != 0)
    Actor_SelectAnimation(object, 9);
  else
    Actor_SelectAnimation(object, movementAnimation);

  if (collision != 0) {
    object->targetX = TARGET_UNSET;
    object->targetY = TARGET_UNSET;
    object->targetZ = TARGET_UNSET;
    object->velocityX = 0;
    object->velocityZ = 0;
    if ((collision & 3) != 0) {
      direction = angle;
      difference = (s16)(direction - object->facing);
      if (difference > 0x1000)
        difference = 0x1000;
      if (difference < -0x1000)
        difference = -0x1000;
      object->facing += difference;
    }
    object->timer = 0;
    object->unk66 = 2;
  } else {
    Actor_SetMoveTarget(object, position.x, position.y, position.z);
    speed = FixedSqrt(Iwram_MulQ16(object->velocityX, object->velocityX) +
                      Iwram_MulQ16(object->velocityZ, object->velocityZ));
    object->velocityX = collision;
    object->velocityZ = collision;
    Vector_AddPolarOffset(speed, (u16)angle,
                          (struct FieldPosition *)&object->velocityX);
    if (object->timer != 0)
      object->timer--;
  }

  camera = (struct WorldCamera *)Ram_HeapSlots->mapWork;
  targetCameraYaw = gWorldMap_KeyYaws[(gInput.held >> 4) & 15];
  yawStep = (s16)(targetCameraYaw - camera->yaw) / 8;
  if (yawStep > 0x200)
    yawStep = 0x200;
  if (yawStep < -0x200)
    yawStep = -0x200;
  if (yawStep > -16 && yawStep < 16)
    yawStep = targetCameraYaw - camera->yaw;
  camera->yaw += yawStep;

  if (object->animationKind == 1) {
    playerSprite = object->sprite;
    terrainCollision =
        WorldMap_GetCollision((struct WorldPosition *)&object->x);
    if (terrainCollision == 9) {
      playerSprite->entry[6] = 1;
      playerSprite->flags = 0;
    } else {
      playerSprite->entry[6] = 9;
      playerSprite->flags = 1;
    }
    if (terrainCollision == 6 && object->timer == 0 && collision == 0) {
      ripple = Actor_Create(0x10d, object->x, object->y, object->z);
      if (ripple != 0) {
        rippleSprite = ripple->sprite;
        Actor_StartScript(ripple, gData_0802ec94);
        ripple->motionFlags = collision;
        ripple->terrainId = 1;
        if (rippleSprite != 0) {
          Animation_ApplyChildArgument(rippleSprite, 1);
          rippleSprite->flags = collision;
          rippleSprite->objectMode = 1;
          rippleSprite->priority = 2;
        }
        object->timer = 10;
      }
    }
  }
  Field_CheckConfiguredKeys();
  if (gInput.pressed & gMainRoutineWork.keys230)
    ((struct EventWork *)Ram_HeapSlots->eventWork)->unk16a = 251;
  object->step++;
  return 1;
}

/// SC_CMD_25: checks the configured keys (Field_CheckConfiguredKeys) and steps
/// the script on. Returns 1.
s32 Field_CheckConfiguredKeysAndCount(struct ActorScriptView *interpreter) {
  Field_CheckConfiguredKeys();
  interpreter->scriptCursor++;
  return 1;
}

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
  struct EventWork *work;

  collision = 0;
  pushed = 0;
  // Debug: holding L spins through four empty loops, a short stall each
  // frame.
  if (gDebugMode != 0) {
    s32 delayKeyMask;

    delayKeyMask = 0x200;
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
  object->speedLimit = px(1);
  object->acceleration = px(1) / 4;
  if (GameFlag_Get(0x17f) != 0 && (gInput.held & KEY_B)) {
    object->speedLimit = px(4);
    object->acceleration = px(1);
  }

  angle = gWorldMap_KeyHeadings[(gInput.held >> 4) & 15];
  if ((u16)angle == 0xffff) {
    collision |= 4;
    goto counted;
  }
  collision = 0;
  SET_POSITION(&position, object->x, object->y, object->z);
  Vector_AddPolarOffset(px(19), (u16)angle, &position);
  if (object->priorityFlags & 0x40) {
    onObject = 0;
    other = (struct Actor *)Ram_HeapSlots->scriptObjects;
    for (angleIndex = 0; angleIndex < 64; angleIndex++, other++) {
      s32 dy;

      if (other->script == NULL)
        continue;
      if (!(other->priorityFlags & 0x80))
        continue;
      if (other == object)
        continue;
      dy = object->y - other->y;
      if ((dy - 0x100000 < 0 ? -(dy - 0x100000) : dy - 0x100000) > 0x7ffff)
        continue;
      if (SAME_CELL(object->x, other->x) && SAME_CELL(object->z, other->z))
        onObject = 1;
      if (SAME_CELL(position.x, other->x) && SAME_CELL(position.z, other->z)) {
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
    SET_POSITION(&testPosition, object->x, object->y, object->z);
    Vector_AddPolarOffset(px(19), (u16)angle + TRIG_TURN / 16, &testPosition);
    if (!MapGrid_CheckCollision(object, &testPosition)) {
      SET_POSITION(&testPosition, object->x, object->y, object->z);
      Vector_AddPolarOffset(px(19), (u16)angle - TRIG_TURN / 16, &testPosition);
      if (!MapGrid_CheckCollision(object, &testPosition)) {
        SET_POSITION(&testPosition, object->x, object->y, object->z);
        Vector_AddPolarOffset(px(19), (u16)angle + TRIG_TURN / 8,
                              &testPosition);
        if (!MapGrid_CheckCollision(object, &testPosition)) {
          SET_POSITION(&testPosition, object->x, object->y, object->z);
          Vector_AddPolarOffset(px(19), (u16)angle - TRIG_TURN / 8,
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
  angleOffsets[headingCount++] = direction + TRIG_TURN / 16;
  angleOffsets[headingCount++] = direction - TRIG_TURN / 16;
  angleOffsets[headingCount++] = direction + TRIG_TURN / 8;
  angleOffsets[headingCount++] = direction - TRIG_TURN / 8;
  angleOffsets[headingCount++] = direction + TRIG_TURN * 3 / 16;
  angleOffsets[headingCount++] = direction - TRIG_TURN * 3 / 16;
  /* FAKEMATCH(dead): an empty once-only loop. */
  do {
  } while (0);
  for (angleIndex = 0; angleIndex < headingCount; angleIndex++) {
    heading = angleOffsets[angleIndex];
    SET_POSITION(&position, object->x, object->y, object->z);
    Vector_AddPolarOffset(px(19), (u16)heading, &position);
    if (MapGrid_CheckCollision(object, &position))
      continue;
    SET_POSITION(&testPosition, object->x, object->y, object->z);
    Vector_AddPolarOffset(px(19), (u16)heading + TRIG_TURN / 16, &testPosition);
    if (MapGrid_CheckCollision(object, &testPosition))
      continue;
    SET_POSITION(&testPosition, object->x, object->y, object->z);
    Vector_AddPolarOffset(px(19), (u16)heading - TRIG_TURN / 16, &testPosition);
    if (MapGrid_CheckCollision(object, &testPosition))
      continue;
    SET_POSITION(&testPosition, object->x, object->y, object->z);
    Vector_AddPolarOffset(px(19), (u16)heading + TRIG_TURN / 8, &testPosition);
    if (MapGrid_CheckCollision(object, &testPosition))
      continue;
    SET_POSITION(&testPosition, object->x, object->y, object->z);
    Vector_AddPolarOffset(px(19), (u16)heading - TRIG_TURN / 8, &testPosition);
    if (!MapGrid_CheckCollision(object, &testPosition))
      goto push_objects;
  }
  SET_POSITION(&position, object->x, object->y, object->z);
  collision |= 1;

push_objects:
  SET_POSITION(&contact, object->x, object->y, object->z);
  Vector_AddPolarOffset(px(15), (u16)heading, &contact);
  other = (struct Actor *)Ram_HeapSlots->scriptObjects;
  for (angleIndex = 63; angleIndex >= 0; angleIndex--, other++) {
    s32 radius;

    radius = object->radius - 2;
    if (other->script == NULL)
      continue;
    if (!(other->collisionFlags & 1))
      continue;
    if (other == object)
      continue;
    if (Field_CheckSpheresOverlap(&other->x, radius, &contact.x,
                                  other->radius - 2) < 0)
      continue;
    if ((other->collisionFlags & 2) && other->movementState == 0) {
      direction = ArcTan2(other->z - object->z, other->x - object->x);
      SET_POSITION(&testPosition, other->x, other->y, other->z);
      heading = direction;
      Vector_AddPolarOffset(px(1) / 4, direction, &testPosition);
      if (!ActorScript_CheckOverlap(other, &testPosition.x)) {
        SET_POSITION(&testPosition, other->x, other->y, other->z);
        Vector_AddPolarOffset(px(21), direction, &testPosition);
        if (!MapGrid_CheckCollision(other, &testPosition)) {
          SET_POSITION(&testPosition, other->x, other->y, other->z);
          Vector_AddPolarOffset(px(21), direction + 0x1000, &testPosition);
          // The same probe is tested twice.
          if (!MapGrid_CheckCollision(other, &testPosition) &&
              !MapGrid_CheckCollision(other, &testPosition)) {
            SET_POSITION(&testPosition, other->x, other->y, other->z);
            Vector_AddPolarOffset(px(21), direction - 0x1000, &testPosition);
            if (!MapGrid_CheckCollision(other, &testPosition)) {
              Vector_AddPolarOffset(px(1) / 4, direction,
                                    (struct FieldPosition *)&other->x);
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
    collision |= 2;
  }
  if (collision == 0 && pushed != 0) {
    object->speedLimit = px(1) / 4;
    object->acceleration = px(1) / 8;
  }

counted:
  work = Ram_HeapSlots->eventWork;
  if (work != NULL) {
    if (collision & 3)
      work->blockedSteps++;
    else
      work->blockedSteps = 0;
    work->heldKeys = gInput.held;
  }
  if (pushed != 0 || collision != 0)
    Actor_SelectAnimation(object, 1);
  else
    Actor_SelectAnimation(object, 2);

  if (gInput.pressed & gMainRoutineWork.keys230) {
    ((struct EventWork *)Ram_HeapSlots->eventWork)->unk16a = 252;
    collision |= 4;
  }

  if (collision != 0) {
    object->targetX = TARGET_UNSET;
    object->targetY = TARGET_UNSET;
    object->targetZ = TARGET_UNSET;
    object->velocityX = 0;
    object->velocityZ = 0;
    if (collision & 3) {
      difference = (s16)((u16)angle - object->facing);
      if (difference > 0x1000)
        difference = 0x400;
      if (difference < -0x1000)
        difference = -0x400;
      object->facing += difference;
    }
    object->timer = 0;
    object->unk66 = 2;
  } else {
    s32 speed;

    Actor_SetMoveTarget(object, position.x, position.y, position.z);
    speed = FixedSqrt(Iwram_MulQ16(object->velocityX, object->velocityX) +
                      Iwram_MulQ16(object->velocityZ, object->velocityZ));
    object->velocityX = collision;
    object->velocityZ = collision;
    Vector_AddPolarOffset(speed, (u16)heading,
                          (struct FieldPosition *)&object->velocityX);
    if (object->timer != 0)
      object->timer--;
    object->actionFlags &= ~1;
    difference = (s16)((u16)angle - object->facing);
    if (difference > 0x1000)
      difference = 0x400;
    if (difference < -0x1000)
      difference = -0x400;
    object->facing += difference;
  }
  if (object->animationKind == 1)
    WorldMap_GetCollision((struct WorldPosition *)&object->x);
  Field_CheckConfiguredKeys();
  object->step++;
  return 1;
}
#undef SAME_CELL

#include "flag_call.h"
#include "module/field/field.h"
#include "module/field/object.h"
#include "module/field/resource_slot.h"

/* VIEW(s28534): tag struct PlayerSprite as struct PlayerSprite__s28534;
 * resource_slot.c has another definition; tried with the definition the rest of
 * the object has, it does not compile in tla-ja:
 * games/tla/src/field/field.c:6683: structure has no member named `objectMode'.
 */
struct PlayerSprite__s28534 {
  u8 unk00[5];
  u8 low05 : 2;
  u8 objectMode : 2;
  u8 high05 : 4;
  u8 unk06[3];
  u8 low09 : 2;
  u8 priority : 2;
  u8 high09 : 4;
  u8 unk0a[0x10];
  u8 flags;
};

/* VIEW(s28534): tag struct PartyKeys as struct PartyKeys__s28534;
 * resource_slot.c has another definition; tried with the definition the rest of
 * the object has, it does not compile in tla-ja:
 * games/tla/src/field/field.c:6622: structure has no member named `keys23c'. */
struct PartyKeys__s28534 {
  u8 unk000[0x23c];
  u16 keys23c;
};

// The heading of each direction-pad combination, -1 for none.
extern const s16 gWorldMap_KeyHeadings[16];

void Actor_SetMoveTarget(struct Actor *actor, s32 x, s32 y, s32 z);

/// Script command 0x20 (control mode 7): the ship sailing the world map, a
/// sibling of Actor_MoveOnWorldMap in The Broken Seal's
/// later Actor_MoveOnWorldMap layout (one probe ahead, then four probes 22.5
/// and 45 degrees off it; six retry headings). Records the kind of ground under
/// the object in the event work (byte 0xd3a), moves at a run, and lets the
/// shortcut keys post requests 252 and, while byte 0xd39 is set, clear flag
/// 0x147 and post 253; turns at most 0x400 a step while moving.
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
  s32 speed;
  s32 targetCameraYaw;
  struct WorldCamera *camera;
  s32 yawStep;
  struct Actor *ripple;
  struct PlayerSprite__s28534 *rippleSprite;
  struct EventWork *work;
  struct WorldEventWork *worldWork;
  u32 kind;

  count = 0;
  terrain = WorldMap_GetCollision((struct WorldPosition *)&object->x);
  worldWork = Ram_HeapSlots->eventWork;
  if (terrain >= 1 && terrain <= 3)
    worldWork->terrainKind = 0;
  if (terrain >= 5 && terrain <= 6)
    worldWork->terrainKind = 1;
  if (terrain == 4)
    worldWork->terrainKind = 2;
  if (terrain >= 7 && terrain <= 12)
    worldWork->terrainKind = 3;

  collision = 0;
  object->speedLimit = px(5) / 4;
  object->acceleration = px(1) / 2;
  angle = gWorldMap_KeyHeadings[(gInput.held >> 4) & 15];
  direction = angle;
  if (direction == 0xffff) {
    collision |= 4;
  } else {
    SET_POSITION(&position, object->x, object->y, object->z);
    Vector_AddPolarOffset(px(8), direction, &position);
    if (gDebugMode == 0 || (gInput.held & KEY_L) == 0) {
      if (WorldMap_IsSailable(object, &position) == 0) {
        SET_POSITION(&testPosition, object->x, object->y, object->z);
        Vector_AddPolarOffset(px(8), direction + 0x1000, &testPosition);
        if (WorldMap_IsSailable(object, &testPosition) == 0) {
          SET_POSITION(&testPosition, object->x, object->y, object->z);
          Vector_AddPolarOffset(px(8), direction - 0x1000, &testPosition);
          if (WorldMap_IsSailable(object, &testPosition) == 0) {
            SET_POSITION(&testPosition, object->x, object->y, object->z);
            Vector_AddPolarOffset(px(8), direction + 0x2000, &testPosition);
            if (WorldMap_IsSailable(object, &testPosition) == 0) {
              SET_POSITION(&testPosition, object->x, object->y, object->z);
              Vector_AddPolarOffset(px(8), direction - 0x2000, &testPosition);
              if (WorldMap_IsSailable(object, &testPosition) == 0)
                goto update_object;
            }
          }
        }
      }
      angleOffsets[count++] = direction + TRIG_TURN / 16;
      angleOffsets[count++] = direction - TRIG_TURN / 16;
      angleOffsets[count++] = direction + TRIG_TURN / 8;
      angleOffsets[count++] = direction - TRIG_TURN / 8;
      angleOffsets[count++] = direction + TRIG_TURN * 3 / 16;
      angleOffsets[count++] = direction - TRIG_TURN * 3 / 16;
      for (angleIndex = 0; angleIndex < count; angleIndex++) {
        direction = angleOffsets[angleIndex];
        SET_POSITION(&position, object->x, object->y, object->z);
        Vector_AddPolarOffset(px(8), direction, &position);
        if (WorldMap_IsSailable(object, &position) != 0)
          continue;
        SET_POSITION(&testPosition, object->x, object->y, object->z);
        Vector_AddPolarOffset(px(8), direction + 0x1000, &testPosition);
        if (WorldMap_IsSailable(object, &testPosition) != 0)
          continue;
        SET_POSITION(&testPosition, object->x, object->y, object->z);
        Vector_AddPolarOffset(px(8), direction - 0x1000, &testPosition);
        if (WorldMap_IsSailable(object, &testPosition) != 0)
          continue;
        SET_POSITION(&testPosition, object->x, object->y, object->z);
        Vector_AddPolarOffset(px(8), direction + 0x2000, &testPosition);
        if (WorldMap_IsSailable(object, &testPosition) != 0)
          continue;
        SET_POSITION(&testPosition, object->x, object->y, object->z);
        Vector_AddPolarOffset(px(8), direction - 0x2000, &testPosition);
        if (WorldMap_IsSailable(object, &testPosition) != 0)
          continue;
        angle = (s16)direction;
        goto update_object;
      }
      collision |= 1;
    }
  }

update_object:
  work = Ram_HeapSlots->eventWork;
  if (work != NULL) {
    if ((collision & 3) != 0)
      work->blockedSteps++;
    else
      work->blockedSteps = 0;
    work->heldKeys = gInput.held;
  }
  if (collision != 0)
    Actor_SelectAnimation(object, 1);
  else
    Actor_SelectAnimation(object, 2);

  if (gInput.pressed & gMainRoutineWork.keys230) {
    ((struct EventWork *)Ram_HeapSlots->eventWork)->unk16a = 252;
    collision |= 4;
  }
  if (gInput.pressed &
      ((struct PartyKeys__s28534 *)&gMainRoutineWork)->keys23c) {
    struct WorldEventWork *keyWork = Ram_HeapSlots->eventWork;
    if (keyWork->unkd39 != 0) {
      GameFlag_Clear(0x147);
      ((struct EventWork *)keyWork)->unk16a = 253;
    }
  }

  if (collision != 0) {
    object->targetX = TARGET_UNSET;
    object->targetY = TARGET_UNSET;
    object->targetZ = TARGET_UNSET;
    object->velocityX = 0;
    object->velocityZ = 0;
    if ((collision & 3) != 0) {
      direction = angle;
      difference = (s16)(direction - object->facing);
      if (difference > 0x1000)
        difference = 0x1000;
      if (difference < -0x1000)
        difference = -0x1000;
      object->facing += difference;
    }
    object->timer = 0;
    object->unk66 = 2;
  } else {
    Actor_SetMoveTarget(object, position.x, position.y, position.z);
    speed = FixedSqrt(Iwram_MulQ16(object->velocityX, object->velocityX) +
                      Iwram_MulQ16(object->velocityZ, object->velocityZ));
    object->velocityX = collision;
    object->velocityZ = collision;
    direction = angle;
    Vector_AddPolarOffset(speed, direction,
                          (struct FieldPosition *)&object->velocityX);
    if (object->timer != 0)
      object->timer--;
    object->actionFlags &= ~1;
    difference = (s16)(direction - object->facing);
    if (difference > 0x1000)
      difference = 0x400;
    if (difference < -0x1000)
      difference = -0x400;
    object->facing += difference;
  }

  camera = (struct WorldCamera *)Ram_HeapSlots->mapWork;
  targetCameraYaw = gWorldMap_KeyYaws[(gInput.held >> 4) & 15];
  yawStep = (s16)(targetCameraYaw - camera->yaw) / 8;
  if (yawStep > 0x200)
    yawStep = 0x200;
  if (yawStep < -0x200)
    yawStep = -0x200;
  if (yawStep > -16 && yawStep < 16)
    yawStep = targetCameraYaw - camera->yaw;
  camera->yaw += yawStep;

  kind = object->animationKind;
  if (kind == 1) {
    WorldMap_GetCollision((struct WorldPosition *)&object->x);
    if (object->timer == 0 && collision == 0) {
      ripple = Actor_Create(0x10d, object->x, object->y, object->z);
      if (ripple != 0) {
        rippleSprite = ripple->sprite;
        Actor_StartScript(ripple, gData_0802ec94);
        ripple->motionFlags = 0;
        ripple->terrainId = kind;
        if (rippleSprite != 0) {
          Animation_ApplyChildArgument(rippleSprite, 2);
          rippleSprite->flags = 0;
          rippleSprite->objectMode = 0;
          rippleSprite->priority = 2;
        }
        object->timer = 10;
      }
    }
  }
  Field_CheckConfiguredKeys();
  object->step++;
  return 1;
}

#include "module/field/field.h"
#include "module/field/object.h"
#include "module/field/resource_slot.h"

/* VIEW(f028aac): tag struct PlayerSprite as struct PlayerSprite__f028aac;
 * resource_slot.c has another definition; tried with the definition the rest of
 * the object has, it does not compile in tla-ja:
 * games/tla/src/field/field.c:6943: structure has no member named `objectMode'.
 */
struct PlayerSprite__f028aac {
  u8 unk00[5];
  u8 low05 : 2;
  u8 objectMode : 2;
  u8 high05 : 4;
  u8 unk06[3];
  u8 low09 : 2;
  u8 priority : 2;
  u8 high09 : 4;
  u8 unk0a[0x10];
  u8 flags;
};

/* VIEW(f028aac): tag struct WorldEventWork as struct WorldEventWork__f028aac;
 * resource_slot.c has another definition; tried with the definition the rest of
 * the object has, it does not compile in tla-ja:
 * games/tla/src/field/field.c:6963: structure has no member named `climbing'.
 */
struct WorldEventWork__f028aac {
  u8 unk000[0xd3a];
  u8 terrainKind;
  s8 climbing;
};

// The heading of each direction-pad combination, -1 for none.
extern const s16 gWorldMap_KeyHeadings[16];

void Actor_SetMoveTarget(struct Actor *actor, s32 x, s32 y, s32 z);

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
  s32 speed;
  s32 targetCameraYaw;
  struct WorldCamera *camera;
  s32 yawStep;
  struct Actor *ripple;
  struct PlayerSprite__f028aac *rippleSprite;
  struct EventWork *work;
  struct WorldEventWork__f028aac *worldWork;
  struct Actor *other;
  u32 kind;
  struct WorldEventWork__f028aac *flightWork;

  count = 0;
  terrain = WorldMap_GetCollision((struct WorldPosition *)&object->x);
  worldWork = Ram_HeapSlots->eventWork;
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
  object->speedLimit = object->y / 16 + 0x14000;
  object->acceleration = px(1) / 2;
  angle = gWorldMap_KeyHeadings[(gInput.held >> 4) & 15];
  direction = angle;
  if (direction == 0xffff) {
    collision |= 4;
    goto update_object;
  }
  SET_POSITION(&position, object->x, object->y, object->z);
  Vector_AddPolarOffset(px(8), direction, &position);
  if (gDebugMode != 0 && (gInput.held & KEY_L) != 0)
    goto update_object;
  if (WorldMap_IsFlyable(object, &position) == 0) {
    SET_POSITION(&testPosition, object->x, object->y, object->z);
    Vector_AddPolarOffset(px(8), direction + 0x1000, &testPosition);
    if (WorldMap_IsFlyable(object, &testPosition) == 0) {
      SET_POSITION(&testPosition, object->x, object->y, object->z);
      Vector_AddPolarOffset(px(8), direction - 0x1000, &testPosition);
      if (WorldMap_IsFlyable(object, &testPosition) == 0) {
        SET_POSITION(&testPosition, object->x, object->y, object->z);
        Vector_AddPolarOffset(px(8), direction + 0x2000, &testPosition);
        if (WorldMap_IsFlyable(object, &testPosition) == 0) {
          SET_POSITION(&testPosition, object->x, object->y, object->z);
          Vector_AddPolarOffset(px(8), direction - 0x2000, &testPosition);
          if (WorldMap_IsFlyable(object, &testPosition) == 0)
            goto check_objects;
        }
      }
    }
  }
  angleOffsets[count++] = direction + TRIG_TURN / 16;
  angleOffsets[count++] = direction - TRIG_TURN / 16;
  angleOffsets[count++] = direction + TRIG_TURN / 8;
  angleOffsets[count++] = direction - TRIG_TURN / 8;
  angleOffsets[count++] = direction + TRIG_TURN * 3 / 16;
  angleOffsets[count++] = direction - TRIG_TURN * 3 / 16;
  for (angleIndex = 0; angleIndex < count; angleIndex++) {
    direction = angleOffsets[angleIndex];
    SET_POSITION(&position, object->x, object->y, object->z);
    Vector_AddPolarOffset(px(8), direction, &position);
    if (WorldMap_IsFlyable(object, &position) != 0)
      continue;
    SET_POSITION(&testPosition, object->x, object->y, object->z);
    Vector_AddPolarOffset(px(8), direction + 0x1000, &testPosition);
    if (WorldMap_IsFlyable(object, &testPosition) != 0)
      continue;
    SET_POSITION(&testPosition, object->x, object->y, object->z);
    Vector_AddPolarOffset(px(8), direction - 0x1000, &testPosition);
    if (WorldMap_IsFlyable(object, &testPosition) != 0)
      continue;
    SET_POSITION(&testPosition, object->x, object->y, object->z);
    Vector_AddPolarOffset(px(8), direction + 0x2000, &testPosition);
    if (WorldMap_IsFlyable(object, &testPosition) != 0)
      continue;
    SET_POSITION(&testPosition, object->x, object->y, object->z);
    Vector_AddPolarOffset(px(8), direction - 0x2000, &testPosition);
    if (WorldMap_IsFlyable(object, &testPosition) != 0)
      continue;
    angle = (s16)direction;
    goto check_objects;
  }
  collision |= 1;

check_objects:
  other = Ram_HeapSlots->scriptObjects;
  for (angleIndex = 63; angleIndex >= 0; angleIndex -= 1, other++) {
    if (other->script != NULL && (other->collisionFlags & 1) &&
        other != object &&
        Field_CheckSpheresOverlap(&other->x, other->radius, &position.x,
                                  object->radius) >= 0)
      collision |= 2;
  }

update_object:
  work = Ram_HeapSlots->eventWork;
  if (work != NULL) {
    if ((collision & 3) != 0)
      work->blockedSteps++;
    else
      work->blockedSteps = 0;
    work->heldKeys = gInput.held;
  }
  if (object->y >= 0x40000)
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
    if ((collision & 3) != 0) {
      direction = angle;
      difference = (s16)(direction - object->facing);
      if (difference > 0x1000)
        difference = 0x1000;
      if (difference < -0x1000)
        difference = -0x1000;
      object->facing += difference;
    }
    object->timer = 0;
    object->unk66 = 2;
  } else {
    Actor_SetMoveTarget(object, position.x, position.y, position.z);
    speed = FixedSqrt(Iwram_MulQ16(object->velocityX, object->velocityX) +
                      Iwram_MulQ16(object->velocityZ, object->velocityZ));
    object->velocityX = collision;
    object->velocityZ = collision;
    direction = angle;
    Vector_AddPolarOffset(speed, direction,
                          (struct FieldPosition *)&object->velocityX);
    if (object->timer != 0)
      object->timer--;
    object->actionFlags &= ~1;
    difference = (s16)(direction - object->facing);
    if (difference > 0x1000)
      difference = 0x400;
    if (difference < -0x1000)
      difference = -0x400;
    object->facing += difference;
  }

  camera = (struct WorldCamera *)Ram_HeapSlots->mapWork;
  targetCameraYaw = Iwram_MulQ16(gWorldMap_KeyYaws[(gInput.held >> 4) & 15],
                                 object->y / 4 + 0x10000);
  yawStep = (s16)(targetCameraYaw - camera->yaw) / 8;
  if (yawStep > 32)
    yawStep = 32;
  if (yawStep < -32)
    yawStep = -32;
  if (yawStep >= -1 && yawStep <= 1)
    yawStep = targetCameraYaw - camera->yaw;
  camera->yaw += yawStep;

  kind = object->animationKind;
  if (kind == 1) {
    if (object->y < 0x40000) {
      if (object->timer == 0 && collision == 0) {
        ripple = Actor_Create(0x10d, object->x, 0, object->z);
        if (ripple != 0) {
          rippleSprite = ripple->sprite;
          Actor_StartScript(ripple, gData_0802ec94);
          ripple->motionFlags = 0;
          ripple->terrainId = kind;
          if (rippleSprite != 0) {
            Animation_ApplyChildArgument(rippleSprite, 1);
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

  flightWork = Ram_HeapSlots->eventWork;
  if (gInput.held & (gMainRoutineWork.keys230 | gMainRoutineWork.unk232)) {
    flightWork->climbing = 1;
    object->y += px(1) / 8;
  } else {
    object->y -= px(1) / 4;
  }
  if (Party_GetPpRatioFar() == 0)
    object->y -= px(1) / 4;
  if (object->y <= 0 && flightWork->climbing != 0)
    ((struct EventWork *)flightWork)->unk16a = 254;
  if (object->y >= 0xc0000)
    object->y = px(12);
  if (object->y <= 0)
    object->y = 0;
  object->step++;
  return 1;
}
