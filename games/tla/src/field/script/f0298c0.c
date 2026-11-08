#include "types.h"
#include "ram_buffer.h"
#include "input.h"
#include "frame.h"
#include "event_work.h"
#include "actor.h"
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

struct Unk_WakeActor {
  u8 unk00[0x63];
  u8 unk63;
};

struct Unk_ClimbEventWork {
  u8 unk000[0xd4a];
  /// Read as the low byte of a halfword: as a u8 or s16 field the read waits
  /// for the sprite store (8 of 965 instructions differ in tla-en).
  u16 unkd4a : 8;
  u16 unkd4b : 8;
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
s32 Func_0802d87c(struct Actor *actor, struct FieldPosition *position);
s32 Func_080c8770(s32 x, s32 z, s32 kind);
void Actor_SelectAnimation(struct Actor *actor, s32 argument);
void Actor_SetMoveTarget(struct Actor *actor, s32 x, s32 y, s32 z);
s32 FixedSqrt(s32 value);
u32 Random16(void);
struct Actor *Func_08023220(s32 kind, s32 x, s32 y, s32 z);
void Actor_StartScript(struct Actor *, const s32 *);
s32 Animation_ApplyChildArgument(struct PlayerSprite *, s32);
void Sound_PlayFar(s32 cue);
void Func_08029810(void);
s32 Func_08029838(struct Actor *actor);

s32 Func_080298c0(struct Actor *actor) {
  struct FieldPosition position;
  struct FieldPosition testPosition;
  s16 angleOffsets[10];
  struct FieldPosition spawn;
  s32 collision;
  s32 heading;
  s32 groundHere;
  s32 y;
  s32 groundAhead;
  s32 angle;
  s32 cellX;
  s32 cellZ;
  s32 angleIndex;
  s32 count;
  u8 *terrainId;
  u8 *wakeTimer;
  struct Actor *effect;
  struct PlayerSprite *sprite;

  heading = 0;
  count = 0;
  cellX = actor->x & 0xfff00000;
  cellZ = actor->z & 0xfff00000;
  y = actor->y;
  terrainId = &actor->terrainId;
  groundHere = Func_0802d45c(*terrainId, cellX, cellZ);
  groundAhead = Func_0802d45c(*terrainId, cellX, cellZ + 0xfff00000);
  if (actor->action != 0) {
    if (actor->targetX != 0x80000000)
      goto done;
    actor->action = 0;
    actor->terrainHeight = groundHere;
  }
  if ((s16)actor->unk66 != 0) {
    if (groundAhead < y) {
      s32 topZ = cellZ + 0xfff80000;

      if (Func_0802d7b0(actor->x, topZ, *terrainId, actor->terrainHeight) ==
          2) {
        actor->y -= 0x10000;
      } else {
        Actor_SetMoveTarget(actor, actor->x, groundAhead, topZ);
        actor->action = 1;
        actor->unk66 = 0;
        goto done;
      }
    }
    if (groundHere > y) {
      actor->unk66 = 0;
      actor->terrainHeight = groundHere;
      actor->motionFlags = 3;
      goto done;
    }

    collision = 0;
    // DEAD: the debug read of the held keys is never used.
    if (gDebugMode != 0)
      (void)*(volatile u32 *)&gInput.held;
    actor->speedLimit = 0x8000;
    actor->acceleration = 0x4000;
    angle = gData_0802ec5c[(gInput.held >> 4) & 15];
    if ((u16)angle == 0xffff) {
      collision = 4;
    } else {
      position.x = actor->x;
      position.y = 0;
      position.z = 0;
      Vector_AddPolarOffset(0x80000, (u16)angle, &position);
      position.y = actor->y - position.z;
      position.z = actor->z;
      Actor_SelectAnimation(actor, 3);
      testPosition.x = actor->x;
      testPosition.y = 0;
      testPosition.z = 0;
      Vector_AddPolarOffset(0x80000, (u16)angle, &testPosition);
      testPosition.y = actor->y - testPosition.z;
      testPosition.z = actor->z;
      if (Func_0802d87c(actor, &testPosition))
        goto climb_blocked;
      testPosition.x = actor->x;
      testPosition.y = 0;
      testPosition.z = 0;
      Vector_AddPolarOffset(0x80000, (u16)angle + 0x1000, &testPosition);
      testPosition.y = actor->y - testPosition.z;
      testPosition.z = actor->z;
      if (Func_0802d87c(actor, &testPosition))
        goto climb_blocked;
      testPosition.x = actor->x;
      testPosition.y = 0;
      testPosition.z = 0;
      Vector_AddPolarOffset(0x80000, (u16)angle - 0x1000, &testPosition);
      testPosition.y = actor->y - testPosition.z;
      testPosition.z = actor->z;
      if (Func_0802d87c(actor, &testPosition))
        goto climb_blocked;
      testPosition.x = actor->x;
      testPosition.y = 0;
      testPosition.z = 0;
      Vector_AddPolarOffset(0x80000, (u16)angle + 0x2000, &testPosition);
      testPosition.y = actor->y - testPosition.z;
      testPosition.z = actor->z;
      if (Func_0802d87c(actor, &testPosition))
        goto climb_blocked;
      testPosition.x = actor->x;
      testPosition.y = 0;
      testPosition.z = 0;
      Vector_AddPolarOffset(0x80000, (u16)angle - 0x2000, &testPosition);
      testPosition.y = actor->y - testPosition.z;
      testPosition.z = actor->z;
      if (!Func_0802d87c(actor, &testPosition))
        goto climb_moved;
    climb_blocked:
      position.x = actor->x;
      position.y = actor->y;
      position.z = actor->z;
      collision |= 1;
    }
  climb_moved:
    if (collision != 0) {
      actor->targetX = 0x80000000;
      actor->targetY = 0x80000000;
      actor->targetZ = 0x80000000;
      actor->velocityX = 0;
      actor->velocityZ = 0;
    } else {
      s32 speed;

      heading = (s16)angle;
      Actor_SetMoveTarget(actor, position.x, position.y, position.z);
      speed = FixedSqrt(Iwram_MulQ16(actor->velocityX, actor->velocityX) +
                        Iwram_MulQ16(actor->velocityZ, actor->velocityZ));
      actor->velocityX = 0;
      actor->velocityZ = 0;
      Vector_AddPolarOffset(speed, (u16)heading,
                            (struct FieldPosition *)&actor->velocityX);
      actor->velocityY = -actor->velocityZ;
      actor->velocityZ = 0;
    }
    // Every 16th frame, a bit of rubble drops beside the climber.
    if ((gFrameCount & 15) == 0) {
      spawn.x = actor->x - Random16() * 6 + 0x30000;
      spawn.y = actor->y;
      spawn.z = actor->z;
      effect = Func_08023220(0x11d, spawn.x, spawn.y, spawn.z);
      if (effect != NULL) {
        effect->terrainHeight = actor->terrainHeight;
        sprite = effect->animation;
        Actor_StartScript(effect, gData_0802ed28);
        effect->update = Func_08029810;
        effect->motionFlags = 2;
        if (collision == 0)
          Sound_PlayFar(596);
        effect->gravity = 0xa3d;
        effect->velocityY = 0;
        effect->scaleX = 0x4ccc;
        effect->scaleY = 0x4ccc;
        if (sprite != NULL) {
          Animation_ApplyChildArgument(sprite, 2);
          sprite->flags = 0;
          sprite->priority = 0;
        }
      }
    }
    Func_08029838(actor);
    goto done;
  }

  // On foot.
  collision = 0;
  actor->speedLimit = 0x10000;
  actor->acceleration = 0x4000;
  angle = gData_0802ec5c[(gInput.held >> 4) & 15];
  if ((u16)angle == 0xffff) {
    collision = 4;
    goto counted;
  }
  position.x = actor->x;
  position.y = actor->y;
  position.z = actor->z;
  Vector_AddPolarOffset(0x80000, (u16)angle, &position);
  if (gDebugMode != 0 && (gInput.held & 0x200)) {
    heading = (s16)angle;
    goto counted;
  }
  if (!Func_0802d87c(actor, &position)) {
    testPosition.x = actor->x;
    testPosition.y = actor->y;
    testPosition.z = actor->z;
    Vector_AddPolarOffset(0x80000, (u16)angle + 0x1000, &testPosition);
    if (!Func_0802d87c(actor, &testPosition)) {
      testPosition.x = actor->x;
      testPosition.y = actor->y;
      testPosition.z = actor->z;
      Vector_AddPolarOffset(0x80000, (u16)angle - 0x1000, &testPosition);
      if (!Func_0802d87c(actor, &testPosition)) {
        testPosition.x = actor->x;
        testPosition.y = actor->y;
        testPosition.z = actor->z;
        Vector_AddPolarOffset(0x80000, (u16)angle + 0x2000, &testPosition);
        if (!Func_0802d87c(actor, &testPosition)) {
          testPosition.x = actor->x;
          testPosition.y = actor->y;
          testPosition.z = actor->z;
          Vector_AddPolarOffset(0x80000, (u16)angle - 0x2000, &testPosition);
          if (!Func_0802d87c(actor, &testPosition)) {
            heading = (s16)angle;
            goto counted;
          }
        }
      }
    }
  }
  {
    u16 direction = angle;

    angleOffsets[count++] = direction + 0x1000;
    angleOffsets[count++] = direction - 0x1000;
    angleOffsets[count++] = direction + 0x2000;
    angleOffsets[count++] = direction - 0x2000;
    angleOffsets[count++] = direction + 0x3000;
    angleOffsets[count++] = direction - 0x3000;
  }
  for (angleIndex = 0; angleIndex < count; angleIndex++) {
    heading = angleOffsets[angleIndex];
    position.x = actor->x;
    position.y = actor->y;
    position.z = actor->z;
    Vector_AddPolarOffset(0x80000, (u16)heading, &position);
    if (Func_0802d87c(actor, &position))
      continue;
    testPosition.x = actor->x;
    testPosition.y = actor->y;
    testPosition.z = actor->z;
    Vector_AddPolarOffset(0x80000, (u16)heading + 0x1000, &testPosition);
    if (Func_0802d87c(actor, &testPosition))
      continue;
    testPosition.x = actor->x;
    testPosition.y = actor->y;
    testPosition.z = actor->z;
    Vector_AddPolarOffset(0x80000, (u16)heading - 0x1000, &testPosition);
    if (Func_0802d87c(actor, &testPosition))
      continue;
    testPosition.x = actor->x;
    testPosition.y = actor->y;
    testPosition.z = actor->z;
    Vector_AddPolarOffset(0x80000, (u16)heading + 0x2000, &testPosition);
    if (Func_0802d87c(actor, &testPosition))
      continue;
    testPosition.x = actor->x;
    testPosition.y = actor->y;
    testPosition.z = actor->z;
    Vector_AddPolarOffset(0x80000, (u16)heading - 0x2000, &testPosition);
    if (!Func_0802d87c(actor, &testPosition))
      goto counted;
  }
  position.x = actor->x;
  position.y = actor->y;
  position.z = actor->z;
  collision |= 1;
  // Up against a face more than 8 pixels high: start climbing it.
  if ((gInput.held & 0x40) && groundAhead > y + 0x80000) {
    cellX = (position.x & 0xfff00000) + 0x80000;
    cellZ = (position.z & 0xfff00000) + 0xfff80000;

    if (Func_080c8770(cellX, cellZ, 2)) {
      actor->motionFlags = 0;
      actor->y += 0x10000;
      actor->unk66 = 1;
      actor->velocityX = 0;
      actor->velocityY = 0;
      actor->velocityZ = 0;
      Actor_SelectAnimation(actor, 3);
      goto done;
    }
  }
  // Down at a drop: climb down the face south of the cell.
  if (gInput.held & 0x80) {
    cellX = (actor->x & 0xfff00000) + 0x80000;
    cellZ = (actor->z & 0xfff00000) + 0x180000;

    if (Func_0802d45c(*terrainId, cellX, cellZ) < y &&
        Func_080c8770(cellX, cellZ, 2)) {
      actor->motionFlags = 0;
      Actor_SetMoveTarget(actor, actor->x, groundHere + 0xfffe0000, cellZ);
      actor->action = 1;
      actor->unk66 = 1;
      actor->velocityX = 0;
      actor->velocityY = 0;
      actor->velocityZ = 0;
      Actor_SelectAnimation(actor, 3);
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
  Actor_SelectAnimation(actor, 1);
  if (collision != 0) {
    actor->targetX = 0x80000000;
    actor->targetY = 0x80000000;
    actor->targetZ = 0x80000000;
    actor->velocityX = 0;
    actor->velocityZ = 0;
    if (collision & 3) {
      s32 difference;
      u16 turn = angle;

      difference = (s16)(turn - actor->angle);
      if (difference > 0x1000)
        difference = 0x1000;
      if (difference < -0x1000)
        difference = -0x1000;
      actor->angle += difference;
    }
    wakeTimer = (u8 *)actor + 98;
    *wakeTimer = 0;
  } else {
    s32 speed;

    Actor_SetMoveTarget(actor, position.x, position.y, position.z);
    speed = FixedSqrt(Iwram_MulQ16(actor->velocityX, actor->velocityX) +
                      Iwram_MulQ16(actor->velocityZ, actor->velocityZ));
    actor->velocityX = 0;
    actor->velocityZ = 0;
    Vector_AddPolarOffset(speed, (u16)heading,
                          (struct FieldPosition *)&actor->velocityX);
    wakeTimer = (u8 *)actor + 98;
    if (*wakeTimer != 0)
      (*wakeTimer)--;
  }
  // The retest reads through the actor, not wakeTimer: that keeps its
  // zero path from skipping it.
  if (((u8 *)actor)[98] == 0 && collision == 0) {
    effect = Func_08023220(0x164, actor->x, actor->y, actor->z - 0x20000);
    if (effect != NULL) {
      effect->terrainHeight = actor->terrainHeight;
      sprite = effect->animation;
      Actor_StartScript(effect, gData_0802ece0);
      effect->priorityFlags = 8;
      // Through a struct member: as a byte offset the store holds back the
      // stack read of collision after it.
      ((struct Unk_WakeActor *)effect)->unk63 = 100;
      effect->motionFlags = 0;
      Sound_PlayFar(596);
      if (sprite != NULL) {
        struct Unk_ClimbEventWork *work = Ram_HeapSlots->eventWork;

        Animation_ApplyChildArgument(sprite, 1);
        sprite->flags = 0;
        // DEAD: overwritten by the next line.
        sprite->priority = 2;
        sprite->priority = work->unkd4a & 3;
      }
      *wakeTimer = 5;
    }
  }
  Func_08029838(actor);

done:
  actor->step++;
  return 1;
}
