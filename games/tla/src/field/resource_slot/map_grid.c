/// @file
/// Moving on the world map by the pad, and decoding a map's tilemaps and
/// palette.
// Part of resource_slot.c.

#include "../resource_slot_internal.h"
#include "module/field/field.h"
#include "module/field/map.h"
#include "module/field/object.h"
#include "module/field/resource_slot.h"
#include "ram_buffer.h"

/* VIEW(f0298c0): tag struct PlayerSprite as struct PlayerSprite__f0298c0;
 * resource_slot.c has another definition; tried with the definition the rest of
 * the object has, it does not compile in tla-ja:
 * games/tla/src/field/resource_slot.c:7305: structure has no member named
 * `entry'. */
struct PlayerSprite__f0298c0 {
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

void Actor_SetMoveTarget(struct Actor *actor, s32 x, s32 y, s32 z);
void Actor_StartScript(struct Actor *, const s32 *);
/* VIEW(f0298c0): routine Animation_ApplyChildArgument as s32
 * Animation_ApplyChildArgument(struct PlayerSprite__f0298c0 *, s32); s28534 has
 * s32 Animation_ApplyChildArgument(struct PlayerSprite *, s32). */
extern s32 Animation_ApplyChildArgument__f0298c0(
    struct PlayerSprite__f0298c0 *,
    s32) __asm__("Animation_ApplyChildArgument");
/* VIEW(f0298c0): routine Func_08029810 as void Func_08029810(void); the
 * definition has void Func_08029810(struct Actor *object). */
extern void Func_08029810__f0298c0(void) __asm__("Func_08029810");
/* VIEW(f0298c0): routine Func_08029838 as s32 Func_08029838(struct Actor
 * *actor); the definition has s32 Func_08029838(s32 arg). */
extern s32 Func_08029838__f0298c0(struct Actor *actor) __asm__("Func_08029838");

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
  struct PlayerSprite__f0298c0 *sprite;

  heading = 0;
  count = 0;
  cellX = actor->x & 0xfff00000;
  cellZ = actor->z & 0xfff00000;
  y = actor->y;
  terrainId = &actor->terrainId;
  groundHere = MapGrid_GetTerrainHeight(*terrainId, cellX, cellZ);
  groundAhead = MapGrid_GetTerrainHeight(*terrainId, cellX, cellZ + 0xfff00000);
  if (actor->timer != 0) {
    if (actor->targetX != 0x80000000)
      goto done;
    actor->timer = 0;
    actor->terrainHeight = groundHere;
  }
  if ((s16)actor->unk66 != 0) {
    if (groundAhead < y) {
      s32 topZ = cellZ + 0xfff80000;

      if (MapGrid_CheckCollisionAt(actor->x, topZ, *terrainId,
                                   actor->terrainHeight) == 2) {
        actor->y -= 0x10000;
      } else {
        Actor_SetMoveTarget(actor, actor->x, groundAhead, topZ);
        actor->timer = 1;
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
      Vector_AddPolarOffset__resource_slot(0x80000, (u16)angle, &position);
      position.y = actor->y - position.z;
      position.z = actor->z;
      Actor_SelectAnimation(actor, 3);
      testPosition.x = actor->x;
      testPosition.y = 0;
      testPosition.z = 0;
      Vector_AddPolarOffset__resource_slot(0x80000, (u16)angle, &testPosition);
      testPosition.y = actor->y - testPosition.z;
      testPosition.z = actor->z;
      if (MapGrid_CheckCollision(actor, &testPosition))
        goto climb_blocked;
      testPosition.x = actor->x;
      testPosition.y = 0;
      testPosition.z = 0;
      Vector_AddPolarOffset__resource_slot(0x80000, (u16)angle + 0x1000,
                                           &testPosition);
      testPosition.y = actor->y - testPosition.z;
      testPosition.z = actor->z;
      if (MapGrid_CheckCollision(actor, &testPosition))
        goto climb_blocked;
      testPosition.x = actor->x;
      testPosition.y = 0;
      testPosition.z = 0;
      Vector_AddPolarOffset__resource_slot(0x80000, (u16)angle - 0x1000,
                                           &testPosition);
      testPosition.y = actor->y - testPosition.z;
      testPosition.z = actor->z;
      if (MapGrid_CheckCollision(actor, &testPosition))
        goto climb_blocked;
      testPosition.x = actor->x;
      testPosition.y = 0;
      testPosition.z = 0;
      Vector_AddPolarOffset__resource_slot(0x80000, (u16)angle + 0x2000,
                                           &testPosition);
      testPosition.y = actor->y - testPosition.z;
      testPosition.z = actor->z;
      if (MapGrid_CheckCollision(actor, &testPosition))
        goto climb_blocked;
      testPosition.x = actor->x;
      testPosition.y = 0;
      testPosition.z = 0;
      Vector_AddPolarOffset__resource_slot(0x80000, (u16)angle - 0x2000,
                                           &testPosition);
      testPosition.y = actor->y - testPosition.z;
      testPosition.z = actor->z;
      if (!MapGrid_CheckCollision(actor, &testPosition))
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
      Vector_AddPolarOffset__resource_slot(
          speed, (u16)heading, (struct FieldPosition *)&actor->velocityX);
      actor->velocityY = -actor->velocityZ;
      actor->velocityZ = 0;
    }
    // Every 16th frame, a bit of rubble drops beside the climber.
    if ((gFrameCount & 15) == 0) {
      spawn.x = actor->x - Random16() * 6 + 0x30000;
      spawn.y = actor->y;
      spawn.z = actor->z;
      effect = Actor_Create(0x11d, spawn.x, spawn.y, spawn.z);
      if (effect != NULL) {
        effect->terrainHeight = actor->terrainHeight;
        sprite = effect->sprite;
        Actor_StartScript(effect, gData_0802ed28);
        effect->update = Func_08029810__f0298c0;
        effect->motionFlags = 2;
        if (collision == 0)
          Sound_PlayFar(596);
        effect->gravity = 0xa3d;
        effect->velocityY = 0;
        effect->scaleX = 0x4ccc;
        effect->scaleY = 0x4ccc;
        if (sprite != NULL) {
          Animation_ApplyChildArgument__f0298c0(sprite, 2);
          sprite->flags = 0;
          sprite->priority = 0;
        }
      }
    }
    Func_08029838__f0298c0(actor);
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
  Vector_AddPolarOffset__resource_slot(0x80000, (u16)angle, &position);
  if (gDebugMode != 0 && (gInput.held & 0x200)) {
    heading = (s16)angle;
    goto counted;
  }
  if (!MapGrid_CheckCollision(actor, &position)) {
    testPosition.x = actor->x;
    testPosition.y = actor->y;
    testPosition.z = actor->z;
    Vector_AddPolarOffset__resource_slot(0x80000, (u16)angle + 0x1000,
                                         &testPosition);
    if (!MapGrid_CheckCollision(actor, &testPosition)) {
      testPosition.x = actor->x;
      testPosition.y = actor->y;
      testPosition.z = actor->z;
      Vector_AddPolarOffset__resource_slot(0x80000, (u16)angle - 0x1000,
                                           &testPosition);
      if (!MapGrid_CheckCollision(actor, &testPosition)) {
        testPosition.x = actor->x;
        testPosition.y = actor->y;
        testPosition.z = actor->z;
        Vector_AddPolarOffset__resource_slot(0x80000, (u16)angle + 0x2000,
                                             &testPosition);
        if (!MapGrid_CheckCollision(actor, &testPosition)) {
          testPosition.x = actor->x;
          testPosition.y = actor->y;
          testPosition.z = actor->z;
          Vector_AddPolarOffset__resource_slot(0x80000, (u16)angle - 0x2000,
                                               &testPosition);
          if (!MapGrid_CheckCollision(actor, &testPosition)) {
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
    Vector_AddPolarOffset__resource_slot(0x80000, (u16)heading, &position);
    if (MapGrid_CheckCollision(actor, &position))
      continue;
    testPosition.x = actor->x;
    testPosition.y = actor->y;
    testPosition.z = actor->z;
    Vector_AddPolarOffset__resource_slot(0x80000, (u16)heading + 0x1000,
                                         &testPosition);
    if (MapGrid_CheckCollision(actor, &testPosition))
      continue;
    testPosition.x = actor->x;
    testPosition.y = actor->y;
    testPosition.z = actor->z;
    Vector_AddPolarOffset__resource_slot(0x80000, (u16)heading - 0x1000,
                                         &testPosition);
    if (MapGrid_CheckCollision(actor, &testPosition))
      continue;
    testPosition.x = actor->x;
    testPosition.y = actor->y;
    testPosition.z = actor->z;
    Vector_AddPolarOffset__resource_slot(0x80000, (u16)heading + 0x2000,
                                         &testPosition);
    if (MapGrid_CheckCollision(actor, &testPosition))
      continue;
    testPosition.x = actor->x;
    testPosition.y = actor->y;
    testPosition.z = actor->z;
    Vector_AddPolarOffset__resource_slot(0x80000, (u16)heading - 0x2000,
                                         &testPosition);
    if (!MapGrid_CheckCollision(actor, &testPosition))
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

    if (MapGrid_GetTerrainHeight(*terrainId, cellX, cellZ) < y &&
        Func_080c8770(cellX, cellZ, 2)) {
      actor->motionFlags = 0;
      Actor_SetMoveTarget(actor, actor->x, groundHere + 0xfffe0000, cellZ);
      actor->timer = 1;
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

      difference = (s16)(turn - actor->facing);
      if (difference > 0x1000)
        difference = 0x1000;
      if (difference < -0x1000)
        difference = -0x1000;
      actor->facing += difference;
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
    Vector_AddPolarOffset__resource_slot(
        speed, (u16)heading, (struct FieldPosition *)&actor->velocityX);
    wakeTimer = (u8 *)actor + 98;
    if (*wakeTimer != 0)
      (*wakeTimer)--;
  }
  // The retest reads through the actor, not wakeTimer: that keeps its
  // zero path from skipping it.
  if (((u8 *)actor)[98] == 0 && collision == 0) {
    effect = Actor_Create(0x164, actor->x, actor->y, actor->z - 0x20000);
    if (effect != NULL) {
      effect->terrainHeight = actor->terrainHeight;
      sprite = effect->sprite;
      Actor_StartScript(effect, gData_0802ece0);
      effect->priorityFlags = 8;
      // Through a struct member: as a byte offset the store holds back the
      // stack read of collision after it.
      ((struct Unk_WakeActor *)effect)->unk63 = 100;
      effect->motionFlags = 0;
      Sound_PlayFar(596);
      if (sprite != NULL) {
        struct Unk_ClimbEventWork *work = Ram_HeapSlots->eventWork;

        Animation_ApplyChildArgument__f0298c0(sprite, 1);
        sprite->flags = 0;
        // DEAD: overwritten by the next line.
        sprite->priority = 2;
        sprite->priority = work->unkd4a & 3;
      }
      *wakeTimer = 5;
    }
  }
  Func_08029838__f0298c0(actor);

done:
  actor->step++;
  return 1;
}

s32 WorldMap_IsWalkable(struct Actor *object, struct FieldPosition *position);

/// Script command 0x22: moves the object on the world map by the direction pad
/// at a fixed pace (faster while L repeats), trying six turned headings when
/// the way is blocked, as Actor_MoveOnWorldMap does; records the held keys in
/// the event work and leaves an object of kind 0x164 behind it every fifth
/// step.
s32 Func_0802a124(struct Actor *object) {
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
  s32 y;
  s32 terrainCollision;
  s32 speed;
  s32 targetCameraYaw;
  struct WorldCamera__resource_slot *camera;
  s32 yawStep;
  struct PlayerSprite__f0298c0 *playerSprite;
  struct Actor *ripple;
  struct PlayerSprite__f0298c0 *rippleSprite;
  struct EventWork *work;
  u8 *timer;

  collision = 0;
  count = 0;
  object->unk66 = collision;
  object->timer = 1;
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
    Vector_AddPolarOffset__resource_slot(0x80000, direction, &position);
    if (gDebugMode == 0 || (gInput.held & 0x200) == 0) {
      leftPosition.x = object->x;
      leftPosition.y = object->y;
      leftPosition.z = object->z;
      Vector_AddPolarOffset__resource_slot(0x80000, direction + 0x1555,
                                           &leftPosition);
      rightPosition.x = object->x;
      /* FAKEMATCH(temporary): y read into its own local before the block; as
       * object->y read in the block, 2 of 470 instructions differ in tla-en. */
      y = object->y;
      // The right probe's y stored in a once-only block; as the plain store,
      // 50 of 470 instructions differ in tla-en.
      do {
        rightPosition.y = y;
      } while (0);
      rightPosition.z = object->z;
      Vector_AddPolarOffset__resource_slot(0x80000, direction - 0x1555,
                                           &rightPosition);
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
          Vector_AddPolarOffset__resource_slot(0x80000, direction, &position);
          leftPosition.x = object->x;
          leftPosition.y = object->y;
          leftPosition.z = object->z;
          Vector_AddPolarOffset__resource_slot(0x80000, direction + 0x1555,
                                               &leftPosition);
          rightPosition.x = object->x;
          rightPosition.y = object->y;
          rightPosition.z = object->z;
          Vector_AddPolarOffset__resource_slot(0x80000, direction - 0x1555,
                                               &rightPosition);
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
    work->heldKeys = gInput.held;
  }
  Actor_SelectAnimation(object, 1);

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
    timer = (u8 *)object + 98;
    *timer = 0;
    object->unk66 = 2;
  } else {
    Actor_SetMoveTarget(object, position.x, position.y, position.z);
    speed = FixedSqrt(Iwram_MulQ16(object->velocityX, object->velocityX) +
                      Iwram_MulQ16(object->velocityZ, object->velocityZ));
    object->velocityX = collision;
    object->velocityZ = collision;
    Vector_AddPolarOffset__resource_slot(
        speed, (u16)angle, (struct FieldPosition *)&object->velocityX);
    timer = (u8 *)object + 98;
    if (*timer != 0)
      (*timer)--;
  }

  camera = (struct WorldCamera__resource_slot *)Ram_HeapSlots->mapWork;
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
    if (terrainCollision == 9)
      playerSprite->entry[6] = 1;
    else
      playerSprite->entry[6] = 9;
  }
  if (*timer == 0 && collision == 0) {
    ripple = Actor_Create(0x164, object->x, object->y, object->z - 0x20000);
    if (ripple != 0) {
      ripple->terrainHeight = object->terrainHeight;
      rippleSprite = ripple->sprite;
      Actor_StartScript(ripple, gData_0802ece0);
      ripple->priorityFlags = 8;
      ((struct Unk0802a124Actor *)ripple)->unk63 = 100;
      ripple->motionFlags = collision;
      if (rippleSprite != 0) {
        Animation_ApplyChildArgument__f0298c0(rippleSprite, 1);
        rippleSprite->flags = collision;
        rippleSprite->priority = 2;
        rippleSprite->priority = 1;
      }
      *timer = 5;
    }
  }
  Func_08029838__f0298c0(object);
  object->step++;
  return 1;
}

void SpriteColor_RemapPixels(u8 *buf, s32 cnt) {
  s32 n;
  u8 *p;
  u8 *table;

  p = buf;
  table = gSpriteColorRemapTables;
  n = cnt - 1;
  if (n != -1) {
    do {
      u32 index = *p;

      n -= 1;
      *p = table[index];
      p += 1;
    } while (n != -1);
  }
}

// Map: unpack decoded block data into gMapBlocks.

/// Decoded buffer [1] selects the method; halfword data starts at [2].
/// size: length including the method byte (even means no data).
/// Method 0: copy unchanged.
/// Method 1: high bytes followed by low bytes; reconstruct by XOR with prior
/// values. Method 2: reconstruct by XOR with prior values.
void TileMap_DecodeStagedBuffer(s32 size) {
  u16 *dst;
  u16 *src;
  s32 n;
  s32 i;

  src = (u16 *)(Ram_MapCellBuffer + 2);
  dst = (u16 *)gMapBlocks;
  n = (size - 1) / 2;
  if (size & 1) {
    switch (((u8 *)gMapCellBuffer)[1]) {
    case 0:
      for (i = 0; i < n; i++)
        *dst++ = *src++;
      break;
    case 1: {
      u32 prev = 0;
      u8 *hi = Ram_MapCellBuffer + 2;
      u8 *lo = hi + n;

      for (i = 0; i < n; i++) {
        u32 v = ((*hi++ << 8) | *lo++) ^ prev;
        *dst++ = v;
        prev = v;
      }
      break;
    }
    case 2: {
      s32 prev = 0;

      for (i = 0; i < n; i++) {
        s32 v = *src++ ^ prev;
        *dst++ = v;
        prev = v;
      }
      break;
    }
    }
  }
}

/// Converts the map cell buffer's 32 KB in place with the ARM routine copied
/// to RAM, keeping a copy of the buffer for it to read.
void Tilemap_ConvertBuffer(void) {
  u8 *saved;
  ConvertFn routine;
  u32 size;

  size = 0x8000;
  saved = Heap_AllocEwram(size);
  Iwram_CopyWords(saved, Ram_MapCellBuffer, size);
  RamCode_Load(routine, Render_ConvertMapCode, Render_ConvertMapCodeSize);
  routine(((u8 *)gMapCellBuffer + 0x8000), Ram_MapCellBuffer, saved);
  Heap_Release(routine);
  Heap_Release(saved);
}

/// Decodes the palette resource of a map's header row (its second halfword)
/// and copies its 224 colors to destination, keeping the current backdrop
/// color in the first entry.
void TileMap_ConvertBuffer(s32 map, void *destination) {
  const u16 *entry;
  u16 *buffer;
  s16 backdrop;

  entry = gMaps[map];
  buffer = Heap_Alloc(0x200);
  backdrop = *(s16 *)0x05000000;
  Decompress_TaggedLz(
      Resource_GetTableEntry(entry[1] + (u32)&ResourceId_Map000), buffer);
  buffer[0] = backdrop;
  Dma_Set(buffer, destination, DMA_COPY_WORDS(112), REG_DMA3);
  Heap_Release(buffer);
}
