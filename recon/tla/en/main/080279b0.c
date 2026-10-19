// Near miss, differ 24 of 503, instruction order only: script command 0x1a,
// the twin of The Broken Seal's Object_MoveOnWorldMap in its Japanese layout
// (three probes 30 degrees apart, six retry headings), with this game's dash
// keys, flag 0x163 beside the debug L key, and Field_CheckConfiguredKeys'
// shortcut setting the event work's unk16a. Left, all scheduling: the opening
// stores of collision and the animation around the party state's load, the
// retry list's first index load, the copy of the right probe's address, the
// yaw table's and the input's pool loads (native earlier), and two ripple
// stores. Measured: angleOffsets[22] as ☀️'s (as [6] the frame is 32 bytes
// short), the ripple sprite's mode and priority as 2-bit fields (as byte
// masks, differ 70); an empty asm passing the party state and the input
// through before the shortcut test orders those loads but swaps r1 and r2
// (differ 30); the opening assignments in other orders (differ 28 to 243).
// Permute 8 minutes: nothing better by differ.
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
  u8 unk1b[0x11];
  u8 *entry;
};

struct PartyKeys {
  u8 unk000[0x23c];
  u16 dashKeys;
};

// The heading of each direction-pad combination, -1 for none.
extern const s16 gData_0802ec5c[16];
// The camera yaw each direction-pad combination turns toward.
extern const s32 gData_0802eca0[16];
extern const s32 gData_0802ec94[];

void Vector_AddPolarOffset(s32 radius, s32 angle,
                           struct FieldPosition *position);
s32 Func_0802db64(struct ObjectRuntime *object, struct FieldPosition *position);
s32 GameFlag_Test(s32 flag);
void ObjectDispatch_ApplyArgumentToChildren(struct ObjectRuntime *object,
                                            s32 argument);
s32 Object_SetMoveTarget(struct ObjectRuntime *object, s32 x, s32 y, s32 z);
s32 FixedSqrt(s32 value);
struct ObjectRuntime *Func_08023220(s32 kind, s32 x, s32 y, s32 z);
void ObjectDispatch_Initialize(struct ObjectRuntime *, const s32 *);
s32 Animation_ApplyChildArgument(struct PlayerSprite *, s32);
s32 Field_CheckConfiguredKeys(void);

s32 Func_080279b0(struct ObjectRuntime *object) {
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
  struct PlayerSprite *playerSprite;
  struct ObjectRuntime *ripple;
  struct PlayerSprite *rippleSprite;
  struct EventWork *work;

  movementAnimation = 2;
  collision = 0;
  count = 0;
  if (gInput.held & ((struct PartyKeys *)&gPartyState)->dashKeys) {
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
  }
  if (collision != 0)
    ObjectDispatch_ApplyArgumentToChildren(object, 9);
  else
    ObjectDispatch_ApplyArgumentToChildren(object, movementAnimation);

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
    Vector_AddPolarOffset(speed, (u16)angle,
                          (struct FieldPosition *)&object->velocityX);
    if (object->action != 0)
      object->action--;
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
    if (terrainCollision == 9) {
      playerSprite->entry[6] = 1;
      playerSprite->flags = 0;
    } else {
      playerSprite->entry[6] = 9;
      playerSprite->flags = 1;
    }
    if (terrainCollision == 6 && object->action == 0 && collision == 0) {
      ripple = Func_08023220(0x10d, object->x, object->y, object->z);
      if (ripple != 0) {
        rippleSprite = ripple->animation;
        ObjectDispatch_Initialize(ripple, gData_0802ec94);
        ripple->motionFlags = collision;
        ripple->terrainId = 1;
        if (rippleSprite != 0) {
          Animation_ApplyChildArgument(rippleSprite, 1);
          rippleSprite->flags = collision;
          rippleSprite->objectMode = 1;
          rippleSprite->priority = 2;
        }
        object->action = 10;
      }
    }
  }
  Field_CheckConfiguredKeys();
  if (gInput.pressed & gPartyState.keys230)
    ((struct EventWork *)Ram_HeapSlots->eventWork)->unk16a = 251;
  object->step++;
  return 1;
}
