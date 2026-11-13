/// @file
/// Script commands that move the leader by the pad, and the configured keys
/// checked from a script.
// Part of resource_slot.c.

#include "../resource_slot_internal.h"

struct WorldCamera {
  u8 unk000[0x11e];
  u16 yaw;
};

/* VIEW(f0279b0): tag struct PlayerSprite as struct PlayerSprite__f0279b0; resource_slot.c has another definition; tried with the definition the rest of the object has, it does not compile in tla-ja: games/tla/src/field/resource_slot.c:5352: structure has no member named `entry'. */
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
extern const s16 gData_0802ec5c[16];
// The camera yaw each direction-pad combination turns toward.
extern const s32 gData_0802eca0[16];
extern const s32 gData_0802ec94[];

void Vector_AddPolarOffset(s32 radius, s32 angle,
                           struct FieldPosition *position);
s32 WorldMap_IsWalkable(struct Actor *object, struct FieldPosition *position);
s32 GameFlag_Test(s32 flag);
void Actor_SelectAnimation(struct Actor *object, s32 argument);
void Actor_SetMoveTarget(struct Actor *actor, s32 x, s32 y, s32 z);
s32 FixedSqrt(s32 value);
struct Actor *Func_08023220(s32 kind, s32 x, s32 y, s32 z);
void Actor_StartScript(struct Actor *, const s32 *);
/* VIEW(f0279b0): routine Animation_ApplyChildArgument as s32 Animation_ApplyChildArgument(struct PlayerSprite__f0279b0 *, s32); f027064 has s32 Animation_ApplyChildArgument(struct PlayerSprite *, s32). */
extern s32 Animation_ApplyChildArgument__f0279b0(struct PlayerSprite__f0279b0 *, s32) __asm__("Animation_ApplyChildArgument");
s32 Field_CheckConfiguredKeys(void);

s32 Func_080279b0(struct Actor *object) {
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
    object->speedLimit = 0x10000;
    object->acceleration = 0x8000;
    movementAnimation = 5;
  } else {
    object->speedLimit = 0x8000;
    object->acceleration = 0x4000;
  }

  angle = gData_0802ec5c[(gInput.held >> 4) & 15];
  direction = angle;
  if (direction == 0xffff) {
    collision |= 4;
  } else {
    collision = 0;
    position.x = object->x;
    position.y = object->y;
    position.z = object->z;
    Vector_AddPolarOffset(0x80000, direction, &position);
    if (gDebugMode == 0 ||
        ((gInput.held & 0x200) == 0 && GameFlag_Test(0x163) == 0)) {
      leftPosition.x = object->x;
      leftPosition.y = object->y;
      leftPosition.z = object->z;
      Vector_AddPolarOffset(0x80000, direction + 0x1555, &leftPosition);
      rightPosition.x = object->x;
      rightPosition.y = object->y;
      rightPosition.z = object->z;
      Vector_AddPolarOffset(0x80000, direction - 0x1555, &rightPosition);
      if ((WorldMap_IsWalkable(object, &position) |
           WorldMap_IsWalkable(object, &leftPosition) |
           WorldMap_IsWalkable(object, &rightPosition)) != 0) {
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
          Vector_AddPolarOffset(0x80000, direction, &position);
          leftPosition.x = object->x;
          leftPosition.y = object->y;
          leftPosition.z = object->z;
          Vector_AddPolarOffset(0x80000, direction + 0x1555, &leftPosition);
          rightPosition.x = object->x;
          rightPosition.y = object->y;
          rightPosition.z = object->z;
          Vector_AddPolarOffset(0x80000, direction - 0x1555, &rightPosition);
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
    object->targetX = 0x80000000;
    object->targetY = 0x80000000;
    object->targetZ = 0x80000000;
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
  targetCameraYaw = gData_0802eca0[(gInput.held >> 4) & 15];
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
    terrainCollision = GetWorldMapCollision((struct WorldPosition *)&object->x);
    if (terrainCollision == 9) {
      playerSprite->entry[6] = 1;
      playerSprite->flags = 0;
    } else {
      playerSprite->entry[6] = 9;
      playerSprite->flags = 1;
    }
    if (terrainCollision == 6 && object->timer == 0 && collision == 0) {
      ripple = Func_08023220(0x10d, object->x, object->y, object->z);
      if (ripple != 0) {
        rippleSprite = ripple->sprite;
        Actor_StartScript(ripple, gData_0802ec94);
        ripple->motionFlags = collision;
        ripple->terrainId = 1;
        if (rippleSprite != 0) {
          Animation_ApplyChildArgument__f0279b0(rippleSprite, 1);
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

s32 MapGrid_CheckCollision(struct Actor *object, struct FieldPosition *position);
s32 Field_CheckSpheresOverlap(s32 *a, s32 radiusA, s32 *b, s32 radiusB);
s32 ScriptObject_CheckOverlap(struct Actor *object, s32 *values);

// Two 16.16 coordinates fall in the same 16-pixel cell, rounding to nearest.
#define SAME_CELL(a, b) ((((a) - (b) + 0x80000) >> 20) == 0)

s32 Func_08027e20(struct Actor *interpreter) {
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
  object->speedLimit = 0x10000;
  object->acceleration = 0x4000;
  if (GameFlag_Test(0x17f) != 0 && (gInput.held & 2)) {
    object->speedLimit = 0x40000;
    object->acceleration = 0x10000;
  }

  angle = gData_0802ec5c[(gInput.held >> 4) & 15];
  if ((u16)angle == 0xffff) {
    collision |= 4;
    goto counted;
  }
  collision = 0;
  position.x = object->x;
  position.y = object->y;
  position.z = object->z;
  Vector_AddPolarOffset(0x130000, (u16)angle, &position);
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
  if (gDebugMode != 0 && (gInput.held & 0x200)) {
    collision = 0;
    heading = angle;
    goto counted;
  }
  if (!MapGrid_CheckCollision(object, &position)) {
    testPosition.x = object->x;
    testPosition.y = object->y;
    testPosition.z = object->z;
    Vector_AddPolarOffset(0x130000, (u16)angle + 0x1000, &testPosition);
    if (!MapGrid_CheckCollision(object, &testPosition)) {
      testPosition.x = object->x;
      testPosition.y = object->y;
      testPosition.z = object->z;
      Vector_AddPolarOffset(0x130000, (u16)angle - 0x1000, &testPosition);
      if (!MapGrid_CheckCollision(object, &testPosition)) {
        testPosition.x = object->x;
        testPosition.y = object->y;
        testPosition.z = object->z;
        Vector_AddPolarOffset(0x130000, (u16)angle + 0x2000, &testPosition);
        if (!MapGrid_CheckCollision(object, &testPosition)) {
          testPosition.x = object->x;
          testPosition.y = object->y;
          testPosition.z = object->z;
          Vector_AddPolarOffset(0x130000, (u16)angle - 0x2000, &testPosition);
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
  angleOffsets[headingCount++] = direction + 0x1000;
  angleOffsets[headingCount++] = direction - 0x1000;
  angleOffsets[headingCount++] = direction + 0x2000;
  angleOffsets[headingCount++] = direction - 0x2000;
  angleOffsets[headingCount++] = direction + 0x3000;
  angleOffsets[headingCount++] = direction - 0x3000;
  // An empty once-only loop keeps the last store before the counter and
  // places the first loop's exit block after this loop; without it, 243 of
  // 827 instructions differ in tla-en.
  do {
  } while (0);
  for (angleIndex = 0; angleIndex < headingCount; angleIndex++) {
    heading = angleOffsets[angleIndex];
    position.x = object->x;
    position.y = object->y;
    position.z = object->z;
    Vector_AddPolarOffset(0x130000, (u16)heading, &position);
    if (MapGrid_CheckCollision(object, &position))
      continue;
    testPosition.x = object->x;
    testPosition.y = object->y;
    testPosition.z = object->z;
    Vector_AddPolarOffset(0x130000, (u16)heading + 0x1000, &testPosition);
    if (MapGrid_CheckCollision(object, &testPosition))
      continue;
    testPosition.x = object->x;
    testPosition.y = object->y;
    testPosition.z = object->z;
    Vector_AddPolarOffset(0x130000, (u16)heading - 0x1000, &testPosition);
    if (MapGrid_CheckCollision(object, &testPosition))
      continue;
    testPosition.x = object->x;
    testPosition.y = object->y;
    testPosition.z = object->z;
    Vector_AddPolarOffset(0x130000, (u16)heading + 0x2000, &testPosition);
    if (MapGrid_CheckCollision(object, &testPosition))
      continue;
    testPosition.x = object->x;
    testPosition.y = object->y;
    testPosition.z = object->z;
    Vector_AddPolarOffset(0x130000, (u16)heading - 0x2000, &testPosition);
    if (!MapGrid_CheckCollision(object, &testPosition))
      goto push_objects;
  }
  position.x = object->x;
  position.y = object->y;
  position.z = object->z;
  collision |= 1;

push_objects:
  contact.x = object->x;
  contact.y = object->y;
  contact.z = object->z;
  Vector_AddPolarOffset(0xf0000, (u16)heading, &contact);
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
      testPosition.x = other->x;
      testPosition.y = other->y;
      testPosition.z = other->z;
      heading = direction;
      Vector_AddPolarOffset(0x4000, direction, &testPosition);
      if (!ScriptObject_CheckOverlap(other, &testPosition.x)) {
        testPosition.x = other->x;
        testPosition.y = other->y;
        testPosition.z = other->z;
        Vector_AddPolarOffset(0x150000, direction, &testPosition);
        if (!MapGrid_CheckCollision(other, &testPosition)) {
          testPosition.x = other->x;
          testPosition.y = other->y;
          testPosition.z = other->z;
          Vector_AddPolarOffset(0x150000, direction + 0x1000, &testPosition);
          // The same probe is tested twice.
          if (!MapGrid_CheckCollision(other, &testPosition) &&
              !MapGrid_CheckCollision(other, &testPosition)) {
            testPosition.x = other->x;
            testPosition.y = other->y;
            testPosition.z = other->z;
            Vector_AddPolarOffset(0x150000, direction - 0x1000, &testPosition);
            if (!MapGrid_CheckCollision(other, &testPosition)) {
              Vector_AddPolarOffset(0x4000, direction,
                                    (struct FieldPosition *)&other->x);
              other->targetX = 0x80000000;
              other->targetY = 0x80000000;
              other->targetZ = 0x80000000;
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
    object->speedLimit = 0x4000;
    object->acceleration = 0x2000;
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
    object->targetX = 0x80000000;
    object->targetY = 0x80000000;
    object->targetZ = 0x80000000;
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
    GetWorldMapCollision((struct WorldPosition *)&object->x);
  Field_CheckConfiguredKeys();
  object->step++;
  return 1;
}
#undef SAME_CELL
