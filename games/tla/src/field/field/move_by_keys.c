/// @file
/// Moving actors by the direction pad: the party's keys, crossing a rope or a
/// tightrope, and moving as Sand, on a map and on the world map.
// Part of field.c.

#include "../field_internal.h"
#include "fixed_math.h"
#include "sounds.h"
#include "gamepak/far_call/field_sequence.h"
#include "module/field/field.h"
#include "module/field/resource_slot.h"
#include "macros.h"
#include "hanging_vine.h"
#include "flag_call.h"
#include "module/field/map.h"
#include "module/field/object.h"
#include "system/ram_buffer.h"

/// The heading of each direction-pad combination, 0xffff for none.

/// The script command 0x1b: moves an object 8 pixels at a time by the
/// direction pad, with up and down changing its height (y) rather than its
/// depth. Refuses steps into an occupied cell, from or onto a cell without
/// attribute bit 6, or onto ground too high (up) or too low (down), and
/// records the held keys in the event work. Returns 1.
s32 Actor_MoveByKeys(struct Actor *object) {
  struct FieldPosition position;
  struct MapCell *sourceCell;
  struct MapCell *destinationCell;
  struct FieldWork *work;
  u16 angle;
  s32 animation;
  s32 collisionFlags;
  s32 deltaZ;
  s32 terrainHeight;
  struct MapWork *map;
  u32 keys;
  s32 half;

  object->acceleration = px(1) / 4;
  animation = 12;
  half = 0x8000;
  object->speedLimit = half;
  if (gDebugMode)
    keys = gInput.held;
  keys = gInput.held;
  {
    const u16 *table = gWorldMap_KeyHeadings;

    angle = *(const u16 *)((const u8 *)table + (((keys >> 4) & 15) << 1));
  }
  if (angle == 0xffff) {
    collisionFlags = 4;
  } else {
    animation = 14;
    if ((angle & 0xf000) != 0) {
      animation = 15;
      if ((angle & 0xf000) != half)
        animation = 10;
    }
    collisionFlags = 0;
    SET_POSITION(&position, 0, 0, 0);
    Vector_AddPolarOffset(px(8), angle, &position);
    position.x += object->x;
    deltaZ = position.z;
    if (deltaZ < 0)
      object->facing = TRIG_TURN * 3 / 4;
    if (deltaZ > 0)
      object->facing = TRIG_TURN / 4;
    position.y = object->y - position.z;
    position.z = object->z;
    map = Ram_HeapSlots->mapWork;
    sourceCell =
        &map->layers[object->terrainId].cells[Actor_GetCellIndexFar(object)];
    destinationCell =
        &map->layers[object->terrainId]
             .cells[position.x / 0x100000 + (position.z / 0x100000) * 128];
    if (MapGrid_IsWallAt((s32 *)&position) != 0 ||
        !(sourceCell->attributeB & 64) || !(destinationCell->attributeB & 64)) {
      collisionFlags = 4;
      animation = 12;
    } else if (gInput.held & 64) {
      terrainHeight = MapGrid_GetTerrainHeight(object->terrainId, position.x,
                                               position.z - 0x100000);
      if (terrainHeight - object->y < 0x100000)
        goto refused;
    } else if (gInput.held & 128) {
      terrainHeight =
          MapGrid_GetTerrainHeight(object->terrainId, position.x, position.z);
      if (terrainHeight - object->y > -0x80000) {
      refused:
        collisionFlags = 1;
        animation = 12;
      }
    }
  }
  work = Ram_HeapSlots->fieldWork;
  if (work != NULL) {
    if (collisionFlags & 3)
      work->blockedSteps++;
    else
      work->blockedSteps = 0;
    work->heldKeys = gInput.held;
  }
  Actor_SelectAnimation(object, animation);
  if (collisionFlags != 0) {
    object->targetX = TARGET_UNSET;
    object->targetY = TARGET_UNSET;
    object->targetZ = TARGET_UNSET;
    object->velocityX = 0;
    object->velocityY = 0;
    object->velocityZ = 0;
  } else {
    Actor_SetMoveTarget(object, position.x, position.y, position.z);
  }
  object->step++;
  return 1;
}

/// Script command 0x1c, the leader's control on a tied rope (render mode 3):
/// the pad moves it from node to node along the rope, a half cell a step.
s32 Actor_CrossRope(struct Actor *object) {
  struct FieldPosition position;
  s32 collisionFlags;
  struct SpriteAngle *sprite;
  struct RopeWork *rope;
  s32 bestDistance;
  s32 value;
  s32 speed;
  struct RopeNode *best;
  struct RopeNode *node;
  struct RopeSlot *slot;
  s32 count;
  u16 angle;
  s32 turn;
  struct Actor *first;
  struct Actor *second;
  struct FieldWork *work;

  sprite = object->sprite;
  rope = *(struct RopeWork **)&Ram_HeapSlots->unk84[6];
  speed = 0x10000;
  collisionFlags = 0;
  object->speedLimit = px(1) / 2;
  object->acceleration = px(1) / 2;
  object->motionFlags = 0;
  angle = gWorldMap_KeyHeadings[(gInput.held >> 4) & 15];
  if (angle == 0xffff) {
    best = (struct RopeNode *)object->watchedObject;
    value = 0;
  } else {
    value = 16;
    SET_POSITION(&position, 0, 0, 0);
    Vector_AddPolarOffset(px(8), angle, &position);
    position.x += object->x;
    position.y = object->y - position.z;
    position.z = object->z;
    rope = *(struct RopeWork **)&Ram_HeapSlots->unk84[6];
    node = rope->nodes;
    best = NULL;
    bestDistance = 0x10000000;
    if (rope == NULL) {
      collisionFlags = 1;
      /* FAKEMATCH(dead): the event work slot copied onto itself. */
      Ram_HeapSlots->fieldWork = Ram_HeapSlots->fieldWork;
      goto done;
    }
    for (count = 0; count < 128; count++, node++) {
      if (node->kind == 1) {
        s32 dx = (position.x - node->x) >> 8;
        s32 dy = (position.y - node->y) >> 8;
        s32 dz = (position.z - node->z) >> 8;
        s32 distance =
            Iwram_MulQ16(dx, dx) + Iwram_MulQ16(dy, dy) + Iwram_MulQ16(dz, dz);

        if (distance <= 0x400 && bestDistance > distance) {
          bestDistance = distance;
          best = node;
        }
      }
    }
  }
  if (best == NULL) {
    // DEAD: collisionFlags is still 0 here, so the OR reads nothing; the source
    // keeps it.
    collisionFlags |= 1;
  } else {
    slot = &rope->slots[best->slot - 1];
    first = Actor_GetByIdFar(slot->first);
    second = Actor_GetByIdFar(slot->second);
    if (first->x > second->x)
      speed = -speed;
    first = Actor_GetByIdFar(slot->first);
    second = Actor_GetByIdFar(slot->second);
    if (first->y > second->y) {
      speed = -speed;
      turn = 0xc000;
    } else {
      turn = 0x4000;
    }
    object->scaleX = speed;
    if (best->x == object->x && best->y == object->y && best->z == object->z) {
      collisionFlags = 1;
    } else {
      slot->swinging = 1;
      slot->amplitude += 0x8000;
      if (slot->amplitude > 0x20000)
        slot->amplitude = 0x20000;
      object->watchedObject = (struct Actor *)best;
      SET_POSITION(&position, best->x, best->y, best->z);
      sprite->angle = best->angle + turn;
    }
  }
done:
  work = Ram_HeapSlots->fieldWork;
  if (work != NULL) {
    if (collisionFlags & 3)
      work->blockedSteps++;
    else
      work->blockedSteps = 0;
    work->heldKeys = gInput.held;
  }
  Actor_SelectAnimation(object, 29);
  Actor_SetAnimationSpeed(object, value);
  if (collisionFlags != 0) {
    object->targetX = TARGET_UNSET;
    object->targetY = TARGET_UNSET;
    object->targetZ = TARGET_UNSET;
    object->velocityX = 0;
    object->velocityY = 0;
    object->velocityZ = 0;
  } else {
    Actor_SetMoveTarget(object, position.x, position.y, position.z);
  }
  object->step++;
  return 1;
}

static inline void TurnToward(struct Actor *object, u16 angle) {
  s16 delta = angle - object->facing;

  if (delta > 0x1000)
    delta = 0x1000;
  if (delta < -0x1000)
    delta = -0x1000;
  object->facing += delta;
}

/// Script command 0x1d, Actor_CrossRope on a tightrope (render mode 4).
s32 Actor_CrossTightrope(struct Actor *object) {
  struct FieldPosition position;
  s32 collisionFlags;
  struct ObjectAnimationState *sprite;
  s32 argument;
  struct TightropeWork *tightrope;
  struct TightropeNode *best;
  struct TightropeNode *point;
  struct TightropeSlot *slot;
  s32 bestDistance;
  u16 angle;
  s32 count;
  struct FieldWork *work;

  sprite = object->sprite;
  argument = 1;
  collisionFlags = 0;
  object->speedLimit = px(1) / 2;
  object->acceleration = px(1) / 2;
  object->motionFlags = 0;
  angle = gWorldMap_KeyHeadings[(gInput.held >> 4) & 15];
  if (angle == 0xffff) {
    collisionFlags = 4;
    best = (struct TightropeNode *)object->watchedObject;
    SET_POSITION(&position, object->x, best->y, object->z);
  } else {
    SET_POSITION(&position, object->x, object->y, object->z);
    Vector_AddPolarOffset(px(8), angle, &position);
    tightrope = *(struct TightropeWork **)&Ram_HeapSlots->unk84[7];
    point = tightrope->nodes;
    best = NULL;
    bestDistance = 0x10000000;
    if (tightrope == NULL) {
      collisionFlags = 1;
      /* FAKEMATCH(dead): the event work slot copied onto itself. */
      Ram_HeapSlots->fieldWork = Ram_HeapSlots->fieldWork;
    } else {
      for (count = 0; count < 128; count++, point++) {
        if (point->kind == 1) {
          s32 dx = (position.x - point->x) >> 8;
          s32 dy = (position.y - point->y) >> 8;
          s32 dz = (position.z - point->z) >> 8;
          s32 distance = Iwram_MulQ16(dx, dx) + Iwram_MulQ16(dy, dy) +
                         Iwram_MulQ16(dz, dz);

          if (distance <= 0x400 && bestDistance > distance) {
            bestDistance = distance;
            best = point;
          }
        }
      }
      if (best == NULL) {
        collisionFlags |= 1;
      } else if (best->x == object->x && best->y == object->y &&
                 best->z == object->z) {
        collisionFlags = 1;
      } else {
        if (best->slot != 0) {
          slot = &tightrope->slots[best->slot - 1];
          slot->wobbling = 1;
          slot->amplitude += 0x8000;
          if (slot->amplitude > 0x20000)
            slot->amplitude = 0x20000;
          object->watchedObject = (struct Actor *)best;
        }
        /* FAKEMATCH(dead): position.x copied onto itself before the copies. */
        position.x = position.x;
        SET_POSITION(&position, best->x, best->y, best->z);
      }
    }
  }
  work = Ram_HeapSlots->fieldWork;
  if (work != NULL) {
    if (collisionFlags & 3)
      work->blockedSteps++;
    else
      work->blockedSteps = 0;
    work->heldKeys = gInput.held;
  }
  if (collisionFlags == 0)
    argument = 2;
  Actor_SelectAnimation(object, argument);
  Actor_SetAnimationSpeed(object, 8);
  sprite->unk00[23] = 4;
  if (collisionFlags != 0) {
    object->targetX = TARGET_UNSET;
    object->targetY = TARGET_UNSET;
    object->targetZ = TARGET_UNSET;
    object->velocityX = 0;
    object->velocityY = 0;
    object->velocityZ = 0;
    if (collisionFlags & 3)
      TurnToward(object, angle);
    object->y = position.y;
  } else {
    object->y = position.y;
    Actor_SetMoveTarget(object, position.x, position.y, position.z);
    if (object->x == position.x && object->z == position.z)
      TurnToward(object, angle);
  }
  object->step++;
  return 1;
}

#define HALF_AT(base, offset) (*(u16 *)((u8 *)(base) + (offset)))

/// Update of a grain the Sand leaves as the leader moves: it turns and
/// jitters sideways at random.
void Sand_JitterGrain(struct Actor *object) {
  u32 first;
  u32 second;
  s32 x;

  HALF_AT(object->sprite, 18) += 0x400;
  first = Random16();
  second = Random16();
  x = object->x;

  second >>= 1;
  first >>= 1;
  object->x = x + (first - second);
}

/// When one of three party key sets is pressed: plays cue 113 if the far call
/// accepts the argument, else posts event request 0x208b and returns 1 while
/// flag 0x146 is clear.
s32 Sand_CheckEndKeys(s32 arg) {
  struct FieldWork *work = Ram_HeapSlots->fieldWork;
  /* FAKEMATCH(pin): pressed held in r0, the register the early zero is returned
   * in. */
  register s32 pressed asm("r0") = 0;

  if (work == NULL)
    return 0;
  if ((gInput.pressed & gMainRoutineWork.selectKeys) ||
      (gInput.pressed & gMainRoutineWork.aKeys) ||
      (gInput.pressed & gMainRoutineWork.startKeys))
    pressed = 1;
  if (pressed) {
    if (Actor_CheckBlockedFar(arg)) {
      Sound_PlayFar(SE_MENU_CANCEL);
    } else if (!GameFlag_Get(0x146)) {
      work->psynergy = 0x208b;
      return 1;
    }
  }
  return 0;
}
#undef gInput.held
#undef CHAIN_WORK
#undef HALF_AT

// The heading of each direction-pad combination, -1 for none.

/// Script command 0x1e (control mode 5): the leader turned to sand by the Sand
/// Psynergy, moved by the direction pad on a field map.
s32 Actor_MoveAsSand(struct Actor *actor) {
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
  groundHere = MapGrid_GetTerrainHeight(*terrainId, cellX, cellZ);
  groundAhead = MapGrid_GetTerrainHeight(*terrainId, cellX, cellZ + 0xfff00000);
  if (actor->timer != 0) {
    if (actor->targetX != TARGET_UNSET)
      goto done;
    actor->timer = 0;
    actor->terrainHeight = groundHere;
  }
  if ((s16)actor->userValue != 0) {
    if (groundAhead < y) {
      s32 topZ = cellZ + 0xfff80000;

      if (MapGrid_CheckCollisionAt(actor->x, topZ, *terrainId,
                                   actor->terrainHeight) == 2) {
        actor->y -= px(1);
      } else {
        Actor_SetMoveTarget(actor, actor->x, groundAhead, topZ);
        actor->timer = 1;
        actor->userValue = 0;
        goto done;
      }
    }
    if (groundHere > y) {
      actor->userValue = 0;
      actor->terrainHeight = groundHere;
      actor->motionFlags = 3;
      goto done;
    }

    collision = 0;
    // DEAD: the debug read of the held keys is never used.
    if (gDebugMode != 0)
      (void)gInput.held;
    actor->speedLimit = px(1) / 2;
    actor->acceleration = px(1) / 4;
    angle = gWorldMap_KeyHeadings[(gInput.held >> 4) & 15];
    if ((u16)angle == 0xffff) {
      collision = 4;
    } else {
      SET_POSITION(&position, actor->x, 0, 0);
      Vector_AddPolarOffset(px(8), (u16)angle, &position);
      position.y = actor->y - position.z;
      position.z = actor->z;
      Actor_SelectAnimation(actor, 3);
      SET_POSITION(&testPosition, actor->x, 0, 0);
      Vector_AddPolarOffset(px(8), (u16)angle, &testPosition);
      testPosition.y = actor->y - testPosition.z;
      testPosition.z = actor->z;
      if (MapGrid_CheckCollision(actor, &testPosition))
        goto climb_blocked;
      SET_POSITION(&testPosition, actor->x, 0, 0);
      Vector_AddPolarOffset(px(8), (u16)angle + TRIG_TURN / 16, &testPosition);
      testPosition.y = actor->y - testPosition.z;
      testPosition.z = actor->z;
      if (MapGrid_CheckCollision(actor, &testPosition))
        goto climb_blocked;
      SET_POSITION(&testPosition, actor->x, 0, 0);
      Vector_AddPolarOffset(px(8), (u16)angle - TRIG_TURN / 16, &testPosition);
      testPosition.y = actor->y - testPosition.z;
      testPosition.z = actor->z;
      if (MapGrid_CheckCollision(actor, &testPosition))
        goto climb_blocked;
      SET_POSITION(&testPosition, actor->x, 0, 0);
      Vector_AddPolarOffset(px(8), (u16)angle + TRIG_TURN / 8, &testPosition);
      testPosition.y = actor->y - testPosition.z;
      testPosition.z = actor->z;
      if (MapGrid_CheckCollision(actor, &testPosition))
        goto climb_blocked;
      SET_POSITION(&testPosition, actor->x, 0, 0);
      Vector_AddPolarOffset(px(8), (u16)angle - TRIG_TURN / 8, &testPosition);
      testPosition.y = actor->y - testPosition.z;
      testPosition.z = actor->z;
      if (!MapGrid_CheckCollision(actor, &testPosition))
        goto climb_moved;
    climb_blocked:
      SET_POSITION(&position, actor->x, actor->y, actor->z);
      collision |= 1;
    }
  climb_moved:
    if (collision != 0) {
      actor->targetX = TARGET_UNSET;
      actor->targetY = TARGET_UNSET;
      actor->targetZ = TARGET_UNSET;
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
      SET_POSITION(&spawn, actor->x - Random16() * 6 + 0x30000, actor->y,
                   actor->z);
      effect = Actor_Create(0x11d, spawn.x, spawn.y, spawn.z);
      if (effect != NULL) {
        effect->terrainHeight = actor->terrainHeight;
        sprite = effect->sprite;
        Actor_StartScript(effect, gSand_GrainScript);
        effect->update = Sand_JitterGrain;
        effect->motionFlags = 2;
        if (collision == 0)
          Sound_PlayFar(SE_596);
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
    Sand_CheckEndKeys(actor);
    goto done;
  }

  // On foot.
  collision = 0;
  actor->speedLimit = px(1);
  actor->acceleration = px(1) / 4;
  angle = gWorldMap_KeyHeadings[(gInput.held >> 4) & 15];
  if ((u16)angle == 0xffff) {
    collision = 4;
    goto counted;
  }
  SET_POSITION(&position, actor->x, actor->y, actor->z);
  Vector_AddPolarOffset(px(8), (u16)angle, &position);
  if (gDebugMode != 0 && (gInput.held & KEY_L)) {
    heading = (s16)angle;
    goto counted;
  }
  if (!MapGrid_CheckCollision(actor, &position)) {
    SET_POSITION(&testPosition, actor->x, actor->y, actor->z);
    Vector_AddPolarOffset(px(8), (u16)angle + TRIG_TURN / 16, &testPosition);
    if (!MapGrid_CheckCollision(actor, &testPosition)) {
      SET_POSITION(&testPosition, actor->x, actor->y, actor->z);
      Vector_AddPolarOffset(px(8), (u16)angle - TRIG_TURN / 16, &testPosition);
      if (!MapGrid_CheckCollision(actor, &testPosition)) {
        SET_POSITION(&testPosition, actor->x, actor->y, actor->z);
        Vector_AddPolarOffset(px(8), (u16)angle + TRIG_TURN / 8, &testPosition);
        if (!MapGrid_CheckCollision(actor, &testPosition)) {
          SET_POSITION(&testPosition, actor->x, actor->y, actor->z);
          Vector_AddPolarOffset(px(8), (u16)angle - TRIG_TURN / 8,
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

    angleOffsets[count++] = direction + TRIG_TURN / 16;
    angleOffsets[count++] = direction - TRIG_TURN / 16;
    angleOffsets[count++] = direction + TRIG_TURN / 8;
    angleOffsets[count++] = direction - TRIG_TURN / 8;
    angleOffsets[count++] = direction + TRIG_TURN * 3 / 16;
    angleOffsets[count++] = direction - TRIG_TURN * 3 / 16;
  }
  for (angleIndex = 0; angleIndex < count; angleIndex++) {
    heading = angleOffsets[angleIndex];
    SET_POSITION(&position, actor->x, actor->y, actor->z);
    Vector_AddPolarOffset(px(8), (u16)heading, &position);
    if (MapGrid_CheckCollision(actor, &position))
      continue;
    SET_POSITION(&testPosition, actor->x, actor->y, actor->z);
    Vector_AddPolarOffset(px(8), (u16)heading + TRIG_TURN / 16, &testPosition);
    if (MapGrid_CheckCollision(actor, &testPosition))
      continue;
    SET_POSITION(&testPosition, actor->x, actor->y, actor->z);
    Vector_AddPolarOffset(px(8), (u16)heading - TRIG_TURN / 16, &testPosition);
    if (MapGrid_CheckCollision(actor, &testPosition))
      continue;
    SET_POSITION(&testPosition, actor->x, actor->y, actor->z);
    Vector_AddPolarOffset(px(8), (u16)heading + TRIG_TURN / 8, &testPosition);
    if (MapGrid_CheckCollision(actor, &testPosition))
      continue;
    SET_POSITION(&testPosition, actor->x, actor->y, actor->z);
    Vector_AddPolarOffset(px(8), (u16)heading - TRIG_TURN / 8, &testPosition);
    if (!MapGrid_CheckCollision(actor, &testPosition))
      goto counted;
  }
  SET_POSITION(&position, actor->x, actor->y, actor->z);
  collision |= 1;
  // Up against a face more than 8 pixels high: start climbing it.
  if ((gInput.held & KEY_UP) && groundAhead > y + 0x80000) {
    cellX = (position.x & 0xfff00000) + 0x80000;
    cellZ = (position.z & 0xfff00000) + 0xfff80000;

    if (MapGrid_IsClimbableCellFar(cellX, cellZ, 2)) {
      actor->motionFlags = 0;
      actor->y += px(1);
      actor->userValue = 1;
      actor->velocityX = 0;
      actor->velocityY = 0;
      actor->velocityZ = 0;
      Actor_SelectAnimation(actor, 3);
      goto done;
    }
  }
  // Down at a drop: climb down the face south of the cell.
  if (gInput.held & KEY_DOWN) {
    cellX = (actor->x & 0xfff00000) + 0x80000;
    cellZ = (actor->z & 0xfff00000) + 0x180000;

    if (MapGrid_GetTerrainHeight(*terrainId, cellX, cellZ) < y &&
        MapGrid_IsClimbableCellFar(cellX, cellZ, 2)) {
      actor->motionFlags = 0;
      Actor_SetMoveTarget(actor, actor->x, groundHere + 0xfffe0000, cellZ);
      actor->timer = 1;
      actor->userValue = 1;
      actor->velocityX = 0;
      actor->velocityY = 0;
      actor->velocityZ = 0;
      Actor_SelectAnimation(actor, 3);
      goto done;
    }
  }

counted:
  {
    struct FieldWork *work = Ram_HeapSlots->fieldWork;

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
    actor->targetX = TARGET_UNSET;
    actor->targetY = TARGET_UNSET;
    actor->targetZ = TARGET_UNSET;
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
    Vector_AddPolarOffset(speed, (u16)heading,
                          (struct FieldPosition *)&actor->velocityX);
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
      Actor_StartScript(effect, gActor_WakeScript);
      effect->priorityFlags = 8;
      // Through a struct member: as a byte offset the store holds back the
      // stack read of collision after it.
      ((struct Unk_WakeActor *)effect)->unk63 = 100;
      effect->motionFlags = 0;
      Sound_PlayFar(SE_596);
      if (sprite != NULL) {
        struct Unk_ClimbEventWork *work = Ram_HeapSlots->fieldWork;

        Animation_ApplyChildArgument(sprite, 1);
        sprite->flags = 0;
        // DEAD: overwritten by the next line.
        sprite->priority = 2;
        sprite->priority = work->unkd4a & 3;
      }
      *wakeTimer = 5;
    }
  }
  Sand_CheckEndKeys(actor);

done:
  actor->step++;
  return 1;
}

/// Script command 0x22 (control mode 9): the leader as sand on the world map
/// (the Sand Psynergy). Moves the object on the world map by the direction pad
/// at a fixed pace (faster while L repeats), trying six turned headings when
/// the way is blocked, as Actor_MoveOnWorldMap does; records the held keys in
/// the event work and leaves an object of kind 0x164 behind it every fifth
/// step.
s32 Actor_MoveAsSandOnWorldMap(struct Actor *object) {
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
  struct WorldCamera *camera;
  s32 yawStep;
  struct PlayerSprite *playerSprite;
  struct Actor *ripple;
  struct PlayerSprite *rippleSprite;
  struct FieldWork *work;
  u8 *timer;

  collision = 0;
  count = 0;
  object->userValue = collision;
  object->timer = 1;
  object->speedLimit = px(1) / 2;
  object->acceleration = px(1) / 4;
  if (gInput.repeat & KEY_L)
    object->speedLimit = px(4);

  angle = gWorldMap_KeyHeadings[(gInput.held >> 4) & 15];
  direction = angle;
  if (direction == 0xffff) {
    collision |= 4;
  } else {
    SET_POSITION(&position, object->x, object->y, object->z);
    Vector_AddPolarOffset(px(8), direction, &position);
    if (gDebugMode == 0 || (gInput.held & KEY_L) == 0) {
      SET_POSITION(&leftPosition, object->x, object->y, object->z);
      Vector_AddPolarOffset(px(8), direction + 0x1555, &leftPosition);
      rightPosition.x = object->x;
      /* FAKEMATCH(temporary): y read into its own local before the block. */
      y = object->y;
      /* FAKEMATCH(dead): the right probe's y stored in a once-only block. */
      do {
        rightPosition.y = y;
      } while (0);
      rightPosition.z = object->z;
      Vector_AddPolarOffset(px(8), direction - 0x1555, &rightPosition);
      if ((WorldMap_IsWalkable(object, &position) |
           WorldMap_IsWalkable(object, &leftPosition) |
           WorldMap_IsWalkable(object, &rightPosition)) != 0) {
        angleOffsets[count++] = direction + TRIG_TURN / 16;
        angleOffsets[count++] = direction - TRIG_TURN / 16;
        angleOffsets[count++] = direction + TRIG_TURN / 8;
        angleOffsets[count++] = direction - TRIG_TURN / 8;
        angleOffsets[count++] = direction + TRIG_TURN * 3 / 16;
        angleOffsets[count++] = direction - TRIG_TURN * 3 / 16;
        for (angleIndex = 0; angleIndex < count; angleIndex++) {
          direction = angleOffsets[angleIndex];
          SET_POSITION(&position, object->x, object->y, object->z);
          Vector_AddPolarOffset(px(8), direction, &position);
          SET_POSITION(&leftPosition, object->x, object->y, object->z);
          Vector_AddPolarOffset(px(8), direction + 0x1555, &leftPosition);
          SET_POSITION(&rightPosition, object->x, object->y, object->z);
          Vector_AddPolarOffset(px(8), direction - 0x1555, &rightPosition);
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
  work = Ram_HeapSlots->fieldWork;
  if (work != NULL) {
    if ((collision & 3) != 0)
      work->blockedSteps++;
    else
      work->blockedSteps = 0;
    work->heldKeys = gInput.held;
  }
  Actor_SelectAnimation(object, 1);

  if (collision != 0) {
    object->targetX = TARGET_UNSET;
    object->targetY = TARGET_UNSET;
    object->targetZ = TARGET_UNSET;
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
    object->userValue = 2;
  } else {
    Actor_SetMoveTarget(object, position.x, position.y, position.z);
    speed = FixedSqrt(Iwram_MulQ16(object->velocityX, object->velocityX) +
                      Iwram_MulQ16(object->velocityZ, object->velocityZ));
    object->velocityX = collision;
    object->velocityZ = collision;
    Vector_AddPolarOffset(speed, (u16)angle,
                          (struct FieldPosition *)&object->velocityX);
    timer = (u8 *)object + 98;
    if (*timer != 0)
      (*timer)--;
  }

  camera = (struct WorldCamera *)Ram_HeapSlots->mapWork;
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
      Actor_StartScript(ripple, gActor_WakeScript);
      ripple->priorityFlags = 8;
      ((struct Unk0802a124Actor *)ripple)->unk63 = 100;
      ripple->motionFlags = collision;
      if (rippleSprite != 0) {
        Animation_ApplyChildArgument(rippleSprite, 1);
        rippleSprite->flags = collision;
        rippleSprite->priority = 2;
        rippleSprite->priority = 1;
      }
      *timer = 5;
    }
  }
  Sand_CheckEndKeys(object);
  object->step++;
  return 1;
}
