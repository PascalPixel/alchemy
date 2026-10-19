// Near miss, differ 64 of 470: script command 0x22, the sibling of
// Func_080279b0 (The Broken Seal's Object_MoveOnWorldMap): moves an object
// on the world map by the direction pad at a fixed pace (L repeating runs
// fast), records the held keys in the event work, and leaves object 0x164
// behind it as a wake every fifth step. Left: the retry list's first store
// indexes by a register the native loads from collision's slot (movs #20;
// add sp builds the list's address), where here the index folds to 0 and
// the list's register use shifts from there; and scheduling, mostly literal
// pool loads the native issues earlier. Measured: collision zeroed again in
// the else branch, count set from collision, count set in the branch or the
// probe test, unk66 stored as 0 (all no change), count left unset (differ
// 223). The byte at 0x62 is the wake timer; the wake's sprite takes priority
// 2 and then 1, both stores kept as the native has them.
#include "types.h"
#include "ram_buffer.h"
#include "input.h"
#include "frame.h"
#include "map.h"
#include "event_work.h"
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
  u8 unk00[9];
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
extern const s32 gData_0802ece0[];

void Vector_AddPolarOffset(s32 radius, s32 angle,
                           struct FieldPosition *position);
s32 Func_0802db64(struct ObjectRuntime *object, struct FieldPosition *position);
void ObjectDispatch_ApplyArgumentToChildren(struct ObjectRuntime *object,
                                            s32 argument);
s32 Object_SetMoveTarget(struct ObjectRuntime *object, s32 x, s32 y, s32 z);
s32 FixedSqrt(s32 value);
struct ObjectRuntime *Func_08023220(s32 kind, s32 x, s32 y, s32 z);
void ObjectDispatch_Initialize(struct ObjectRuntime *, const s32 *);
s32 Animation_ApplyChildArgument(struct PlayerSprite *, s32);
s32 Func_08029838(struct ObjectRuntime *object);

s32 Func_0802a124(struct ObjectRuntime *object) {
  struct FieldPosition position;
  struct FieldPosition leftPosition;
  struct FieldPosition rightPosition;
  s16 angleOffsets[22];
  s32 collision;
  s32 angle;
  u16 direction;
  s32 angleIndex;
  s32 count;
  s32 difference;
  s32 terrainCollision;
  s32 speed;
  s32 targetCameraYaw;
  struct WorldCamera *camera;
  s32 yawStep;
  struct PlayerSprite *playerSprite;
  struct ObjectRuntime *wake;
  struct PlayerSprite *wakeSprite;
  struct EventWork *work;
  u8 *wakeTimer;

  collision = 0;
  count = 0;
  object->unk66 = collision;
  object->action = 1;
  object->speedLimit = 0x8000;
  object->acceleration = 0x4000;
  if (gInput.repeat & 0x200)
    object->speedLimit = 0x40000;

  angle = gData_0802ec5c[(gInput.held >> 4) & 15];
  direction = angle;
  if (direction == 0xffff) {
    collision |= 4;
  } else {
    position.x = object->x;
    position.y = object->y;
    position.z = object->z;
    Vector_AddPolarOffset(0x80000, direction, &position);
    if (gDebugMode == 0 || (gInput.held & 0x200) == 0) {
      leftPosition.x = object->x;
      leftPosition.y = object->y;
      leftPosition.z = object->z;
      Vector_AddPolarOffset(0x80000, direction + 0x1555, &leftPosition);
      rightPosition.x = object->x;
      rightPosition.y = object->y;
      rightPosition.z = object->z;
      Vector_AddPolarOffset(0x80000, direction - 0x1555, &rightPosition);
      if ((Func_0802db64(object, &position) |
           Func_0802db64(object, &leftPosition) |
           Func_0802db64(object, &rightPosition)) != 0) {
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
          if ((Func_0802db64(object, &position) |
               Func_0802db64(object, &leftPosition) |
               Func_0802db64(object, &rightPosition)) != 0)
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
    work->heldKeys = gInput.held;
  }
  ObjectDispatch_ApplyArgumentToChildren(object, 1);

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
    wakeTimer = (u8 *)object + 98;
    *wakeTimer = 0;
    object->unk66 = 2;
  } else {
    Object_SetMoveTarget(object, position.x, position.y, position.z);
    speed = FixedSqrt(Iwram_MulQ16(object->velocityX, object->velocityX) +
                      Iwram_MulQ16(object->velocityZ, object->velocityZ));
    object->velocityX = collision;
    object->velocityZ = collision;
    Vector_AddPolarOffset(speed, (u16)angle,
                          (struct FieldPosition *)&object->velocityX);
    wakeTimer = (u8 *)object + 98;
    if (*wakeTimer != 0)
      (*wakeTimer)--;
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
    playerSprite = object->animation;
    terrainCollision = GetWorldMapCollision((struct WorldPosition *)&object->x);
    if (terrainCollision == 9)
      playerSprite->entry[6] = 1;
    else
      playerSprite->entry[6] = 9;
  }
  if (*wakeTimer == 0 && collision == 0) {
    wake = Func_08023220(0x164, object->x, object->y, object->z - 0x20000);
    if (wake != 0) {
      wake->terrainHeight = object->terrainHeight;
      wakeSprite = wake->animation;
      ObjectDispatch_Initialize(wake, gData_0802ece0);
      wake->priorityFlags = 8;
      ((u8 *)wake)[99] = 100;
      wake->motionFlags = collision;
      if (wakeSprite != 0) {
        Animation_ApplyChildArgument(wakeSprite, 1);
        wakeSprite->flags = collision;
        wakeSprite->priority = 2;
        wakeSprite->priority = 1;
      }
      *wakeTimer = 5;
    }
  }
  Func_08029838(object);
  object->step++;
  return 1;
}
