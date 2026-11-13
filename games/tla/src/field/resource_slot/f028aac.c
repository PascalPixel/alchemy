/// @file
/// One script command not yet understood.
// Part of resource_slot.c.

#include "../resource_slot_internal.h"

/* VIEW(f028aac): tag struct PlayerSprite as struct PlayerSprite__f028aac; resource_slot.c has another definition; tried with the definition the rest of the object has, it does not compile in tla-ja: games/tla/src/field/resource_slot.c:6227: structure has no member named `objectMode'. */
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

/* VIEW(f028aac): tag struct WorldEventWork as struct WorldEventWork__f028aac; resource_slot.c has another definition; tried with the definition the rest of the object has, it does not compile in tla-ja: games/tla/src/field/resource_slot.c:6247: structure has no member named `climbing'. */
struct WorldEventWork__f028aac {
  u8 unk000[0xd3a];
  u8 terrainKind;
  s8 climbing;
};

// The heading of each direction-pad combination, -1 for none.
extern const s16 gData_0802ec5c[16];
// The camera yaw each direction-pad combination turns toward.
extern const s32 gData_0802eca0[16];
extern const s32 gData_0802ec94[];

void Vector_AddPolarOffset(s32 radius, s32 angle,
                           struct FieldPosition *position);
s32 Func_0802dbd0(struct Actor *object, struct FieldPosition *position);
s32 Field_CheckSpheresOverlap(s32 *a, s32 radius_a, s32 *b, s32 radius_b);
void Actor_SelectAnimation(struct Actor *object, s32 argument);
void Actor_SetMoveTarget(struct Actor *actor, s32 x, s32 y, s32 z);
s32 FixedSqrt(s32 value);
struct Actor *Func_08023220(s32 kind, s32 x, s32 y, s32 z);
void Actor_StartScript(struct Actor *, const s32 *);
/* VIEW(f028aac): routine Animation_ApplyChildArgument as s32 Animation_ApplyChildArgument(struct PlayerSprite__f028aac *, s32); f027064 has s32 Animation_ApplyChildArgument(struct PlayerSprite *, s32). */
extern s32 Animation_ApplyChildArgument__f028aac(struct PlayerSprite__f028aac *, s32) __asm__("Animation_ApplyChildArgument");
s32 Func_080ad318(void);

s32 Func_08028aac(struct Actor *object) {
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
  terrain = GetWorldMapCollision((struct WorldPosition *)&object->x);
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
  object->acceleration = 0x8000;
  angle = gData_0802ec5c[(gInput.held >> 4) & 15];
  direction = angle;
  if (direction == 0xffff) {
    collision |= 4;
    goto update_object;
  }
  position.x = object->x;
  position.y = object->y;
  position.z = object->z;
  Vector_AddPolarOffset(0x80000, direction, &position);
  if (gDebugMode != 0 && (gInput.held & 0x200) != 0)
    goto update_object;
  if (Func_0802dbd0(object, &position) == 0) {
    testPosition.x = object->x;
    testPosition.y = object->y;
    testPosition.z = object->z;
    Vector_AddPolarOffset(0x80000, direction + 0x1000, &testPosition);
    if (Func_0802dbd0(object, &testPosition) == 0) {
      testPosition.x = object->x;
      testPosition.y = object->y;
      testPosition.z = object->z;
      Vector_AddPolarOffset(0x80000, direction - 0x1000, &testPosition);
      if (Func_0802dbd0(object, &testPosition) == 0) {
        testPosition.x = object->x;
        testPosition.y = object->y;
        testPosition.z = object->z;
        Vector_AddPolarOffset(0x80000, direction + 0x2000, &testPosition);
        if (Func_0802dbd0(object, &testPosition) == 0) {
          testPosition.x = object->x;
          testPosition.y = object->y;
          testPosition.z = object->z;
          Vector_AddPolarOffset(0x80000, direction - 0x2000, &testPosition);
          if (Func_0802dbd0(object, &testPosition) == 0)
            goto check_objects;
        }
      }
    }
  }
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
    if (Func_0802dbd0(object, &position) != 0)
      continue;
    testPosition.x = object->x;
    testPosition.y = object->y;
    testPosition.z = object->z;
    Vector_AddPolarOffset(0x80000, direction + 0x1000, &testPosition);
    if (Func_0802dbd0(object, &testPosition) != 0)
      continue;
    testPosition.x = object->x;
    testPosition.y = object->y;
    testPosition.z = object->z;
    Vector_AddPolarOffset(0x80000, direction - 0x1000, &testPosition);
    if (Func_0802dbd0(object, &testPosition) != 0)
      continue;
    testPosition.x = object->x;
    testPosition.y = object->y;
    testPosition.z = object->z;
    Vector_AddPolarOffset(0x80000, direction + 0x2000, &testPosition);
    if (Func_0802dbd0(object, &testPosition) != 0)
      continue;
    testPosition.x = object->x;
    testPosition.y = object->y;
    testPosition.z = object->z;
    Vector_AddPolarOffset(0x80000, direction - 0x2000, &testPosition);
    if (Func_0802dbd0(object, &testPosition) != 0)
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
  targetCameraYaw = Iwram_MulQ16(gData_0802eca0[(gInput.held >> 4) & 15],
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
        ripple = Func_08023220(0x10d, object->x, 0, object->z);
        if (ripple != 0) {
          rippleSprite = ripple->sprite;
          Actor_StartScript(ripple, gData_0802ec94);
          ripple->motionFlags = 0;
          ripple->terrainId = kind;
          if (rippleSprite != 0) {
            Animation_ApplyChildArgument__f028aac(rippleSprite, 1);
            rippleSprite->flags = 0;
            rippleSprite->objectMode = 1;
            rippleSprite->priority = 2;
          }
          object->timer = 10;
        }
      }
    } else {
      Actor_SetChildByteValue(object, gFrameCount & kind);
    }
  }

  flightWork = Ram_HeapSlots->eventWork;
  if (gInput.held & (gMainRoutineWork.keys230 | gMainRoutineWork.unk232)) {
    flightWork->climbing = 1;
    object->y += 0x2000;
  } else {
    object->y -= 0x4000;
  }
  if (Func_080ad318() == 0)
    object->y -= 0x4000;
  if (object->y <= 0 && flightWork->climbing != 0)
    ((struct EventWork *)flightWork)->unk16a = 254;
  if (object->y >= 0xc0000)
    object->y = 0xc0000;
  if (object->y <= 0)
    object->y = 0;
  object->step++;
  return 1;
}
