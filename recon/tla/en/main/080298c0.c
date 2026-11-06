// Differ 24 of 965 (tla-en, 2026-11-06): the wake timer's retest reads the byte through
// the object, so its zero path keeps the native's retest (26 to 24); what is left is
// scheduling at the first climb's call, two copies of object, and the wake block's loads.
// Before: five spellings of the decrement left 26; permuter 5 minutes 48.
// Draft, differ 26 of 965 (tla-en, 2026-11-05): the debug read is volatile and the retry offsets start at 0x1000 (differ 122 to 26). CLIMBING after the first climb's animation call is a measured form, not yet understood: before it, differ 80 by cross-jumping, though the native stores it before. Older notes follow.
// Draft, differ 122 of 965, skeleton gap 32 (tla-en, 2026-11-01)
// Script command 0x1e, walking that turns into climbing: on foot it moves by
// the direction pad as SC_PLAYER does without pushing (a refused step with up
// held starts a climb onto ground more than 8 pixels above when Func_080c8770
// allows it there; with down held, a climb down to ground below the next cell
// south); while climbing (unk66) the pad moves the object up and down the face
// at half a pixel a frame (the probes take the step's depth as height), it
// reaches the top or bottom when the ground ahead or under it passes its
// height, and every 16th frame a falling bit of rubble (object 0x11d) drops
// beside it. Walking leaves object 0x164 every fifth step, as Func_0802a124's
// wake. Both modes end with Func_08029838.
// Left: the climbing branch's dead read of the held keys (6 instructions plus
// its reloads) needs a volatile read, which this draft leaves out: with
// *(volatile u32 *)&gInput.held there, as Actor_MoveByKeys has it, the draft
// measures differ 90, skeleton gap 26. wait_advance.c and Actor_MoveByKeys also
// reread held, so one `volatile u32 held` at its declaration may settle all
// three (S2). The opening's load order (the native has y, z, the terrain
// address, x, the mask; statement orders tried move the allocation or leave
// this one); the first climb's `movs r1, #3; bl` cross-jumps into the second's
// here because sched2 leaves the argument last; two reloads of collision in
// the wake block. Allocation hangs on small things: action and unk66 are
// written as fields (through a pointer a halfword constant comes from the
// pool), count must be set at the top, and cellX and cellZ are reused for the
// face, which gives the callee-saved registers the native uses.
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

// Actor reads its unk66 as signed here.
struct ClimbingActor {
  u8 unk00[0x66];
  s16 climbing;
};
#define CLIMBING(object) (((struct ClimbingActor *)(object))->climbing)

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
s32 Func_0802d87c(struct Actor *object, struct FieldPosition *position);
s32 Func_080c8770(s32 x, s32 z, s32 kind);
void Actor_SelectAnimation(struct Actor *object, s32 argument);
s32 Actor_SetMoveTarget(struct Actor *object, s32 x, s32 y, s32 z);
s32 FixedSqrt(s32 value);
u32 Random16(void);
struct Actor *Func_08023220(s32 kind, s32 x, s32 y, s32 z);
void Actor_StartScript(struct Actor *, const s32 *);
s32 Animation_ApplyChildArgument(struct PlayerSprite *, s32);
void Sound_PlayFar(s32 cue);
void Func_08029810(void);
s32 Func_08029838(struct Actor *object);

s32 Func_080298c0(struct Actor *object) {
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
  cellX = object->x & 0xfff00000;
  cellZ = object->z & 0xfff00000;
  y = object->y;
  terrainId = &object->terrainId;
  groundHere = Func_0802d45c(*terrainId, cellX, cellZ);
  groundAhead = Func_0802d45c(*terrainId, cellX, cellZ + 0xfff00000);
  if (object->action != 0) {
    if (object->targetX != 0x80000000)
      goto done;
    object->action = heading;
    object->terrainHeight = groundHere;
  }
  if (CLIMBING(object) != 0) {
    if (groundAhead < y) {
      s32 topZ = cellZ + 0xfff80000;

      if (Func_0802d7b0(object->x, topZ, *terrainId, object->terrainHeight) ==
          2) {
        object->y -= 0x10000;
      } else {
        Actor_SetMoveTarget(object, object->x, groundAhead, topZ);
        object->action = 1;
        CLIMBING(object) = heading;
        goto done;
      }
    }
    if (groundHere > y) {
      CLIMBING(object) = 0;
      object->terrainHeight = groundHere;
      object->motionFlags = 3;
      goto done;
    }

    collision = 0;
    // DEAD: the debug read of the held keys is never used.
    if (gDebugMode != 0)
      (void)*(volatile u32 *)&gInput.held;
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
      Actor_SelectAnimation(object, 3);
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
      Actor_SetMoveTarget(object, position.x, position.y, position.z);
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
        Actor_StartScript(effect, gData_0802ed28);
        effect->update = Func_08029810;
        effect->motionFlags = 2;
        if (collision == 0)
          Sound_PlayFar(596);
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
  if (gDebugMode != 0 && (gInput.held & 0x200)) {
    heading = (s16)angle;
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
          if (!Func_0802d87c(object, &testPosition)) {
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
    cellX = (position.x & 0xfff00000) + 0x80000;
    cellZ = (position.z & 0xfff00000) + 0xfff80000;

    if (Func_080c8770(cellX, cellZ, 2)) {
      object->motionFlags = 0;
      object->y += 0x10000;
      object->velocityX = 0;
      object->velocityY = 0;
      object->velocityZ = 0;
      Actor_SelectAnimation(object, 3);
      CLIMBING(object) = 1;
      goto done;
    }
  }
  // Down at a drop: climb down the face south of the cell.
  if (gInput.held & 0x80) {
    cellX = (object->x & 0xfff00000) + 0x80000;
    cellZ = (object->z & 0xfff00000) + 0x180000;

    if (Func_0802d45c(*terrainId, cellX, cellZ) < y &&
        Func_080c8770(cellX, cellZ, 2)) {
      object->motionFlags = 0;
      Actor_SetMoveTarget(object, object->x, groundHere + 0xfffe0000, cellZ);
      object->action = 1;
      CLIMBING(object) = 1;
      object->velocityX = 0;
      object->velocityY = 0;
      object->velocityZ = 0;
      Actor_SelectAnimation(object, 3);
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
  Actor_SelectAnimation(object, 1);
  if (collision != 0) {
    object->targetX = 0x80000000;
    object->targetY = 0x80000000;
    object->targetZ = 0x80000000;
    object->velocityX = 0;
    object->velocityZ = 0;
    if (collision & 3) {
      s32 difference;
      u16 turn = angle;

      difference = (s16)(turn - object->angle);
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

    Actor_SetMoveTarget(object, position.x, position.y, position.z);
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
  // The retest reads through the object, not wakeTimer: that keeps its
  // zero path from skipping it.
  if (((u8 *)object)[98] == 0 && collision == 0) {
    effect = Func_08023220(0x164, object->x, object->y, object->z - 0x20000);
    if (effect != NULL) {
      effect->terrainHeight = object->terrainHeight;
      sprite = effect->animation;
      Actor_StartScript(effect, gData_0802ece0);
      effect->priorityFlags = 8;
      ((u8 *)effect)[99] = 100;
      effect->motionFlags = collision;
      Sound_PlayFar(596);
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
