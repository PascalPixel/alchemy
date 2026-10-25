// Draft, differ 633 of 965: script command 0x1e, walking that turns into
// climbing: on foot it moves by the direction pad as SC_PLAYER does without
// pushing (a refused step with up held starts a climb onto ground more than
// 8 pixels above when Func_080c8770 allows it there; with down held, a climb
// down to ground below the next cell south); while climbing (unk66) the pad
// moves the object up and down the face at half a pixel a frame (the probes
// take the step's depth as height), it reaches the top or bottom when the
// ground ahead or under it passes its height, and every 16th frame a
// falling bit of rubble (object 0x11d) drops beside it. Walking leaves
// object 0x164 every fifth step, as Func_0802a124's wake. Both modes end
// with Func_08029838. Structure and frame follow the listing (the retry
// list is 10 headings long); left: register
// allocation and scheduling throughout.
#include "types.h"
#include "ram_buffer.h"
#include "input.h"
#include "frame.h"
#include "event_work.h"
#include "object_runtime.h"
#include "iwram_call.h"

struct FieldPosition {
  s32 x;
  s32 y;
  s32 z;
};

struct PlayerSprite {
  u8 unk00[9];
  u8 low09 : 2;
  u8 priority : 2;
  u8 high09 : 4;
  u8 unk0a[0x10];
  u8 flags;
};

struct ClimbEventWork {
  u8 unk000[0xd4a];
  u8 wakePriority;
};

extern u8 gDebugMode;
extern u32 gFrameCount;
// The heading of each direction-pad combination, -1 for none.
extern const s16 gData_0802ec5c[16];
extern const s32 gData_0802ed28[];
extern const s32 gData_0802ece0[];

void Vector_AddPolarOffset(s32 radius, s32 angle,
                           struct FieldPosition *position);
s32 Func_0802d45c(s32 terrainId, s32 x, s32 z);
s32 Func_0802d7b0(s32 x, s32 z, s32 layer, s32 height);
s32 Func_0802d87c(struct ObjectRuntime *object, struct FieldPosition *position);
s32 Func_080c8770(s32 x, s32 z, s32 kind);
void ObjectDispatch_SelectAnimation(struct ObjectRuntime *object, s32 argument);
s32 Object_SetMoveTarget(struct ObjectRuntime *object, s32 x, s32 y, s32 z);
s32 FixedSqrt(s32 value);
u32 Random16(void);
struct ObjectRuntime *Func_08023220(s32 kind, s32 x, s32 y, s32 z);
void ObjectDispatch_Initialize(struct ObjectRuntime *, const s32 *);
s32 Animation_ApplyChildArgument(struct PlayerSprite *, s32);
void Audio_PlayCue(s32 cue);
void Func_08029810(void);
s32 Func_08029838(struct ObjectRuntime *object);

s32 Func_080298c0(struct ObjectRuntime *object) {
  struct FieldPosition position;
  struct FieldPosition testPosition;
  s16 angleOffsets[10];
  struct FieldPosition spawn;
  s32 collision;
  s32 heading;
  s32 y;
  s32 groundHere;
  s32 groundAhead;
  s32 cellX;
  s32 cellZ;
  s32 angle;
  s32 angleIndex;
  u8 *terrainId;
  s16 *action;
  s16 *climbing;
  u8 *wakeTimer;
  struct ObjectRuntime *effect;
  struct PlayerSprite *sprite;

  heading = 0;
  y = object->y;
  terrainId = &object->terrainId;
  cellX = object->x & 0xfff00000;
  cellZ = object->z & 0xfff00000;
  groundHere = Func_0802d45c(*terrainId, cellX, cellZ);
  groundAhead = Func_0802d45c(*terrainId, cellX, cellZ + 0xfff00000);
  action = &object->action;
  if (*action != 0) {
    if (object->targetX != 0x80000000)
      goto done;
    *action = heading;
    object->terrainHeight = groundHere;
  }
  climbing = (s16 *)&object->unk66;
  if (*climbing != 0) {
    if (groundAhead < y) {
      s32 topZ = cellZ + 0xfff80000;

      if (Func_0802d7b0(object->x, topZ, *terrainId, object->terrainHeight) ==
          2) {
        object->y -= 0x10000;
      } else {
        Object_SetMoveTarget(object, object->x, groundAhead, topZ);
        *action = 1;
        *climbing = heading;
        goto done;
      }
    }
    if (groundHere > y) {
      *climbing = 0;
      object->terrainHeight = groundHere;
      object->motionFlags = 3;
      goto done;
    }

    collision = 0;
    // DEAD: the debug read of the held keys is never used.
    if (gDebugMode != 0)
      (void)gInput.held;
    object->speedLimit = 0x8000;
    object->acceleration = 0x4000;
    angle = gData_0802ec5c[(gInput.held >> 4) & 15];
    if ((u16)angle == 0xffff) {
      collision = 4;
    } else {
      position.x = object->x;
      position.y = collision;
      position.z = collision;
      Vector_AddPolarOffset(0x80000, (u16)angle, &position);
      position.y = object->y - position.z;
      position.z = object->z;
      ObjectDispatch_SelectAnimation(object, 3);
      testPosition.x = object->x;
      testPosition.y = collision;
      testPosition.z = collision;
      Vector_AddPolarOffset(0x80000, (u16)angle, &testPosition);
      testPosition.y = object->y - testPosition.z;
      testPosition.z = object->z;
      if (Func_0802d87c(object, &testPosition))
        goto climb_blocked;
      testPosition.x = object->x;
      testPosition.y = collision;
      testPosition.z = collision;
      Vector_AddPolarOffset(0x80000, (u16)angle + 0x1000, &testPosition);
      testPosition.y = object->y - testPosition.z;
      testPosition.z = object->z;
      if (Func_0802d87c(object, &testPosition))
        goto climb_blocked;
      testPosition.x = object->x;
      testPosition.y = collision;
      testPosition.z = collision;
      Vector_AddPolarOffset(0x80000, (u16)angle - 0x1000, &testPosition);
      testPosition.y = object->y - testPosition.z;
      testPosition.z = object->z;
      if (Func_0802d87c(object, &testPosition))
        goto climb_blocked;
      testPosition.x = object->x;
      testPosition.y = collision;
      testPosition.z = collision;
      Vector_AddPolarOffset(0x80000, (u16)angle + 0x2000, &testPosition);
      testPosition.y = object->y - testPosition.z;
      testPosition.z = object->z;
      if (Func_0802d87c(object, &testPosition))
        goto climb_blocked;
      testPosition.x = object->x;
      testPosition.y = collision;
      testPosition.z = collision;
      Vector_AddPolarOffset(0x80000, (u16)angle - 0x2000, &testPosition);
      testPosition.y = object->y - testPosition.z;
      testPosition.z = object->z;
      if (!Func_0802d87c(object, &testPosition))
        goto climb_moved;
    climb_blocked:
      position.x = object->x;
      position.y = object->y;
      position.z = object->z;
      collision |= 1;
    }
  climb_moved:
    if (collision != 0) {
      object->targetX = 0x80000000;
      object->targetY = 0x80000000;
      object->targetZ = 0x80000000;
      object->velocityX = 0;
      object->velocityZ = 0;
    } else {
      s32 speed;

      heading = (s16)angle;
      Object_SetMoveTarget(object, position.x, position.y, position.z);
      speed = FixedSqrt(Iwram_MulQ16(object->velocityX, object->velocityX) +
                        Iwram_MulQ16(object->velocityZ, object->velocityZ));
      object->velocityX = collision;
      object->velocityZ = collision;
      Vector_AddPolarOffset(speed, (u16)heading,
                            (struct FieldPosition *)&object->velocityX);
      object->velocityY = -object->velocityZ;
      object->velocityZ = collision;
    }
    // Every 16th frame, a bit of rubble drops beside the climber.
    if ((gFrameCount & 15) == 0) {
      s32 frame = gFrameCount & 15;

      spawn.x = object->x - Random16() * 6 + 0x30000;
      spawn.y = object->y;
      spawn.z = object->z;
      effect = Func_08023220(0x11d, spawn.x, spawn.y, spawn.z);
      if (effect != NULL) {
        effect->terrainHeight = object->terrainHeight;
        sprite = effect->animation;
        ObjectDispatch_Initialize(effect, gData_0802ed28);
        effect->update = Func_08029810;
        effect->motionFlags = 2;
        if (collision == 0)
          Audio_PlayCue(596);
        effect->gravity = 0xa3d;
        effect->velocityY = frame;
        effect->scaleX = 0x4ccc;
        effect->scaleY = 0x4ccc;
        if (sprite != NULL) {
          Animation_ApplyChildArgument(sprite, 2);
          sprite->flags = frame;
          sprite->priority = 0;
        }
      }
    }
    Func_08029838(object);
    goto done;
  }

  // On foot.
  collision = 0;
  object->speedLimit = 0x10000;
  object->acceleration = 0x4000;
  angle = gData_0802ec5c[(gInput.held >> 4) & 15];
  if ((u16)angle == 0xffff) {
    collision = 4;
    goto counted;
  }
  position.x = object->x;
  position.y = object->y;
  position.z = object->z;
  Vector_AddPolarOffset(0x80000, (u16)angle, &position);
  if (gDebugMode != 0) {
    heading = (s16)angle;
    if (gInput.held & 0x200)
      goto counted;
  }
  if (!Func_0802d87c(object, &position)) {
    testPosition.x = object->x;
    testPosition.y = object->y;
    testPosition.z = object->z;
    Vector_AddPolarOffset(0x80000, (u16)angle + 0x1000, &testPosition);
    if (!Func_0802d87c(object, &testPosition)) {
      testPosition.x = object->x;
      testPosition.y = object->y;
      testPosition.z = object->z;
      Vector_AddPolarOffset(0x80000, (u16)angle - 0x1000, &testPosition);
      if (!Func_0802d87c(object, &testPosition)) {
        testPosition.x = object->x;
        testPosition.y = object->y;
        testPosition.z = object->z;
        Vector_AddPolarOffset(0x80000, (u16)angle + 0x2000, &testPosition);
        if (!Func_0802d87c(object, &testPosition)) {
          testPosition.x = object->x;
          testPosition.y = object->y;
          testPosition.z = object->z;
          Vector_AddPolarOffset(0x80000, (u16)angle - 0x2000, &testPosition);
          heading = (s16)angle;
          if (!Func_0802d87c(object, &testPosition))
            goto counted;
        }
      }
    }
  }
  {
    u16 direction = angle;

    angleOffsets[0] = direction + 0x1000;
    angleOffsets[1] = direction - 0x1000;
    angleOffsets[2] = direction + 0x2000;
    angleOffsets[3] = direction - 0x2000;
    angleOffsets[4] = direction + 0x3000;
    angleOffsets[5] = direction - 0x3000;
  }
  for (angleIndex = 0; angleIndex < 6; angleIndex++) {
    heading = angleOffsets[angleIndex];
    position.x = object->x;
    position.y = object->y;
    position.z = object->z;
    Vector_AddPolarOffset(0x80000, (u16)heading, &position);
    if (Func_0802d87c(object, &position))
      continue;
    testPosition.x = object->x;
    testPosition.y = object->y;
    testPosition.z = object->z;
    Vector_AddPolarOffset(0x80000, (u16)heading + 0x1000, &testPosition);
    if (Func_0802d87c(object, &testPosition))
      continue;
    testPosition.x = object->x;
    testPosition.y = object->y;
    testPosition.z = object->z;
    Vector_AddPolarOffset(0x80000, (u16)heading - 0x1000, &testPosition);
    if (Func_0802d87c(object, &testPosition))
      continue;
    testPosition.x = object->x;
    testPosition.y = object->y;
    testPosition.z = object->z;
    Vector_AddPolarOffset(0x80000, (u16)heading + 0x2000, &testPosition);
    if (Func_0802d87c(object, &testPosition))
      continue;
    testPosition.x = object->x;
    testPosition.y = object->y;
    testPosition.z = object->z;
    Vector_AddPolarOffset(0x80000, (u16)heading - 0x2000, &testPosition);
    if (!Func_0802d87c(object, &testPosition))
      goto counted;
  }
  position.x = object->x;
  position.y = object->y;
  position.z = object->z;
  collision |= 1;
  // Up against a face more than 8 pixels high: start climbing it.
  if ((gInput.held & 0x40) && groundAhead > y + 0x80000) {
    s32 faceX = (position.x & 0xfff00000) + 0x80000;
    s32 faceZ = (position.z & 0xfff00000) + 0xfff80000;

    if (Func_080c8770(faceX, faceZ, 2)) {
      object->motionFlags = 0;
      object->y += 0x10000;
      *climbing = 1;
      object->velocityX = 0;
      object->velocityY = 0;
      object->velocityZ = 0;
      ObjectDispatch_SelectAnimation(object, 3);
      goto done;
    }
  }
  // Down at a drop: climb down the face south of the cell.
  if (gInput.held & 0x80) {
    s32 faceX = (object->x & 0xfff00000) + 0x80000;
    s32 faceZ = (object->z & 0xfff00000) + 0x180000;

    if (Func_0802d45c(*terrainId, faceX, faceZ) < y &&
        Func_080c8770(faceX, faceZ, 2)) {
      object->motionFlags = 0;
      Object_SetMoveTarget(object, object->x, groundHere + 0xfffe0000, faceZ);
      *action = 1;
      *climbing = 1;
      object->velocityX = 0;
      object->velocityY = 0;
      object->velocityZ = 0;
      ObjectDispatch_SelectAnimation(object, 3);
      goto done;
    }
  }

counted:
  {
    struct EventWork *work = Ram_HeapSlots->eventWork;

    if (work != NULL) {
      if (collision & 3)
        work->blockedSteps++;
      else
        work->blockedSteps = 0;
      work->heldKeys = gInput.held;
    }
  }
  ObjectDispatch_SelectAnimation(object, 1);
  if (collision != 0) {
    object->targetX = 0x80000000;
    object->targetY = 0x80000000;
    object->targetZ = 0x80000000;
    object->velocityX = 0;
    object->velocityZ = 0;
    if (collision & 3) {
      s32 difference;

      difference = (s16)((u16)angle - object->angle);
      if (difference > 0x1000)
        difference = 0x1000;
      if (difference < -0x1000)
        difference = -0x1000;
      object->angle += difference;
    }
    wakeTimer = (u8 *)object + 98;
    *wakeTimer = 0;
  } else {
    s32 speed;

    Object_SetMoveTarget(object, position.x, position.y, position.z);
    speed = FixedSqrt(Iwram_MulQ16(object->velocityX, object->velocityX) +
                      Iwram_MulQ16(object->velocityZ, object->velocityZ));
    object->velocityX = collision;
    object->velocityZ = collision;
    Vector_AddPolarOffset(speed, (u16)heading,
                          (struct FieldPosition *)&object->velocityX);
    wakeTimer = (u8 *)object + 98;
    if (*wakeTimer != 0)
      (*wakeTimer)--;
  }
  if (*wakeTimer == 0 && collision == 0) {
    effect = Func_08023220(0x164, object->x, object->y, object->z - 0x20000);
    if (effect != NULL) {
      effect->terrainHeight = object->terrainHeight;
      sprite = effect->animation;
      ObjectDispatch_Initialize(effect, gData_0802ece0);
      effect->priorityFlags = 8;
      ((u8 *)effect)[99] = 100;
      effect->motionFlags = collision;
      Audio_PlayCue(596);
      if (sprite != NULL) {
        struct ClimbEventWork *work = Ram_HeapSlots->eventWork;

        Animation_ApplyChildArgument(sprite, 1);
        sprite->flags = collision;
        sprite->priority = 2;
        sprite->priority = work->wakePriority & 3;
      }
      *wakeTimer = 5;
    }
  }
  Func_08029838(object);

done:
  object->step++;
  return 1;
}
