#include "types.h"
#include "ram_buffer.h"
#include "input.h"
#include "frame.h"
#include "map.h"
#include "event_work.h"
#include "party_state.h"
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

struct PartyKeys {
  u8 unk000[0x23c];
  u16 keys23c;
};

struct WorldEventWork {
  u8 unk000[0xd39];
  s8 unkd39;
  u8 terrainKind;
};

// The heading of each direction-pad combination, -1 for none.
extern const s16 gData_0802ec5c[16];
// The camera yaw each direction-pad combination turns toward.
extern const s32 gData_0802eca0[16];
extern const s32 gData_0802ec94[];

void Vector_AddPolarOffset(s32 radius, s32 angle,
                           struct FieldPosition *position);
s32 Func_0802db88(struct ObjectRuntime *object, struct FieldPosition *position);
void GameFlag_ClearBit(s32 flag);
void ObjectDispatch_SelectAnimation(struct ObjectRuntime *object,
                                            s32 argument);
s32 Object_SetMoveTarget(struct ObjectRuntime *object, s32 x, s32 y, s32 z);
s32 FixedSqrt(s32 value);
struct ObjectRuntime *Func_08023220(s32 kind, s32 x, s32 y, s32 z);
void ObjectDispatch_Initialize(struct ObjectRuntime *, const s32 *);
s32 Animation_ApplyChildArgument(struct PlayerSprite *, s32);
s32 Field_CheckConfiguredKeys(void);

/// Script command 0x20, a sibling of Func_080279b0 in The Broken Seal's later
/// Object_MoveOnWorldMap layout (one probe ahead, then four probes 22.5 and 45
/// degrees off it; six retry headings). Records the kind of ground under the
/// object in the event work (byte 0xd3a), moves at a run, and lets the shortcut
/// keys post requests 252 and, while byte 0xd39 is set, clear flag 0x147 and
/// post 253; turns at most 0x400 a step while moving.
s32 Func_08028534(struct ObjectRuntime *object) {
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
  u32 kind;

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

  collision = 0;
  object->speedLimit = 0x14000;
  object->acceleration = 0x8000;
  /* FAKEMATCH(barrier): the input and the heading table load after the
   * acceleration store; as plain statements, 4 of 636 instructions differ in
   * tla-en. */
  asm("");
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
      if (Func_0802db88(object, &position) == 0) {
        testPosition.x = object->x;
        testPosition.y = object->y;
        testPosition.z = object->z;
        Vector_AddPolarOffset(0x80000, direction + 0x1000, &testPosition);
        if (Func_0802db88(object, &testPosition) == 0) {
          testPosition.x = object->x;
          testPosition.y = object->y;
          testPosition.z = object->z;
          Vector_AddPolarOffset(0x80000, direction - 0x1000, &testPosition);
          if (Func_0802db88(object, &testPosition) == 0) {
            testPosition.x = object->x;
            testPosition.y = object->y;
            testPosition.z = object->z;
            Vector_AddPolarOffset(0x80000, direction + 0x2000, &testPosition);
            if (Func_0802db88(object, &testPosition) == 0) {
              testPosition.x = object->x;
              testPosition.y = object->y;
              testPosition.z = object->z;
              Vector_AddPolarOffset(0x80000, direction - 0x2000,
                                    &testPosition);
              if (Func_0802db88(object, &testPosition) == 0)
                goto update_object;
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
      /* FAKEMATCH(barrier): the retry count is spilled before the first index
       * is set; as plain statements, 8 of 636 instructions differ in tla-en.
       */
      asm("");
      for (angleIndex = 0; angleIndex < count; angleIndex++) {
        direction = angleOffsets[angleIndex];
        position.x = object->x;
        position.y = object->y;
        position.z = object->z;
        Vector_AddPolarOffset(0x80000, direction, &position);
        if (Func_0802db88(object, &position) != 0)
          continue;
        testPosition.x = object->x;
        testPosition.y = object->y;
        testPosition.z = object->z;
        Vector_AddPolarOffset(0x80000, direction + 0x1000, &testPosition);
        if (Func_0802db88(object, &testPosition) != 0)
          continue;
        testPosition.x = object->x;
        testPosition.y = object->y;
        testPosition.z = object->z;
        Vector_AddPolarOffset(0x80000, direction - 0x1000, &testPosition);
        if (Func_0802db88(object, &testPosition) != 0)
          continue;
        testPosition.x = object->x;
        testPosition.y = object->y;
        testPosition.z = object->z;
        Vector_AddPolarOffset(0x80000, direction + 0x2000, &testPosition);
        if (Func_0802db88(object, &testPosition) != 0)
          continue;
        testPosition.x = object->x;
        testPosition.y = object->y;
        testPosition.z = object->z;
        Vector_AddPolarOffset(0x80000, direction - 0x2000, &testPosition);
        if (Func_0802db88(object, &testPosition) != 0)
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
    ObjectDispatch_SelectAnimation(object, 1);
  else
    ObjectDispatch_SelectAnimation(object, 2);

  if (gInput.pressed & gPartyState.keys230) {
    ((struct EventWork *)Ram_HeapSlots->eventWork)->unk16a = 252;
    collision |= 4;
  }
  if (gInput.pressed & ((struct PartyKeys *)&gPartyState)->keys23c) {
    struct WorldEventWork *keyWork = Ram_HeapSlots->eventWork;
    if (keyWork->unkd39 != 0) {
      GameFlag_ClearBit(0x147);
      ((struct EventWork *)keyWork)->unk16a = 253;
    }
  }

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
  targetCameraYaw = gData_0802eca0[(gInput.held >> 4) & 15];
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
    GetWorldMapCollision((struct WorldPosition *)&object->x);
    if (object->action == 0 && collision == 0) {
      ripple = Func_08023220(0x10d, object->x, object->y, object->z);
      if (ripple != 0) {
        rippleSprite = ripple->animation;
        ObjectDispatch_Initialize(ripple, gData_0802ec94);
        ripple->motionFlags = 0;
        ripple->terrainId = kind;
        if (rippleSprite != 0) {
          Animation_ApplyChildArgument(rippleSprite, 2);
          rippleSprite->flags = 0;
          rippleSprite->objectMode = 0;
          rippleSprite->priority = 2;
        }
        object->action = 10;
      }
    }
  }
  Field_CheckConfiguredKeys();
  object->step++;
  return 1;
}
