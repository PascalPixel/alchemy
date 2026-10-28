// Draft, differ 6 of 736 in tla-en under -mthumb-slow-loads (2026-10-26). A
// device search (each statement followed by an empty asm or a one-pass block,
// wrapped in one, or swapped with the next, kept when it helps) reaches 2 with
// an empty asm after `object->acceleration = 0x8000;`, and finds nothing
// closer.
// Near miss, differ 46 of 736, instruction order only: script command 0x21,
// the flying sibling of Func_08028534. Moves an object over the world map by
// the direction pad at a pace that grows with its height, refuses steps into
// other solid objects, climbs while either of two shortcut keys is held and
// sinks otherwise (posting request 254 on landing), and scales the camera's
// turn by the height. Left: the native loads object->y and literal pool
// words before it builds nearby constants (movs/lsls), where agscc builds
// the constant first, at each of about a dozen sites. Measured: the object
// loop reuses angleIndex counting down from 63, as ☀️'s player control does
// (a separate index, differ 351: it lands in r4 and is saved around the
// call); the animation kind in a local, the high branch written last
// (differ 173 testing the field twice); separate locals for the two event
// work loads (as one, the frame grows by a word).
#include "types.h"
#include "ram_buffer.h"
#include "input.h"
#include "frame.h"
#include "map.h"
#include "event_work.h"
#include "adventure.h"
#include "object_runtime.h"
#include "iwram_call.h"

struct FieldPosition {
  s32 x;
  s32 y;
  s32 z;
};

struct WorldCamera {
  u8 unk000[0x11e];
  u16 yaw;
};

struct PlayerSprite {
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

struct WorldEventWork {
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
s32 Func_0802dbd0(struct ObjectRuntime *object, struct FieldPosition *position);
s32 Runtime_CheckRadiusOverlap(s32 *a, s32 radius_a, s32 *b, s32 radius_b);
void ObjectDispatch_SelectAnimation(struct ObjectRuntime *object,
                                            s32 argument);
void ObjectDispatch_SetChildByteValue(struct ObjectRuntime *object, s32 value);
s32 Object_SetMoveTarget(struct ObjectRuntime *object, s32 x, s32 y, s32 z);
s32 FixedSqrt(s32 value);
struct ObjectRuntime *Func_08023220(s32 kind, s32 x, s32 y, s32 z);
void ObjectDispatch_Initialize(struct ObjectRuntime *, const s32 *);
s32 Animation_ApplyChildArgument(struct PlayerSprite *, s32);
s32 Func_080ad318(void);

s32 Func_08028aac(struct ObjectRuntime *object) {
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
  struct ObjectRuntime *ripple;
  struct PlayerSprite *rippleSprite;
  struct EventWork *work;
  struct WorldEventWork *worldWork;
  struct ObjectRuntime *other;
  u32 kind;
  struct WorldEventWork *flightWork;

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
        Runtime_CheckRadiusOverlap(&other->x, other->radius, &position.x,
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
    ObjectDispatch_SelectAnimation(object, 6);
  else if (collision != 0)
    ObjectDispatch_SelectAnimation(object, 1);
  else
    ObjectDispatch_SelectAnimation(object, 2);

  if (collision != 0) {
    object->targetX = 0x80000000;
    object->targetY = 0x80000000;
    object->targetZ = 0x80000000;
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
    object->unk66 = 2;
  } else {
    Object_SetMoveTarget(object, position.x, position.y, position.z);
    speed = FixedSqrt(Iwram_MulQ16(object->velocityX, object->velocityX) +
                      Iwram_MulQ16(object->velocityZ, object->velocityZ));
    object->velocityX = collision;
    object->velocityZ = collision;
    direction = angle;
    Vector_AddPolarOffset(speed, direction,
                          (struct FieldPosition *)&object->velocityX);
    if (object->action != 0)
      object->action--;
    object->actionFlags &= ~1;
    difference = (s16)(direction - object->angle);
    if (difference > 0x1000)
      difference = 0x400;
    if (difference < -0x1000)
      difference = -0x400;
    object->angle += difference;
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
      if (object->action == 0 && collision == 0) {
        ripple = Func_08023220(0x10d, object->x, 0, object->z);
        if (ripple != 0) {
          rippleSprite = ripple->animation;
          ObjectDispatch_Initialize(ripple, gData_0802ec94);
          ripple->motionFlags = 0;
          ripple->terrainId = kind;
          if (rippleSprite != 0) {
            Animation_ApplyChildArgument(rippleSprite, 1);
            rippleSprite->flags = 0;
            rippleSprite->objectMode = 1;
            rippleSprite->priority = 2;
          }
          object->action = 10;
        }
      }
    } else {
      ObjectDispatch_SetChildByteValue(object, gFrameCount & kind);
    }
  }

  flightWork = Ram_HeapSlots->eventWork;
  if (gInput.held & (gAdventure.keys230 | gAdventure.unk232)) {
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
